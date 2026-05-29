#include "rc_input.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <string.h>
#include <stdatomic.h>

static const char *TAG = "rc_input";

static rmt_channel_handle_t s_rx_channels[RC_NUM_CHANNELS];
static rc_channel_t s_channels[RC_NUM_CHANNELS];
static _Atomic rc_signal_state_t s_signal_state;
static _Atomic int64_t s_last_valid_us;
static _Atomic bool s_initialized;

static const gpio_num_t s_ch_gpios[RC_NUM_CHANNELS] = {
    GPIO_RC_CH1,
    GPIO_RC_CH2,
    GPIO_RC_CH3,
    GPIO_RC_CH4,
    GPIO_RC_CH5,
};

static bool IRAM_ATTR rc_rx_done_callback(rmt_channel_handle_t channel,
                                           const rmt_rx_done_event_data_t *event_data,
                                           void *user_data)
{
    int ch_idx = (int)(intptr_t)user_data;
    rmt_symbol_word_t *symbols = (rmt_symbol_word_t *)event_data->received_symbols;
    size_t num_symbols = event_data->num_symbols;

    for (size_t i = 0; i < num_symbols; i++) {
        /* duration1 = high time, duration0 = low time.
         * For standard RC PWM: one symbol per frame.
         * duration1 is the pulse width in RMT ticks.
         * 1 us per tick at 1 MHz resolution. */
        uint32_t high_ticks = symbols[i].duration1;
        uint16_t pulse_us = (uint16_t)(high_ticks);

        if (pulse_us >= RC_MIN_VALID_US && pulse_us <= RC_MAX_VALID_US) {
            s_channels[ch_idx].pulse_us = pulse_us;
            s_channels[ch_idx].valid = true;
            s_channels[ch_idx].last_update_us = esp_timer_get_time();
            atomic_store(&s_last_valid_us, s_channels[ch_idx].last_update_us);
            atomic_store(&s_signal_state, RC_SIGNAL_OK);
        }
    }

    return false;
}

esp_err_t rc_input_init(void)
{
    if (atomic_load(&s_initialized)) {
        return ESP_OK;
    }

    memset(s_channels, 0, sizeof(s_channels));
    atomic_store(&s_signal_state, RC_SIGNAL_LOST);
    atomic_store(&s_last_valid_us, 0);

    for (int i = 0; i < RC_NUM_CHANNELS; i++) {
        rmt_rx_channel_config_t rx_config = {
            .gpio_num = s_ch_gpios[i],
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = RMT_RX_RESOLUTION_HZ,
            .mem_block_symbols = 64,
            .flags = {
                .invert_in = false,
                .with_dma = false,
                .io_loop_back = false,
            },
        };

        esp_err_t ret = rmt_new_rx_channel(&rx_config, &s_rx_channels[i]);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to create RMT RX ch%d: %s", i, esp_err_to_name(ret));
            return ret;
        }

        rmt_rx_event_callbacks_t cbs = {
            .on_recv_done = rc_rx_done_callback,
        };
        ret = rmt_rx_register_event_callbacks(s_rx_channels[i], &cbs, (void *)(intptr_t)i);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to register RX callback ch%d: %s", i, esp_err_to_name(ret));
            return ret;
        }

        ret = rmt_enable(s_rx_channels[i]);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to enable RMT RX ch%d: %s", i, esp_err_to_name(ret));
            return ret;
        }

        /* Start receiving — buffer for one RMT symbol */
        rmt_symbol_word_t rx_symbol;
        rmt_receive_config_t recv_config = {
            .signal_range_min_ns = RMT_RX_MIN_NS,
            .signal_range_max_ns = RMT_RX_MAX_NS,
        };
        ret = rmt_receive(s_rx_channels[i], &rx_symbol, sizeof(rx_symbol), &recv_config);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to start receive ch%d: %s", i, esp_err_to_name(ret));
            return ret;
        }

        ESP_LOGI(TAG, "RC input ch%d initialized on GPIO %d", i, s_ch_gpios[i]);
    }

    atomic_store(&s_initialized, true);
    ESP_LOGI(TAG, "RC input initialized (%d channels)", RC_NUM_CHANNELS);
    return ESP_OK;
}

bool rc_input_get_pulse(int channel, uint16_t *pulse_us)
{
    if (channel < 0 || channel >= RC_NUM_CHANNELS) {
        return false;
    }

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
    if (channel < 0 || channel >= RC_NUM_CHANNELS) {
        return false;
    }

    uint16_t pulse = 0;
    if (!rc_input_get_pulse(channel, &pulse)) {
        return false;
    }

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
