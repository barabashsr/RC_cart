#include "servo.h"
#include "rc_input.h"
#include "driver/mcpwm_prelude.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <stdlib.h>

static const char *TAG = "servo";

/* MCPWM handles — single 1 MHz timer feeds both operators */
static mcpwm_timer_handle_t s_timer = NULL;
static mcpwm_oper_handle_t s_oper_throttle = NULL;
static mcpwm_oper_handle_t s_oper_brake = NULL;
static mcpwm_cmpr_handle_t s_cmpr_throttle = NULL;
static mcpwm_cmpr_handle_t s_cmpr_brake = NULL;
static mcpwm_gen_handle_t s_gen_throttle = NULL;
static mcpwm_gen_handle_t s_gen_brake = NULL;

static bool s_initialized = false;
static uint16_t s_center_us = RC_CENTER_US;
static uint16_t s_combined_pulse_us = RC_CENTER_US;

/* Calibrated Ch3 range — set by servo_set_ch3_calibration() */
static uint16_t s_ch3_min_us = RC_MIN_VALID_US;
static uint16_t s_ch3_max_us = RC_MAX_VALID_US;
static uint16_t s_ch3_center_us = RC_CENTER_US;

typedef enum { ZONE_IDLE, ZONE_THROTTLE, ZONE_BRAKE } zone_t;
static zone_t s_zone = ZONE_IDLE;

/* State per servo: target (desired) and current (EWMA-smoothed output) */
static uint16_t s_target_throttle_us, s_current_throttle_us;
static uint16_t s_target_brake_us, s_current_brake_us;

static uint16_t s_throttle_pct = 0;
static uint16_t s_brake_pct = 0;
static bool s_failsafe_active = false;

static void apply_pulse(mcpwm_cmpr_handle_t cmpr, uint16_t pulse_us)
{
    if (pulse_us < SERVO_HW_MIN_PULSE_US) pulse_us = SERVO_HW_MIN_PULSE_US;
    if (pulse_us > SERVO_HW_MAX_PULSE_US) pulse_us = SERVO_HW_MAX_PULSE_US;
    mcpwm_comparator_set_compare_value(cmpr, pulse_us);
}

static uint16_t map_pct(uint16_t value, uint16_t min_us, uint16_t max_us)
{
    if (value <= min_us) return 0;
    if (value >= max_us) return 100;
    return (uint16_t)((value - min_us) * 100UL / (max_us - min_us));
}

static uint16_t ramp_toward(uint16_t curr, uint16_t target, int32_t step)
{
    int32_t diff = (int32_t)target - (int32_t)curr;
    if (diff > step)       return (uint16_t)((int32_t)curr + step);
    else if (diff < -step) return (uint16_t)((int32_t)curr - step);
    else                   return target;
}

static void throttle_brake_callback(uint16_t pulse_us, void *user_data)
{
    (void)user_data;
    s_combined_pulse_us = pulse_us;

    int32_t offset = (int32_t)pulse_us - (int32_t)s_center_us;

    /* Schmitt trigger for zone switching:
     *   offset >  +ENTER  → THROTTLE
     *   offset <  -ENTER  → BRAKE
     *   |offset| < EXIT  → IDLE
     *   between EXIT and ENTER: hold previous zone
     * Deadband on subscription is 3 us (RC_CALLBACK_NOISE_US) so the callback
     * fires on every meaningful pulse change — Schmitt handles stability. */
    if (offset > SERVO_HYST_ENTER_US) {
        s_zone = ZONE_THROTTLE;
    } else if (offset < -SERVO_HYST_ENTER_US) {
        s_zone = ZONE_BRAKE;
    } else if (abs(offset) < SERVO_HYST_EXIT_US) {
        s_zone = ZONE_IDLE;
    }

    /* First active stick input after failsafe recovery — resume normal ramp speed */
    if (s_failsafe_active && (s_zone == ZONE_THROTTLE || s_zone == ZONE_BRAKE)) {
        s_failsafe_active = false;
    }

    switch (s_zone) {
    case ZONE_THROTTLE: {
        uint16_t span_in  = s_ch3_max_us - (s_ch3_center_us + SERVO_HYST_ENTER_US);
        uint16_t span_out = THROTTLE_FULL_US - THROTTLE_IDLE_US;
        int32_t pos = (int32_t)pulse_us - (int32_t)(s_ch3_center_us + SERVO_HYST_ENTER_US);
        if (pos < 0) pos = 0;
        if ((uint32_t)pos > span_in) pos = span_in;
        s_target_throttle_us = THROTTLE_IDLE_US + (uint32_t)pos * span_out / span_in;
        s_target_brake_us = BRAKE_RELEASED_US;
        s_throttle_pct = map_pct(pulse_us, s_ch3_center_us, s_ch3_max_us);
        s_brake_pct = 0;
        break;
    }
    case ZONE_BRAKE: {
        uint16_t span_in  = (s_ch3_center_us - SERVO_HYST_ENTER_US) - s_ch3_min_us;
        uint16_t span_out = BRAKE_FULL_US - BRAKE_RELEASED_US;
        int32_t pos = (int32_t)(s_ch3_center_us - SERVO_HYST_ENTER_US) - (int32_t)pulse_us;
        if (pos < 0) pos = 0;
        if ((uint32_t)pos > span_in) pos = span_in;
        s_target_brake_us = BRAKE_RELEASED_US + (uint32_t)pos * span_out / span_in;
        s_target_throttle_us = THROTTLE_IDLE_US;
        s_throttle_pct = 0;
        s_brake_pct = 100 - map_pct(pulse_us, s_ch3_min_us, s_ch3_center_us);
        break;
    }
    default: /* ZONE_IDLE */
        s_target_throttle_us = THROTTLE_IDLE_US;
        s_target_brake_us = BRAKE_RELEASED_US;
        s_throttle_pct = 0;
        s_brake_pct = 0;
        break;
    }

    /* Debug: every 50th callback */
    static int dbg = 0;
    if (++dbg >= 50) {
        dbg = 0;
        ESP_LOGI(TAG, "Ch3=%u zone=%d | span_in=%u/%u | T:%u->%u (%u%%) B:%u->%u (%u%%)",
                 pulse_us, s_zone,
                 (unsigned int)((s_zone == ZONE_THROTTLE)
                     ? s_ch3_max_us - (s_ch3_center_us + SERVO_HYST_ENTER_US)
                     : (s_ch3_center_us - SERVO_HYST_ENTER_US) - s_ch3_min_us),
                 (unsigned int)((s_zone == ZONE_THROTTLE)
                     ? THROTTLE_FULL_US - THROTTLE_IDLE_US
                     : BRAKE_FULL_US - BRAKE_RELEASED_US),
                 s_current_throttle_us, s_target_throttle_us, s_throttle_pct,
                 s_current_brake_us, s_target_brake_us, s_brake_pct);
    }
}

esp_err_t servo_init(void)
{
    ESP_LOGI(TAG, "Initializing MCPWM servos (GPIO %d/%d)...",
             GPIO_SERVO_THROTTLE, GPIO_SERVO_BRAKE);

    /* 1. Timer: 1 MHz resolution = 1 us per tick, 20000 ticks = 20 ms period = 50 Hz */
    mcpwm_timer_config_t timer_cfg = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD_TICKS,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_timer(&timer_cfg, &s_timer), TAG, "new_timer");

    /* 2. Throttle operator + comparator + generator */
    mcpwm_operator_config_t oper_cfg = { .group_id = 0 };
    ESP_RETURN_ON_ERROR(mcpwm_new_operator(&oper_cfg, &s_oper_throttle), TAG, "new_oper_t");
    ESP_RETURN_ON_ERROR(mcpwm_operator_connect_timer(s_oper_throttle, s_timer), TAG, "connect_t");

    mcpwm_comparator_config_t cmpr_cfg = {
        .flags.update_cmp_on_tez = true,  /* Latch new compare value at timer zero only */
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_comparator(s_oper_throttle, &cmpr_cfg, &s_cmpr_throttle), TAG, "new_cmpr_t");

    mcpwm_generator_config_t gen_cfg = { .gen_gpio_num = GPIO_SERVO_THROTTLE };
    ESP_RETURN_ON_ERROR(mcpwm_new_generator(s_oper_throttle, &gen_cfg, &s_gen_throttle), TAG, "new_gen_t");

    /* 3. Brake operator + comparator + generator */
    ESP_RETURN_ON_ERROR(mcpwm_new_operator(&oper_cfg, &s_oper_brake), TAG, "new_oper_b");
    ESP_RETURN_ON_ERROR(mcpwm_operator_connect_timer(s_oper_brake, s_timer), TAG, "connect_b");
    ESP_RETURN_ON_ERROR(mcpwm_new_comparator(s_oper_brake, &cmpr_cfg, &s_cmpr_brake), TAG, "new_cmpr_b");

    mcpwm_generator_config_t gen_cfg_b = { .gen_gpio_num = GPIO_SERVO_BRAKE };
    ESP_RETURN_ON_ERROR(mcpwm_new_generator(s_oper_brake, &gen_cfg_b, &s_gen_brake), TAG, "new_gen_b");

    /* 4. PWM waveform: HIGH at timer zero (start of pulse), LOW at compare match (end of pulse) */
    ESP_RETURN_ON_ERROR(mcpwm_generator_set_action_on_timer_event(s_gen_throttle,
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)), TAG, "tez_t");
    ESP_RETURN_ON_ERROR(mcpwm_generator_set_action_on_compare_event(s_gen_throttle,
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, s_cmpr_throttle, MCPWM_GEN_ACTION_LOW)), TAG, "cmp_t");
    ESP_RETURN_ON_ERROR(mcpwm_generator_set_action_on_timer_event(s_gen_brake,
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)), TAG, "tez_b");
    ESP_RETURN_ON_ERROR(mcpwm_generator_set_action_on_compare_event(s_gen_brake,
        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, s_cmpr_brake, MCPWM_GEN_ACTION_LOW)), TAG, "cmp_b");

    /* 5. Init state to idle (released) — failsafe triggers on actual signal loss, not boot */
    s_target_throttle_us = s_current_throttle_us = THROTTLE_IDLE_US;
    s_target_brake_us    = s_current_brake_us    = BRAKE_RELEASED_US;
    s_zone = ZONE_IDLE;

    apply_pulse(s_cmpr_throttle, s_current_throttle_us);
    apply_pulse(s_cmpr_brake, s_current_brake_us);

    ESP_RETURN_ON_ERROR(mcpwm_timer_enable(s_timer), TAG, "timer_enable");
    ESP_RETURN_ON_ERROR(mcpwm_timer_start_stop(s_timer, MCPWM_TIMER_START_NO_STOP), TAG, "timer_start");

    /* 6. Subscribe to RC channel with noise-only deadband — Schmitt handles zone stability */
    rc_channel_subscription_t sub = {
        .channel = RC_IDX_THROTTLE_BRAKE,
        .callback = throttle_brake_callback,
        .deadband_us = RC_CALLBACK_NOISE_US,
    };
    rc_input_subscribe(&sub);

    s_initialized = true;
    ESP_LOGI(TAG, "MCPWM servos ready (1 MHz timer, 50 Hz PWM)");
    return ESP_OK;
}

void servo_set_center_us(uint16_t center_us)
{
    s_center_us = center_us;
    ESP_LOGI(TAG, "Center set to %u us", center_us);
}

void servo_set_ch3_calibration(uint16_t min_us, uint16_t center_us, uint16_t max_us)
{
    s_ch3_min_us = min_us;
    s_ch3_center_us = center_us;
    s_ch3_max_us = max_us;
    s_center_us = center_us;
    ESP_LOGI(TAG, "Ch3 calibrated: %u-%u-%u us", min_us, center_us, max_us);
}

void servo_set_throttle(uint16_t pulse_us)
{
    s_target_throttle_us = pulse_us;
    s_current_throttle_us = pulse_us;
    apply_pulse(s_cmpr_throttle, pulse_us);
}

void servo_set_brake(uint16_t pulse_us)
{
    s_target_brake_us = pulse_us;
    s_current_brake_us = pulse_us;
    apply_pulse(s_cmpr_brake, pulse_us);
}

void servo_failsafe(void)
{
    s_target_throttle_us = FAILSAFE_THROTTLE_US;
    s_target_brake_us    = FAILSAFE_BRAKE_US;
    s_zone = ZONE_IDLE;
    s_failsafe_active = true;
    ESP_LOGW(TAG, "Failsafe: throttle=%u brake=%u (ramping)",
             (unsigned int)FAILSAFE_THROTTLE_US, (unsigned int)FAILSAFE_BRAKE_US);
}

void servo_disable(void)
{
    s_initialized = false;
    s_zone = ZONE_IDLE;
    mcpwm_timer_start_stop(s_timer, MCPWM_TIMER_STOP_EMPTY);
    ESP_LOGW(TAG, "Servos disabled (timer stopped)");
}

void servo_update(void)
{
    if (!s_initialized) return;

    int32_t tdiff = (int32_t)s_target_throttle_us - (int32_t)s_current_throttle_us;
    int32_t bdiff = (int32_t)s_target_brake_us    - (int32_t)s_current_brake_us;

    int32_t step = s_failsafe_active ? SERVO_FAILSAFE_STEP : SERVO_RAMP_STEP;
    s_current_throttle_us = ramp_toward(s_current_throttle_us, s_target_throttle_us, step);
    s_current_brake_us    = ramp_toward(s_current_brake_us,    s_target_brake_us,    step);

    apply_pulse(s_cmpr_throttle, s_current_throttle_us);
    apply_pulse(s_cmpr_brake, s_current_brake_us);

    /* Log large deltas at most once per 500ms to avoid flooding */
    static int64_t s_last_ramp_log_us = 0;
    int64_t now = esp_timer_get_time();
    if ((abs(tdiff) > 100 || abs(bdiff) > 100) && (now - s_last_ramp_log_us) > 500000) {
        s_last_ramp_log_us = now;
        ESP_LOGW(TAG, "!RAMP! T:%u->%u(%+" PRId32 ") B:%u->%u(%+" PRId32 ") z:%d fs:%d",
                 s_current_throttle_us, s_target_throttle_us, tdiff,
                 s_current_brake_us, s_target_brake_us, bdiff, s_zone, s_failsafe_active);
    }
}

uint16_t servo_get_combined_pulse_us(void) { return s_combined_pulse_us; }
uint16_t servo_get_throttle_pct(void)      { return s_throttle_pct; }
uint16_t servo_get_brake_pct(void)         { return s_brake_pct; }
