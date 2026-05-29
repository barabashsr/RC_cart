/**
 * @file main.c
 * @brief RC Cart Controller — ESP32-S3 firmware entry point
 *
 * Task layout:
 *   Core 0: control_task  — reads RC, updates servos & steering, engine logic
 *   Core 0: safety_task   — E-stop, signal loss monitoring
 *   Core 1: display_task  — OLED refresh
 *   Core 1: ramp_task     — RMT step pulse generator (spawned by rmt_pulse_gen)
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

/* ---- Helper: map RC pulse to percent ---- */
static uint16_t map_to_percent(uint16_t pulse, uint16_t min_us, uint16_t max_us)
{
    if (pulse <= min_us) return 0;
    if (pulse >= max_us) return 100;
    return (uint16_t)((pulse - min_us) * 100UL / (max_us - min_us));
}

/* ---- Control Task (Core 0) ---- */
static void control_task(void *arg)
{
    TickType_t period = pdMS_TO_TICKS(TASK_PERIOD_CONTROL_MS);
    TickType_t last_wake = xTaskGetTickCount();

    ESP_LOGI(TAG, "Control task started on Core %d", xPortGetCoreID());

    while (1) {
        vTaskDelayUntil(&last_wake, period);

        /* --- Read RC inputs --- */
        uint16_t steer_us = RC_CENTER_US;
        uint16_t throttle_us = THROTTLE_IDLE_US;
        uint16_t brake_us = BRAKE_RELEASED_US;
        bool start_req = false;
        bool lights_on = false;
        bool signal_ok = rc_input_is_signal_ok();

        if (signal_ok) {
            rc_input_get_pulse(RC_CHAN_STEERING, &steer_us);
            rc_input_get_pulse(RC_CHAN_THROTTLE, &throttle_us);
            rc_input_get_pulse(RC_CHAN_BRAKE, &brake_us);
            start_req = rc_input_is_discrete_on(RC_CHAN_ENGINE_START);
            lights_on = rc_input_is_discrete_on(RC_CHAN_LIGHTS);

            s_failsafe_active = false;

            /* --- Steering update --- */
            steering_update(steer_us);

            /* --- Servo update --- */
            uint16_t throttle_pulse = THROTTLE_IDLE_US;
            uint16_t brake_pulse = BRAKE_RELEASED_US;

            /* Throttle mapping: RC stick up = more gas */
            if (throttle_us > RC_CENTER_US + RC_STICK_DEADBAND_US) {
                throttle_pulse = throttle_us;
                brake_pulse = BRAKE_RELEASED_US;
            }
            /* Brake mapping: RC stick down = more brake */
            else if (brake_us > RC_CENTER_US + RC_STICK_DEADBAND_US) {
                throttle_pulse = THROTTLE_IDLE_US;
                brake_pulse = brake_us;
            }

            /* Clamp */
            if (throttle_pulse < THROTTLE_IDLE_US) throttle_pulse = THROTTLE_IDLE_US;
            if (throttle_pulse > THROTTLE_FULL_US) throttle_pulse = THROTTLE_FULL_US;
            if (brake_pulse < BRAKE_RELEASED_US) brake_pulse = BRAKE_RELEASED_US;
            if (brake_pulse > BRAKE_FULL_US) brake_pulse = BRAKE_FULL_US;

            servo_set_throttle(throttle_pulse);
            servo_set_brake(brake_pulse);

        } else {
            /* --- Failsafe --- */
            if (!s_failsafe_active) {
                ESP_LOGW(TAG, "SIGNAL LOST — entering failsafe");
                s_failsafe_active = true;
                steering_stop();
                servo_failsafe();
                engine_kill();
            }
        }

        /* --- Engine control --- */
        engine_update(start_req);

        /* --- Lights --- */
        gpio_set_level(GPIO_LIGHTS_RELAY, lights_on ? 1 : 0);

        /* --- LED status --- */
        static int led_toggle = 0;
        led_toggle = (led_toggle + 1) % 50;
        if (!signal_ok && (led_toggle < 25)) {
            gpio_set_level(GPIO_LED_STATUS, LED_ACTIVE_LEVEL);
        } else if (signal_ok && engine_get_state() == ENGINE_RUNNING) {
            gpio_set_level(GPIO_LED_STATUS, LED_ACTIVE_LEVEL);
        } else {
            gpio_set_level(GPIO_LED_STATUS, !LED_ACTIVE_LEVEL);
        }
    }
}

/* ---- Safety Task (Core 0) ---- */
static void safety_task(void *arg)
{
    TickType_t period = pdMS_TO_TICKS(TASK_PERIOD_SAFETY_MS);
    TickType_t last_wake = xTaskGetTickCount();

    ESP_LOGI(TAG, "Safety task started on Core %d", xPortGetCoreID());

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
                gpio_set_level(GPIO_LED_STATUS, 1);  /* Solid LED */
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

/* ---- Display Task (Core 1) ---- */
static void display_task(void *arg)
{
    TickType_t period = pdMS_TO_TICKS(TASK_PERIOD_DISPLAY_MS);
    TickType_t last_wake = xTaskGetTickCount();

    ESP_LOGI(TAG, "Display task started on Core %d", xPortGetCoreID());

    while (1) {
        vTaskDelayUntil(&last_wake, period);

        display_state_t ds;
        memset(&ds, 0, sizeof(ds));

        rc_input_get_pulse(RC_CHAN_STEERING, &ds.rc_steer_us);
        rc_input_get_pulse(RC_CHAN_THROTTLE, &ds.rc_throttle_us);
        rc_input_get_pulse(RC_CHAN_BRAKE, &ds.rc_brake_us);
        ds.rc_signal_ok = rc_input_is_signal_ok();

        ds.steering_angle_deg = steering_get_angle_deg();
        ds.throttle_pct = map_to_percent(ds.rc_throttle_us, RC_CENTER_US, RC_MAX_VALID_US);
        ds.brake_pct = map_to_percent(ds.rc_brake_us, RC_CENTER_US, RC_MAX_VALID_US);
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

        /* If RC stick is moving, notify display activity */
        if (ds.rc_steer_us > RC_CENTER_US + 100 || ds.rc_steer_us < RC_CENTER_US - 100 ||
            ds.rc_throttle_us > RC_CENTER_US + 100 || ds.rc_brake_us > RC_CENTER_US + 100) {
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

    /* GPIO init: LED and lights relay */
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

    /* Initialize subsystems */
    ESP_LOGI(TAG, "Initializing RC input...");
    ESP_ERROR_CHECK(rc_input_init());

    ESP_LOGI(TAG, "Initializing servos...");
    ESP_ERROR_CHECK(servo_init());

    ESP_LOGI(TAG, "Initializing steering...");
    ESP_ERROR_CHECK(steering_init());

    ESP_LOGI(TAG, "Initializing engine control...");
    ESP_ERROR_CHECK(engine_init());

    ESP_LOGI(TAG, "Initializing safety monitor...");
    ESP_ERROR_CHECK(safety_init());

    ESP_LOGI(TAG, "Initializing display...");
    ESP_ERROR_CHECK(display_init());

    /* Create tasks */
    xTaskCreatePinnedToCore(control_task, "control", TASK_STACK_CONTROL,
                            NULL, TASK_PRIO_CONTROL, NULL, CORE_CONTROL);
    xTaskCreatePinnedToCore(safety_task, "safety", TASK_STACK_SAFETY,
                            NULL, TASK_PRIO_SAFETY, NULL, CORE_CONTROL);
    xTaskCreatePinnedToCore(display_task, "display", TASK_STACK_DISPLAY,
                            NULL, TASK_PRIO_DISPLAY, NULL, CORE_DISPLAY);

    ESP_LOGI(TAG, "All tasks started. Ready.");

    /* Wait for RC signal before enabling outputs */
    while (!rc_input_is_signal_ok()) {
        ESP_LOGI(TAG, "Waiting for RC signal...");
        gpio_set_level(GPIO_LED_STATUS, 1);
        vTaskDelay(pdMS_TO_TICKS(250));
        gpio_set_level(GPIO_LED_STATUS, 0);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
    ESP_LOGI(TAG, "RC signal detected — system armed");
}
