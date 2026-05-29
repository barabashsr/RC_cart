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

/**
 * @brief Initialize steering motor (RMT pulse generator for step/dir)
 * @return ESP_OK on success
 */
esp_err_t steering_init(void);

/**
 * @brief Set target steering angle from RC stick pulse
 * @param rc_pulse_us RC steering channel pulse width (1000-2000us typically)
 * Maps RC pulse to physical steering angle via config params.
 */
void steering_update(uint16_t rc_pulse_us);

/**
 * @brief Immediately stop steering motion
 */
void steering_stop(void);

/**
 * @brief Get current steering angle in degrees (positive = right)
 * @return Current angle
 */
float steering_get_angle_deg(void);

/**
 * @brief Get steering state
 */
steering_state_t steering_get_state(void);

/**
 * @brief Check if limit switch is triggered
 * @return true if at limit
 */
bool steering_at_limit(void);

#ifdef __cplusplus
}
#endif
