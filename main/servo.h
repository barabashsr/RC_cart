#pragma once

#include "cart_config.h"
#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize servo outputs (LEDC for throttle & brake)
 * @return ESP_OK on success
 */
esp_err_t servo_init(void);

/**
 * @brief Set throttle servo position
 * @param pulse_us Pulse width in microseconds (typically 1000-2000)
 */
void servo_set_throttle(uint16_t pulse_us);

/**
 * @brief Set brake servo position
 * @param pulse_us Pulse width in microseconds (typically 1000-2000)
 */
void servo_set_brake(uint16_t pulse_us);

/**
 * @brief Apply failsafe positions immediately
 */
void servo_failsafe(void);

/**
 * @brief Disable both servos (0 pulse — coast)
 */
void servo_disable(void);

#ifdef __cplusplus
}
#endif
