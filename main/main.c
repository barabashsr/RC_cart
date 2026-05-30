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

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "main";

static bool s_failsafe_active = false;
static bool s_estop_was_active = false;

static uint16_t map_to_percent(uint16_t pulse, uint16_t min_us, uint16_t max_us)
{
    if (pulse <= min_us) return 0;
    if (pulse >= max_us) return 100;
    return (uint16_t)((pulse - min_us) * 100UL / (max_us - min_us));
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
                steering_stop();
                servo_failsafe();
                engine_kill();
            }
        } else {
            s_failsafe_active = false;
        }

        /* Engine state machine tick */
        engine_update();

        /* LED status */
        static int debug_tick = 0;
        static int led_toggle = 0;
        led_toggle = (led_toggle + 1) % 50;
        debug_tick++;

        gpio_set_level(GPIO_LED_STATUS,
            (!signal_ok && (led_toggle < 25)) ||
            (signal_ok && engine_get_state() == ENGINE_RUNNING)
                ? LED_ACTIVE_LEVEL : !LED_ACTIVE_LEVEL);

        /* Periodic debug dump (every ~1s = 200 cycles × 5ms) */
        if (debug_tick >= 200) {
            debug_tick = 0;
            uint16_t ch1, ch3, ch6;
            bool ok1 = rc_input_get_pulse(RC_IDX_STEERING, &ch1);
            bool ok3 = rc_input_get_pulse(RC_IDX_THROTTLE_BRAKE, &ch3);
            bool ok6 = rc_input_get_pulse(RC_IDX_STARTER, &ch6);
            ESP_LOGI(TAG, "RC Ch1=%u Ch3=%u Ch6=%u | signal=%s engine=%d",
                     ok1 ? ch1 : 0, ok3 ? ch3 : 0, ok6 ? ch6 : 0,
                     signal_ok ? "OK" : "LOST", engine_get_state());
        }
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
                steering_stop();
                servo_failsafe();
                engine_kill();
            }
        } else {
            s_estop_was_active = false;
        }

        if (event == SAFETY_SIGNAL_LOST && !s_failsafe_active) {
            s_failsafe_active = true;
            steering_stop();
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
        ds.rc_throttle_brake_us = servo_get_combined_pulse_us();
        ds.rc_signal_ok = rc_input_is_signal_ok();

        ds.steering_angle_deg = steering_get_angle_deg();

        /* Compute throttle/brake pct from combined stick value */
        uint16_t tb = ds.rc_throttle_brake_us;
        if (tb > RC_CENTER_US + RC_DEADBAND_STICK_US) {
            ds.throttle_pct = map_to_percent(tb, RC_CENTER_US, RC_MAX_VALID_US);
            ds.brake_pct = 0;
        } else if (tb < RC_CENTER_US - RC_DEADBAND_STICK_US) {
            ds.throttle_pct = 0;
            ds.brake_pct = map_to_percent(tb, RC_MIN_VALID_US, RC_CENTER_US);
        } else {
            ds.throttle_pct = 0;
            ds.brake_pct = 0;
        }

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
            ds.rc_throttle_brake_us > RC_CENTER_US + 100 || ds.rc_throttle_brake_us < RC_CENTER_US - 100) {
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

    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << GPIO_LED_STATUS) | (1ULL << GPIO_LIGHTS_RELAY),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_conf);
    gpio_set_level(GPIO_LED_STATUS, !LED_ACTIVE_LEVEL);
    gpio_set_level(GPIO_LIGHTS_RELAY, 0);

    ESP_LOGI(TAG, "Initializing RC input (6 channels)...");
    ESP_ERROR_CHECK(rc_input_init());

    ESP_LOGI(TAG, "Calibrating steering center (set wheels straight)...");
    uint16_t center_us = RC_CENTER_US;
    if (rc_input_calibrate_center(&center_us) == ESP_OK) {
        ESP_LOGI(TAG, "Steering center: %u us", center_us);
        steering_set_center_us(center_us);
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
