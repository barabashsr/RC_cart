#pragma once

#include "cart_config.h"
#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Result of 2-axis calibration */
typedef struct {
    uint16_t ch1_min_us;
    uint16_t ch1_center_us;
    uint16_t ch1_max_us;
    uint16_t ch3_min_us;
    uint16_t ch3_center_us;
    uint16_t ch3_max_us;
    bool from_nvs;
} calibration_result_t;

/**
 * Run interactive calibration on OLED, sampling Ch1 and Ch3.
 * Saves to NVS. Falls back to NVS if no stick movement detected.
 * Returns ESP_OK on success.
 */
esp_err_t calibration_run(calibration_result_t *result);

#ifdef __cplusplus
}
#endif
