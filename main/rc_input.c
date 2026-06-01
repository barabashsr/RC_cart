#include "rc_input.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdatomic.h>

static const char *TAG = "rc_input";

static rc_channel_t s_channels[RC_NUM_CHANNELS];
static _Atomic rc_signal_state_t s_signal_state;
static _Atomic int64_t s_last_valid_us;
static _Atomic bool s_initialized;

static const gpio_num_t s_ch_gpios[RC_NUM_CHANNELS] = {
    GPIO_RC_CH1, GPIO_RC_CH2, GPIO_RC_CH3,
    GPIO_RC_CH4, GPIO_RC_CH5, GPIO_RC_CH6,
};

/* Edge-timing state — written by ISR */
static volatile uint64_t s_rising_ts[RC_NUM_CHANNELS];

/* ---- Subscription system ---- */

typedef struct {
    rc_channel_subscription_t sub;
    uint16_t last_reported_us;
    bool active;
} sub_entry_t;

static sub_entry_t s_subs[RC_MAX_SUBSCRIPTIONS];
static int s_sub_count = 0;

/* ---- GPIO Edge ISR ---- */

static void IRAM_ATTR rc_gpio_isr(void *arg)
{
    int ch_idx = *(int *)arg;
    int level = gpio_get_level(s_ch_gpios[ch_idx]);

    if (level) {
        /* Rising edge — capture start timestamp */
        s_rising_ts[ch_idx] = esp_timer_get_time();
    } else {
        /* Falling edge — compute pulse width */
        uint64_t now = esp_timer_get_time();
        uint64_t rise = s_rising_ts[ch_idx];
        if (rise == 0) return;  /* No rising edge captured yet */

        uint64_t pulse_us64 = now - rise;
        if (pulse_us64 >= RC_MIN_VALID_US && pulse_us64 <= RC_MAX_VALID_US) {
            uint16_t pulse = (uint16_t)pulse_us64;
            s_channels[ch_idx].pulse_us = pulse;
            s_channels[ch_idx].valid = true;
            s_channels[ch_idx].last_update_us = (int64_t)now;
            atomic_store(&s_last_valid_us, (int64_t)now);
            atomic_store(&s_signal_state, RC_SIGNAL_OK);
        }
    }
}

/* ---- Init ---- */

esp_err_t rc_input_init(void)
{
    if (atomic_load(&s_initialized)) {
        return ESP_OK;
    }

    memset(s_channels, 0, sizeof(s_channels));
    for (int i = 0; i < RC_NUM_CHANNELS; i++) s_rising_ts[i] = 0;
    memset(s_subs, 0, sizeof(s_subs));
    s_sub_count = 0;
    atomic_store(&s_signal_state, RC_SIGNAL_LOST);
    atomic_store(&s_last_valid_us, 0);

    /* Install GPIO ISR service */
    gpio_install_isr_service(0);

    /* Channel index storage for ISR context (must be DRAM) */
    static int ch_idx_data[RC_NUM_CHANNELS];

    for (int i = 0; i < RC_NUM_CHANNELS; i++) {
        ch_idx_data[i] = i;

        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << s_ch_gpios[i]),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_ANYEDGE,
        };
        esp_err_t ret = gpio_config(&io_conf);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "GPIO config ch%d (pin %d): %s", i + 1, s_ch_gpios[i], esp_err_to_name(ret));
            return ret;
        }

        ret = gpio_isr_handler_add(s_ch_gpios[i], rc_gpio_isr, &ch_idx_data[i]);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "ISR add ch%d: %s", i + 1, esp_err_to_name(ret));
            return ret;
        }

        ESP_LOGI(TAG, "RC ch%d on GPIO %d", i + 1, s_ch_gpios[i]);
    }

    atomic_store(&s_initialized, true);
    ESP_LOGI(TAG, "RC input ready (%d channels, GPIO edge ISR)", RC_NUM_CHANNELS);
    return ESP_OK;
}

/* ---- Pulse access ---- */

bool rc_input_get_pulse(int channel, uint16_t *pulse_us)
{
    if (channel < 0 || channel >= RC_NUM_CHANNELS) return false;

    *pulse_us = s_channels[channel].pulse_us;

    int64_t now = esp_timer_get_time();
    int64_t age_us = now - s_channels[channel].last_update_us;
    bool fresh = (age_us < RC_SIGNAL_LOSS_TIMEOUT_MS * 1000);

    if (!fresh) {
        s_channels[channel].valid = false;
    }

    return s_channels[channel].valid;
}

bool rc_input_is_discrete_on(int channel)
{
    if (channel < 0 || channel >= RC_NUM_CHANNELS) return false;

    uint16_t pulse = 0;
    if (!rc_input_get_pulse(channel, &pulse)) return false;

    return (pulse > RC_DISCRETE_ON_THRESHOLD_US);
}

rc_signal_state_t rc_input_get_state(void)
{
    return atomic_load(&s_signal_state);
}

bool rc_input_is_signal_ok(void)
{
    int64_t now = esp_timer_get_time();
    int64_t last = atomic_load(&s_last_valid_us);
    return (now - last) < (RC_SIGNAL_LOSS_TIMEOUT_MS * 1000);
}

int64_t rc_input_get_last_valid_us(void)
{
    return atomic_load(&s_last_valid_us);
}

/* ---- Calibration ---- */

esp_err_t rc_input_calibrate_center(uint16_t *center_us_out)
{
    if (!center_us_out) return ESP_ERR_INVALID_ARG;

    int64_t deadline_us = esp_timer_get_time() + RC_CALIBRATE_TIMEOUT_MS * 1000;
    uint32_t sum = 0;
    int samples = 0;

    ESP_LOGI(TAG, "Calibrating steering center (%d samples, %d ms)...",
             RC_CALIBRATE_SAMPLES, RC_CALIBRATE_TIMEOUT_MS);

    while (samples < RC_CALIBRATE_SAMPLES) {
        uint16_t pulse;
        if (rc_input_get_pulse(RC_IDX_STEERING, &pulse)) {
            sum += pulse;
            samples++;
        }

        if (esp_timer_get_time() > deadline_us) {
            if (samples > 0) break;
            ESP_LOGW(TAG, "Calibration timeout");
            return ESP_ERR_TIMEOUT;
        }

        vTaskDelay(pdMS_TO_TICKS(RC_EXPECTED_FRAME_MS));
    }

    uint16_t center = (uint16_t)(sum / samples);
    ESP_LOGI(TAG, "Center calibrated: %u us (%d samples)", center, samples);

    *center_us_out = center;
    return ESP_OK;
}

/* ---- Subscription system ---- */

esp_err_t rc_input_subscribe(const rc_channel_subscription_t *sub)
{
    if (!sub || !sub->callback) return ESP_ERR_INVALID_ARG;
    if (sub->channel < 0 || sub->channel >= RC_NUM_CHANNELS) return ESP_ERR_INVALID_ARG;

    if (s_sub_count >= RC_MAX_SUBSCRIPTIONS) {
        ESP_LOGE(TAG, "Subscription table full (%d/%d)", s_sub_count, RC_MAX_SUBSCRIPTIONS);
        return ESP_ERR_NO_MEM;
    }

    s_subs[s_sub_count].sub = *sub;
    s_subs[s_sub_count].last_reported_us = 0;
    s_subs[s_sub_count].active = true;
    s_sub_count++;

    ESP_LOGI(TAG, "Subscribed ch%d deadband=%u us", sub->channel + 1, sub->deadband_us);
    return ESP_OK;
}

void rc_input_process_callbacks(void)
{
    for (int i = 0; i < s_sub_count; i++) {
        sub_entry_t *entry = &s_subs[i];
        if (!entry->active) continue;

        int ch = entry->sub.channel;
        uint16_t current_us = 0;
        if (!rc_input_get_pulse(ch, &current_us)) continue;

        uint16_t last = entry->last_reported_us;
        int32_t diff = (int32_t)current_us - (int32_t)last;
        if (diff < 0) diff = -diff;

        if (last == 0 || diff >= RC_CALLBACK_NOISE_US) {
            entry->last_reported_us = current_us;
            entry->sub.callback(current_us, entry->sub.user_data);
        }
    }
}
