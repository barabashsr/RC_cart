#include "engine.h"
#include "driver/gpio.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "engine";

static engine_state_t s_state = ENGINE_OFF;
static bool s_starter_active = false;
static int64_t s_crank_start_us = 0;
static int64_t s_cooldown_start_us = 0;
static int64_t s_running_start_us = 0;
static int64_t s_last_rpm_update_us = 0;

#if CFG_ENABLE_RPM
static pcnt_unit_handle_t s_pcnt_unit = NULL;
static pcnt_channel_handle_t s_pcnt_channel = NULL;
static _Atomic int64_t s_pulse_count = 0;
static uint16_t s_current_rpm = 0;
#endif

static void starter_on(void)
{
    if (!s_starter_active) {
        gpio_set_level(GPIO_STARTER_RELAY, STARTER_RELAY_ACTIVE_LEVEL);
        s_starter_active = true;
        s_crank_start_us = esp_timer_get_time();
        ESP_LOGI(TAG, "Starter ON");
    }
}

static void starter_off(void)
{
    if (s_starter_active) {
        gpio_set_level(GPIO_STARTER_RELAY, !STARTER_RELAY_ACTIVE_LEVEL);
        s_starter_active = false;
        ESP_LOGI(TAG, "Starter OFF");
    }
}

static void ignition_kill_pulse(void)
{
    gpio_set_level(GPIO_IGNITION_KILL, IGNITION_KILL_ACTIVE_LEVEL);
    ESP_LOGI(TAG, "Ignition KILL active for %d ms", IGNITION_KILL_PULSE_MS);
    vTaskDelay(pdMS_TO_TICKS(IGNITION_KILL_PULSE_MS));
    gpio_set_level(GPIO_IGNITION_KILL, !IGNITION_KILL_ACTIVE_LEVEL);
    ESP_LOGI(TAG, "Ignition KILL released");
}

esp_err_t engine_init(void)
{
    /* Configure starter relay GPIO */
    gpio_config_t starter_conf = {
        .pin_bit_mask = (1ULL << GPIO_STARTER_RELAY),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    esp_err_t ret = gpio_config(&starter_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure starter GPIO");
        return ret;
    }
    gpio_set_level(GPIO_STARTER_RELAY, !STARTER_RELAY_ACTIVE_LEVEL);

    /* Configure ignition kill GPIO */
    gpio_config_t ign_conf = {
        .pin_bit_mask = (1ULL << GPIO_IGNITION_KILL),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ret = gpio_config(&ign_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure ignition kill GPIO");
        return ret;
    }
    gpio_set_level(GPIO_IGNITION_KILL, !IGNITION_KILL_ACTIVE_LEVEL);

#if CFG_ENABLE_RPM
    /* Configure RPM PCNT */
    pcnt_unit_config_t unit_cfg = {
        .high_limit = 32767,
        .low_limit = -32768,
        .flags = { .accum_count = true },
    };
    ret = pcnt_new_unit(&unit_cfg, &s_pcnt_unit);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create RPM PCNT unit: %s", esp_err_to_name(ret));
        return ret;
    }

    pcnt_chan_config_t chan_cfg = {
        .edge_gpio_num = GPIO_RPM_PULSE,
        .level_gpio_num = -1,
    };
    ret = pcnt_new_channel(s_pcnt_unit, &chan_cfg, &s_pcnt_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create RPM PCNT channel: %s", esp_err_to_name(ret));
        return ret;
    }

    if (RPM_SIGNAL_ACTIVE_EDGE) {
        pcnt_channel_set_edge_action(s_pcnt_channel,
            PCNT_CHANNEL_EDGE_ACTION_INCREASE,
            PCNT_CHANNEL_EDGE_ACTION_HOLD);
    } else {
        pcnt_channel_set_edge_action(s_pcnt_channel,
            PCNT_CHANNEL_EDGE_ACTION_HOLD,
            PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    }
    pcnt_channel_set_level_action(s_pcnt_channel,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP);

    pcnt_unit_enable(s_pcnt_unit);
    pcnt_unit_clear_count(s_pcnt_unit);
    pcnt_unit_start(s_pcnt_unit);

    s_last_rpm_update_us = esp_timer_get_time();
    ESP_LOGI(TAG, "RPM sensor enabled on GPIO %d", GPIO_RPM_PULSE);
#endif

    s_state = ENGINE_OFF;
    ESP_LOGI(TAG, "Engine control initialized");
    return ESP_OK;
}

void engine_update(bool start_request)
{
    int64_t now = esp_timer_get_time();

    /* Update RPM measurement */
#if CFG_ENABLE_RPM
    int64_t elapsed_us = now - s_last_rpm_update_us;
    if (elapsed_us >= RPM_UPDATE_INTERVAL_MS * 1000) {
        int count = 0;
        pcnt_unit_get_count(s_pcnt_unit, &count);
        pcnt_unit_clear_count(s_pcnt_unit);

        /* RPM = (count / elapsed_min) / pulses_per_rev */
        float elapsed_min = (float)elapsed_us / 60e6f;
        if (elapsed_min > 0) {
            s_current_rpm = (uint16_t)((float)count / RPM_SPARKS_PER_REV / elapsed_min);
        } else {
            s_current_rpm = 0;
        }
        s_last_rpm_update_us = now;
    }
#endif

    switch (s_state) {

    case ENGINE_OFF:
        if (start_request) {
            starter_on();
            s_state = ENGINE_CRANKING;
        }
        break;

    case ENGINE_CRANKING: {
        /* Check if engine started */
#if CFG_ENABLE_RPM
        if (s_current_rpm > RPM_RUNNING_THRESHOLD) {
            if (s_running_start_us == 0) {
                s_running_start_us = now;
            } else if ((now - s_running_start_us) >= RPM_RUNNING_CONFIRM_MS * 1000) {
                starter_off();
                s_state = ENGINE_RUNNING;
                s_running_start_us = 0;
                ESP_LOGI(TAG, "Engine RUNNING (RPM=%u)", s_current_rpm);
                break;
            }
        } else {
            s_running_start_us = 0;
        }
#endif

        /* Max crank timeout */
        if ((now - s_crank_start_us) > STARTER_MAX_CRANK_MS * 1000) {
            starter_off();
            s_cooldown_start_us = now;
            s_state = ENGINE_COOLDOWN;
            ESP_LOGW(TAG, "Engine failed to start (max crank time). Cooldown...");
            break;
        }

        /* User released start switch */
        if (!start_request) {
            starter_off();
            /* If we were cranking and user released, assume started */
#if CFG_ENABLE_RPM
            s_state = ENGINE_OFF;
#else
            s_state = ENGINE_RUNNING;  /* Assume running if RPM sensor disabled */
            ESP_LOGI(TAG, "Engine assumed RUNNING (no RPM sensor)");
#endif
        }
        break;
    }

    case ENGINE_RUNNING:
#if CFG_ENABLE_RPM
        /* Detect stall */
        if (s_current_rpm < RPM_RUNNING_THRESHOLD) {
            s_state = ENGINE_STALLED;
            ESP_LOGW(TAG, "Engine STALLED");
        }
#endif
        /* Start switch in STOP position = kill engine */
        if (!start_request) {
            ignition_kill_pulse();
            s_state = ENGINE_OFF;
            ESP_LOGI(TAG, "Engine stopped by user");
        }
        break;

    case ENGINE_COOLDOWN:
        if ((now - s_cooldown_start_us) > STARTER_COOLDOWN_MS * 1000) {
            s_state = ENGINE_OFF;
            ESP_LOGI(TAG, "Cooldown complete — ready to start");
        }
        break;

    case ENGINE_STALLED:
        /* Requires user to cycle start switch to off then on */
        if (!start_request) {
            s_state = ENGINE_OFF;
            ESP_LOGI(TAG, "Engine state reset (stall acknowledged)");
        }
        break;

    case ENGINE_ERROR:
        /* Stay in error until reboot */
        break;
    }
}

engine_state_t engine_get_state(void)
{
    return s_state;
}

uint16_t engine_get_rpm(void)
{
#if CFG_ENABLE_RPM
    return s_current_rpm;
#else
    return 0;
#endif
}

void engine_kill(void)
{
    starter_off();
    if (s_state == ENGINE_RUNNING) {
        ignition_kill_pulse();
    }
    s_state = ENGINE_OFF;
    ESP_LOGI(TAG, "Engine killed (emergency)");
}
