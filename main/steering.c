#include "steering.h"
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

static void limit_sw_isr(void *arg);

esp_err_t steering_init(void)
{
    /* Configure limit switch inputs */
    gpio_config_t limit_conf = {
        .pin_bit_mask = (1ULL << GPIO_LIMIT_LEFT) | (1ULL << GPIO_LIMIT_RIGHT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE,
    };
    esp_err_t ret = gpio_config(&limit_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure limit switches");
        return ret;
    }

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_LIMIT_LEFT, limit_sw_isr, (void *)0);
    gpio_isr_handler_add(GPIO_LIMIT_RIGHT, limit_sw_isr, (void *)1);

    /* Create pulse generator */
    ret = rmt_pulse_gen_create(&s_pulse_gen, GPIO_STEP, GPIO_STEP_DIR);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create pulse generator");
        return ret;
    }

#if CFG_ENABLE_STEERING_FEEDBACK
    /* Create position tracker for closed-loop feedback */
    ret = pcnt_tracker_create(&s_tracker, GPIO_STEP);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create position tracker");
        return ret;
    }
    rmt_pulse_gen_set_tracker(s_pulse_gen, s_tracker);
#endif

    /* Initialize hardware */
    ret = rmt_pulse_gen_init(s_pulse_gen);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init pulse generator");
        return ret;
    }

#if CFG_ENABLE_STEERING_FEEDBACK
    ret = pcnt_tracker_init(s_tracker);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init position tracker");
        return ret;
    }
#endif

    s_state = STEERING_IDLE;
    s_current_angle = 0.0f;
    s_current_pulses = 0;
    s_initialized = true;

    ESP_LOGI(TAG, "Steering initialized: %d pulses/deg, max ±%d deg",
             (int)STEERING_PULSES_PER_DEGREE, STEERING_MAX_ANGLE_DEG);
    return ESP_OK;
}

void steering_update(uint16_t rc_pulse_us)
{
    if (!s_initialized) return;

    /* Check limits */
    int left_triggered = (gpio_get_level(GPIO_LIMIT_LEFT) == STEERING_LIMIT_ACTIVE_LEVEL);
    int right_triggered = (gpio_get_level(GPIO_LIMIT_RIGHT) == STEERING_LIMIT_ACTIVE_LEVEL);

    /* Map RC pulse to target angle */
    float target_angle;
    if (rc_pulse_us < RC_CENTER_US - RC_STICK_DEADBAND_US) {
        /* Left turn */
        float ratio = (float)(RC_CENTER_US - rc_pulse_us) /
                      (float)(RC_CENTER_US - RC_MIN_VALID_US);
        target_angle = -ratio * STEERING_MAX_ANGLE_DEG;
        if (left_triggered) target_angle = s_current_angle; /* Hold at limit */
        s_state = STEERING_MOVING;
    } else if (rc_pulse_us > RC_CENTER_US + RC_STICK_DEADBAND_US) {
        /* Right turn */
        float ratio = (float)(rc_pulse_us - RC_CENTER_US) /
                      (float)(RC_MAX_VALID_US - RC_CENTER_US);
        target_angle = ratio * STEERING_MAX_ANGLE_DEG;
        if (right_triggered) target_angle = s_current_angle;
        s_state = STEERING_MOVING;
    } else {
        /* Deadband — hold position */
        target_angle = s_current_angle;
        if (!rmt_pulse_gen_is_running(s_pulse_gen)) {
            s_state = STEERING_IDLE;
        }
    }

    /* Clamp */
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

    /* Convert angle to pulses */
    int32_t target_pulses = (int32_t)roundf(target_angle * STEERING_PULSES_PER_DEGREE);
    int32_t delta_pulses = target_pulses - s_current_pulses;

    /* Deadband in pulses */
    if (abs(delta_pulses) < 5 && fabsf(target_angle - s_current_angle) < 0.1f) {
        return; /* Already at target */
    }

    /* Compute max velocity based on angular speed limit */
    float degrees_per_sec = STEERING_MAX_ANGLE_DEG * 2.0f / (float)STEERING_RAMP_TIME_MS * 1000.0f;
    float max_vel_pps = degrees_per_sec * STEERING_PULSES_PER_DEGREE;
    if (max_vel_pps > STEERING_MAX_SPEED_PPS) max_vel_pps = STEERING_MAX_SPEED_PPS;

    /* Issue position move */
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

float steering_get_angle_deg(void)
{
#if CFG_ENABLE_STEERING_FEEDBACK
    if (s_tracker) {
        int64_t pos = pcnt_tracker_get_position(s_tracker);
        return (float)pos / STEERING_PULSES_PER_DEGREE;
    }
#endif
    return s_current_angle;
}

steering_state_t steering_get_state(void)
{
    return s_state;
}

bool steering_at_limit(void)
{
    int left = gpio_get_level(GPIO_LIMIT_LEFT);
    int right = gpio_get_level(GPIO_LIMIT_RIGHT);
    return (left == STEERING_LIMIT_ACTIVE_LEVEL || right == STEERING_LIMIT_ACTIVE_LEVEL);
}

static void limit_sw_isr(void *arg)
{
    int side = (int)(intptr_t)arg;
    if (side == 0) {
        s_state = STEERING_AT_LIMIT_LEFT;
    } else {
        s_state = STEERING_AT_LIMIT_RIGHT;
    }
    if (s_pulse_gen) {
        rmt_pulse_gen_stop(s_pulse_gen, STEERING_DECELERATION_PPS2 * 2.0f);
    }
}
