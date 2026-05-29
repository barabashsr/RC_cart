#pragma once

#include "cart_config.h"
#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SAFETY_OK = 0,
    SAFETY_ESTOP_ACTIVE,
    SAFETY_SIGNAL_LOST,
    SAFETY_LIMIT_TRIGGERED,
} safety_event_t;

typedef void (*safety_callback_t)(safety_event_t event);

/**
 * @brief Initialize safety monitoring (E-stop ISR, watchdog)
 * @return ESP_OK on success
 */
esp_err_t safety_init(void);

/**
 * @brief Run safety check (call every SAFETY_CHECK_INTERVAL_MS)
 * Checks: E-stop state, RC signal, limits
 * @return Current safety event
 */
safety_event_t safety_check(void);

/**
 * @brief Check if E-stop is currently pressed
 */
bool safety_is_estop_active(void);

/**
 * @brief Check if any safety violation is active
 */
bool safety_is_ok(void);

/**
 * @brief Register callback for safety events
 */
void safety_set_callback(safety_callback_t cb);

#ifdef __cplusplus
}
#endif
