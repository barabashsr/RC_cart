#include "servo.h"
#include "rc_input.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include <stdlib.h>

static const char *TAG = "servo";

#define LEDC_TIMER_THROTTLE         LEDC_TIMER_0
#define LEDC_TIMER_BRAKE            LEDC_TIMER_1
#define LEDC_CHANNEL_THROTTLE       LEDC_CHANNEL_0
#define LEDC_CHANNEL_BRAKE          LEDC_CHANNEL_1

#define LEDC_DUTY_RESOLUTION        LEDC_TIMER_14_BIT
#define LEDC_MAX_DUTY               16383

/* Hysteresis zones for combined throttle+brake stick */
typedef enum { ZONE_IDLE, ZONE_THROTTLE, ZONE_BRAKE } zone_t;

static bool s_initialized = false;
static uint16_t s_center_us = RC_CENTER_US;
static uint16_t s_combined_pulse_us = RC_CENTER_US;

/* Zone hysteresis */
static zone_t s_zone = ZONE_IDLE;

/* Ramp state: current output pulses (applied to LEDC), target pulses (from callback) */
static uint16_t s_curr_throttle_us, s_target_throttle_us;
static uint16_t s_curr_brake_us,    s_target_brake_us;

/* Display stats */
static uint16_t s_throttle_pct = 0;
static uint16_t s_brake_pct = 0;

static void set_pulse(ledc_channel_t channel, uint16_t pulse_us)
{
    if (!s_initialized) return;
    if (pulse_us < SERVO_MIN_PULSE_US) pulse_us = SERVO_MIN_PULSE_US;
    if (pulse_us > SERVO_MAX_PULSE_US) pulse_us = SERVO_MAX_PULSE_US;
    uint32_t duty = (uint32_t)pulse_us * LEDC_MAX_DUTY / SERVO_PERIOD_US;
    if (duty > LEDC_MAX_DUTY) duty = LEDC_MAX_DUTY;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}

static void snap_all(void)
{
    s_curr_throttle_us = s_target_throttle_us;
    s_curr_brake_us = s_target_brake_us;
    set_pulse(LEDC_CHANNEL_THROTTLE, s_curr_throttle_us);
    set_pulse(LEDC_CHANNEL_BRAKE, s_curr_brake_us);
}

static uint16_t map_range(uint16_t x, uint16_t in_min, uint16_t in_max,
                          uint16_t out_min, uint16_t out_max)
{
    if (x <= in_min) return out_min;
    if (x >= in_max) return out_max;
    return out_min + (uint32_t)(x - in_min) * (out_max - out_min) / (in_max - in_min);
}

static uint16_t map_pct(uint16_t value, uint16_t min_us, uint16_t max_us)
{
    if (value <= min_us) return 0;
    if (value >= max_us) return 100;
    return (uint16_t)((value - min_us) * 100UL / (max_us - min_us));
}

static void servo_ramp(uint16_t *curr, uint16_t target)
{
    if (*curr == target) return;
    int16_t step = (int16_t)SERVO_RAMP_STEP_PER_TICK;
    if (target > *curr) {
        if ((uint16_t)(target - *curr) < step) *curr = target;
        else *curr += step;
    } else {
        if ((uint16_t)(*curr - target) < step) *curr = target;
        else *curr -= step;
    }
}

static void throttle_brake_callback(uint16_t pulse_us, void *user_data)
{
    (void)user_data;
    s_combined_pulse_us = pulse_us;

    int32_t offset = (int32_t)pulse_us - (int32_t)s_center_us;

    /* Schmitt trigger for zone switching */
    if (offset > SERVO_HYST_ENTER_US) {
        s_zone = ZONE_THROTTLE;
    } else if (offset < -SERVO_HYST_ENTER_US) {
        s_zone = ZONE_BRAKE;
    } else if (abs(offset) < SERVO_HYST_EXIT_US) {
        s_zone = ZONE_IDLE;
    }
    /* Between EXIT and ENTER thresholds: hold previous zone */

    switch (s_zone) {
    case ZONE_THROTTLE:
        s_target_throttle_us = map_range(pulse_us,
                                         s_center_us + SERVO_HYST_ENTER_US, RC_MAX_VALID_US,
                                         THROTTLE_IDLE_US, THROTTLE_FULL_US);
        s_target_brake_us = BRAKE_RELEASED_US;
        s_throttle_pct = map_pct(pulse_us, s_center_us, RC_MAX_VALID_US);
        s_brake_pct = 0;
        break;
    case ZONE_BRAKE:
        s_target_throttle_us = THROTTLE_IDLE_US;
        s_target_brake_us = map_range(pulse_us,
                                      RC_MIN_VALID_US, s_center_us - SERVO_HYST_ENTER_US,
                                      BRAKE_FULL_US, BRAKE_RELEASED_US);
        s_throttle_pct = 0;
        s_brake_pct = 100 - map_pct(pulse_us, RC_MIN_VALID_US, s_center_us);
        break;
    default: /* ZONE_IDLE */
        s_target_throttle_us = THROTTLE_IDLE_US;
        s_target_brake_us = BRAKE_RELEASED_US;
        s_throttle_pct = 0;
        s_brake_pct = 0;
        break;
    }

    /* Debug: every 50th call */
    static int dbg = 0;
    if (++dbg >= 50) {
        dbg = 0;
        ESP_LOGI(TAG, "Ch3=%u us zone=%d | T: %u→%u us (%u%%) B: %u→%u us (%u%%)",
                 pulse_us, s_zone,
                 s_curr_throttle_us, s_target_throttle_us, s_throttle_pct,
                 s_curr_brake_us, s_target_brake_us, s_brake_pct);
    }
}

esp_err_t servo_init(void)
{
    ledc_timer_config_t timer_throttle = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_DUTY_RESOLUTION,
        .timer_num = LEDC_TIMER_THROTTLE,
        .freq_hz = SERVO_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    esp_err_t ret = ledc_timer_config(&timer_throttle);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "throttle timer: %s", esp_err_to_name(ret)); return ret; }

    ledc_timer_config_t timer_brake = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_DUTY_RESOLUTION,
        .timer_num = LEDC_TIMER_BRAKE,
        .freq_hz = SERVO_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ret = ledc_timer_config(&timer_brake);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "brake timer: %s", esp_err_to_name(ret)); return ret; }

    ledc_channel_config_t ch_throttle = {
        .gpio_num = GPIO_SERVO_THROTTLE,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_THROTTLE,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_THROTTLE,
        .duty = 0, .hpoint = 0,
    };
    ret = ledc_channel_config(&ch_throttle);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "throttle ch: %s", esp_err_to_name(ret)); return ret; }

    ledc_channel_config_t ch_brake = {
        .gpio_num = GPIO_SERVO_BRAKE,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_BRAKE,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_BRAKE,
        .duty = 0, .hpoint = 0,
    };
    ret = ledc_channel_config(&ch_brake);
    if (ret != ESP_OK) { ESP_LOGE(TAG, "brake ch: %s", esp_err_to_name(ret)); return ret; }

    /* Init ramp state — start at failsafe values */
    s_curr_throttle_us = THROTTLE_IDLE_US;
    s_target_throttle_us = THROTTLE_IDLE_US;
    s_curr_brake_us = BRAKE_RELEASED_US;
    s_target_brake_us = BRAKE_RELEASED_US;
    s_zone = ZONE_IDLE;

    servo_failsafe();

    rc_channel_subscription_t sub = {
        .channel = RC_IDX_THROTTLE_BRAKE,
        .callback = throttle_brake_callback,
        .deadband_us = RC_DEADBAND_TOGGLE_US,
    };
    rc_input_subscribe(&sub);

    s_initialized = true;
    ESP_LOGI(TAG, "Servos ready (GPIO %d/%d, ch%d, ramp %u us/s)",
             GPIO_SERVO_THROTTLE, GPIO_SERVO_BRAKE,
             RC_IDX_THROTTLE_BRAKE + 1, SERVO_RAMP_US_PER_SEC);
    return ESP_OK;
}

void servo_set_center_us(uint16_t center_us)
{
    s_center_us = center_us;
    ESP_LOGI(TAG, "Servo center set to %u us", center_us);
}

void servo_set_throttle(uint16_t pulse_us)
{
    s_target_throttle_us = pulse_us;
    s_curr_throttle_us = pulse_us;
    set_pulse(LEDC_CHANNEL_THROTTLE, pulse_us);
}

void servo_set_brake(uint16_t pulse_us)
{
    s_target_brake_us = pulse_us;
    s_curr_brake_us = pulse_us;
    set_pulse(LEDC_CHANNEL_BRAKE, pulse_us);
}

void servo_failsafe(void)
{
    s_target_throttle_us = FAILSAFE_THROTTLE_US;
    s_target_brake_us    = FAILSAFE_BRAKE_US;
    s_zone = ZONE_IDLE;
    snap_all();
    ESP_LOGW(TAG, "Failsafe: throttle=%u brake=%u",
             s_curr_throttle_us, s_curr_brake_us);
}

void servo_disable(void)
{
    s_target_throttle_us = 0;
    s_target_brake_us = 0;
    s_zone = ZONE_IDLE;
    snap_all();
}

void servo_update(void)
{
    if (!s_initialized) return;

    servo_ramp(&s_curr_throttle_us, s_target_throttle_us);
    servo_ramp(&s_curr_brake_us, s_target_brake_us);

    set_pulse(LEDC_CHANNEL_THROTTLE, s_curr_throttle_us);
    set_pulse(LEDC_CHANNEL_BRAKE, s_curr_brake_us);
}

uint16_t servo_get_combined_pulse_us(void) { return s_combined_pulse_us; }
uint16_t servo_get_throttle_pct(void)      { return s_throttle_pct; }
uint16_t servo_get_brake_pct(void)         { return s_brake_pct; }
