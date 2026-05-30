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

esp_err_t engine_init(void);
void engine_update(void);
engine_state_t engine_get_state(void);
uint16_t engine_get_rpm(void);
void engine_kill(void);

#ifdef __cplusplus
}
#endif
