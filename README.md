# DHT22 NodeMCU Sensor Project

## 🚀 Quick Setup

### 1. Configure Secrets
Copy the template file and add your credentials:
```bash
cp include/secrets.h.template include/secrets.h
```

Then edit `include/secrets.h` with your actual values:
- **WIFI_SSID**: Your WiFi network name
- **WIFI_PASSWORD**: Your WiFi password  
- **MQTT_HOST**: Your MQTT broker (default: broker.emqx.io)
- **TIME_ZONE**: Your timezone (e.g., "EST5EDT", "PST8PDT")

### 2. Hardware Connections
- **DHT22 VCC** → NodeMCU 3.3V
- **DHT22 Data** → NodeMCU D2 (GPIO 4)
- **DHT22 GND** → NodeMCU GND

### 3. Build and Upload
```bash
platformio run --target upload
```

### 4. Monitor Serial Output
```bash
platformio device monitor
```

## 🎛️ Control Commands

### Serial Console (115200 baud)
- `help` or `h` - Show available commands
- `stop` - Stop sensor readings
- `start` - Resume sensor readings
- `status` - Show system status

### MQTT Commands
Send commands to: `sensors/dht22/commands`
Responses published to: `sensors/dht22/data`

Same commands as serial console.

## 📊 Data Format

### Sensor Data (JSON)
```json
{
  "time": "2025-10-20T14:30:15Z",
  "humidity": 45.2,
  "temperature": 23.5
}
```

### Command Responses (JSON)
```json
{
  "status": "stopped",
  "message": "Sensor readings stopped"
}
```

## 🔒 Security Notes

- `secrets.h` is excluded from version control
- Never commit real credentials
- Use the template file for sharing code

## 📁 Project Structure
```
├── include/
│   ├── secrets.h.template    # Template with placeholder values
│   └── secrets.h            # Your actual credentials (gitignored)
├── src/
│   └── main.cpp             # Main application code
├── platformio.ini           # PlatformIO configuration
└── README.md               # This file
```