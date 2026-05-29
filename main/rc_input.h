#pragma once

#include "cart_config.h"
#include "driver/rmt_rx.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * @brief Initialize RC input (RMT RX channels for each RC channel)
 * @return ESP_OK on success
 */
esp_err_t rc_input_init(void);

/**
 * @brief Get latest pulse width for a channel
 * @param channel Channel index (0-based)
 * @param[out] pulse_us Latest pulse width in microseconds
 * @return true if valid data available
 */
bool rc_input_get_pulse(int channel, uint16_t *pulse_us);

/**
 * @brief Check if a discrete channel is in ON position
 * @param channel Channel index
 * @return true if switch is ON (>RC_DISCRETE_ON_THRESHOLD_US)
 */
bool rc_input_is_discrete_on(int channel);

/**
 * @brief Get overall signal state
 * @return Current signal state
 */
rc_signal_state_t rc_input_get_state(void);

/**
 * @brief Check if RC signal is currently valid (all channels have recent pulses)
 * @return true if signal is OK
 */
bool rc_input_is_signal_ok(void);

/**
 * @brief Get timestamp of last valid signal (used for failsafe timeout)
 * @return Timestamp in microseconds
 */
int64_t rc_input_get_last_valid_us(void);

#ifdef __cplusplus
}
#endif
