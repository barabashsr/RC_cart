/**
 * @file pcnt_tracker.h
 * @brief PCNT-based position tracker for stepper motor
 *
 * Uses ESP32-P4 PCNT peripheral for hardware pulse counting.
 * Extends 16-bit counter to int64_t via overflow ISR handling.
 * Input signal comes from RMT step output via internal GPIO loopback.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque PCNT position tracker handle
 */
typedef struct pcnt_tracker pcnt_tracker_t;

/**
 * @brief Create a PCNT position tracker
 *
 * @param[out] tracker Pointer to store created tracker handle
 * @param pulse_gpio GPIO pin for pulse input (should match RMT step output)
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t pcnt_tracker_create(pcnt_tracker_t** tracker, gpio_num_t pulse_gpio);

/**
 * @brief Destroy a PCNT position tracker
 *
 * @param tracker Tracker handle to destroy
 */
void pcnt_tracker_destroy(pcnt_tracker_t* tracker);

/**
 * @brief Initialize the PCNT hardware
 *
 * Configures PCNT unit, channel, and overflow watch points.
 * Must be called before using other tracker functions.
 *
 * @param tracker Tracker handle
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t pcnt_tracker_init(pcnt_tracker_t* tracker);

/**
 * @brief Reset position to specified value
 *
 * Atomically sets the position counter to the specified value.
 *
 * @param tracker Tracker handle
 * @param position New position value (default 0)
 * @return ESP_OK on success
 */
esp_err_t pcnt_tracker_reset(pcnt_tracker_t* tracker, int64_t position);

/**
 * @brief Get current position in pulses
 *
 * Thread-safe read of current position. Combines hardware counter
 * with overflow accumulator.
 *
 * @param tracker Tracker handle
 * @return Current position in pulses (may be negative)
 */
int64_t pcnt_tracker_get_position(const pcnt_tracker_t* tracker);

/**
 * @brief Set counting direction
 *
 * Configures whether pulses increment or decrement the position.
 * Must be called BEFORE motion starts (during direction setup time).
 *
 * @param tracker Tracker handle
 * @param forward true = pulses increment position, false = decrement
 */
void pcnt_tracker_set_direction(pcnt_tracker_t* tracker, bool forward);

/**
 * @brief Check if tracker is initialized
 *
 * @param tracker Tracker handle
 * @return true if init() succeeded
 */
bool pcnt_tracker_is_initialized(const pcnt_tracker_t* tracker);

#ifdef __cplusplus
}
#endif
