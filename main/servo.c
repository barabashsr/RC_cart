#include "servo.h"
#include "rc_input.h"
#include "driver/ledc.h"
#include "esp_log.h"

static const char *TAG = "servo";

#define LEDC_TIMER_THROTTLE         LEDC_TIMER_0
#define LEDC_TIMER_BRAKE            LEDC_TIMER_1
#define LEDC_CHANNEL_THROTTLE       LEDC_CHANNEL_0
#define LEDC_CHANNEL_BRAKE          LEDC_CHANNEL_1

#define LEDC_DUTY_RESOLUTION        LEDC_TIMER_14_BIT
#define LEDC_MAX_DUTY               16383

static bool s_initialized = false;
static uint16_t s_combined_pulse_us = RC_CENTER_US;

static void set_pulse(ledc_channel_t channel, uint16_t pulse_us)
{
    if (!s_initialized) return;
    uint32_t duty = (uint32_t)pulse_us * LEDC_MAX_DUTY / SERVO_PERIOD_US;
    if (duty > LEDC_MAX_DUTY) duty = LEDC_MAX_DUTY;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}

static void throttle_brake_callback(uint16_t pulse_us, void *user_data)
{
    (void)user_data;
    s_combined_pulse_us = pulse_us;

    uint16_t throttle_pulse = THROTTLE_IDLE_US;
    uint16_t brake_pulse = BRAKE_RELEASED_US;

    if (pulse_us > RC_CENTER_US + RC_DEADBAND_STICK_US) {
        /* Upper half: throttle */
        throttle_pulse = pulse_us;
        if (throttle_pulse > THROTTLE_FULL_US) throttle_pulse = THROTTLE_FULL_US;
        brake_pulse = BRAKE_RELEASED_US;
    } else if (pulse_us < RC_CENTER_US - RC_DEADBAND_STICK_US) {
        /* Lower half: brake */
        throttle_pulse = THROTTLE_IDLE_US;
        brake_pulse = pulse_us;
        if (brake_pulse < BRAKE_RELEASED_US) brake_pulse = BRAKE_RELEASED_US;
    }

    set_pulse(LEDC_CHANNEL_THROTTLE, throttle_pulse);
    set_pulse(LEDC_CHANNEL_BRAKE, brake_pulse);
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

    servo_failsafe();

    /* Subscribe to combined throttle+brake channel */
    rc_channel_subscription_t sub = {
        .channel = RC_IDX_THROTTLE_BRAKE,
        .callback = throttle_brake_callback,
        .deadband_us = RC_DEADBAND_STICK_US,
    };
    rc_input_subscribe(&sub);

    s_initialized = true;
    ESP_LOGI(TAG, "Servos ready (GPIO %d/%d, ch%d/throttle+brake combined)",
             GPIO_SERVO_THROTTLE, GPIO_SERVO_BRAKE, RC_IDX_THROTTLE_BRAKE + 1);
    return ESP_OK;
}

void servo_set_throttle(uint16_t pulse_us) { set_pulse(LEDC_CHANNEL_THROTTLE, pulse_us); }
void servo_set_brake(uint16_t pulse_us)    { set_pulse(LEDC_CHANNEL_BRAKE, pulse_us); }

void servo_failsafe(void)
{
    servo_set_throttle(FAILSAFE_THROTTLE_US);
    servo_set_brake(FAILSAFE_BRAKE_US);
    ESP_LOGW(TAG, "Failsafe applied: throttle=%u us, brake=%u us",
             FAILSAFE_THROTTLE_US, FAILSAFE_BRAKE_US);
}

void servo_disable(void)
{
    servo_set_throttle(0);
    servo_set_brake(0);
}

uint16_t servo_get_combined_pulse_us(void)
{
    return s_combined_pulse_us;
}
