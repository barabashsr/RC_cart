#pragma once

#include "cart_config.h"
#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t servo_init(void);
void servo_set_throttle(uint16_t pulse_us);
void servo_set_brake(uint16_t pulse_us);
void servo_failsafe(void);
void servo_disable(void);

/**
 * @brief Get last known combined-stick pulse (for OLED display)
 */
uint16_t servo_get_combined_pulse_us(void);

#ifdef __cplusplus
}
#endif
