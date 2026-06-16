# SPAM: Three-Phase Electricity Monitoring System (IoT Node)

![ESP32](https://img.shields.io/badge/ESP32-Espressif-blue)
![C++](https://img.shields.io/badge/Language-C++-00599C)
![Supabase](https://img.shields.io/badge/Database-Supabase-3ECF8E)

This repository contains the IoT hardware and embedded C++ codebase for **SPAM (Three-Phase Electricity Monitoring System)**. This system is designed to provide real-time monitoring of three-phase electrical loads (Voltage and Current) using an ESP32 microcontroller, and streams the data directly to a Supabase cloud database.

This IoT node is part of a larger full-stack project that utilizes Fuzzy Logic Sugeno to classify electrical load conditions (Underload, Normal, Overload) and visualizes the data via a web dashboard.

## 🚀 Features
* **Real-Time Data Acquisition:** Reads Voltage (Vrms) and Current (Irms) across 3 distinct phases simultaneously.
* **Direct-to-Cloud Integration:** Constructs JSON payloads and sends data directly to a Supabase PostgreSQL database via HTTP POST.
* **Auto-Reconnect:** Built-in failsafe to automatically reconnect to WiFi if the connection drops.
* **Non-Blocking Serial Output:** Easy debugging with real-time serial monitor readouts.

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 Development Board
* **Voltage Sensors:** 3x ZMPT101B AC Voltage Sensors
* **Current Sensors:** 3x SCT-013 Non-Invasive AC Current Sensors
* **Other:** Breadboard, jumper wires, and appropriate burden resistors for the current sensors (if not built into the sensor module).

### Pin Configuration
Ensure your sensors are wired to the ESP32 according to the following mapping:

| Phase | Sensor Type | ESP32 GPIO Pin | Variable in Code |
| :--- | :--- | :--- | :--- |
| **Line 1** | Current (SCT-013) | `GPIO 32` | `sct1` |
| **Line 1** | Voltage (ZMPT101B) | `GPIO 15` | `v1` |
| **Line 2** | Current (SCT-013) | `GPIO 33` | `sct2` |
| **Line 2** | Voltage (ZMPT101B) | `GPIO 2` | `v2` |
| **Line 3** | Current (SCT-013) | `GPIO 34` | `sct3` |
| **Line 3** | Voltage (ZMPT101B) | `GPIO 4` | `v3` |

## 📚 Software Dependencies
To compile and upload this code, you will need the Arduino IDE with the ESP32 board manager installed, along with the following libraries:
* `WiFi.h` (Built-in for ESP32)
* `HTTPClient.h` (Built-in for ESP32)
* [EmonLib](https://github.com/openenergymonitor/EmonLib) - For calculating Current RMS.
* [ZMPT101B](https://github.com/RobTillaart/ZMPT101B) (or similar compatible library) - For calculating Voltage RMS.

## ⚙️ Setup and Configuration

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/yourusername/SPAM-IoT-Node.git](https://github.com/yourusername/SPAM-IoT-Node.git)
