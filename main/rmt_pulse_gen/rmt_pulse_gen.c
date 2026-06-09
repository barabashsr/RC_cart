/**
 * @file rmt_pulse_gen.c
 * @brief RMT-based pulse generator implementation
 *
 * Architecture:
 * - Command queue filled by ramp generator task (FPU allowed)
 * - RMT simple encoder callback reads from queue (ISR context - no FPU)
 * - Trapezoidal velocity profile with smooth acceleration
 * - DMA for efficient pulse generation
 */

#include "rmt_pulse_gen.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_encoder.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
/* P4-specific debug registers removed for ESP32-S3 portability */
#include <stdlib.h>
#include <stdatomic.h>
#include <math.h>
#include <string.h>

/* Debug: track if encoder callback was ever invoked */
static volatile bool s_encoder_callback_invoked = false;

/* Debug: count encoder callback invocations */
static volatile uint32_t s_encode_call_count = 0;

static const char* TAG = "rmt_pulse_gen";

/* RMT Configuration */
#define RMT_RESOLUTION_HZ       16000000    /* 16 MHz = 62.5ns per tick */
#define RMT_MEM_BLOCK_SYMBOLS   64          /* Memory block size (ESP32-S3: 64 symbols) */
#define RMT_QUEUE_LEN           32          /* Command queue length */
#define RMT_QUEUE_LEN_MASK      (RMT_QUEUE_LEN - 1)
#define RMT_PART_SIZE           24          /* Symbols per encoder callback */
#define RMT_MAX_STEPS_PER_CMD   255         /* Max steps per command */

/* Frequency limits */
#define MIN_PULSE_FREQ_HZ       1           /* Minimum pulse frequency (absolute floor) */
#define MIN_START_FREQ_HZ       250         /* Min freq for ramp start (keeps ticks in uint16_t) */
#define MAX_PULSE_FREQ_HZ       500000      /* Maximum pulse frequency */

/* Timing */
#define RAMP_TASK_STACK_SIZE    4096
#define RAMP_TASK_PRIORITY      (configMAX_PRIORITIES - 2)
#define RAMP_TASK_PERIOD_MS     5           /* Ramp task period */
#define POSITION_UPDATE_MS      10          /* Target latency for position updates */
#define FORWARD_PLANNING_TICKS  (RMT_RESOLUTION_HZ / 10)   /* 100ms forward planning */
#define MIN_QUEUE_CMDS          16          /* Minimum commands before ticks limit */
#define DIR_SETUP_US            5           /* Direction setup time in microseconds */
#define RMT_MIN_CMD_TICKS       3200        /* Min ticks per cmd (5000 Hz, keeps duration < 32767) */
#define RMT_HALF_TICK_MIN       40          /* 2.5 µs min pulse width at 16 MHz (stepper driver req) */

/* Command flags */
#define CMD_FLAG_LAST           0x01
#define CMD_FLAG_DIRECTION      0x02        /* Set if forward */
#define CMD_FLAG_TOGGLE_DIR     0x04

/**
 * @brief Ramp generator states
 */
typedef enum {
    RAMP_IDLE = 0,
    RAMP_ACCELERATING,
    RAMP_CRUISING,
    RAMP_DECELERATING
} ramp_state_t;

/**
 * @brief Step command structure
 */
typedef struct {
    uint16_t ticks;     /* Period in RMT ticks (uint16_t, min ~244 Hz at 16 MHz) */
    uint8_t steps;      /* Steps per command */
    uint8_t flags;      /* Command flags */
} step_command_t;

/**
 * @brief Queue state tracking
 */
typedef struct {
    volatile int32_t position;      /* Position at end of queue */
    volatile bool direction;        /* Direction at end of queue */
    volatile uint32_t ticks_queued; /* Total ticks in queue */
} queue_state_t;

/**
 * @brief Ramp parameters (atomic for thread safety)
 */
typedef struct {
    _Atomic float target_velocity;
    _Atomic float acceleration;
    _Atomic int32_t target_position;
    _Atomic bool position_mode;
    _Atomic bool params_changed;
    /* Direction change handling */
    _Atomic bool direction_change_pending;
    _Atomic bool pending_direction;
    _Atomic float pending_velocity;
} ramp_params_t;

/**
 * @brief RMT pulse generator internal structure
 */
struct rmt_pulse_gen {
    /* GPIO configuration */
    gpio_num_t step_gpio;
    gpio_num_t dir_gpio;

    /* RMT handles */
    rmt_channel_handle_t rmt_channel;
    rmt_encoder_handle_t rmt_encoder;

    /* Command queue */
    step_command_t queue[RMT_QUEUE_LEN];
    _Atomic uint8_t read_idx;
    _Atomic uint8_t write_idx;
    queue_state_t queue_end;

    /* Ramp generator state */
    ramp_params_t params;
    ramp_state_t ramp_state;
    float current_velocity;
    uint32_t current_ticks;
    int32_t ramp_steps_up;
    int32_t ramp_steps_down;

    /* Position tracking */
    _Atomic int64_t pulse_count;
    _Atomic bool direction;

    /* State flags */
    _Atomic bool initialized;
    _Atomic bool running;
    _Atomic bool rmt_stopped;
    _Atomic bool completion_pending;
    bool last_chunk_had_steps;

    /* Ramp task */
    TaskHandle_t ramp_task_handle;
    SemaphoreHandle_t ramp_semaphore;

    /* Callbacks */
    motion_complete_cb_t completion_callback;
    void* callback_user_data;
    pcnt_tracker_t* position_tracker;
};

/* Forward declarations */
static void ramp_task_func(void* arg);
static void fill_queue(rmt_pulse_gen_t* gen);
static step_command_t generate_next_command(rmt_pulse_gen_t* gen);
static int32_t calculate_decel_distance(float velocity, float decel);
static uint8_t calculate_steps_for_latency(uint32_t ticks);
static bool is_queue_full(const rmt_pulse_gen_t* gen);
static uint8_t queue_space(const rmt_pulse_gen_t* gen);
static bool push_command(rmt_pulse_gen_t* gen, const step_command_t* cmd);
static void clear_queue(rmt_pulse_gen_t* gen);
static void start_rmt_transmission(rmt_pulse_gen_t* gen);

static size_t IRAM_ATTR encode_callback(const void* data, size_t data_size,
                                        size_t symbols_written, size_t symbols_free,
                                        rmt_symbol_word_t* symbols, bool* done, void* arg);
static bool IRAM_ATTR on_transmit_done(rmt_channel_handle_t channel,
                                        const rmt_tx_done_event_data_t* event_data,
                                        void* user_ctx);
static size_t IRAM_ATTR fill_symbols(rmt_pulse_gen_t* gen, rmt_symbol_word_t* symbols,
                                      step_command_t* cmd);

esp_err_t rmt_pulse_gen_create(rmt_pulse_gen_t** gen, gpio_num_t step_gpio, gpio_num_t dir_gpio)
{
    if (!gen) {
        return ESP_ERR_INVALID_ARG;
    }

    rmt_pulse_gen_t* g = calloc(1, sizeof(rmt_pulse_gen_t));
    if (!g) {
        return ESP_ERR_NO_MEM;
    }

    g->step_gpio = step_gpio;
    g->dir_gpio = dir_gpio;
    g->rmt_channel = NULL;
    g->rmt_encoder = NULL;
    atomic_store(&g->initialized, false);
    atomic_store(&g->running, false);
    atomic_store(&g->rmt_stopped, true);
    atomic_store(&g->direction, true);
    atomic_store(&g->pulse_count, 0);

    /* Initialize queue state */
    g->queue_end.position = 0;
    g->queue_end.direction = true;
    g->queue_end.ticks_queued = 0;

    *gen = g;
    return ESP_OK;
}

void rmt_pulse_gen_destroy(rmt_pulse_gen_t* gen)
{
    if (!gen) {
        return;
    }

    /* Stop ramp task */
    if (gen->ramp_task_handle) {
        atomic_store(&gen->running, false);
        atomic_store(&gen->rmt_stopped, true);
        if (gen->ramp_semaphore) {
            xSemaphoreGive(gen->ramp_semaphore);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        vTaskDelete(gen->ramp_task_handle);
        gen->ramp_task_handle = NULL;
    }

    if (gen->ramp_semaphore) {
        vSemaphoreDelete(gen->ramp_semaphore);
        gen->ramp_semaphore = NULL;
    }

    if (atomic_load(&gen->initialized)) {
        rmt_pulse_gen_stop_immediate(gen);
        if (gen->rmt_channel) {
            rmt_disable(gen->rmt_channel);
            rmt_del_channel(gen->rmt_channel);
        }
        if (gen->rmt_encoder) {
            rmt_del_encoder(gen->rmt_encoder);
        }
    }

    free(gen);
}

esp_err_t rmt_pulse_gen_init(rmt_pulse_gen_t* gen)
{
    if (!gen) {
        return ESP_ERR_INVALID_ARG;
    }

    if (atomic_load(&gen->initialized)) {
        ESP_LOGW(TAG, "Generator already initialized");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Initializing RMT pulse generator on GPIO %d (step), %d (dir)",
             gen->step_gpio, gen->dir_gpio);

    /* Configure direction GPIO */
    gpio_config_t dir_conf = {
        .pin_bit_mask = (1ULL << gen->dir_gpio),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    esp_err_t ret = gpio_config(&dir_conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure direction GPIO: %s", esp_err_to_name(ret));
        return ret;
    }
    gpio_set_level(gen->dir_gpio, 1);  /* Default forward */

    /* Create RMT TX channel with DMA and loopback */
    rmt_tx_channel_config_t tx_config = {
        .gpio_num = gen->step_gpio,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = RMT_RESOLUTION_HZ,
        .mem_block_symbols = RMT_MEM_BLOCK_SYMBOLS,
        .trans_queue_depth = 1,
        .intr_priority = 0,
        .flags = {
            .invert_out = 0,
            .with_dma = 0,          /* Disable DMA for callback-based encoder */
            .io_loop_back = 0,      /* Disable loopback (MCP_example pattern) */
            .io_od_mode = 0,
            .allow_pd = 0,
        },
    };

    ret = rmt_new_tx_channel(&tx_config, &gen->rmt_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create RMT TX channel: %s", esp_err_to_name(ret));
        return ret;
    }

    /* Create simple encoder with callback */
    rmt_simple_encoder_config_t enc_config = {
        .callback = encode_callback,
        .arg = gen,
        .min_chunk_size = RMT_PART_SIZE,
    };
    ret = rmt_new_simple_encoder(&enc_config, &gen->rmt_encoder);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create encoder: %s", esp_err_to_name(ret));
        rmt_del_channel(gen->rmt_channel);
        gen->rmt_channel = NULL;
        return ret;
    }

    /* Register transmit done callback */
    rmt_tx_event_callbacks_t cbs = {
        .on_trans_done = on_transmit_done,
    };
    ret = rmt_tx_register_event_callbacks(gen->rmt_channel, &cbs, gen);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register callbacks: %s", esp_err_to_name(ret));
        rmt_del_encoder(gen->rmt_encoder);
        rmt_del_channel(gen->rmt_channel);
        gen->rmt_encoder = NULL;
        gen->rmt_channel = NULL;
        return ret;
    }

    /* Enable channel */
    ret = rmt_enable(gen->rmt_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable RMT channel: %s", esp_err_to_name(ret));
        rmt_del_encoder(gen->rmt_encoder);
        rmt_del_channel(gen->rmt_channel);
        gen->rmt_encoder = NULL;
        gen->rmt_channel = NULL;
        return ret;
    }
    ESP_LOGI(TAG, "RMT channel enabled on GPIO %d", gen->step_gpio);

    /* Create ramp generator task */
    gen->ramp_semaphore = xSemaphoreCreateBinary();
    if (!gen->ramp_semaphore) {
        ESP_LOGE(TAG, "Failed to create semaphore");
        rmt_disable(gen->rmt_channel);
        rmt_del_encoder(gen->rmt_encoder);
        rmt_del_channel(gen->rmt_channel);
        gen->rmt_encoder = NULL;
        gen->rmt_channel = NULL;
        return ESP_ERR_NO_MEM;
    }

    BaseType_t result = xTaskCreatePinnedToCore(
        ramp_task_func,
        "rmt_ramp",
        RAMP_TASK_STACK_SIZE,
        gen,
        RAMP_TASK_PRIORITY,
        &gen->ramp_task_handle,
        1  /* Pin to Core 1 */
    );
    if (result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create ramp task");
        vSemaphoreDelete(gen->ramp_semaphore);
        rmt_disable(gen->rmt_channel);
        rmt_del_encoder(gen->rmt_encoder);
        rmt_del_channel(gen->rmt_channel);
        gen->ramp_semaphore = NULL;
        gen->rmt_encoder = NULL;
        gen->rmt_channel = NULL;
        return ESP_ERR_NO_MEM;
    }

    atomic_store(&gen->initialized, true);
    ESP_LOGI(TAG, "RMT pulse generator initialized successfully");
    return ESP_OK;
}

esp_err_t rmt_pulse_gen_start_move(rmt_pulse_gen_t* gen, int32_t pulses,
                                    float max_velocity, float acceleration)
{
    if (!gen || !atomic_load(&gen->initialized)) {
        return ESP_ERR_INVALID_STATE;
    }

    if (pulses == 0) {
        return ESP_OK;  /* Nothing to do */
    }

    if (max_velocity <= 0 || max_velocity > MAX_PULSE_FREQ_HZ) {
        ESP_LOGE(TAG, "Invalid velocity: %.1f", max_velocity);
        return ESP_ERR_INVALID_ARG;
    }

    if (acceleration <= 0) {
        ESP_LOGE(TAG, "Invalid acceleration: %.1f", acceleration);
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGD(TAG, "Starting move: pulses=%ld, vel=%.1f, accel=%.1f",
             (long)pulses, max_velocity, acceleration);

    /* Determine direction */
    bool new_direction = (pulses >= 0);
    int32_t abs_pulses = (pulses >= 0) ? pulses : -pulses;

    /* If already running, stop and restart fresh (yarobot pattern).
     * The RMT gap is ~50-100 µs — imperceptible with mechanical reduction. */
    if (atomic_load(&gen->running)) {
        rmt_pulse_gen_stop_immediate(gen);
    }

    /* Set direction GPIO */
    gpio_set_level(gen->dir_gpio, new_direction ? 1 : 0);
    esp_rom_delay_us(DIR_SETUP_US);

    /* Update atomic parameters */
    atomic_store(&gen->params.target_position, abs_pulses);
    atomic_store(&gen->params.target_velocity, max_velocity);
    atomic_store(&gen->params.acceleration, acceleration);
    atomic_store(&gen->params.position_mode, true);
    atomic_store(&gen->direction, new_direction);

    /* Reset ramp state */
    clear_queue(gen);
    gen->current_velocity = 0.0f;
    gen->current_ticks = RMT_MIN_CMD_TICKS;  /* Start with valid ticks (keeps duration < 32767) */
    gen->ramp_steps_up = 0;
    gen->ramp_steps_down = 0;
    gen->ramp_state = RAMP_ACCELERATING;
    gen->queue_end.position = 0;
    gen->queue_end.direction = new_direction;
    gen->queue_end.ticks_queued = 0;

    /* Set direction on position tracker */
    if (gen->position_tracker) {
        pcnt_tracker_set_direction(gen->position_tracker, new_direction);
    }

    /* Start motion */
    atomic_store(&gen->running, true);
    atomic_store(&gen->rmt_stopped, false);
    start_rmt_transmission(gen);

    /* Wake ramp task */
    xSemaphoreGive(gen->ramp_semaphore);

    return ESP_OK;
}

esp_err_t rmt_pulse_gen_start_velocity(rmt_pulse_gen_t* gen,
                                        float velocity, float acceleration)
{
    if (!gen || !atomic_load(&gen->initialized)) {
        return ESP_ERR_INVALID_STATE;
    }

    float abs_velocity = fabsf(velocity);
    if (abs_velocity < MIN_PULSE_FREQ_HZ || abs_velocity > MAX_PULSE_FREQ_HZ) {
        ESP_LOGE(TAG, "Invalid velocity: %.1f", velocity);
        return ESP_ERR_INVALID_ARG;
    }

    if (acceleration <= 0) {
        ESP_LOGE(TAG, "Invalid acceleration: %.1f", acceleration);
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGD(TAG, "Starting velocity: vel=%.1f, accel=%.1f", velocity, acceleration);

    bool new_direction = (velocity >= 0);

    /* Update atomic parameters */
    atomic_store(&gen->params.target_velocity, abs_velocity);
    atomic_store(&gen->params.acceleration, acceleration);
    atomic_store(&gen->params.position_mode, false);

    if (atomic_load(&gen->running)) {
        /* Already running - check for direction change */
        bool current_direction = atomic_load(&gen->direction);

        if (new_direction != current_direction) {
            /* Direction change required - decel to stop first */
            ESP_LOGD(TAG, "Direction change: %d -> %d, decelerating first",
                     current_direction, new_direction);

            /* Store pending state for after stop */
            atomic_store(&gen->params.pending_direction, new_direction);
            atomic_store(&gen->params.pending_velocity, abs_velocity);
            atomic_store(&gen->params.direction_change_pending, true);

            /* Set target velocity to 0 (stop) - fill_queue will handle the rest */
            atomic_store(&gen->params.target_velocity, 0.0f);
        }
        /* Adjust velocity (or initiate stop for direction change) */
        atomic_store(&gen->params.params_changed, true);
    } else {
        /* Starting fresh */
        /* Set direction GPIO */
        gpio_set_level(gen->dir_gpio, new_direction ? 1 : 0);
        esp_rom_delay_us(DIR_SETUP_US);
        atomic_store(&gen->direction, new_direction);

        clear_queue(gen);
        gen->current_velocity = 0.0f;
        gen->current_ticks = RMT_MIN_CMD_TICKS;  /* Start with valid ticks (keeps duration < 32767) */
        gen->ramp_steps_up = 0;
        gen->ramp_steps_down = 0;
        gen->ramp_state = RAMP_ACCELERATING;
        gen->queue_end.position = 0;
        gen->queue_end.direction = new_direction;
        gen->queue_end.ticks_queued = 0;

        if (gen->position_tracker) {
            pcnt_tracker_set_direction(gen->position_tracker, new_direction);
        }

        atomic_store(&gen->running, true);
        atomic_store(&gen->rmt_stopped, false);
        start_rmt_transmission(gen);
    }

    xSemaphoreGive(gen->ramp_semaphore);

    return ESP_OK;
}

esp_err_t rmt_pulse_gen_stop(rmt_pulse_gen_t* gen, float deceleration)
{
    if (!gen || !atomic_load(&gen->running)) {
        return ESP_ERR_INVALID_STATE;
    }

    if (deceleration <= 0) {
        ESP_LOGE(TAG, "Invalid deceleration: %.1f", deceleration);
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGD(TAG, "Stopping with decel: %.1f", deceleration);

    /* Calculate stop distance from current velocity */
    float current_vel = gen->current_velocity;
    int32_t stop_distance = calculate_decel_distance(current_vel, deceleration);

    /* Flush existing queue — stops cruising commands from executing.
     * This ensures deceleration starts immediately rather than after
     * playing out ~100ms of buffered full-speed commands. */
    clear_queue(gen);

    /* queue_end.position is now 0 after clear_queue.
     * Set target_position to just the stop_distance — the decel profile
     * will generate exactly stop_distance more pulses from this point. */
    atomic_store(&gen->params.target_position, stop_distance);
    atomic_store(&gen->params.acceleration, deceleration);
    atomic_store(&gen->params.position_mode, true);
    atomic_store(&gen->params.params_changed, true);

    gen->ramp_state = RAMP_DECELERATING;
    gen->ramp_steps_down = 0;

    xSemaphoreGive(gen->ramp_semaphore);

    return ESP_OK;
}

void rmt_pulse_gen_stop_immediate(rmt_pulse_gen_t* gen)
{
    if (!gen || !atomic_load(&gen->running)) {
        return;
    }

    ESP_LOGD(TAG, "Immediate stop");

    atomic_store(&gen->rmt_stopped, true);

    if (gen->rmt_channel) {
        rmt_disable(gen->rmt_channel);
        rmt_enable(gen->rmt_channel);
    }

    atomic_store(&gen->running, false);
    gen->ramp_state = RAMP_IDLE;
    gen->current_velocity = 0.0f;
    clear_queue(gen);

    /* Sync position tracker from ISR pulse count after aborted queue.
     * pulse_count_ is never reset — it tracks the cumulative absolute position. */
    if (gen->position_tracker) {
        int64_t actual = atomic_load(&gen->pulse_count);
        pcnt_tracker_reset(gen->position_tracker, actual);
        ESP_LOGD(TAG, "stopImmediate: synced tracker to %lld", (long long)actual);
    }
}

bool rmt_pulse_gen_is_running(const rmt_pulse_gen_t* gen)
{
    return gen && atomic_load(&gen->running);
}

float rmt_pulse_gen_get_velocity(const rmt_pulse_gen_t* gen)
{
    if (!gen || !atomic_load(&gen->running)) {
        return 0.0f;
    }
    return gen->current_velocity;
}

int64_t rmt_pulse_gen_get_pulse_count(const rmt_pulse_gen_t* gen)
{
    return gen ? atomic_load(&gen->pulse_count) : 0;
}

/* Debug: get encoder callback count */
uint32_t rmt_pulse_gen_get_encode_count(void)
{
    return s_encode_call_count;
}

void rmt_pulse_gen_set_tracker(rmt_pulse_gen_t* gen, pcnt_tracker_t* tracker)
{
    if (gen) {
        gen->position_tracker = tracker;
    }
}

void rmt_pulse_gen_set_callback(rmt_pulse_gen_t* gen, motion_complete_cb_t cb, void* user_data)
{
    if (gen) {
        gen->completion_callback = cb;
        gen->callback_user_data = user_data;
    }
}

/* Ramp generator task */
static void ramp_task_func(void* arg)
{
    rmt_pulse_gen_t* gen = (rmt_pulse_gen_t*)arg;
    uint32_t debug_counter = 0;

    ESP_LOGI(TAG, "Ramp task started");

    while (true) {
        /* Wait for wake-up signal or timeout */
        xSemaphoreTake(gen->ramp_semaphore, pdMS_TO_TICKS(RAMP_TASK_PERIOD_MS));

        /* Periodic status: every ~1 second (200 * 5ms) */
        debug_counter++;
        if (debug_counter >= 200 && atomic_load(&gen->running)) {
            debug_counter = 0;
            uint8_t rp = atomic_load(&gen->read_idx);
            uint8_t wp = atomic_load(&gen->write_idx);
            uint8_t depth = (wp - rp) & RMT_QUEUE_LEN_MASK;

            ESP_LOGD(TAG, "RMT: q=%d/%d cb=%lu vel=%.0f",
                     depth, RMT_QUEUE_LEN,
                     (unsigned long)s_encode_call_count,
                     gen->current_velocity);
        }

        /* Handle completion notification */
        bool pending = atomic_load(&gen->completion_pending);
        if (pending) {
            atomic_store(&gen->completion_pending, false);
            atomic_store(&gen->running, false);
            gen->ramp_state = RAMP_IDLE;
            if (gen->completion_callback) {
                int64_t pulses = atomic_load(&gen->pulse_count);
                gen->completion_callback(gen->callback_user_data, pulses);
            }
        }

        /* Fill queue if motion in progress */
        if (gen->ramp_state != RAMP_IDLE) {
            fill_queue(gen);
        }
    }
}

static void fill_queue(rmt_pulse_gen_t* gen)
{
    /* Check for pending direction change after deceleration to stop */
    if (atomic_load(&gen->params.direction_change_pending)) {
        if (gen->current_velocity < MIN_START_FREQ_HZ) {
            /* Stopped - now change direction and accelerate to pending target */
            bool new_dir = atomic_load(&gen->params.pending_direction);
            float new_vel = atomic_load(&gen->params.pending_velocity);

            ESP_LOGI(TAG, "Direction change complete: dir=%d, target_vel=%.0f pps",
                     new_dir, new_vel);

            /* Change direction GPIO */
            gpio_set_level(gen->dir_gpio, new_dir ? 1 : 0);
            esp_rom_delay_us(DIR_SETUP_US);
            atomic_store(&gen->direction, new_dir);

            /* Update position tracker direction */
            if (gen->position_tracker) {
                pcnt_tracker_set_direction(gen->position_tracker, new_dir);
            }

            /* Update queue end direction */
            gen->queue_end.direction = new_dir;

            /* Now accelerate to the pending target velocity */
            atomic_store(&gen->params.target_velocity, new_vel);
            atomic_store(&gen->params.direction_change_pending, false);

            /* Reset ramp state for acceleration in new direction */
            gen->ramp_state = RAMP_ACCELERATING;
            gen->ramp_steps_up = 0;
            gen->current_velocity = 0.0f;
        }
    }

    /* Check for parameter changes */
    if (atomic_exchange(&gen->params.params_changed, false)) {
        float target_vel = atomic_load(&gen->params.target_velocity);
        bool position_mode = atomic_load(&gen->params.position_mode);
        float accel = atomic_load(&gen->params.acceleration);

        /* Smooth velocity transitions for both modes (important for heavy parts with high inertia) */
        if (!position_mode) {
            /* Velocity (jog) mode - smooth ramping to new target */
            if (gen->current_velocity < MIN_START_FREQ_HZ) {
                /* Starting from near-zero - accelerate to target */
                gen->ramp_state = RAMP_ACCELERATING;
                gen->ramp_steps_up = 0;
                ESP_LOGD(TAG, "Velocity mode: accelerating from %.0f to %.0f pps", gen->current_velocity, target_vel);
            } else if (fabsf(gen->current_velocity - target_vel) < 1.0f) {
                /* Already at target velocity */
                gen->ramp_state = RAMP_CRUISING;
            } else if (gen->current_velocity > target_vel) {
                /* Decelerating to lower target */
                gen->ramp_state = RAMP_DECELERATING;
                gen->ramp_steps_down = 0;
                ESP_LOGD(TAG, "Velocity mode: decelerating from %.0f to %.0f pps", gen->current_velocity, target_vel);
            } else {
                /* Accelerating to higher target */
                gen->ramp_state = RAMP_ACCELERATING;
                /* Calculate equivalent ramp steps for current velocity */
                gen->ramp_steps_up = (int32_t)((gen->current_velocity * gen->current_velocity) / (2.0f * accel));
                ESP_LOGD(TAG, "Velocity mode: accelerating from %.0f to %.0f pps", gen->current_velocity, target_vel);
            }
        } else {
            /* Position mode - smooth transitions */
            if (gen->current_velocity > target_vel) {
                gen->ramp_state = RAMP_DECELERATING;
                gen->ramp_steps_down = 0;
            } else if (gen->current_velocity < target_vel && gen->ramp_state == RAMP_CRUISING) {
                gen->ramp_state = RAMP_ACCELERATING;
                gen->ramp_steps_up = (int32_t)((gen->current_velocity * gen->current_velocity) / (2.0f * accel));
            }
        }
    }

    /* Calculate queue depth */
    uint8_t rp = atomic_load(&gen->read_idx);
    uint8_t wp = atomic_load(&gen->write_idx);
    uint8_t queue_depth = (wp - rp) & RMT_QUEUE_LEN_MASK;


    /* Fill queue while space available */
    int cmds_added = 0;
    while (queue_space(gen) > 0 && gen->ramp_state != RAMP_IDLE) {
        /* Apply ticks limit only if minimum commands queued */
        if (queue_depth >= MIN_QUEUE_CMDS &&
            gen->queue_end.ticks_queued >= FORWARD_PLANNING_TICKS) {
            break;
        }

        step_command_t cmd = generate_next_command(gen);

        if (cmd.ticks == 0 && cmd.steps == 0) {
            /* Motion complete */
            break;
        }

        push_command(gen, &cmd);
        queue_depth++;
        cmds_added++;


        if (cmd.flags & CMD_FLAG_LAST) {
            gen->ramp_state = RAMP_IDLE;
            break;
        }
    }
}

static step_command_t generate_next_command(rmt_pulse_gen_t* gen)
{
    step_command_t cmd = {0};

    float target_vel = atomic_load(&gen->params.target_velocity);
    float accel = atomic_load(&gen->params.acceleration);
    int32_t target_pos = atomic_load(&gen->params.target_position);
    bool position_mode = atomic_load(&gen->params.position_mode);

    /* Calculate remaining distance (position mode) */
    int32_t remaining = INT32_MAX;
    if (position_mode) {
        if (atomic_load(&gen->direction)) {
            remaining = target_pos - gen->queue_end.position;
        } else {
            remaining = target_pos + gen->queue_end.position;
        }
        if (remaining <= 0) {
            cmd.flags |= CMD_FLAG_LAST;
            return cmd;
        }
    }

    /* Set direction flag */
    if (atomic_load(&gen->direction)) {
        cmd.flags |= CMD_FLAG_DIRECTION;
    }

    switch (gen->ramp_state) {
        case RAMP_ACCELERATING: {
            /* v = sqrt(2 * a * d) */
            float velocity = sqrtf(2.0f * accel * (float)gen->ramp_steps_up);

            if (velocity >= target_vel) {
                velocity = target_vel;
                gen->ramp_state = RAMP_CRUISING;
            }

            /* Check for deceleration (position mode) */
            if (position_mode) {
                int32_t decel_dist = calculate_decel_distance(velocity, accel);
                if (remaining <= decel_dist) {
                    gen->ramp_state = RAMP_DECELERATING;
                    gen->ramp_steps_down = 0;
                }
            }

            /* Clamp to minimum start frequency (keeps RMT ticks in uint16_t range) */
            if (velocity < MIN_START_FREQ_HZ) {
                velocity = MIN_START_FREQ_HZ;
            }

            gen->current_velocity = velocity;
            gen->current_ticks = RMT_RESOLUTION_HZ / (uint32_t)velocity;

            uint8_t steps = calculate_steps_for_latency(gen->current_ticks);
            gen->ramp_steps_up += steps;

            cmd.ticks = (gen->current_ticks > 65535) ? 65535 : (uint16_t)gen->current_ticks;
            cmd.steps = steps;
            break;
        }

        case RAMP_CRUISING: {
            if (position_mode) {
                int32_t decel_dist = calculate_decel_distance(gen->current_velocity, accel);
                if (remaining <= decel_dist) {
                    gen->ramp_state = RAMP_DECELERATING;
                    gen->ramp_steps_down = 0;
                }
            }

            /* Calculate steps first - needed for velocity adjustment */
            uint8_t steps = calculate_steps_for_latency(gen->current_ticks);

            /* Check if target velocity changed (velocity mode) */
            if (!position_mode && gen->current_velocity != target_vel) {
                /* Smoothly adjust velocity toward target */
                float delta_v = target_vel - gen->current_velocity;
                /* Calculate velocity change for this command (multiple steps)
                 * dv = a * dt, where dt = steps / velocity (time for all steps in command)
                 */
                float v_change = accel * (float)steps / gen->current_velocity;

                if (fabsf(delta_v) <= v_change) {
                    gen->current_velocity = target_vel;
                } else if (delta_v > 0) {
                    gen->current_velocity += v_change;
                } else {
                    gen->current_velocity -= v_change;
                }

                /* Clamp to minimum */
                if (gen->current_velocity < MIN_START_FREQ_HZ) {
                    gen->current_velocity = MIN_START_FREQ_HZ;
                }

                gen->current_ticks = RMT_RESOLUTION_HZ / (uint32_t)gen->current_velocity;
                /* Recalculate steps for new ticks */
                steps = calculate_steps_for_latency(gen->current_ticks);
            }

            cmd.ticks = (gen->current_ticks > 65535) ? 65535 : (uint16_t)gen->current_ticks;
            cmd.steps = steps;
            break;
        }

        case RAMP_DECELERATING: {
            if (position_mode) {
                /* Position mode: decelerate to stop at target position */
                int32_t remaining_to_target;
                if (atomic_load(&gen->direction)) {
                    remaining_to_target = target_pos - gen->queue_end.position;
                } else {
                    remaining_to_target = target_pos + gen->queue_end.position;
                }

                if (remaining_to_target <= 0) {
                    cmd.flags |= CMD_FLAG_LAST;
                    break;
                }

                /* v = sqrt(2 * a * remaining) */
                float velocity = sqrtf(2.0f * accel * (float)remaining_to_target);

                if (velocity < MIN_START_FREQ_HZ) {
                    cmd.flags |= CMD_FLAG_LAST;
                    break;
                }

                gen->current_velocity = velocity;
                gen->current_ticks = RMT_RESOLUTION_HZ / (uint32_t)velocity;

                uint8_t steps = calculate_steps_for_latency(gen->current_ticks);
                if (steps > remaining_to_target) {
                    steps = (uint8_t)remaining_to_target;
                }

                gen->ramp_steps_down += steps;

                cmd.ticks = (gen->current_ticks > 65535) ? 65535 : (uint16_t)gen->current_ticks;
                cmd.steps = steps;

                if (steps == remaining_to_target) {
                    cmd.flags |= CMD_FLAG_LAST;
                }
            } else {
                /* Velocity mode: decelerate smoothly toward target velocity */
                float delta_v = gen->current_velocity - target_vel;

                /* Calculate steps first - needed for velocity adjustment */
                uint8_t steps = calculate_steps_for_latency(gen->current_ticks);

                if (delta_v <= 0 || gen->current_velocity <= target_vel) {
                    /* Reached target velocity - switch to cruising */
                    gen->current_velocity = target_vel;
                    gen->current_ticks = RMT_RESOLUTION_HZ / (uint32_t)target_vel;
                    gen->ramp_state = RAMP_CRUISING;
                    steps = calculate_steps_for_latency(gen->current_ticks);
                } else {
                    /* Continue decelerating - account for multiple steps per command
                     * dv = a * dt, where dt = steps / velocity
                     */
                    float v_change = accel * (float)steps / gen->current_velocity;

                    if (delta_v <= v_change) {
                        gen->current_velocity = target_vel;
                        gen->ramp_state = RAMP_CRUISING;
                    } else {
                        gen->current_velocity -= v_change;
                    }

                    /* Clamp to minimum (for stopping) */
                    if (target_vel < MIN_START_FREQ_HZ && gen->current_velocity < MIN_START_FREQ_HZ) {
                        cmd.flags |= CMD_FLAG_LAST;
                        break;
                    }

                    gen->current_ticks = RMT_RESOLUTION_HZ / (uint32_t)gen->current_velocity;
                    steps = calculate_steps_for_latency(gen->current_ticks);
                }

                cmd.ticks = gen->current_ticks;
                cmd.steps = steps;
            }
            break;
        }

        default:
            break;
    }

    /* Clamp steps to remaining (position mode) */
    if (position_mode && remaining < INT32_MAX && cmd.steps > 0) {
        if ((int32_t)cmd.steps > remaining) {
            cmd.steps = (uint8_t)remaining;
            cmd.flags |= CMD_FLAG_LAST;
        }
    }

    return cmd;
}

static int32_t calculate_decel_distance(float velocity, float decel)
{
    return (int32_t)((velocity * velocity) / (2.0f * decel));
}

static uint8_t calculate_steps_for_latency(uint32_t ticks)
{
    uint32_t freq = RMT_RESOLUTION_HZ / ticks;
    uint32_t max_steps = freq * POSITION_UPDATE_MS / 1000;

    if (max_steps < 1) max_steps = 1;
    if (max_steps > RMT_MAX_STEPS_PER_CMD) max_steps = RMT_MAX_STEPS_PER_CMD;

    return (uint8_t)max_steps;
}

static bool is_queue_full(const rmt_pulse_gen_t* gen)
{
    uint8_t rp = atomic_load(&gen->read_idx);
    uint8_t wp = atomic_load(&gen->write_idx);
    return ((wp + 1) & RMT_QUEUE_LEN_MASK) == rp;
}

static uint8_t queue_space(const rmt_pulse_gen_t* gen)
{
    uint8_t rp = atomic_load(&gen->read_idx);
    uint8_t wp = atomic_load(&gen->write_idx);
    return (rp - wp - 1) & RMT_QUEUE_LEN_MASK;
}

static bool push_command(rmt_pulse_gen_t* gen, const step_command_t* cmd)
{
    if (is_queue_full(gen)) {
        return false;
    }

    uint8_t wp = atomic_load(&gen->write_idx);
    gen->queue[wp] = *cmd;
    atomic_store(&gen->write_idx, (wp + 1) & RMT_QUEUE_LEN_MASK);

    /* Update queue end state */
    if (cmd->flags & CMD_FLAG_DIRECTION) {
        gen->queue_end.position += cmd->steps;
    } else {
        gen->queue_end.position -= cmd->steps;
    }
    gen->queue_end.ticks_queued += (uint32_t)cmd->ticks * cmd->steps;

    return true;
}

static void clear_queue(rmt_pulse_gen_t* gen)
{
    atomic_store(&gen->read_idx, 0);
    atomic_store(&gen->write_idx, 0);
    gen->queue_end.position = 0;
    gen->queue_end.direction = atomic_load(&gen->direction);
    gen->queue_end.ticks_queued = 0;
}

static void start_rmt_transmission(rmt_pulse_gen_t* gen)
{
    /* Fill initial commands */
    fill_queue(gen);

    /* Start RMT transmission */
    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
        .flags = {
            .eot_level = 0,
            .queue_nonblocking = 0,
        },
    };

    static uint8_t dummy = 0;  /* Must not be 'const' - RMT requires internal RAM, not flash */
    esp_err_t ret = rmt_transmit(gen->rmt_channel, gen->rmt_encoder, &dummy, sizeof(dummy), &tx_config);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start RMT transmission: %s", esp_err_to_name(ret));
        atomic_store(&gen->running, false);
        gen->ramp_state = RAMP_IDLE;
    }
}

/* ISR: Encoder callback */
static size_t IRAM_ATTR encode_callback(const void* data, size_t data_size,
                                        size_t symbols_written, size_t symbols_free,
                                        rmt_symbol_word_t* symbols, bool* done, void* arg)
{
    rmt_pulse_gen_t* gen = (rmt_pulse_gen_t*)arg;
    *done = false;
    s_encode_call_count++;


    if (symbols_free < RMT_PART_SIZE) {
        return 0;
    }

    /* Check if already stopped */
    if (atomic_load_explicit(&gen->rmt_stopped, memory_order_acquire)) {
        *done = true;
        return 0;
    }

    /* Check queue */
    uint8_t rp = atomic_load_explicit(&gen->read_idx, memory_order_acquire);
    uint8_t wp = atomic_load_explicit(&gen->write_idx, memory_order_acquire);

    /* Queue empty - fill pause and set stopped */
    if (rp == wp) {
        atomic_store_explicit(&gen->rmt_stopped, true, memory_order_release);
        for (size_t i = 0; i < RMT_PART_SIZE; i++) {
            symbols[i].val = 0;
        }
        symbols[0].duration0 = 100;
        symbols[0].level0 = 0;
        symbols[0].duration1 = 2;   // Must be non-zero to avoid EOF
        symbols[0].level1 = 0;
        return RMT_PART_SIZE;
    }

    /* Get current command */
    step_command_t* cmd = &gen->queue[rp & RMT_QUEUE_LEN_MASK];

    /* Fill symbols */
    size_t steps_generated = fill_symbols(gen, symbols, cmd);

    /* Update pulse count */
    if (steps_generated > 0) {
        bool dir = atomic_load_explicit(&gen->direction, memory_order_relaxed);
        int64_t delta = dir ? (int64_t)steps_generated : -(int64_t)steps_generated;
        atomic_fetch_add_explicit(&gen->pulse_count, delta, memory_order_relaxed);
    }

    /* Wake ramp task if queue running low */
    uint8_t queue_depth = (wp - rp) & RMT_QUEUE_LEN_MASK;
    if (queue_depth < 8 && gen->ramp_semaphore) {
        BaseType_t higher_priority_woken = pdFALSE;
        xSemaphoreGiveFromISR(gen->ramp_semaphore, &higher_priority_woken);
        if (higher_priority_woken) {
            portYIELD_FROM_ISR();
        }
    }

    return RMT_PART_SIZE;
}

/* ISR: Fill RMT symbols from command
 *
 * Single-symbol-per-step (50% duty cycle):
 * One RMT symbol = one step pulse. Unused symbols get idle fill.
 * Minimum frequency is ~244 Hz (16 MHz / 65535 ticks) — clamped to
 * MIN_START_FREQ_HZ (250 Hz) in generateNextCommand.
 */
static size_t IRAM_ATTR fill_symbols(rmt_pulse_gen_t* gen, rmt_symbol_word_t* symbols,
                                      step_command_t* cmd)
{
    uint8_t steps = cmd->steps;
    uint16_t ticks = cmd->ticks;

    /* Pause command */
    if (steps == 0) {
        gen->last_chunk_had_steps = false;
        uint16_t ticks_per_symbol = ticks / RMT_PART_SIZE;
        if (ticks_per_symbol < 2) ticks_per_symbol = 2;

        for (uint8_t i = 0; i < RMT_PART_SIZE; i++) {
            symbols[i].duration0 = ticks_per_symbol;
            symbols[i].level0 = 0;
            symbols[i].duration1 = ticks_per_symbol;
            symbols[i].level1 = 0;
        }

        atomic_store_explicit(&gen->read_idx,
            (atomic_load_explicit(&gen->read_idx, memory_order_relaxed) + 1) & RMT_QUEUE_LEN_MASK,
            memory_order_release);
        return 0;
    }

    gen->last_chunk_had_steps = true;

    /* Determine how many steps to generate this callback */
    uint8_t steps_to_do = steps;
    if (steps_to_do > RMT_PART_SIZE) {
        steps_to_do = RMT_PART_SIZE;
    }

    /* Calculate 50% duty cycle timing (integer math only) */
    uint16_t half_ticks_high = ticks >> 1;
    uint16_t half_ticks_low = ticks - half_ticks_high;

    /* Ensure minimum pulse width for stepper driver */
    if (half_ticks_high < 1) half_ticks_high = 1;
    if (half_ticks_low < 1) half_ticks_low = 1;
    if (half_ticks_high < RMT_HALF_TICK_MIN) half_ticks_high = RMT_HALF_TICK_MIN;
    if (half_ticks_low < RMT_HALF_TICK_MIN) half_ticks_low = RMT_HALF_TICK_MIN;

    if (steps_to_do < RMT_PART_SIZE) {
        /* Fill unused symbols with idle first */
        uint8_t idle_count = RMT_PART_SIZE - steps_to_do;

        for (uint8_t i = 0; i < idle_count; i++) {
            symbols[i].duration0 = 2;
            symbols[i].level0 = 0;
            symbols[i].duration1 = 2;
            symbols[i].level1 = 0;
        }

        /* Generate step pulses */
        for (uint8_t i = 0; i < steps_to_do; i++) {
            symbols[idle_count + i].duration0 = half_ticks_low;
            symbols[idle_count + i].level0 = 0;
            symbols[idle_count + i].duration1 = half_ticks_high;
            symbols[idle_count + i].level1 = 1;
        }
    } else {
        /* All PART_SIZE symbols are step pulses */
        for (uint8_t i = 0; i < RMT_PART_SIZE; i++) {
            symbols[i].duration0 = half_ticks_low;
            symbols[i].level0 = 0;
            symbols[i].duration1 = half_ticks_high;
            symbols[i].level1 = 1;
        }
    }

    /* Update command or advance */
    uint8_t remaining = steps - steps_to_do;

    /* Update ticks_queued */
    uint32_t consumed_ticks = (uint32_t)ticks * steps_to_do;
    if (gen->queue_end.ticks_queued >= consumed_ticks) {
        gen->queue_end.ticks_queued -= consumed_ticks;
    } else {
        gen->queue_end.ticks_queued = 0;
    }

    if (remaining == 0) {
        /* Command consumed - advance read index */
        atomic_store_explicit(&gen->read_idx,
            (atomic_load_explicit(&gen->read_idx, memory_order_relaxed) + 1) & RMT_QUEUE_LEN_MASK,
            memory_order_release);
    } else {
        /* Partial consumption */
        cmd->steps = remaining;
    }

    return steps_to_do;
}

/* ISR: Transmit done callback */
static bool IRAM_ATTR on_transmit_done(rmt_channel_handle_t channel,
                                        const rmt_tx_done_event_data_t* event_data,
                                        void* user_ctx)
{
    rmt_pulse_gen_t* gen = (rmt_pulse_gen_t*)user_ctx;

    atomic_store_explicit(&gen->running, false, memory_order_release);
    atomic_store_explicit(&gen->completion_pending, true, memory_order_release);

    /* Wake ramp task for completion callback */
    if (gen->ramp_semaphore) {
        BaseType_t higher_priority_woken = pdFALSE;
        xSemaphoreGiveFromISR(gen->ramp_semaphore, &higher_priority_woken);
        if (higher_priority_woken) {
            portYIELD_FROM_ISR();
        }
    }

    return false;
}
