#pragma once

#include "cart_config.h"
#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ENGINE_OFF = 0,
    ENGINE_CRANKING,
    ENGINE_RUNNING,
    ENGINE_COOLDOWN,
    ENGINE_STALLED,
    ENGINE_ERROR,
} engine_state_t;

/**
 * @brief Initialize engine control (starter relay, ignition kill, RPM sensor)
 * @return ESP_OK on success
 */
esp_err_t engine_init(void);

/**
 * @brief Update engine state machine (call every control cycle)
 * @param start_request true if RC switch requests starting
 */
void engine_update(bool start_request);

/**
 * @brief Get current engine state
 */
engine_state_t engine_get_state(void);

/**
 * @brief Get current RPM (valid only if CFG_ENABLE_RPM=1)
 * @return RPM value, 0 if sensor disabled or engine off
 */
uint16_t engine_get_rpm(void);

/**
 * @brief Emergency kill engine immediately
 */
void engine_kill(void);

#ifdef __cplusplus
}
#endif
