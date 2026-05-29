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
    uint16_t rc_throttle_us;
    uint16_t rc_brake_us;
    bool rc_signal_ok;

    float steering_angle_deg;
    float steering_target_deg;

    uint16_t throttle_pct;
    uint16_t brake_pct;

    uint16_t rpm;
    bool engine_running;

    bool estop_active;
    bool failsafe_active;

    const char *engine_state_str;
} display_state_t;

/**
 * @brief Initialize SH1106 OLED display (I2C)
 * @return ESP_OK on success, or ESP_ERR_NOT_SUPPORTED if CFG_ENABLE_OLED=0
 */
esp_err_t display_init(void);

/**
 * @brief Update display content (call at ~10Hz)
 * @param state Current system state to render
 */
void display_update(const display_state_t *state);

/**
 * @brief Turn display on/off (power saving)
 */
void display_set_power(bool on);

/**
 * @brief Notify display of user activity (resets sleep timer)
 */
void display_notify_activity(void);

#ifdef __cplusplus
}
#endif
