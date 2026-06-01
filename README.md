# RC Cart Controller

ESP32-S3 firmware for remote-controlling a gasoline cart/mini-buggy via standard hobby RC radio.

- 2x 150kg servos (throttle, brake) via 50Hz PWM
- 1x StepperOnline 400W step/dir servo (steering) via RMT
- 6-ch RC receiver input (GPIO edge interrupts)
- SH1106 128x64 OLED display (I2C)
- Engine starter + ignition kill control (active LOW for optocouplers)
- Built-in WS2812 RGB LED status indicator
- Failsafe on signal loss, E-stop support
- Steering enable — disables motor on signal loss

---

## Pinout (ESP32-S3-DevKit-C)

Physical order, top-to-bottom on each header.

### Left Header (J1) — Cart Connections

```
Pin#  GPIO  Dir   Function
─────────────────────────────
 1    3V3   —     (3.3V power rail)
 2    RST   —     (reset)
 3    4     OUT   STEP      Steering motor step pulse (RMT TX)
 4    5     OUT   DIR       Steering motor direction
 5    6     OUT   ENA       Steering motor enable (HIGH = on)
 6    7     OUT   Servo T   Throttle servo (LEDC, 50Hz)
 7    15    OUT   Servo B   Brake servo (LEDC, 50Hz)
 8    16    OUT   Ign Kill  Ignition kill relay (active LOW, optocoupler)
 9    17    OUT   Starter   Starter motor relay (active LOW, optocoupler)
10    18    IN    Lim R     Steering right limit switch (active LOW)
11    8     IN    Lim L     Steering left limit switch (active LOW)
12    3     —     ⛔ STRAPPING — do not use (JTAG)
13    46    —     ⛔ STRAPPING — do not use (boot log)
14    9     IN    RC Ch1    Steering stick (rightmost, easy access)
15    10    IN    RC Ch2    Spare
16    11    IN    RC Ch3    Throttle+Brake combined stick
17    12    IN    RC Ch4    Spare
18    13    IN    RC Ch5    Spare
19    14    IN    RC Ch6    Engine start/shutdown toggle
20    5V    —     (5V power rail)
```

### Right Header (J3) — Board Peripherals

```
Pin#  GPIO  Dir   Function
─────────────────────────────
 1    5V    —     (5V power rail)
 2    GND   —     (ground)
 3    1     IN    E-Stop    Emergency stop (active LOW, pulled up)
 4    2     IN    RPM       RPM sensor input (PCNT, when CFG_ENABLE_RPM=1)
 5    42    IN    Enc A     Rotary encoder phase A (CLK)
 6    41    IN    Enc B     Rotary encoder phase B (DT)
 7    40    IN    Enc Btn   Rotary encoder push button (SW)
 8    39    I/O   I2C SDA   SH1106 OLED
 9    38    I/O   I2C SCL   SH1106 OLED
10    37    —     ⛔ PSRAM   (octal PSRAM on WROOM-1)
11    36    —     ⛔ PSRAM
12    35    —     ⛔ PSRAM
13    0     —     ⛔ STRAPPING — do not use (boot mode)
14    GND   —     (ground)
15    EN    —     (chip enable)
16    48    OUT   RGB LED   Built-in WS2812 addressable LED
17    47    OUT   Lights    Lights relay (active HIGH)
18    21    —     free
19    20    —     ⛔ USB D+   (programming/monitor)
20    19    —     ⛔ USB D-   (programming/monitor)
```

---

## Wiring Diagrams

### Servo Connections

```
         ┌─────────────┐
 5V PSU ─┤+            │
 GND    ─┤- (thick)    │
         │  150kg Servo │
 ESP32   ─┤signal        │  (white/yellow wire)
 GPIO 7  ─┤  (Throttle) │
         └─────────────┘

         ┌─────────────┐
 5V PSU ─┤+            │
 GND    ─┤- (thick)    │
         │  150kg Servo │
 ESP32   ─┤signal        │
 GPIO 15 ─┤  (Brake)    │
         └─────────────┘
```

### StepperOnline Step/Dir Connections

```
ESP32-S3                           StepperOnline CL57T / iSV57T
┌────────────┐                    ┌─────────────────────┐
│ GPIO 4 ●─────── 1kΩ ─────● PUL+│  Step+               │
│ GND    ●──────────────────● PUL-│  Step-               │
│ GPIO 5 ●─────── 1kΩ ─────● DIR+│  Direction+          │
│ GND    ●──────────────────● DIR-│  Direction-          │
│ GPIO 6 ●─────── 1kΩ ─────● ENA+│  Enable+             │
│ GND    ●──────────────────● ENA-│  Enable-             │
│ 48VDC  ●──────────────────● V+  │  Motor power         │
│ GND    ●──────────────────● GND │                      │
└────────────┘                    └─────────────────────┘

NOTE: Use 1kΩ series resistors on PUL+/DIR+/ENA+ to limit current.
      GPIOs are 3.3V; use level shifters if drive requires 5V signaling.
      Always connect GND between ESP32 and drive.
      Steering enable: GPIO 6 HIGH = motor on, LOW = disabled (failsafe).
```

### RC Receiver Wiring

```
RC Receiver                         ESP32-S3
┌──────────┐                      ┌──────────┐
│ Ch1 (S)  │──────────────────────● GPIO 9   │  Steering
│ Ch2      │──────────────────────● GPIO 10  │  Spare
│ Ch3 (T/B)│──────────────────────● GPIO 11  │  Throttle+Brake combined
│ Ch4      │──────────────────────● GPIO 12  │  Spare
│ Ch5      │──────────────────────● GPIO 13  │  Spare
│ Ch6 (St) │──────────────────────● GPIO 14  │  Engine start/shutdown
│ GND      │──────────────────────● GND      │
│ +5V/BAT  │── (from receiver     │          │
│           │     own BEC/battery) │          │
└──────────┘                      └──────────┘

Standard RC PWM: 50Hz frame, 1.0-2.0ms pulse.
All 6 channels are read via GPIO edge interrupts (1us accuracy).
```

### RPM Sensor Circuit (Comparator, THT)

```
  Spark plug wire (HV insulation INTACT)
       │
       ┴   ┌──────── copper tape wrap (10-20mm wide, 1 turn around insulation)
       │   │  or 2-3 turns of thin wire wrapped
       └───┘
       │
       ├── R1 10kΩ ── GND
       │
       ├── D1 1N4148 ── GND   (anode to GND, cathode to signal → clamp negative)
       │
       ├── D2 1N4148 ── 3.3V  (anode to signal, cathode to 3.3V → clamp positive)
       │
       └── C1 100nF ── GND     (optional: small filtering)
       │
       └──────────────●── LM393N pin 3 (IN1+)
                      │
   LM393N (DIP-8):
                         ┌─────┐
   GPIO 16 ◀──────●──────┤1 OUT1│  8 ├── 3.3V (or 5V)
                  │      │2 IN1-│  7 ├── NC
   10kΩ to 3.3V   │      │3 IN1+│  6 ├── GND
                  │      │4 GND │  5 ├── NC
                  │      └─────┘
                  │
   ┌──────────────┘
   │  R2 100kΩ feedback (hysteresis, ~300mV window)
   │  Connected from pin 1 (OUT1) back to pin 3 (IN1+)
   │
   ├── R3 10kΩ pot wiper → pin 2 (IN1-)
   │   Pot ends: GND ↔ 3.3V    (set ~50-100mV threshold)
   │
   └── R4 10kΩ pullup → 3.3V  (LM393 open-collector output needs pullup)

  Twisted pair (signal + GND) for 1m run to ESP32.
  Add 1nF cap at GPIO 16 to GND for HF noise filtering.

  BOM (all through-hole):
    LM393N (DIP-8)        ×1
    1N4148 (DO-35)        ×2
    10kΩ resistor ¼W      ×3
    100kΩ resistor ¼W     ×1
    10kΩ trim pot          ×1
    100nF ceramic cap     ×1
    1nF ceramic cap       ×1 (at ESP32 end)
```

---

## E-Stop & Safety Wiring

```
         ┌────────┐
  3.3V ─┤ R 10kΩ ├──●── GPIO 1 (E-Stop)
         └────────┘  │
                     ├── NC contact ── GND   (push to OPEN)
                     │   (normally closed, opens on press)
                     │
         ┌────────┐  │
         │ Button │──┘
         └────────┘
```

---

## OLED Display Layout (128×64 pixels)

```
┌──────────────────────────────────────────────────┐
│ RUN  45%  <<                                     │  Row 0  — Status
│──────────────────────────────────────────────────│  Row 9  — divider
│ S ████████░░░░░░░░░░  -15°                       │  Row 11 — Steering bar + angle
│ T █████████████████░   85%                       │  Row 16 — Throttle bar + %
│ B ░░░░░░░░░░░░░░░░░░    0%                       │  Row 21 — Brake bar + %
│──────────────────────────────────────────────────│  Row 26 — divider
│ RPM: 2850                                        │  Row 28 — Engine RPM
│ ST:  ├────●───────────────────┤                  │  Row 37 — Steering graphic
│──────────────────────────────────────────────────│  Row 47 — divider
│ RC:OK  OFF                              v0.1.0   │  Row 49 — Status + version
└──────────────────────────────────────────────────┘

Legend:
  S/T/B — Steering/Throttle/Brake bars (0-100%)
  RPM  — Engine RPM (shown only if CFG_ENABLE_RPM=1)
  ST   — Steering angle gauge (— = left, ● = center, —● = right)
  RC   — Signal status: OK / --
  OFF  — Engine state: OFF / CRNK / RUN / COOL / STAL / ERR
```

---

## Steering Mechanical Reduction

| Stage | Ratio | Type |
|-------|-------|------|
| Timing belt | 20:72 (1:3.6) | Motor pulley : steering shaft pulley |
| Planetary reducer | 1:15 | Gearbox on motor output |
| **Total** | **1:54** | 54 motor revolutions per 1 steering shaft revolution |

---

## Servo Driver Configuration (DSY-RS Series)

The driver's electronic gear handles the 54:1 mechanical reduction. Firmware outputs
0.1° per step at the steering shaft (3600 pulses = 360° = 1 shaft revolution).

### Driver Parameters

| Parameter | Value | Meaning |
|-----------|-------|---------|
| **P00.00** | 1 | Position control mode |
| **P04.00** | 0 | Pulse command source (pulse+direction) |
| **P04.05** | 200 | Command pulses per motor revolution (numerator) |
| **P04.07** | 3 | Electronic gear numerator |
| **P04.09** | 1 | Electronic gear denominator |

### Calculation

```
Motor revs = command_pulses × P04.07 / (P04.09 × P04.05)
           = 3600 × 3 / (1 × 200)
           = 10800 / 200
           = 54 motor revs ← 1 shaft revolution  ✓

Resolution = 360° / 3600 pulses = 0.1° per pulse
```

### Wiring

```
ESP32 GPIO 4 (STEP) → PULSE+ (pin 9 on DIDO port)
ESP32 GPIO 5 (DIR)  → SIGH+  (pin 11 on DIDO port)
ESP32 GPIO 6 (ENA)  → DI1    (pin 3 — see servo enable note)
ESP32 GND            → PULSE- (pin 10), SIGH- (pin 12)
```

**Servo enable**: Either wire 24V to ICOM (pin 6) and DI1 (pin 3), or set the
software parameter trick (P02.01=0, P02.00=0x0001, then re-power).

---

## Configuration

All settings are in a single file: **`main/cart_config.h`**

### Quick Tuning Guide

| Parameter | Default | What to Change |
|-----------|---------|----------------|
| `STEERING_MAX_ANGLE_DEG` | 35 | Max steering angle from center |
| `STEERING_PULSES_PER_DEGREE` | 10 | 0.1° per step at shaft — match driver P04.05 if changing |
| `STEERING_MAX_SPEED_PPS` | 10000 | Lower if motor skips steps |
| `STEERING_ACCELERATION_PPS2` | 50000 | Lower if motor jerks on start |
| `STEERING_POS_DEADBAND_PULSES` | 2 | Stop corrections within this many pulses of target |
| `THROTTLE_IDLE_US` | 1000 | Servo idle pulse (carb fully closed) |
| `THROTTLE_FULL_US` | 2000 | Servo full-open pulse |
| `BRAKE_RELEASED_US` | 1000 | Brake off pulse |
| `BRAKE_FULL_US` | 2000 | Full brake pulse |
| `STARTER_MAX_CRANK_MS` | 3000 | Maximum cranking time |
| `RPM_RUNNING_THRESHOLD` | 300 | RPM above which engine is "running" |

### Compile-Time Feature Toggles

```c
#define CFG_ENABLE_RPM              0   // 1 = RPM sensor, 0 = no sensor
#define CFG_ENABLE_OLED             1   // 1 = SH1106 OLED, 0 = no display
#define CFG_ENABLE_STEERING_FEEDBACK 1  // 1 = PCNT closed-loop, 0 = open-loop
```

---

## Build & Flash

```bash
# One-time setup
idf.py set-target esp32s3
idf.py menuconfig   # Review: RMT, PCNT, LEDC, I2C enabled

# Build
idf.py build

# Flash (USB-CDC)
idf.py flash monitor

# Or via USB-UART
idf.py -p /dev/ttyUSB0 flash monitor
```

---

## RC Channel Calibration

1. Measure your RC receiver output with an oscilloscope.
2. Record min/center/max pulses for each channel:
   ```c
   #define RC_MIN_VALID_US   800    // Shortest valid pulse
   #define RC_CENTER_US      1500   // Neutral
   #define RC_MAX_VALID_US   2200   // Longest valid pulse
   ```
3. If using discrete switches (Ch4, Ch5), test threshold:
   ```c
   #define RC_DISCRETE_OFF_THRESHOLD_US  1300  // Below = OFF
   #define RC_DISCRETE_ON_THRESHOLD_US   1700  // Above = ON
   ```

Typical values:
- Futaba: 1100-1900us, center 1520us
- Spektrum: 1100-1900us, center 1500us
- FlySky: 1000-2000us, center 1500us
- FrSky: 988-2012us, center 1500us

---

## Architecture

```
     RC Receiver (50Hz PWM)
           │  Ch1-Steering
           │  Ch2-Throttle
           │  Ch3-Brake
           │  Ch4-Engine Start  (discrete)
           │  Ch5-Lights        (discrete)
           │
     ┌─────┴──────────────────┐
     │    ESP32-S3 (dual-core) │
     │                        │
     │  Core 0:               │
     │   RC Input (RMT RX)    │── ISR: capture 5 PWM channels
     │   Control Task (5ms)   │── Map RC → servo pulses, steering angle
     │   Safety Task (5ms)    │── E-stop, signal loss, failsafe
     │                        │
     │  Core 1:               │
     │   Display Task (100ms) │── SH1106 OLED refresh
     │   Ramp Task (5ms ISR)  │── Step pulse generation (RMT TX)
     │                        │
      │  Outputs:              │
      │   LEDC → Throttle (GPIO 7)  │── 50Hz servo PWM
      │   LEDC → Brake (GPIO 15)    │── 50Hz servo PWM
      │   RMT  → Steering (4/5/6)   │── Step/Dir + PCNT loopback
      │   RMT  → RGB LED (GPIO 48)  │── WS2812 built-in LED
      │   GPIO → Relays, Lights     │
      │                        │
      │  Inputs:               │
      │   GPIO → RC Ch1-6 (9-14)    │── 50Hz PWM edge ISR
      │   PCNT → Step count (GPIO 4)│── Closed-loop position feedback
      │   GPIO → E-Stop (1), Limits (8,18)
     └────────────────────────┘
```

### Failsafe Behavior (Signal Loss)

| Condition | Throttle | Brake | Steering | Engine | RGB LED |
|-----------|----------|-------|----------|--------|---------|
| RC signal OK | Normal | Normal | Normal | Normal | Green (running) / Blue (armed) |
| RC signal lost | 1000us (idle) | 2000us (full) | Disabled | Kill | Fast blink red |
| E-Stop pressed | Disabled | Disabled | Disabled | Kill | Solid red |
| Boot (no signal) | Idle | Full brake | Free | Off | Dim blue |

---

## License

MIT
