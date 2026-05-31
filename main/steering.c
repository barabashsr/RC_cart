#include "steering.h"
#include "rc_input.h"
#include "rmt_pulse_gen/rmt_pulse_gen.h"
#include "rmt_pulse_gen/pcnt_tracker.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <math.h>

static const char *TAG = "steering";

static rmt_pulse_gen_t *s_pulse_gen = NULL;
#if CFG_ENABLE_STEERING_FEEDBACK
static pcnt_tracker_t *s_tracker = NULL;
#endif
static steering_state_t s_state = STEERING_IDLE;
static float s_current_angle = 0.0f;
static int32_t s_current_pulses = 0;
static bool s_initialized = false;
static uint16_t s_center_us = RC_CENTER_US;

static void limit_sw_isr(void *arg);

static void steering_callback(uint16_t pulse_us, void *user_data)
{
    (void)user_data;
    steering_update(pulse_us);
}

esp_err_t steering_init(void)
{
    gpio_config_t limit_conf = {
        .pin_bit_mask = (1ULL << GPIO_LIMIT_LEFT) | (1ULL << GPIO_LIMIT_RIGHT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    esp_err_t ret = gpio_config(&limit_conf);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "limit GPIO: %s", esp_err_to_name(ret)); return ret; }

    /* ISR service already installed by rc_input_init — just add handlers */
    gpio_isr_handler_add(GPIO_LIMIT_LEFT, limit_sw_isr, (void *)0);
    gpio_isr_handler_add(GPIO_LIMIT_RIGHT, limit_sw_isr, (void *)1);

    ret = rmt_pulse_gen_create(&s_pulse_gen, GPIO_STEP, GPIO_STEP_DIR);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "pulse gen create: %s", esp_err_to_name(ret)); return ret; }

#if CFG_ENABLE_STEERING_FEEDBACK
    ret = pcnt_tracker_create(&s_tracker, GPIO_STEP);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "tracker create: %s", esp_err_to_name(ret)); return ret; }
    rmt_pulse_gen_set_tracker(s_pulse_gen, s_tracker);
#endif

    ret = rmt_pulse_gen_init(s_pulse_gen);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "pulse gen init: %s", esp_err_to_name(ret)); return ret; }

#if CFG_ENABLE_STEERING_FEEDBACK
    ret = pcnt_tracker_init(s_tracker);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "tracker init: %s", esp_err_to_name(ret)); return ret; }
#endif

    s_state = STEERING_IDLE;
    s_current_angle = 0.0f;
    s_current_pulses = 0;

    /* Configure steering enable GPIO */
    gpio_config_t ena_conf = {
        .pin_bit_mask = (1ULL << GPIO_STEP_ENABLE),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,   /* LOW when unconfigured = safe (disabled) */
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&ena_conf);
    gpio_set_level(GPIO_STEP_ENABLE, 1);  /* Default enabled */

    s_initialized = true;

    /* Subscribe to steering channel */
    rc_channel_subscription_t sub = {
        .channel = RC_IDX_STEERING,
        .callback = steering_callback,
        .deadband_us = RC_DEADBAND_STICK_US,
    };
    rc_input_subscribe(&sub);

    ESP_LOGI(TAG, "Steering ready: %d pulses/deg, max ±%d deg, ch%d",
             (int)STEERING_PULSES_PER_DEGREE, STEERING_MAX_ANGLE_DEG, RC_IDX_STEERING + 1);
    return ESP_OK;
}

void steering_set_center_us(uint16_t center_us)
{
    s_center_us = center_us;
    ESP_LOGI(TAG, "Center set to %u us", center_us);
}

void steering_update(uint16_t rc_pulse_us)
{
    if (!s_initialized) return;

    int left_triggered = (gpio_get_level(GPIO_LIMIT_LEFT) == STEERING_LIMIT_ACTIVE_LEVEL);
    int right_triggered = (gpio_get_level(GPIO_LIMIT_RIGHT) == STEERING_LIMIT_ACTIVE_LEVEL);

    float target_angle;
    if (rc_pulse_us < s_center_us - RC_DEADBAND_STICK_US) {
        float ratio = (float)(s_center_us - rc_pulse_us) /
                      (float)(s_center_us - RC_MIN_VALID_US);
        target_angle = -ratio * STEERING_MAX_ANGLE_DEG;
        if (left_triggered) target_angle = s_current_angle;
        s_state = STEERING_MOVING;
    } else if (rc_pulse_us > s_center_us + RC_DEADBAND_STICK_US) {
        float ratio = (float)(rc_pulse_us - s_center_us) /
                      (float)(RC_MAX_VALID_US - s_center_us);
        target_angle = ratio * STEERING_MAX_ANGLE_DEG;
        if (right_triggered) target_angle = s_current_angle;
        s_state = STEERING_MOVING;
    } else {
        target_angle = s_current_angle;
        if (!rmt_pulse_gen_is_running(s_pulse_gen)) {
            s_state = STEERING_IDLE;
        }
    }

    if (target_angle > STEERING_MAX_ANGLE_DEG) target_angle = STEERING_MAX_ANGLE_DEG;
    if (target_angle < -STEERING_MAX_ANGLE_DEG) target_angle = -STEERING_MAX_ANGLE_DEG;

    if (left_triggered && target_angle < s_current_angle) {
        target_angle = s_current_angle;
        s_state = STEERING_AT_LIMIT_LEFT;
    }
    if (right_triggered && target_angle > s_current_angle) {
        target_angle = s_current_angle;
        s_state = STEERING_AT_LIMIT_RIGHT;
    }

    int32_t target_pulses = (int32_t)roundf(target_angle * STEERING_PULSES_PER_DEGREE);
    int32_t delta_pulses = target_pulses - s_current_pulses;

    if (abs(delta_pulses) < 5 && fabsf(target_angle - s_current_angle) < 0.1f) return;

    float degrees_per_sec = STEERING_MAX_ANGLE_DEG * 2.0f / (float)STEERING_RAMP_TIME_MS * 1000.0f;
    float max_vel_pps = degrees_per_sec * STEERING_PULSES_PER_DEGREE;
    if (max_vel_pps > STEERING_MAX_SPEED_PPS) max_vel_pps = STEERING_MAX_SPEED_PPS;

    if (delta_pulses != 0) {
        s_current_pulses = target_pulses;
        s_current_angle = (float)target_pulses / STEERING_PULSES_PER_DEGREE;
        rmt_pulse_gen_start_move(s_pulse_gen, delta_pulses,
                                 max_vel_pps, STEERING_ACCELERATION_PPS2);
    }
}

void steering_stop(void)
{
    if (!s_initialized) return;
    rmt_pulse_gen_stop(s_pulse_gen, STEERING_DECELERATION_PPS2);
    s_state = STEERING_IDLE;
}

void steering_enable(void)
{
    if (!s_initialized) return;
    gpio_set_level(GPIO_STEP_ENABLE, 1);
    ESP_LOGI(TAG, "Steering ENABLE (GPIO%d HIGH)", GPIO_STEP_ENABLE);
}

void steering_disable(void)
{
    if (!s_initialized) return;
    steering_stop();
    gpio_set_level(GPIO_STEP_ENABLE, 0);
    ESP_LOGI(TAG, "Steering DISABLE (GPIO%d LOW)", GPIO_STEP_ENABLE);
}

float steering_get_angle_deg(void)
{
#if CFG_ENABLE_STEERING_FEEDBACK
    if (s_tracker) {
        return (float)pcnt_tracker_get_position(s_tracker) / STEERING_PULSES_PER_DEGREE;
    }
#endif
    return s_current_angle;
}

steering_state_t steering_get_state(void) { return s_state; }

bool steering_at_limit(void)
{
    return (gpio_get_level(GPIO_LIMIT_LEFT) == STEERING_LIMIT_ACTIVE_LEVEL ||
            gpio_get_level(GPIO_LIMIT_RIGHT) == STEERING_LIMIT_ACTIVE_LEVEL);
}

static void limit_sw_isr(void *arg)
{
    int side = (int)(intptr_t)arg;
    s_state = (side == 0) ? STEERING_AT_LIMIT_LEFT : STEERING_AT_LIMIT_RIGHT;
    if (s_pulse_gen) {
        rmt_pulse_gen_stop(s_pulse_gen, STEERING_DECELERATION_PPS2 * 2.0f);
    }
}
