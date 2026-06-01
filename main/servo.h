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
void servo_update(void);
void servo_failsafe(void);
void servo_disable(void);

void servo_set_center_us(uint16_t center_us);
void servo_set_ch3_calibration(uint16_t min_us, uint16_t center_us, uint16_t max_us);
uint16_t servo_get_combined_pulse_us(void);
uint16_t servo_get_throttle_pct(void);
uint16_t servo_get_brake_pct(void);

#ifdef __cplusplus
}
#endif
