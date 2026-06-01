#include "calibration.h"
#include "rc_input.h"
#include "display.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>

static const char *TAG = "calib";

static bool sample_axis(int channel, const char *label,
                        uint16_t *min_out, uint16_t *center_out, uint16_t *max_out)
{
    uint32_t sum = 0;
    uint32_t count = 0;
    uint16_t vmin = 3000;
    uint16_t vmax = 500;

    int segments = 20;
    int64_t seg_us = (int64_t)CALIBRATE_AXIS_DURATION_MS * 1000 / segments;

    for (int seg = 0; seg < segments; seg++) {
        int64_t seg_end = esp_timer_get_time() + seg_us;

        while (esp_timer_get_time() < seg_end) {
            uint16_t pulse;
            if (rc_input_get_pulse(channel, &pulse)) {
                if (pulse < vmin) vmin = pulse;
                if (pulse > vmax) vmax = pulse;
                sum += pulse;
                count++;
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        display_show_calibration(channel == RC_IDX_STEERING ? "STEER <<  >>" : "THR   ^   v ",
                                 label, seg + 1, segments - seg - 1);
    }

    if (count == 0) {
        ESP_LOGW(TAG, "Ch%d: no pulses received", channel + 1);
        return false;
    }

    *min_out = vmin;
    *max_out = vmax;
    *center_out = (uint16_t)(sum / count);

    ESP_LOGI(TAG, "Ch%d: min=%u center=%u max=%u (%" PRIu32 " samples)",
             channel + 1, *min_out, *center_out, *max_out, count);
    return true;
}

static bool stick_moved(uint16_t min, uint16_t max, uint16_t def_min, uint16_t def_max)
{
    int32_t range = (int32_t)max - (int32_t)min;
    int32_t def_range = (int32_t)def_max - (int32_t)def_min;
    return range > (def_range / 4);
}

static esp_err_t nvs_save(const calibration_result_t *r)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(CALIBRATE_NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    nvs_set_u16(handle, "ch1_min", r->ch1_min_us);
    nvs_set_u16(handle, "ch1_max", r->ch1_max_us);
    nvs_set_u16(handle, "ch1_ctr", r->ch1_center_us);
    nvs_set_u16(handle, "ch3_min", r->ch3_min_us);
    nvs_set_u16(handle, "ch3_max", r->ch3_max_us);
    nvs_set_u16(handle, "ch3_ctr", r->ch3_center_us);
    nvs_set_u8(handle, "stored", 0xAB);
    nvs_commit(handle);
    nvs_close(handle);
    return ESP_OK;
}

static bool nvs_load(calibration_result_t *r)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(CALIBRATE_NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return false;

    uint8_t marker = 0;
    nvs_get_u8(handle, "stored", &marker);
    if (marker != 0xAB) {
        nvs_close(handle);
        return false;
    }

    nvs_get_u16(handle, "ch1_min", &r->ch1_min_us);
    nvs_get_u16(handle, "ch1_max", &r->ch1_max_us);
    nvs_get_u16(handle, "ch1_ctr", &r->ch1_center_us);
    nvs_get_u16(handle, "ch3_min", &r->ch3_min_us);
    nvs_get_u16(handle, "ch3_max", &r->ch3_max_us);
    nvs_get_u16(handle, "ch3_ctr", &r->ch3_center_us);
    nvs_close(handle);

    ESP_LOGI(TAG, "Loaded from NVS: Ch1 %u-%u (ctr %u)  Ch3 %u-%u (ctr %u)",
             r->ch1_min_us, r->ch1_max_us, r->ch1_center_us,
             r->ch3_min_us, r->ch3_max_us, r->ch3_center_us);
    return true;
}

esp_err_t calibration_run(calibration_result_t *result)
{
    memset(result, 0, sizeof(*result));

    /* Default fallback ranges */
    const uint16_t def_ch1_min = RC_MIN_VALID_US;
    const uint16_t def_ch1_max = RC_MAX_VALID_US;
    const uint16_t def_ch1_ctr = RC_CENTER_US;
    const uint16_t def_ch3_min = RC_MIN_VALID_US;
    const uint16_t def_ch3_max = RC_MAX_VALID_US;
    const uint16_t def_ch3_ctr = RC_CENTER_US;

    /* Sample Ch1 (steering) */
    display_show_calibration("STEER <<  >>", "MOVE now...   ", 0, 20);
    vTaskDelay(pdMS_TO_TICKS(800));

    uint16_t ch1_min, ch1_ctr, ch1_max;
    bool ch1_ok = sample_axis(RC_IDX_STEERING, "STEER <<  >>", &ch1_min, &ch1_ctr, &ch1_max);
    bool ch1_moved = ch1_ok && stick_moved(ch1_min, ch1_max, def_ch1_min, def_ch1_max);

    /* Sample Ch3 (throttle/brake) */
    display_show_calibration("THR   ^   v ", "MOVE now...   ", 0, 20);
    vTaskDelay(pdMS_TO_TICKS(800));

    uint16_t ch3_min, ch3_ctr, ch3_max;
    bool ch3_ok = sample_axis(RC_IDX_THROTTLE_BRAKE, "THR   ^   v ", &ch3_min, &ch3_ctr, &ch3_max);
    bool ch3_moved = ch3_ok && stick_moved(ch3_min, ch3_max, def_ch3_min, def_ch3_max);

    /* Decide: use measured, NVS, or defaults */
    if (ch1_moved && ch3_moved) {
        result->ch1_min_us = ch1_min;
        result->ch1_center_us = ch1_ctr;
        result->ch1_max_us = ch1_max;
        result->ch3_min_us = ch3_min;
        result->ch3_center_us = ch3_ctr;
        result->ch3_max_us = ch3_max;
        result->from_nvs = false;

        display_show_calibration("CALIBRATION", "SAVING...", 20, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        esp_err_t err = nvs_save(result);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "NVS save failed: %s", esp_err_to_name(err));
        }
    } else if (nvs_load(result)) {
        result->from_nvs = true;
    } else {
        ESP_LOGW(TAG, "No movement detected and no NVS data — using defaults");
        result->ch1_min_us = def_ch1_min;
        result->ch1_center_us = def_ch1_ctr;
        result->ch1_max_us = def_ch1_max;
        result->ch3_min_us = def_ch3_min;
        result->ch3_center_us = def_ch3_ctr;
        result->ch3_max_us = def_ch3_max;
        result->from_nvs = false;
    }

    ESP_LOGI(TAG, "Ch1: %u-%u (ctr %u)  Ch3: %u-%u (ctr %u)  from_nvs=%d",
             result->ch1_min_us, result->ch1_max_us, result->ch1_center_us,
             result->ch3_min_us, result->ch3_max_us, result->ch3_center_us,
             result->from_nvs);
    return ESP_OK;
}
