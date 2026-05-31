/**
 * @file main.c
 * @brief RC Cart Controller — ESP32-S3 firmware entry point
 *
 * Architecture (callback-driven):
 *   Each function module subscribes to its RC channel via rc_input_subscribe().
 *   rc_input_process_callbacks() fires callbacks from control_task at 5ms.
 *   Adding a new function = subscribe + callback handler — zero changes here.
 *
 * Task layout:
 *   Core 0: control_task  — rc_input_process_callbacks() + engine_update() + failsafe
 *   Core 0: safety_task   — E-stop, signal loss monitoring
 *   Core 1: display_task  — OLED refresh
 *   Core 1: ramp_task     — RMT step pulse generator (spawned internally)
 */

#include "cart_config.h"
#include "rc_input.h"
#include "servo.h"
#include "steering.h"
#include "engine.h"
#include "safety.h"
#include "display.h"

#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "main";

static bool s_failsafe_active = false;
static bool s_estop_was_active = false;
static rmt_channel_handle_t s_rgb_chan = NULL;
static rmt_encoder_handle_t s_rgb_encoder = NULL;

/* ---- WS2812 RGB LED helper ---- */
static esp_err_t rgb_led_init(void)
{
    rmt_tx_channel_config_t tx_cfg = {
        .gpio_num = GPIO_RGB_LED,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10000000,  /* 10 MHz = 100 ns per tick */
        .mem_block_symbols = 64,
        .trans_queue_depth = 4,
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_cfg, &s_rgb_chan));

    rmt_bytes_encoder_config_t enc_cfg = {
        .bit0 = { .duration0 = 4, .level0 = 1, .duration1 = 8, .level1 = 0 },  /* 400ns H, 800ns L */
        .bit1 = { .duration0 = 8, .level0 = 1, .duration1 = 4, .level1 = 0 },  /* 800ns H, 400ns L */
        .flags.msb_first = 1,
    };
    ESP_ERROR_CHECK(rmt_new_bytes_encoder(&enc_cfg, &s_rgb_encoder));
    ESP_ERROR_CHECK(rmt_enable(s_rgb_chan));
    return ESP_OK;
}

static void rgb_led_set(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t grb[3] = { g, r, b };
    rmt_transmit_config_t tx_cfg = { .loop_count = 0 };
    rmt_transmit(s_rgb_chan, s_rgb_encoder, grb, 3, &tx_cfg);
    rmt_tx_wait_all_done(s_rgb_chan, 10);
}

/* ---- RGB LED status patterns ---- */
static void rgb_led_update(bool signal_ok, bool estop, bool engine_running, bool failsafe)
{
    if (estop) {
        rgb_led_set(255, 0, 0);    /* solid red = E-stop */
    } else if (failsafe || !signal_ok) {
        static int blink = 0;
        blink = (blink + 1) % 100;
        rgb_led_set(blink < 50 ? 255 : 0, 0, 0);  /* fast blink red */
    } else if (engine_running) {
        rgb_led_set(0, 255, 0);    /* green = running */
    } else {
        rgb_led_set(0, 0, 255);    /* blue = armed, signal OK */
    }
}

/* ---- Control Task (Core 0, 5ms) ---- */
static void control_task(void *arg)
{
    (void)arg;
    TickType_t period = pdMS_TO_TICKS(TASK_PERIOD_CONTROL_MS);
    TickType_t last_wake = xTaskGetTickCount();

    ESP_LOGI(TAG, "Control task on Core %d", xPortGetCoreID());

    while (1) {
        vTaskDelayUntil(&last_wake, period);

        /* Fire all RC channel subscriptions */
        rc_input_process_callbacks();

        /* Check signal and handle failsafe */
        bool signal_ok = rc_input_is_signal_ok();
        if (!signal_ok) {
            if (!s_failsafe_active) {
                ESP_LOGW(TAG, "SIGNAL LOST — entering failsafe");
                s_failsafe_active = true;
                steering_disable();
                servo_failsafe();
                engine_kill();
            }
        } else {
            if (s_failsafe_active) {
                s_failsafe_active = false;
                steering_enable();
                ESP_LOGI(TAG, "Signal recovered — steering re-enabled");
            }
        }

        /* Engine state machine tick */
        engine_update();

        /* RGB LED status */
        rgb_led_update(signal_ok, s_estop_was_active,
                       engine_get_state() == ENGINE_RUNNING, s_failsafe_active);
    }
}

/* ---- Safety Task (Core 0, 5ms) ---- */
static void safety_task(void *arg)
{
    (void)arg;
    TickType_t period = pdMS_TO_TICKS(TASK_PERIOD_SAFETY_MS);
    TickType_t last_wake = xTaskGetTickCount();

    ESP_LOGI(TAG, "Safety task on Core %d", xPortGetCoreID());

    while (1) {
        vTaskDelayUntil(&last_wake, period);

        safety_event_t event = safety_check();

        if (event == SAFETY_ESTOP_ACTIVE) {
            if (!s_estop_was_active) {
                s_estop_was_active = true;
                ESP_LOGW(TAG, "E-STOP ACTIVE");
                steering_disable();
                servo_failsafe();
                engine_kill();
            }
        } else {
            s_estop_was_active = false;
        }

        if (event == SAFETY_SIGNAL_LOST && !s_failsafe_active) {
            s_failsafe_active = true;
            steering_disable();
            servo_failsafe();
            engine_kill();
        }
    }
}

/* ---- Display Task (Core 1, 100ms) ---- */
static void display_task(void *arg)
{
    (void)arg;
    TickType_t period = pdMS_TO_TICKS(TASK_PERIOD_DISPLAY_MS);
    TickType_t last_wake = xTaskGetTickCount();

    ESP_LOGI(TAG, "Display task on Core %d", xPortGetCoreID());

    while (1) {
        vTaskDelayUntil(&last_wake, period);

        display_state_t ds;
        memset(&ds, 0, sizeof(ds));

        rc_input_get_pulse(RC_IDX_STEERING, &ds.rc_steer_us);
        ds.rc_signal_ok = rc_input_is_signal_ok();

        ds.steering_angle_deg = steering_get_angle_deg();
        ds.throttle_pct = servo_get_throttle_pct();
        ds.brake_pct = servo_get_brake_pct();

        ds.rpm = engine_get_rpm();
        ds.engine_running = (engine_get_state() == ENGINE_RUNNING);
        ds.estop_active = safety_is_estop_active();
        ds.failsafe_active = s_failsafe_active;

        switch (engine_get_state()) {
            case ENGINE_OFF:       ds.engine_state_str = "OFF"; break;
            case ENGINE_CRANKING:  ds.engine_state_str = "CRNK"; break;
            case ENGINE_RUNNING:   ds.engine_state_str = "RUN"; break;
            case ENGINE_COOLDOWN:  ds.engine_state_str = "COOL"; break;
            case ENGINE_STALLED:   ds.engine_state_str = "STAL"; break;
            case ENGINE_ERROR:     ds.engine_state_str = "ERR"; break;
            default:               ds.engine_state_str = "???"; break;
        }

        display_update(&ds);

        if (ds.rc_steer_us > RC_CENTER_US + 100 || ds.rc_steer_us < RC_CENTER_US - 100 ||
            ds.throttle_pct > 5 || ds.brake_pct > 5) {
            display_notify_activity();
        }
    }
}

/* ---- app_main ---- */
void app_main(void)
{
    ESP_LOGI(TAG, "============================================");
    ESP_LOGI(TAG, "  %s v%s", FIRMWARE_NAME, FIRMWARE_VERSION);
    ESP_LOGI(TAG, "  Target: ESP32-S3");
    ESP_LOGI(TAG, "============================================");

    /* GPIO outputs (lights relay only — RGB LED uses RMT) */
    gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << GPIO_LIGHTS_RELAY),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);
    gpio_set_level(GPIO_LIGHTS_RELAY, 0);

    ESP_LOGI(TAG, "Initializing RGB LED...");
    ESP_ERROR_CHECK(rgb_led_init());
    rgb_led_set(0, 0, 128);  /* dim blue = booting */

    ESP_LOGI(TAG, "Initializing RC input (6 channels)...");
    ESP_ERROR_CHECK(rc_input_init());

    ESP_LOGI(TAG, "Calibrating steering center (set wheels straight)...");
    uint16_t center_us = RC_CENTER_US;
    if (rc_input_calibrate_center(&center_us) == ESP_OK) {
        ESP_LOGI(TAG, "Steering center: %u us", center_us);
        steering_set_center_us(center_us);
        servo_set_center_us(center_us);
    } else {
        ESP_LOGW(TAG, "Calibration failed — default %u us", center_us);
    }

    ESP_LOGI(TAG, "Initializing servos...");
    ESP_ERROR_CHECK(servo_init());

    ESP_LOGI(TAG, "Initializing steering...");
    ESP_ERROR_CHECK(steering_init());

    ESP_LOGI(TAG, "Initializing engine control...");
    ESP_ERROR_CHECK(engine_init());

    ESP_LOGI(TAG, "Initializing safety monitor...");
    ESP_ERROR_CHECK(safety_init());

    ESP_LOGI(TAG, "Initializing display...");
    if (display_init() != ESP_OK) {
        ESP_LOGW(TAG, "Display not found — continuing without OLED");
    }

    xTaskCreatePinnedToCore(control_task, "control", TASK_STACK_CONTROL,
                            NULL, TASK_PRIO_CONTROL, NULL, CORE_CONTROL);
    xTaskCreatePinnedToCore(safety_task, "safety", TASK_STACK_SAFETY,
                            NULL, TASK_PRIO_SAFETY, NULL, CORE_CONTROL);
    xTaskCreatePinnedToCore(display_task, "display", TASK_STACK_DISPLAY,
                            NULL, TASK_PRIO_DISPLAY, NULL, CORE_DISPLAY);

    ESP_LOGI(TAG, "All tasks started. SYSTEM ARMED.");
}
