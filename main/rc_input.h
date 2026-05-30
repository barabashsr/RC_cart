#pragma once

#include "cart_config.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Signal state ---- */

typedef enum {
    RC_SIGNAL_OK = 0,
    RC_SIGNAL_LOST,
    RC_SIGNAL_RECOVERING,
} rc_signal_state_t;

typedef struct {
    uint16_t pulse_us;
    bool valid;
    int64_t last_update_us;
} rc_channel_t;

/* ---- Callback subscription system ---- */

typedef void (*rc_channel_callback_t)(uint16_t pulse_us, void *user_data);

typedef struct {
    int channel;                    /* 0-based index (Ch1=0 ... Ch6=5) */
    rc_channel_callback_t callback;
    void *user_data;
    uint16_t deadband_us;           /* Min change to fire callback (0 = every cycle) */
} rc_channel_subscription_t;

/* ---- Public API ---- */

esp_err_t rc_input_init(void);

bool rc_input_get_pulse(int channel, uint16_t *pulse_us);
bool rc_input_is_discrete_on(int channel);

rc_signal_state_t rc_input_get_state(void);
bool rc_input_is_signal_ok(void);
int64_t rc_input_get_last_valid_us(void);

esp_err_t rc_input_calibrate_center(uint16_t *center_us_out);

/**
 * @brief Subscribe to an RC channel. Fires callback each control cycle
 *        when the channel value changes by more than deadband_us.
 * @return ESP_OK on success, ESP_ERR_NO_MEM if subscription table is full.
 */
esp_err_t rc_input_subscribe(const rc_channel_subscription_t *sub);

/**
 * @brief Process all subscriptions (call from control_task every 5ms).
 *        Iterates all channels, compares to last-known value, fires callbacks
 *        for any that exceeded their deadband.
 */
void rc_input_process_callbacks(void);

#ifdef __cplusplus
}
#endif
