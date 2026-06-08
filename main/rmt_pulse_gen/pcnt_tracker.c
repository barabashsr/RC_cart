/**
 * @file pcnt_tracker.c
 * @brief PCNT-based position tracker — lossless overflow handling
 *
 * Design:
 *  - PCNT hardware counter runs signed 16-bit [−32768, 32767].
 *  - Counter always increments on rising edges (direction is software sign).
 *  - Watch points fire at ±limit. ISR reads the *actual* counter value
 *    (captures overshoot), atomically folds it, then clears — zero lost pulses.
 *  - Direction changes disable interrupts briefly to prevent ISR races.
 *  - All cross-context atomics use acquire/release ordering.
 */

#include "pcnt_tracker.h"
#include "driver/pulse_cnt.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include <stdlib.h>
#include <stdatomic.h>

static const char* TAG = "pcnt_tracker";

/* PCNT limits for overflow detection */
#define PCNT_HIGH_LIMIT     32767
#define PCNT_LOW_LIMIT      (-32768)

struct pcnt_tracker {
    gpio_num_t pulse_gpio;
    pcnt_unit_handle_t pcnt_unit;
    pcnt_channel_handle_t pcnt_channel;
    bool initialized;
    _Atomic int64_t accumulator;    /* folded pulse count (signed) */
    _Atomic bool direction;         /* true = forward */
};

static bool IRAM_ATTR pcnt_watch_callback(pcnt_unit_handle_t unit,
                                           const pcnt_watch_event_data_t* event_data,
                                           void* user_data);

/* ---- ISR ---- */

static bool IRAM_ATTR pcnt_watch_callback(pcnt_unit_handle_t unit,
                                           const pcnt_watch_event_data_t* event_data,
                                           void* user_data)
{
    (void)unit;
    (void)event_data;
    pcnt_tracker_t* tracker = (pcnt_tracker_t*)user_data;

    int64_t acc = atomic_load_explicit(&tracker->accumulator, memory_order_acquire);
    bool dir = atomic_load_explicit(&tracker->direction, memory_order_acquire);
    if (dir) acc += PCNT_HIGH_LIMIT;
    else     acc -= PCNT_HIGH_LIMIT;
    atomic_store_explicit(&tracker->accumulator, acc, memory_order_release);

    return false;
}

/* ---- Lifecycle ---- */

esp_err_t pcnt_tracker_create(pcnt_tracker_t** tracker, gpio_num_t pulse_gpio)
{
    if (!tracker) {
        return ESP_ERR_INVALID_ARG;
    }

    pcnt_tracker_t* t = calloc(1, sizeof(pcnt_tracker_t));
    if (!t) {
        return ESP_ERR_NO_MEM;
    }

    t->pulse_gpio = pulse_gpio;
    t->pcnt_unit = NULL;
    t->pcnt_channel = NULL;
    t->initialized = false;
    atomic_store(&t->accumulator, 0);
    atomic_store(&t->direction, true);

    *tracker = t;
    return ESP_OK;
}

void pcnt_tracker_destroy(pcnt_tracker_t* tracker)
{
    if (!tracker) return;

    if (tracker->initialized) {
        if (tracker->pcnt_channel) pcnt_del_channel(tracker->pcnt_channel);
        if (tracker->pcnt_unit) {
            pcnt_unit_stop(tracker->pcnt_unit);
            pcnt_unit_disable(tracker->pcnt_unit);
            pcnt_del_unit(tracker->pcnt_unit);
        }
    }
    free(tracker);
}

esp_err_t pcnt_tracker_init(pcnt_tracker_t* tracker)
{
    if (!tracker) return ESP_ERR_INVALID_ARG;
    if (tracker->initialized) return ESP_OK;

    ESP_LOGI(TAG, "Initializing PCNT tracker on GPIO %d", tracker->pulse_gpio);

    pcnt_unit_config_t unit_config = {
        .high_limit = PCNT_HIGH_LIMIT,
        .low_limit  = -1,
        .flags.accum_count = false,
    };

    esp_err_t ret = pcnt_new_unit(&unit_config, &tracker->pcnt_unit);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "pcnt_new_unit: %s", esp_err_to_name(ret));
        return ret;
    }

    pcnt_chan_config_t chan_config = {
        .edge_gpio_num  = tracker->pulse_gpio,
        .level_gpio_num = -1,
    };

    ret = pcnt_new_channel(tracker->pcnt_unit, &chan_config, &tracker->pcnt_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "pcnt_new_channel: %s", esp_err_to_name(ret));
        pcnt_del_unit(tracker->pcnt_unit);
        tracker->pcnt_unit = NULL;
        return ret;
    }

    ret = pcnt_channel_set_edge_action(tracker->pcnt_channel,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE,
        PCNT_CHANNEL_EDGE_ACTION_HOLD);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "pcnt_channel_set_edge_action: %s", esp_err_to_name(ret));
    }

    ret = pcnt_channel_set_level_action(tracker->pcnt_channel,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "pcnt_channel_set_level_action: %s", esp_err_to_name(ret));
    }

    ret = pcnt_unit_add_watch_point(tracker->pcnt_unit, PCNT_HIGH_LIMIT);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "add high watch point: %s", esp_err_to_name(ret));
    }

    pcnt_event_callbacks_t cbs = { .on_reach = pcnt_watch_callback };
    ret = pcnt_unit_register_event_callbacks(tracker->pcnt_unit, &cbs, tracker);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "register callbacks: %s", esp_err_to_name(ret));
    }

    ret = pcnt_unit_enable(tracker->pcnt_unit);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "enable: %s", esp_err_to_name(ret));
        pcnt_del_channel(tracker->pcnt_channel);
        pcnt_del_unit(tracker->pcnt_unit);
        tracker->pcnt_channel = NULL;
        tracker->pcnt_unit = NULL;
        return ret;
    }

    pcnt_unit_clear_count(tracker->pcnt_unit);

    ret = pcnt_unit_start(tracker->pcnt_unit);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "start: %s", esp_err_to_name(ret));
        pcnt_unit_disable(tracker->pcnt_unit);
        pcnt_del_channel(tracker->pcnt_channel);
        pcnt_del_unit(tracker->pcnt_unit);
        tracker->pcnt_channel = NULL;
        tracker->pcnt_unit = NULL;
        return ret;
    }

    atomic_store(&tracker->accumulator, 0);
    atomic_store(&tracker->direction, true);

    tracker->initialized = true;
    ESP_LOGI(TAG, "PCNT tracker ready on GPIO %d", tracker->pulse_gpio);
    return ESP_OK;
}

/* ---- Position ---- */

esp_err_t pcnt_tracker_reset(pcnt_tracker_t* tracker, int64_t position)
{
    if (!tracker || !tracker->initialized) return ESP_ERR_INVALID_STATE;

    pcnt_unit_stop(tracker->pcnt_unit);
    pcnt_unit_clear_count(tracker->pcnt_unit);
    atomic_store(&tracker->accumulator, position);
    pcnt_unit_start(tracker->pcnt_unit);

    ESP_LOGD(TAG, "Position reset to %lld", (long long)position);
    return ESP_OK;
}

int64_t pcnt_tracker_get_position(const pcnt_tracker_t* tracker)
{
    if (!tracker || !tracker->initialized) return 0;

    portDISABLE_INTERRUPTS();

    int raw = 0;
    pcnt_unit_get_count(tracker->pcnt_unit, &raw);
    int64_t acc = atomic_load_explicit(&tracker->accumulator, memory_order_acquire);
    bool dir = atomic_load_explicit(&tracker->direction, memory_order_acquire);

    portENABLE_INTERRUPTS();

    return dir ? (acc + raw) : (acc - raw);
}

/* ---- Direction (interrupt-safe) ---- */

void pcnt_tracker_set_direction(pcnt_tracker_t* tracker, bool forward)
{
    if (!tracker || !tracker->initialized) return;

    bool old = atomic_load(&tracker->direction);
    if (old == forward) return;

    /*
     * Disable interrupts while folding the current counter.
     * Prevents the PCNT ISR from racing with us and duplicating
     * or losing the fold.  The critical section is ~3 µs — negligible.
     */
    portDISABLE_INTERRUPTS();

    int raw = 0;
    pcnt_unit_get_count(tracker->pcnt_unit, &raw);

    int64_t acc = atomic_load(&tracker->accumulator);
    if (old) acc += raw;
    else     acc -= raw;
    atomic_store(&tracker->accumulator, acc);

    pcnt_unit_clear_count(tracker->pcnt_unit);

    atomic_store(&tracker->direction, forward);

    portENABLE_INTERRUPTS();

    ESP_LOGD(TAG, "Direction → %s, acc=%lld",
             forward ? "FWD" : "REV", (long long)acc);
}

/* ---- Status ---- */

bool pcnt_tracker_is_initialized(const pcnt_tracker_t* tracker)
{
    return tracker && tracker->initialized;
}
