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
static uint16_t s_center_us = RC_CENTER_US;
static uint16_t s_combined_pulse_us = RC_CENTER_US;
static uint16_t s_throttle_pct = 0;
static uint16_t s_brake_pct = 0;

static void set_pulse(ledc_channel_t channel, uint16_t pulse_us)
{
    if (!s_initialized) return;
    uint32_t duty = (uint32_t)pulse_us * LEDC_MAX_DUTY / SERVO_PERIOD_US;
    if (duty > LEDC_MAX_DUTY) duty = LEDC_MAX_DUTY;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}

static uint16_t map_pct(uint16_t value, uint16_t min_us, uint16_t max_us)
{
    if (value <= min_us) return 0;
    if (value >= max_us) return 100;
    return (uint16_t)((value - min_us) * 100UL / (max_us - min_us));
}

static void throttle_brake_callback(uint16_t pulse_us, void *user_data)
{
    (void)user_data;
    s_combined_pulse_us = pulse_us;

    uint16_t throttle_pulse = THROTTLE_IDLE_US;
    uint16_t brake_pulse = BRAKE_RELEASED_US;

    if (pulse_us > s_center_us + RC_DEADBAND_STICK_US) {
        /* Upper half: throttle */
        throttle_pulse = pulse_us;
        if (throttle_pulse > THROTTLE_FULL_US) throttle_pulse = THROTTLE_FULL_US;
        brake_pulse = BRAKE_RELEASED_US;
        s_throttle_pct = map_pct(pulse_us, s_center_us, RC_MAX_VALID_US);
        s_brake_pct = 0;
    } else if (pulse_us < s_center_us - RC_DEADBAND_STICK_US) {
        /* Lower half: brake — closer to min = more brake */
        throttle_pulse = THROTTLE_IDLE_US;
        brake_pulse = pulse_us;
        if (brake_pulse > BRAKE_FULL_US) brake_pulse = BRAKE_FULL_US;
        s_throttle_pct = 0;
        s_brake_pct = 100 - map_pct(pulse_us, RC_MIN_VALID_US, s_center_us);
    } else {
        /* Deadband */
        s_throttle_pct = 0;
        s_brake_pct = 0;
    }

    set_pulse(LEDC_CHANNEL_THROTTLE, throttle_pulse);
    set_pulse(LEDC_CHANNEL_BRAKE, brake_pulse);

    /* Debug: every 50th call */
    static int dbg = 0;
    if (++dbg >= 50) {
        dbg = 0;
        ESP_LOGI(TAG, "Ch3=%u us | T=%u us (%u%%) B=%u us (%u%%)",
                 pulse_us, throttle_pulse, s_throttle_pct, brake_pulse, s_brake_pct);
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

    servo_failsafe();

    rc_channel_subscription_t sub = {
        .channel = RC_IDX_THROTTLE_BRAKE,
        .callback = throttle_brake_callback,
        .deadband_us = RC_DEADBAND_STICK_US,
    };
    rc_input_subscribe(&sub);

    s_initialized = true;
    ESP_LOGI(TAG, "Servos ready (GPIO %d/%d, ch%d)",
             GPIO_SERVO_THROTTLE, GPIO_SERVO_BRAKE, RC_IDX_THROTTLE_BRAKE + 1);
    return ESP_OK;
}

void servo_set_center_us(uint16_t center_us)
{
    s_center_us = center_us;
    ESP_LOGI(TAG, "Servo center set to %u us", center_us);
}

void servo_set_throttle(uint16_t pulse_us) { set_pulse(LEDC_CHANNEL_THROTTLE, pulse_us); }
void servo_set_brake(uint16_t pulse_us)    { set_pulse(LEDC_CHANNEL_BRAKE, pulse_us); }

void servo_failsafe(void)
{
    servo_set_throttle(FAILSAFE_THROTTLE_US);
    servo_set_brake(FAILSAFE_BRAKE_US);
    ESP_LOGW(TAG, "Failsafe: throttle=%u brake=%u",
             FAILSAFE_THROTTLE_US, FAILSAFE_BRAKE_US);
}

void servo_disable(void) { servo_set_throttle(0); servo_set_brake(0); }

uint16_t servo_get_combined_pulse_us(void) { return s_combined_pulse_us; }
uint16_t servo_get_throttle_pct(void)      { return s_throttle_pct; }
uint16_t servo_get_brake_pct(void)         { return s_brake_pct; }
