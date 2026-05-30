#pragma once

#include "cart_config.h"
#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t rc_steer_us;
    uint16_t rc_throttle_brake_us;
    bool rc_signal_ok;

    float steering_angle_deg;

    uint16_t throttle_pct;
    uint16_t brake_pct;

    uint16_t rpm;
    bool engine_running;

    bool estop_active;
    bool failsafe_active;

    const char *engine_state_str;
} display_state_t;

esp_err_t display_init(void);
void display_update(const display_state_t *state);
void display_set_power(bool on);
void display_notify_activity(void);

#ifdef __cplusplus
}
#endif
