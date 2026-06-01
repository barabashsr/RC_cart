/**
 * @file cart_config.h
 * @brief Master configuration header — ALL constants, pins, and tuning values
 *
 * This is the ONLY file you need to edit for hardware setup.
 * No magic numbers anywhere else in the codebase.
 * Every compile-time option is here.
 *
 * To disable a feature: comment out its #define or set the enable flag to 0.
 */

#pragma once

#include "driver/gpio.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * 1. FEATURE ENABLES (compile-time on/off)
 *===========================================================================*/

#define CFG_ENABLE_RPM                0       /* 1 = RPM sensor enabled, 0 = disabled */
#define CFG_ENABLE_OLED               1       /* 1 = OLED display enabled, 0 = disabled */
#define CFG_ENABLE_STEERING_FEEDBACK  1       /* 1 = position encoder on steering, 0 = open-loop step counting */

/*===========================================================================
 * 2. GPIO PIN ASSIGNMENTS
 *
 * Board layout (ESP32-S3-DevKit-C):
 *   Left header  — Cart connections: RC, motor, servos, relays, limit switches
 *   Right header — Board peripherals: I2C, encoder, safety, LED
 *===========================================================================*/

/* === Left header: Cart connections === */

/* Steering motor: Step/Dir/Enable (4,5,6 consecutive) */
#define GPIO_STEP                    4       /* Step pulse (RMT TX ch0) */
#define GPIO_STEP_DIR                5       /* Direction */
#define GPIO_STEP_ENABLE             6       /* Enable (HIGH = motor on) */

/* Servo PWM outputs */
#define GPIO_SERVO_THROTTLE          7       /* Throttle servo (LEDC) */
#define GPIO_SERVO_BRAKE             15      /* Brake servo (LEDC) */

/* RC receiver inputs (9-14 consecutive) */
#define GPIO_RC_CH1                  9       /* Ch1: Steering stick */
#define GPIO_RC_CH2                  10      /* Ch2: Spare */
#define GPIO_RC_CH3                  11      /* Ch3: Throttle+Brake combined stick */
#define GPIO_RC_CH4                  12      /* Ch4: Spare */
#define GPIO_RC_CH5                  13      /* Ch5: Spare */
#define GPIO_RC_CH6                  14      /* Ch6: Engine start/shutdown toggle */

/* Relay outputs */
#define GPIO_IGNITION_KILL           16      /* Ignition kill relay (active LOW) */
#define GPIO_STARTER_RELAY           17      /* Starter motor relay (active LOW) */

/* Limit switches (cart-mounted, left header) */
#define GPIO_LIMIT_LEFT              8       /* Steering left endstop (active LOW) */
#define GPIO_LIMIT_RIGHT             18      /* Steering right endstop (active LOW) */

/* === Right header: Board peripherals === */

/* RPM sensor */
#if CFG_ENABLE_RPM
#define GPIO_RPM_PULSE               2       /* RPM pulse input (PCNT, from comparator) */
#endif

/* E-Stop */
#define GPIO_ESTOP                   1       /* E-Stop (active LOW, pulled up) */

/* Rotary encoder (consecutive 40-42) */
#define GPIO_ENC_A                   42      /* Encoder phase A (CLK) */
#define GPIO_ENC_B                   41      /* Encoder phase B (DT) */
#define GPIO_ENC_BTN                 40      /* Encoder push button (SW, active LOW) */

/* I2C OLED (consecutive 38-39) */
#if CFG_ENABLE_OLED
#define GPIO_I2C_SDA                 39
#define GPIO_I2C_SCL                 38
#endif

/* Built-in WS2812 RGB LED */
#define GPIO_RGB_LED                 48      /* WS2812 addressable LED */
#define RGB_LED_RMT_CHANNEL          1       /* RMT channel (ch0 used for step gen) */

/* Lights relay (right header — freed left-side pins for limit switches) */
#define GPIO_LIGHTS_RELAY            47      /* Lights relay (active HIGH) */

/*===========================================================================
 * 3. RC RECEIVER CONFIGURATION
 *===========================================================================*/

#define RC_NUM_CHANNELS              6       /* Total RC channels to monitor */
#define RC_MAX_SUBSCRIPTIONS         12      /* Max callback subscriptions */
#define RC_EXPECTED_FRAME_MS         20      /* Standard 50Hz frame rate */

/* Pulse width thresholds (microseconds) */
#define RC_MIN_VALID_US              800     /* Below this = signal loss / no signal */
#define RC_MAX_VALID_US              2200    /* Above this = invalid / glitch */
#define RC_CENTER_US                 1530    /* Neutral position (hardware default, calibrated at boot) */
#define RC_CALIBRATE_SAMPLES         50      /* Samples collected for steering center auto-calibration */
#define RC_CALIBRATE_TIMEOUT_MS      2000    /* Max wait for valid signal during calibration */

/* Discrete channel thresholds */
#define RC_DISCRETE_OFF_THRESHOLD_US 1300    /* Below this = switch OFF */
#define RC_DISCRETE_ON_THRESHOLD_US  1700    /* Above this = switch ON */
#define RC_DISCRETE_DEBOUNCE_MS      100     /* Must hold state this long to register */

/* Signal loss detection */
#define RC_SIGNAL_LOSS_TIMEOUT_MS    50      /* No valid pulse within this = signal lost */
#define RC_SIGNAL_RECOVERY_MS        200     /* Must have valid signal this long to recover */

/* Deadband per channel type */
#define RC_DEADBAND_STICK_US         40      /* Analog stick: ignore jitter below this */
#define RC_DEADBAND_TOGGLE_US        200     /* Discrete switch: ignore jitter below this */

/*=== Channel indices (0-based, matching GPIO_RC_CHx array order) ===*/
#define RC_IDX_STEERING              0       /* Ch1 — steering stick */
#define RC_IDX_THROTTLE_BRAKE        2       /* Ch3 — combined throttle+brake stick */
#define RC_IDX_STARTER               5       /* Ch6 — engine start/shutdown toggle */

/*===========================================================================
 * 4. SERVO CONFIGURATION
 *===========================================================================*/

/* MCPWM timer: 1 MHz resolution = 1 us per tick, 20 ms period = 50 Hz */
#define SERVO_TIMEBASE_RESOLUTION_HZ  1000000
#define SERVO_TIMEBASE_PERIOD_TICKS   20000

/* Servo timing per manufacturer spec: 0°=0.5ms, 90°=1.5ms, 180°=2.5ms */
#define SERVO_IDLE_PULSE_US          500     /* Pulse at 0° (0.5ms) */
#define SERVO_SPAN_180DEG_US         2000    /* 2500-500 = 2000 us for full 180° */
#define SERVO_ANGLE_TO_US(deg)  (SERVO_IDLE_PULSE_US + (uint32_t)(deg) * SERVO_SPAN_180DEG_US / 180)

/* Servo hardware limits */
#define SERVO_HW_MIN_PULSE_US         500     /* 0° */
#define SERVO_HW_MAX_PULSE_US         2500    /* 180° */

/* ----- Throttle servo (all in degrees, tune here) ----- */
#define THROTTLE_IDLE_ANGLE_DEG       0       /* Carb closed */
#define THROTTLE_FULL_ANGLE_DEG       45      /* WOT */
#define THROTTLE_FAILSAFE_ANGLE_DEG   0       /* Idle on signal loss */

/* ----- Brake servo (all in degrees, tune here) ----- */
#define BRAKE_RELEASED_ANGLE_DEG      0       /* No braking */
#define BRAKE_FULL_ANGLE_DEG          45      /* Full brake */
#define BRAKE_FAILSAFE_ANGLE_DEG      BRAKE_FULL_ANGLE_DEG   /* Full brake on signal loss */

/* ----- Derived pulse widths (internal) ----- */
#define THROTTLE_IDLE_US              SERVO_ANGLE_TO_US(THROTTLE_IDLE_ANGLE_DEG)
#define THROTTLE_FULL_US              SERVO_ANGLE_TO_US(THROTTLE_FULL_ANGLE_DEG)
#define BRAKE_RELEASED_US             SERVO_ANGLE_TO_US(BRAKE_RELEASED_ANGLE_DEG)
#define BRAKE_FULL_US                 SERVO_ANGLE_TO_US(BRAKE_FULL_ANGLE_DEG)
#define FAILSAFE_THROTTLE_US          SERVO_ANGLE_TO_US(THROTTLE_FAILSAFE_ANGLE_DEG)
#define FAILSAFE_BRAKE_US             SERVO_ANGLE_TO_US(BRAKE_FAILSAFE_ANGLE_DEG)

/* Hysteresis for throttle/brake zone switching (Schmitt trigger) */
#define SERVO_HYST_ENTER_US          40      /* Must be this far from center to leave idle */
#define SERVO_HYST_EXIT_US           20      /* Must be this close to center to return to idle */

/* Minimum RC change to fire callback (filters electrical noise only) */
#define RC_CALLBACK_NOISE_US         3       /* 3 us — below typical RC jitter */

/* Calibration */
#define CALIBRATE_AXIS_DURATION_MS   3000    /* Sample each axis for 3 seconds */
#define CALIBRATE_NVS_NAMESPACE      "rc_calib"

/* Servo ramp: limits rate of change to prevent jerks and power spikes */
#define SERVO_RAMP_US_PER_SEC        3000    /* 3000 us/s — full brake release in ~200ms */
#define SERVO_RAMP_STEP              ((SERVO_RAMP_US_PER_SEC) * TASK_PERIOD_CONTROL_MS / 1000)  /* 15 us/tick */

/*===========================================================================
 * 5. STEERING MOTOR CONFIGURATION (StepperOnline Step/Dir)
 *
 * 0.1° resolution at the steering shaft. Mechanical reduction (54:1 belt +
 * planetary) is handled by the driver's P04.05 / P04.07 / P04.09 electronic
 * gear — our firmware just outputs pulses at 0.1° per step.
 *===========================================================================*/

#define STEERING_PULSES_PER_DEGREE    10      /* 10 pulses/deg = 0.1° per step at shaft */

#define STEERING_MAX_ANGLE_DEG        35      /* Maximum steering angle from center (± this many degrees) */
#define STEERING_MAX_PULSES           (int32_t)(STEERING_MAX_ANGLE_DEG * STEERING_PULSES_PER_DEGREE)

/* Motion dynamics — scaled for 0.1° resolution (~700 pulses full range) */
#define STEERING_MAX_SPEED_PPS        10000   /* 1000 deg/s = full range in 70 ms */
#define STEERING_ACCELERATION_PPS2    50000   /* Reach top speed in 200 ms */
#define STEERING_DECELERATION_PPS2    50000   /* Same as accel */

/* Closed-loop position deadband (pulses) */
#define STEERING_POS_DEADBAND_PULSES  2       /* Stop corrective moves within 0.2° of target */

/* Endstop behavior */
#define STEERING_LIMIT_ACTIVE_LEVEL   0       /* 0 = active LOW, pulled up internally */

/*===========================================================================
 * 6. ENGINE CONFIGURATION (Starter + Ignition + RPM)
 *===========================================================================*/

/* Starter motor */
#define STARTER_MAX_CRANK_MS         3000    /* Maximum time starter may be engaged */
#define STARTER_COOLDOWN_MS          5000    /* Wait time before allowing re-crank */
#define STARTER_RELAY_ACTIVE_LEVEL   0       /* 0 = LOW engages starter relay (drives optocoupler) */

/* Ignition kill */
#define IGNITION_KILL_PULSE_MS       500     /* Pulse duration to kill engine */
#define IGNITION_KILL_ACTIVE_LEVEL   0       /* 0 = LOW kills engine (drives optocoupler) */

/* RPM measurement */
#if CFG_ENABLE_RPM
#define RPM_SIGNAL_ACTIVE_EDGE       1       /* 1 = count on rising edge, 0 = falling edge */
#define RPM_PULSES_PER_REV           1       /* 1 spark per 2 revolutions (4-stroke) */
#define RPM_SPARKS_PER_REV           0.5f    /* 4-stroke: 1 spark per 2 revolutions */
#define RPM_MEASUREMENT_WINDOW_MS    250     /* Measurement time window */
#define RPM_UPDATE_INTERVAL_MS       250     /* How often to publish new RPM value */
#define RPM_RUNNING_THRESHOLD        300     /* Engine considered running above this RPM */
#define RPM_RUNNING_CONFIRM_MS       500     /* Must stay above threshold for this long */
#define RPM_STALL_TIMEOUT_MS         2000    /* No pulses for this long -> engine stalled */
#endif

/*===========================================================================
 * 7. SAFETY CONFIGURATION
 *===========================================================================*/

#define SAFETY_CHECK_INTERVAL_MS     5       /* Main safety loop period */
#define SAFETY_ESTOP_DEBOUNCE_MS     20      /* E-stop button debounce */

/* Failsafe: what happens on signal loss */
#define FAILSAFE_RETURN_TO_CENTER    1       /* 1 = return to center on signal loss */

/*===========================================================================
 * 8. OLED DISPLAY CONFIGURATION (SH1106 128x64, I2C)
 *===========================================================================*/

#if CFG_ENABLE_OLED
#define OLED_I2C_ADDR                0x3C    /* SH1106 7-bit I2C address */
#define OLED_I2C_PORT                I2C_NUM_0
#define OLED_I2C_FREQ_HZ             100000  /* 100 kHz — works with internal pull-ups */
#define OLED_WIDTH                   128
#define OLED_HEIGHT                  64
#define OLED_PAGES                   8       /* 64/8 = 8 pages */
#define OLED_REFRESH_MS              100     /* Display update interval */
#define OLED_TIMEOUT_MS              30000   /* Screen off after 30s inactivity */
#endif

/*===========================================================================
 * 9. RMT PERIPHERAL CONFIGURATION
 *===========================================================================*/

/* RMT TX (step pulse generation) */
#define RMT_RESOLUTION_HZ            16000000    /* 16 MHz = 62.5 ns per tick */
#define RMT_TX_CHANNEL               0           /* RMT channel for step generation */
#define RMT_MEM_BLOCK_SYMBOLS        64          /* RMT memory block size (S3: 64 symbols) */
#define RMT_QUEUE_LEN                32          /* Command queue length */
#define RMT_PART_SIZE                24          /* Symbols per encoder callback */
#define RMT_MAX_STEPS_PER_CMD        255         /* Max steps per command */
#define RMT_MAX_SYMBOL_TICKS         32000       /* Max ticks per RMT symbol (15-bit limit) */
#define RMT_MIN_CMD_TICKS            3200

/*===========================================================================
 * 10. FREERTOS TASK CONFIGURATION
 *===========================================================================*/

#define TASK_STACK_CONTROL           4096
#define TASK_PRIO_CONTROL            8
#define TASK_PERIOD_CONTROL_MS       5

#define TASK_STACK_SAFETY            2048
#define TASK_PRIO_SAFETY             10
#define TASK_PERIOD_SAFETY_MS        5

#define TASK_STACK_DISPLAY           3072
#define TASK_PRIO_DISPLAY            2
#define TASK_PERIOD_DISPLAY_MS       100

#define TASK_STACK_RAMP              4096
#define TASK_PRIO_RAMP              (configMAX_PRIORITIES - 2)

/* Core pinning */
#define CORE_CONTROL                 0
#define CORE_RAMP                    1
#define CORE_DISPLAY                 1

/*===========================================================================
 * 11. SYSTEM CONSTANTS
 *===========================================================================*/

#define FIRMWARE_VERSION             "0.2.0"
#define FIRMWARE_NAME                "RC Cart Controller"

/*===========================================================================
 * 12. COMPILE-TIME VALIDATION
 *===========================================================================*/

_Static_assert(STEERING_MAX_ANGLE_DEG > 4 && STEERING_MAX_ANGLE_DEG < 91,
               "STEERING_MAX_ANGLE_DEG must be between 5 and 90");
_Static_assert(SERVO_HW_MIN_PULSE_US >= 50 && SERVO_HW_MAX_PULSE_US <= 2500,
               "Servo pulse range looks suspicious. Standard is 50-2500 us");

#ifdef __cplusplus
}
#endif
