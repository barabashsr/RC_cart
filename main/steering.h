#pragma once

#include "cart_config.h"
#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    STEERING_IDLE = 0,
    STEERING_MOVING,
    STEERING_AT_LIMIT_LEFT,
    STEERING_AT_LIMIT_RIGHT,
    STEERING_ERROR,
} steering_state_t;

esp_err_t steering_init(void);
void steering_set_center_us(uint16_t center_us);
void steering_update(uint16_t rc_pulse_us);
void steering_stop(void);
void steering_enable(void);
void steering_disable(void);
float steering_get_angle_deg(void);
steering_state_t steering_get_state(void);
bool steering_at_limit(void);

#ifdef __cplusplus
}
#endif
