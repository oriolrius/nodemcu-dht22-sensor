# DHT22 NodeMCU IoT Sensor

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP8266-orange)](https://platformio.org/)
[![MQTT](https://img.shields.io/badge/Protocol-MQTT-purple)](https://mqtt.org/)

IoT temperature and humidity sensor using NodeMCU ESP8266 and DHT22. Features real-time MQTT connectivity, serial console control, and JSON data publishing.

## Features

- **Real-time monitoring** - Temperature and humidity readings every 5 seconds
- **MQTT connectivity** - Publish data to any MQTT broker
- **Dual control interface** - Control via Serial console or MQTT commands
- **JSON data format** - Easy integration with dashboards and analytics
- **NTP time sync** - Accurate timestamps on all readings
- **Remote control** - Start/stop sensor readings remotely

## Documentation

**[Student Guide](docs/README.md)** - Comprehensive guide for students and beginners covering:

- System architecture with diagrams
- Arduino programming fundamentals (`setup()` and `loop()`)
- Library explanations and code walkthrough
- Bill of Materials (BoM) and costs (~$20 total)
- Real-world IoT applications
- Glossary and FAQ

## Quick Start

### 1. Hardware Setup

| DHT22 Pin | NodeMCU Pin | Description |
| --------- | ----------- | ----------- |
| VCC (+)   | 3.3V        | Power       |
| DATA      | D2 (GPIO4)  | Data signal |
| GND (-)   | GND         | Ground      |

### 2. Configure Secrets

```bash
cp include/secrets.h.template include/secrets.h
```

Edit `include/secrets.h` with your credentials:

| Parameter         | Description                              |
| ----------------- | ---------------------------------------- |
| `WIFI_SSID`     | Your WiFi network name                   |
| `WIFI_PASSWORD` | Your WiFi password                       |
| `MQTT_HOST`     | MQTT broker (default: broker.emqx.io)    |
| `TIME_ZONE`     | Your timezone (e.g., "CET-1CEST,M3.5.0") |

### 3. Build and Upload

```bash
# Build and upload firmware
pio run --target upload

# Monitor serial output
pio device monitor
```

## Control Commands

Commands work via **Serial Console** (115200 baud) and **MQTT**:

| Command    | Description             |
| ---------- | ----------------------- |
| `help`   | Show available commands |
| `start`  | Start sensor readings   |
| `stop`   | Stop sensor readings    |
| `status` | Show system status      |

### MQTT Topics

| Topic                      | Direction | Description        |
| -------------------------- | --------- | ------------------ |
| `sensors/dht22/data`     | Publish   | Sensor data output |
| `sensors/dht22/commands` | Subscribe | Remote commands    |

## Data Format

### Sensor Reading (JSON)

```json
{
  "time": "2025-12-12T14:30:15Z",
  "humidity": 45.2,
  "temperature": 23.5
}
```

### Command Response (JSON)

```json
{
  "status": "stopped",
  "message": "Sensor readings stopped"
}
```

## Project Structure

```
nodemcu-dht22-sensor/
├── docs/
│   ├── README.md                # Student guide and documentation
│   └── assets/
│       ├── IoTdiagram.png       # System architecture diagram
│       └── CompleteFlow.png     # Data flow diagram
├── include/
│   ├── secrets.h.template       # Template for credentials
│   └── secrets.h                # Your credentials (gitignored)
├── src/
│   └── main.cpp                 # Main application code
├── platformio.ini               # PlatformIO configuration
├── LICENSE                      # MIT License
└── README.md                    # This file
```

## Hardware Requirements

| Component       | Approximate Cost     |
| --------------- | -------------------- |
| NodeMCU ESP8266 | $4 - $6              |
| DHT22 (AM2302)  | $3 - $5              |
| Breadboard      | $2 - $3              |
| Jumper wires    | $1 - $2              |
| Micro USB cable | $2 - $4              |
| **Total** | **~$15 - $20** |

## Free Services for Testing

| Service                                                         | Purpose             |
| --------------------------------------------------------------- | ------------------- |
| [broker.emqx.io](https://www.emqx.com/en/mqtt/public-mqtt5-broker) | Public MQTT broker  |
| [MQTTX Web](https://mqttx.app/web-client)                          | MQTT client/monitor |
| [Grafana Cloud](https://grafana.com/products/cloud/)               | Data visualization  |

## Security Notes

- `secrets.h` is excluded from version control via `.gitignore`
- Never commit real credentials to the repository
- Use the template file (`secrets.h.template`) for sharing code

## License

This project is licensed under the [MIT License](LICENSE) - ideal for learning and educational purposes.
