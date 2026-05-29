/**
 * @file rmt_pulse_gen.h
 * @brief RMT-based pulse generator for stepper motor control
 *
 * Uses ESP32-P4 RMT peripheral with DMA for efficient pulse generation.
 * Implements trapezoidal velocity profiles with smooth acceleration/deceleration.
 * Supports both position moves and continuous velocity (jog) modes.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "driver/gpio.h"
#include "pcnt_tracker.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque RMT pulse generator handle
 */
typedef struct rmt_pulse_gen rmt_pulse_gen_t;

/**
 * @brief Motion completion callback
 *
 * @param user_data User context pointer
 * @param total_pulses Total pulses generated during motion
 */
typedef void (*motion_complete_cb_t)(void* user_data, int64_t total_pulses);

/**
 * @brief Create an RMT pulse generator
 *
 * @param[out] gen Pointer to store created generator handle
 * @param step_gpio GPIO pin for step pulse output
 * @param dir_gpio GPIO pin for direction output
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t rmt_pulse_gen_create(rmt_pulse_gen_t** gen, gpio_num_t step_gpio, gpio_num_t dir_gpio);

/**
 * @brief Destroy an RMT pulse generator
 *
 * @param gen Generator handle to destroy
 */
void rmt_pulse_gen_destroy(rmt_pulse_gen_t* gen);

/**
 * @brief Initialize the RMT hardware
 *
 * Configures RMT TX channel with DMA and encoder.
 * Creates ramp generator task.
 *
 * @param gen Generator handle
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t rmt_pulse_gen_init(rmt_pulse_gen_t* gen);

/**
 * @brief Start a position move
 *
 * Moves the specified number of pulses with trapezoidal velocity profile.
 * Positive pulses = forward, negative = reverse.
 *
 * @param gen Generator handle
 * @param pulses Number of pulses to move (signed for direction)
 * @param max_velocity Maximum velocity in pulses/second
 * @param acceleration Acceleration in pulses/second^2
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t rmt_pulse_gen_start_move(rmt_pulse_gen_t* gen, int32_t pulses,
                                    float max_velocity, float acceleration);

/**
 * @brief Start continuous velocity mode (jog)
 *
 * Runs at constant velocity until stop() is called.
 * Positive velocity = forward, negative = reverse.
 *
 * @param gen Generator handle
 * @param velocity Target velocity in pulses/second (signed for direction)
 * @param acceleration Acceleration in pulses/second^2
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t rmt_pulse_gen_start_velocity(rmt_pulse_gen_t* gen,
                                        float velocity, float acceleration);

/**
 * @brief Stop with controlled deceleration
 *
 * Decelerates to stop using the specified deceleration rate.
 *
 * @param gen Generator handle
 * @param deceleration Deceleration in pulses/second^2
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t rmt_pulse_gen_stop(rmt_pulse_gen_t* gen, float deceleration);

/**
 * @brief Immediate stop (no deceleration)
 *
 * Stops pulse generation immediately. Position may be inaccurate.
 *
 * @param gen Generator handle
 */
void rmt_pulse_gen_stop_immediate(rmt_pulse_gen_t* gen);

/**
 * @brief Check if generator is running
 *
 * @param gen Generator handle
 * @return true if motion is in progress
 */
bool rmt_pulse_gen_is_running(const rmt_pulse_gen_t* gen);

/**
 * @brief Get current velocity
 *
 * @param gen Generator handle
 * @return Current velocity in pulses/second
 */
float rmt_pulse_gen_get_velocity(const rmt_pulse_gen_t* gen);

/**
 * @brief Get total pulse count
 *
 * @param gen Generator handle
 * @return Total pulses generated (for internal tracking)
 */
int64_t rmt_pulse_gen_get_pulse_count(const rmt_pulse_gen_t* gen);

/**
 * @brief Set position tracker for feedback
 *
 * @param gen Generator handle
 * @param tracker Position tracker handle (can be NULL)
 */
void rmt_pulse_gen_set_tracker(rmt_pulse_gen_t* gen, pcnt_tracker_t* tracker);

/**
 * @brief Set motion completion callback
 *
 * @param gen Generator handle
 * @param cb Callback function
 * @param user_data User context pointer passed to callback
 */
void rmt_pulse_gen_set_callback(rmt_pulse_gen_t* gen, motion_complete_cb_t cb, void* user_data);

/**
 * @brief Debug: get encoder callback count
 *
 * @return Number of times the encoder callback was invoked
 */
uint32_t rmt_pulse_gen_get_encode_count(void);

#ifdef __cplusplus
}
#endif
