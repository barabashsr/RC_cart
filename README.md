# RC Cart Controller

ESP32-S3 firmware for remote-controlling a gasoline cart/mini-buggy via standard hobby RC radio.

- 2× 150kg servos (throttle, brake) via 50Hz PWM
- 1× StepperOnline 400W step/dir servo (steering) via RMT
- 5-ch RC receiver input (RMT RX) + optional RPM sensor (PCNT)
- SH1106 128x64 OLED display (I2C)
- Engine starter + ignition kill control
- Failsafe on signal loss, E-stop support

---

## Pinout

```
ESP32-S3                        Function
┌───────────────────┐
│ GPIO  4 → STEP   │──▶ StepperOnline PUL+ (steering motor step pulse)
│ GPIO  5 → DIR    │──▶ StepperOnline DIR+ (steering motor direction)
│ GPIO  6 → Servo1 │──▶ Throttle servo signal (LEDC, 50Hz)
│ GPIO  7 → Servo2 │──▶ Brake servo signal (LEDC, 50Hz)
│ GPIO  8 → Relay1 │──▶ Starter motor relay (active HIGH)
│ GPIO  9 → RC Ch1 │──◀ RC receiver Ch1 (steering stick)
│ GPIO 10 → RC Ch2 │──◀ RC receiver Ch2 (throttle stick)
│ GPIO 11 → RC Ch3 │──◀ RC receiver Ch3 (brake)
│ GPIO 12 → RC Ch4 │──◀ RC receiver Ch4 (engine START switch, discrete)
│ GPIO 13 → RC Ch5 │──◀ RC receiver Ch5 (lights / spare)
│ GPIO 14 → Relay2 │──▶ Ignition kill relay (active HIGH)
│ GPIO 15 → Relay3 │──▶ Lights relay (active HIGH)
│ GPIO 16 → RPM    │──◀ RPM comparator input (ADXL/PCNT)
│ GPIO 17 → SDA    │─── SH1106 OLED SDA
│ GPIO 18 → SCL    │─── SH1106 OLED SCL
│ GPIO 21 → E-Stop │──◀ Emergency stop button (active LOW, pull-up)
│ GPIO 38 → LED    │──▶ Status LED (active HIGH)
│ GPIO 47 → Lim L  │──◀ Steering left limit switch (active LOW)
│ GPIO 48 → Lim R  │──◀ Steering right limit switch (active LOW)
│                   │
│ GND ──────────────│── Common ground for all peripherals
│ 3.3V ─────────────│── Logic supply (do NOT power servos from this!)
│ 5V ───────────────│── Servo power supply (separate high-current 5V)
└───────────────────┘
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
GPIO 6  ─┤  (Throttle) │
         └─────────────┘

         ┌─────────────┐
 5V PSU ─┤+            │
 GND    ─┤- (thick)    │
         │  150kg Servo │
ESP32   ─┤signal        │
GPIO 7  ─┤  (Brake)    │
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
│ GPIO   ●─────── (opt) ────● ENA+│  Enable+ (optional)  │
│ GND    ●──────────────────● ENA-│  Enable-             │
│ 48VDC  ●──────────────────● V+  │  Motor power         │
│ GND    ●──────────────────● GND │                      │
└────────────┘                    └─────────────────────┘

NOTE: Use 1kΩ series resistors on PUL+/DIR+ to limit current.
      GPIOs are 3.3V; use level shifters if drive requires 5V signaling.
      Always connect GND between ESP32 and drive.
```

### RC Receiver Wiring

```
RC Receiver                         ESP32-S3
┌──────────┐                      ┌──────────┐
│ Ch1 (S)  │──────────────────────● GPIO 9   │
│ Ch2 (T)  │──────────────────────● GPIO 10  │
│ Ch3 (B)  │──────────────────────● GPIO 11  │
│ Ch4 (St) │──────────────────────● GPIO 12  │
│ Ch5 (L)  │──────────────────────● GPIO 13  │
│ GND      │──────────────────────● GND      │
│ +5V/BAT  │── (from receiver     │          │
│           │     own BEC/battery) │          │
└──────────┘                      └──────────┘

Standard RC PWM: 50Hz frame, 1.0-2.0ms pulse.
Center = 1.5ms (neutral).
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
  3.3V ─┤ R 10kΩ ├──●── GPIO 21 (E-Stop)
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

## Configuration

All settings are in a single file: **`main/cart_config.h`**

### Quick Tuning Guide

| Parameter | Default | What to Change |
|-----------|---------|----------------|
| `STEERING_MICROSTEP` | 16 | Match DIP switches on your StepperOnline drive |
| `STEERING_REDUCER_RATIO` | 5.0 | Your gear reducer ratio |
| `STEERING_BELT_RATIO` | 2.0 | Timing belt pulley ratio |
| `STEERING_MAX_ANGLE_DEG` | 35.0 | Max steering angle |
| `STEERING_MAX_SPEED_PPS` | 200000 | Lower if motor skips steps |
| `THROTTLE_IDLE_US` | 1000 | Servo idle pulse (carb fully closed) |
| `THROTTLE_FULL_US` | 2000 | Servo full-open pulse |
| `BRAKE_RELEASED_US` | 1000 | Brake off pulse |
| `BRAKE_FULL_US` | 2000 | Full brake pulse |
| `STARTER_MAX_CRANK_MS` | 3000 | Maximum cranking time |
| `RPM_RUNNING_THRESHOLD` | 300 | RPM above which engine is "running" |

### Compile-Time Feature Toggles

```c
#define CFG_ENABLE_RPM              1   // 1 = RPM sensor, 0 = no sensor
#define CFG_ENABLE_OLED             1   // 1 = SH1106 OLED, 0 = no display
#define CFG_ENABLE_STEERING_FEEDBACK 0  // 1 = encoder feedback, 0 = open-loop
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
     │   LEDC Ch0 → Throttle  │── 50Hz servo PWM
     │   LEDC Ch1 → Brake     │── 50Hz servo PWM
     │   RMT TX  → Steering   │── Step/Dir pulses
     │   GPIO   → Starter relay, Ignition kill, Lights
     │                        │
     │  Inputs:               │
     │   PCNT   → RPM pickup (Schmitt comparator)
     │   GPIO   → E-Stop, Limit switches
     └────────────────────────┘
```

### Failsafe Behavior (Signal Loss)

| Condition | Throttle | Brake | Steering | Engine | LEDs |
|-----------|----------|-------|----------|--------|------|
| RC signal OK | Normal | Normal | Normal | Normal | Slow blink |
| RC signal lost | 1000us (idle) | 2000us (full) | Hold | Kill | Fast blink |
| E-Stop pressed | Disabled | Disabled | Disabled | Kill | Solid on |
| Boot (no signal) | Idle | Full brake | Center | Off | Slow blink |

---

## License

MIT
