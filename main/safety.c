#include "safety.h"
#include "rc_input.h"
#include "steering.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "safety";

static safety_callback_t s_callback = NULL;
static bool s_estop_active = false;
static bool s_signal_lost = false;
static int64_t s_signal_lost_start_us = 0;
static int64_t s_last_check_us = 0;

static void IRAM_ATTR estop_isr(void *arg)
{
    s_estop_active = true;
}

esp_err_t safety_init(void)
{
    /* Configure E-Stop as input with pullup */
    gpio_config_t estop_conf = {
        .pin_bit_mask = (1ULL << GPIO_ESTOP),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    esp_err_t ret = gpio_config(&estop_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure E-Stop GPIO");
        return ret;
    }

    /* ISR service already installed by rc_input_init */
    gpio_isr_handler_add(GPIO_ESTOP, estop_isr, NULL);

    s_estop_active = (gpio_get_level(GPIO_ESTOP) == 0);
    s_last_check_us = esp_timer_get_time();

    ESP_LOGI(TAG, "Safety initialized (E-Stop on GPIO %d, active LOW)", GPIO_ESTOP);
    return ESP_OK;
}

safety_event_t safety_check(void)
{
    int64_t now = esp_timer_get_time();

    /* Check E-Stop */
    bool estop_now = (gpio_get_level(GPIO_ESTOP) == 0);
    if (estop_now) {
        s_estop_active = true;
    }

    if (s_estop_active) {
        if (s_callback) s_callback(SAFETY_ESTOP_ACTIVE);
        return SAFETY_ESTOP_ACTIVE;
    }

    /* Check RC signal */
    bool signal_ok = rc_input_is_signal_ok();
    if (!signal_ok) {
        if (!s_signal_lost) {
            s_signal_lost = true;
            s_signal_lost_start_us = now;
            ESP_LOGW(TAG, "RC signal LOST");
        }
        if ((now - s_signal_lost_start_us) > RC_SIGNAL_LOSS_TIMEOUT_MS * 1000) {
            if (s_callback) s_callback(SAFETY_SIGNAL_LOST);
            return SAFETY_SIGNAL_LOST;
        }
    } else {
        if (s_signal_lost && (now - s_signal_lost_start_us) > RC_SIGNAL_RECOVERY_MS * 1000) {
            s_signal_lost = false;
            ESP_LOGI(TAG, "RC signal RECOVERED");
        }
    }

    /* Check steering limits */
    if (steering_at_limit()) {
        if (s_callback) s_callback(SAFETY_LIMIT_TRIGGERED);
        return SAFETY_LIMIT_TRIGGERED;
    }

    s_last_check_us = now;
    return SAFETY_OK;
}

bool safety_is_estop_active(void)
{
    return s_estop_active;
}

bool safety_is_ok(void)
{
    return (!s_estop_active && rc_input_is_signal_ok());
}

void safety_set_callback(safety_callback_t cb)
{
    s_callback = cb;
}
