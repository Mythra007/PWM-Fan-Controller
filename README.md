# PWM-Fan-Controller
Designed a multi-modal Arduino system that adjusts fan speed, heater, servo, and buzzer dynamically based on temperature, with manual override and LCD-based real-time feedback.
# Advanced Temperature-Based PWM Fan Controller

This project implements a smart fan speed controller using **PWM**, a **temperature sensor (TMP36)**, **LCD display**, **servo motor**, **heater**, and a **buzzer**. It dynamically adjusts fan speed based on temperature, supports manual override, and provides real-time feedback through audio and visual indicators.

---

## Features

### Temperature-Based Automatic Mode
- Reads temperature from TMP36 sensor
- Controls fan speed via PWM (0–255) between 24°C–50°C
- Fan OFF below 24°C, runs at full speed above 50°C
- Visual representation of fan speed with servo motor (0°–90°)
- Activates heater when temp < 10°C
- Buzzer tone changes with cold (<10°C) or hot (>35°C) conditions

### Real-Time LCD Feedback
- I2C 16x2 LCD shows:
  - Current temperature (e.g., `Temp: 32.56`)
  - Fan PWM value and fan level (`Fan: 178 | L: 3`)
  - System status: `HOT`, `COOLING...`, `HEATING...`, `NORMAL`, or `FREEZE`
- LCD auto-updates every 2 seconds with dual-screen display

### Manual Override Mode
- Three push buttons allow fixed fan speeds:
  - **Button 1 (D6)** → Fan Level 1 → PWM 51 → Servo 18°
  - **Button 2 (D7)** → Fan Level 3 → PWM 153 → Servo 54°
  - **Button 3 (D8)** → Fan Level 5 → PWM 255 → Servo 90°
- Overrides automatic control
- Turns off heater and buzzer
- LCD displays “Manual Fan L:x” and “Manual Override”

---

## Hardware Used
- Arduino Uno
- TMP36 Temperature Sensor
- DC Fan (PWM controlled)
- Servo Motor
- I2C 16x2 LCD Display
- Buzzer
- Heater (Simulated via digital output or LED)
- Push Buttons (D6, D7, D8)
- Power Supply, Breadboard, Jumper Wires

---

## Working Logic

### Fan Control Logic:
| Temp (°C) | PWM Output | Fan Level | Note                          |
|-----------|-------------|-----------|-------------------------------|
| <10       | 0           | 0         | Heater ON, Buzzer (low tone) |
| 10–24     | 0           | 0         | Normal zone                  |
| 25–50     | 9–255       | 1–5       | PWM increases with temp      |
| >50       | 255         | 5         | Max cooling, Buzzer (high)   |

### Manual Mode Mapping:
| Button | Fan Level | PWM | Servo Angle |
|--------|-----------|-----|--------------|
| None   | Auto      | -   | -            |
| D6     | 1         | 51  | 18°          |
| D7     | 3         | 153 | 54°          |
| D8     | 5         | 255 | 90°          |

### Buzzer Behavior:
- **<10°C** → Low-pitch tone (800Hz → 300Hz)
- **10–35°C** → OFF
- **>35°C** → High-pitch tone (1000Hz → 2000Hz)

---

## Concepts Covered
- PWM signal generation
- Analog voltage-to-temperature conversion
- Dynamic value mapping (`map()`, `constrain()`)
- Servo control
- I2C LCD interfacing
- Button-based mode switching (manual override)
- Multi-modal feedback design



## Applications
- Smart ventilation systems
- Climate control in greenhouses
- Industrial cooling with user override
- Embedded systems prototyping


