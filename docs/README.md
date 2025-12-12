# 📚 Student Guide

## 🌡️ Project: IoT Temperature and Humidity Sensor

This document is designed for students who want to understand how an IoT (Internet of Things) device works without needing deep technical knowledge.

---

## 🎯 What does this project do?

This project turns a small electronic device into a **smart sensor connected to the Internet** that:

1. **Measures** temperature and humidity every 5 seconds
2. **Sends** this data automatically to the Internet
3. **Receives** remote commands to control it
4. **Publishes** data in a format that any application can read

> 💡 **Marketing Analogy**: Imagine an automated "brand ambassador" that constantly reports information about its environment and responds to instructions from headquarters.

---

## 🏗️ System Architecture

### General Diagram

<p align="center">
  <img src="assets/IoTdiagram.png" alt="IoT System Architecture" width="80%">
</p>

### Data Flow

<p align="center">
  <img src="assets/CompleteFlow.png" alt="Data Flow Diagram" width="80%">
</p>

---

## 🧩 System Components

### 1. 📡 NodeMCU - The "Brain" of the Device

**What is it?** A small computer the size of a credit card with built-in WiFi.

**Marketing Analogy**: It's like an **automated Community Manager** - it receives information, processes it, and publishes it on the "social network" (in this case, the MQTT server).

### 2. 🌡️ DHT22 Sensor - The "Data Collector"

**What is it?** A sensor that measures air temperature and humidity.

**Marketing Analogy**: It's like an **automatic surveyor** constantly collecting data from the "market" (the physical environment).

### 3. 📨 MQTT - The "Messaging System"

**What is it?** A lightweight communication protocol designed for IoT devices.

**Marketing Analogy**: It works like **a corporate WhatsApp group**:

- The device "publishes" messages to a "channel" (topic)
- Any "subscribed" application automatically receives those messages
- It can also receive "orders" from other applications

---

## 📊 Data Format

### What does the sent data look like?

The sensor sends information in **JSON** format (a standard Internet format):

```json
{
  "time": "2025-12-12T14:30:15Z",
  "humidity": 45.2,
  "temperature": 23.5
}
```

| Field           | Meaning                        | Example              |
| --------------- | ------------------------------ | -------------------- |
| `time`        | Exact date and time of reading | December 12, 2:30 PM |
| `humidity`    | Relative air humidity in %     | 45.2%                |
| `temperature` | Temperature in Celsius         | 23.5°C              |

> 💡 **For Marketing**: This format allows easy integration of data into dashboards, spreadsheets, or analytics systems like Google Data Studio.

---

## 📚 Libraries: The Building Blocks

### What are Libraries?

Think of libraries as **pre-built tools** that save you from writing everything from scratch. Instead of coding how to connect to WiFi from zero, you use a library that already knows how to do it!

**Marketing Analogy**: Libraries are like using **Canva templates** instead of designing everything in Photoshop from scratch - faster and easier!

### Libraries Used in Our Project

At the top of our code, you'll see these **includes**:

```cpp
#include <Arduino.h>       // Basic Arduino functions
#include <ESP8266WiFi.h>   // WiFi connection
#include <PubSubClient.h>  // MQTT messaging
#include <ArduinoJson.h>   // JSON data creation
#include <ESPDateTime.h>   // Date and time
#include "DHT.h"           // DHT22 sensor reading
```

**What each library does:**

| Library            | Purpose                  | Example Use                               |
| ------------------ | ------------------------ | ----------------------------------------- |
| `Arduino.h`      | Core Arduino functions   | `Serial.println()`, `delay()`         |
| `ESP8266WiFi.h`  | WiFi connectivity        | Connect to your home/office WiFi          |
| `PubSubClient.h` | MQTT communication       | Send and receive messages via MQTT        |
| `ArduinoJson.h`  | JSON data formatting     | Create `{"temperature": 23.5}` messages |
| `ESPDateTime.h`  | Time and date management | Get current time from Internet            |
| `DHT.h`          | Read DHT22 sensor        | Get temperature and humidity readings     |

> 💡 **For Students**: These libraries are **free and open source**. Thousands of developers contribute to make IoT programming easier!

---

## 🧩 Connecting Everything Together

### The Complete Picture

Here's how all the pieces work together:

```cpp
// 1. INCLUDE LIBRARIES (Get the tools)
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// 2. CONFIGURE HARDWARE (Define what pins to use)
#define DHTPIN 4          // Sensor connected to GPIO 4 (D2)
#define DHTTYPE DHT22     // Sensor type: DHT22

// 3. CREATE OBJECTS (Prepare the tools)
DHT dht(DHTPIN, DHTTYPE);           // Temperature sensor
WiFiClient net;                      // WiFi connection
PubSubClient client(net);            // MQTT client

// 4. SETUP (Run once at startup)
void setup() {
  Serial.begin(115115);              // Start console
  connectWiFi();                     // Connect to Internet
  connectMQTT();                     // Connect to MQTT server
  dht.begin();                       // Start sensor
}

// 5. LOOP (Run forever)
void loop() {
  float h = dht.readHumidity();      // Read humidity
  float t = dht.readTemperature();   // Read temperature
  
  publishMessage();                  // Send to Internet
  delay(5000);                       // Wait 5 seconds
}
```

**Step-by-step flow:**

1. **Include libraries** → Get all the tools you need
2. **Configure hardware** → Tell the program where things are connected
3. **Create objects** → Prepare instances of WiFi, MQTT, and sensor
4. **Setup** → Initialize everything once
5. **Loop** → Do the main work repeatedly

---

## 🎯 Why This Approach is Powerful

### Advantages of Arduino Programming

| Advantage                  | Description                                          | Marketing Parallel                      |
| -------------------------- | ---------------------------------------------------- | --------------------------------------- |
| **Simplicity**       | No need to understand low-level electronics          | Like using WordPress vs coding in HTML  |
| **Libraries**        | Reusable code from community                         | Using templates instead of from scratch |
| **Clear Structure**  | `setup()` and `loop()` are intuitive             | Planning phase vs. execution phase      |
| **Active Community** | Millions of examples and tutorials online            | Like having a huge marketing community  |
| **Cross-platform**   | Same code works on many different Arduino boards     | Write once, deploy everywhere           |
| **Open Source**      | Free tools, free libraries, free to share and modify | Like Creative Commons for code          |

---

## 💻 Understanding Arduino Programming

### The Two Essential Functions: `setup()` and `loop()`

Every Arduino program (called a "sketch") has **two main functions** that are mandatory. Think of them as the **foundation** of your program:

#### 🏁 `setup()` - The Initialization Phase

**What it does:** Runs **ONCE** when the device powers on or resets.

**Purpose:** Prepare everything the device needs before starting its main work.

**Marketing Analogy**: Like the **morning briefing** before starting the work day - you configure your tools, check connections, and prepare your workspace.

**Simplified Example from Our Project:**

```cpp
void setup() {
  Serial.begin(115200);              // 1. Open communication via USB
  
  connectWiFi();                     // 2. Connect to WiFi network
  NTPConnect();                      // 3. Sync time with Internet
  connectMQTT();                     // 4. Connect to MQTT server
  dht.begin();                       // 5. Initialize the sensor
  
  Serial.println("System ready!");   // 6. Confirm everything is ready
  showHelp();                        // 7. Show available commands
}
```

**What each line does:**

1. **`Serial.begin(115200)`** - Opens the "conversation" between the device and your computer (like opening WhatsApp Web)
2. **`connectWiFi()`** - Connects to your WiFi network (like connecting your phone to WiFi)
3. **`NTPConnect()`** - Gets the correct time from Internet servers (like syncing your phone's clock)
4. **`connectMQTT()`** - Connects to the messaging server (like logging into Slack)
5. **`dht.begin()`** - Prepares the temperature sensor to work (like turning on your thermometer)
6. **`Serial.println()`** - Prints a message to confirm all is ready
7. **`showHelp()`** - Shows what commands you can use

> 💡 **Important**: `setup()` runs only once! If you need something to happen repeatedly, it goes in `loop()`.

---

#### 🔄 `loop()` - The Continuous Work Cycle

**What it does:** Runs **FOREVER** in a continuous cycle, over and over again.

**Purpose:** This is where the main work happens - reading sensors, sending data, checking for commands.

**Marketing Analogy**: Like your **daily routine** that repeats every day - check email, respond to messages, publish content, check email again, and repeat.

**Simplified Example from Our Project:**

```cpp
void loop() {
  // 1. Check if there are commands from the computer
  handleSerialCommands();
  
  // 2. Maintain MQTT connection (reconnect if disconnected)
  if (!client.connected()) {
    connectMQTT();
  } else {
    client.loop();  // Check for incoming MQTT messages
  }
  
  // 3. If sensor is active, read and publish data
  if (sensorActive) {
    h = dht.readHumidity();        // Read humidity
    t = dht.readTemperature();     // Read temperature
  
    // Print to console
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("°C  Humidity: ");
    Serial.print(h);
    Serial.println("%");
  
    publishMessage();              // Send data to Internet
    delay(5000);                   // Wait 5 seconds
  } else {
    delay(100);                    // If stopped, just wait briefly
  }
}
```

**The loop cycle explained:**

1. **Check for commands** - Did someone send a command via USB or MQTT?
2. **Maintain connections** - Is MQTT still connected? If not, reconnect.
3. **Read sensor** - Get current temperature and humidity
4. **Display data** - Show readings on the console
5. **Publish to Internet** - Send data to the MQTT server
6. **Wait** - Pause for 5 seconds before repeating

> 🔁 **Important**: After finishing, `loop()` automatically starts again from the beginning. It's an infinite cycle!

---

## 🔑 Key Code Concepts

### 📍 Communication Channels (MQTT Topics)

```text
📤 DATA Channel:     sensors/dht22/data      ← The sensor publishes data here
📥 COMMANDS Channel: sensors/dht22/commands  ← Commands are sent to the sensor here
```

**Marketing Analogy**:

- It's like having two different **corporate hashtags**:
  - `#SensorData` - where the sensor publishes its updates
  - `#SensorCommands` - where you send instructions to the sensor

### ⏱️ Publishing Frequency

```text
FREQ = 5000 milliseconds = 5 seconds
```

Every 5 seconds, the sensor:

1. Reads temperature and humidity
2. Creates a JSON message
3. Publishes it to the Internet

**Marketing Analogy**: It's like scheduling **automatic posts every 5 seconds** - an ultra-fast "editorial calendar" for data.

---

## 🎮 Remote Control Commands

The sensor can receive commands via **USB cable** or **Internet (MQTT)**:

| Command    | Action        | Practical Use                     |
| ---------- | ------------- | --------------------------------- |
| `help`   | Shows help    | See available commands            |
| `start`  | Starts sensor | Begin collecting and sending data |
| `stop`   | Stops sensor  | Pause collection (saves energy)   |
| `status` | View status   | Check if connected and working    |

> 💡 **For Marketing**: You can control the sensor remotely from anywhere in the world, simply by sending a message to the commands channel.

---

## 📤 How Data is Sent to the Internet

### Creating and Publishing JSON Data

Here's the actual code that creates and sends data to the Internet:

```cpp
void publishMessage() {
  // 1. Create a JSON document (like filling out a form)
  StaticJsonDocument<200> doc;
  
  // 2. Fill in the fields
  doc["time"] = DateTime.toISOString();    // Current time
  doc["humidity"] = h;                     // Humidity value
  doc["temperature"] = t;                  // Temperature value
  
  // 3. Convert to text format
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  
  // 4. Send to the Internet!
  client.publish(PUBLISH_TOPIC, jsonBuffer);
}
```

**What happens step by step:**

1. **Create a container** - `StaticJsonDocument<200>` is like creating an empty form
2. **Fill the form** - Add time, humidity, and temperature
3. **Convert to text** - Transform it into a format that can be sent over Internet
4. **Publish** - Send it to the MQTT server on the `sensors/dht22/data` topic

**Result:** A message like this is sent every 5 seconds:

```json
{
  "time": "2025-12-12T14:30:15Z",
  "humidity": 45.2,
  "temperature": 23.5
}
```

---

## 📥 How Commands are Received

### Processing Remote Commands

The sensor can receive commands from the Internet and act on them:

```cpp
void messageReceived(char *topic, byte *payload, unsigned int length) {
  // 1. Convert the received message to text
  String command = "";
  for (unsigned int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  command.trim();           // Remove extra spaces
  command.toLowerCase();    // Convert to lowercase
  
  // 2. Show what was received
  Serial.print("Received MQTT [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(command);
  
  // 3. Process the command
  processCommand(command, "MQTT");
}
```

**Example:** If someone sends `"stop"` to the `sensors/dht22/commands` topic:

1. The device receives the message
2. Converts it to text: `"stop"`
3. Processes it with `processCommand()`
4. Stops reading the sensor
5. Sends back a confirmation message

---

## 🔧 Simplified Beginner Example

If you're just starting, here's a **super simple** Arduino program that shows the basic structure:

```cpp
#include <Arduino.h>

// This runs ONCE when the device starts
void setup() {
  Serial.begin(115200);           // Start USB communication
  Serial.println("Hello World!"); // Print a message
  Serial.println("I am ready!");
}

// This runs FOREVER in a loop
void loop() {
  Serial.println("I am alive!"); // Print a message
  delay(1000);                   // Wait 1 second
  // Then automatically repeats!
}
```

**What you'll see in the console:**

```
Hello World!
I am ready!
I am alive!
I am alive!
I am alive!
... (repeats forever every second)
```

**Explanation:**

- **`setup()`**: Prints "Hello World!" and "I am ready!" just once
- **`loop()`**: Prints "I am alive!" every second, forever
- **`delay(1000)`**: Pauses for 1000 milliseconds (1 second)

> 💡 **Try it yourself!** This is a great first program to understand how Arduino works.

---

## 🎮 Remote Control Commands

The sensor can receive commands via **USB cable** or **Internet (MQTT)**:

| Command    | Action        | Practical Use                     |
| ---------- | ------------- | --------------------------------- |
| `help`   | Shows help    | See available commands            |
| `start`  | Starts sensor | Begin collecting and sending data |
| `stop`   | Stops sensor  | Pause collection (saves energy)   |
| `status` | View status   | Check if connected and working    |

> 💡 **For Marketing**: You can control the sensor remotely from anywhere in the world, simply by sending a message to the commands channel.

---

## 💼 Digital Marketing Applications

### What could this be used for in the real world?

| Application                | Description                    | Marketing Value                  |
| -------------------------- | ------------------------------ | -------------------------------- |
| **Retail Analytics** | Measure store conditions       | Correlate temperature with sales |
| **Smart Buildings**  | Office monitoring              | Data for sustainability reports  |
| **Events**           | Environmental control at fairs | Ensure visitor comfort           |
| **Logistics**        | Cold chain                     | Guarantee product quality        |
| **Hospitality**      | Hotels and restaurants         | Optimize customer experience     |

### Use Case Example

> **Scenario**: A clothing store wants to know if temperature affects sales.
>
> **Solution**: Install sensors in different areas, automatically collect data, and cross-reference with POS sales data.
>
> **Potential Insight**: "When temperature exceeds 24°C, cold beverage sales increase by 30%"

---

## 📈 Project BoM (Bill of Materials)

### Hardware Components

This is everything you need to build your own IoT temperature and humidity sensor:

| Component                   | Description                                     | Quantity | Unit Price (USD)          | Total (USD)      | Where to Buy                                 |
| --------------------------- | ----------------------------------------------- | -------- | ------------------------- | ---------------- | -------------------------------------------- |
| **NodeMCU ESP8266**   | WiFi-enabled microcontroller board              | 1        | $4.00 - $6.00             | $5.00            | AliExpress, Amazon, local electronics stores |
| **DHT22 (AM2302)**    | Digital temperature and humidity sensor         | 1        | $3.00 - $5.00             | $4.00            | AliExpress, Amazon, Adafruit, SparkFun       |
| **Breadboard**        | Solderless prototyping board (400 tie-points)   | 1        | $2.00 - $3.00             | $2.50            | Electronics stores, online retailers         |
| **Jumper Wires**      | Male-to-Male (set of 10-20)                     | 1        | $1.00 - $2.00             | $1.50            | Electronics stores (usually sold in packs)   |
| **Micro USB Cable**   | For power and programming                       | 1        | $2.00 - $4.00             | $3.00            | Most phone charger cables work               |
| **USB Power Adapter** | 5V, 1A minimum (optional if using computer USB) | 1        | $3.00 - $5.00             | $4.00            | Phone chargers work perfectly                |
|                             |                                                 |          | **TOTAL HARDWARE:** | **$20.00** |                                              |

### Software Costs

| Item                        | Description                                               | Cost            |
| --------------------------- | --------------------------------------------------------- | --------------- |
| **PlatformIO**        | Development environment                                   | **FREE**  |
| **Arduino Libraries** | DHT sensor, MQTT, JSON, DateTime                          | **FREE**  |
| **MQTT Broker**       | Using public brokers (broker.emqx.io, test.mosquitto.org) | **FREE**  |
| **VS Code**           | Code editor                                               | **FREE**  |
|                             | **TOTAL SOFTWARE:**                                 | **$0.00** |

### Platforms for Learning and Testing

Perfect for students - all these platforms offer free tiers ideal for educational projects:

| Platform/Service                                                                             | Description                                | Free Tier Features                                                                                                                                 | Cost           | Registration          |
| -------------------------------------------------------------------------------------------- | ------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------- | -------------- | --------------------- |
| **MQTTX Broker** (broker.emqx.io)                                                      | Public MQTT broker for IoT messaging       | • Unlimited connections<br />• No registration required <br />• Multiple protocol support <br />• Perfect for testing and learning             | **FREE** | Not required          |
| **Grafana Cloud** ([Free tier]([https://grafana.com]))                                    | Data visualization and monitoring platform | • 10,000 series metrics<br />• 50 GB logs per month <br />• 14-day retention <br />• Pre-built dashboards <br />• Beautiful charts and graphs | **FREE** | Free account required |
| **MQTT Explorer Web**<br />[https://mqttx.app/web-client]([https://mqttx.app/web-client]) | MQTT client for monitoring                 | • Visual topic hierarchy<br />• Real-time message viewing <br />• Publish/subscribe testing <br />• Windows/Mac/Linux support                  | **FREE** | No account needed     |
|                                                                                              |                                            |                                                                                                                                                    |                |                       |

**Why These Platforms Are Great for Students:**

> 📊 **Grafana Cloud**:
>
> - Create professional dashboards like those used by real companies
> - Visualize your sensor data with beautiful charts
> - Monitor temperature trends over time
> - Set up alerts (e.g., "notify me if temperature > 30°C")
> - Share dashboards with classmates or instructors

> 🔌 **MQTTX Broker (broker.emqx.io)**:
>
> - No setup required - just connect and start sending data
> - Reliable and maintained by EMQX team
> - Used by thousands of IoT developers worldwide
> - Multiple servers globally for better connectivity
> - Supports both MQTT and WebSocket protocols

### Detailed Wiring Connections

**Connection Table:**

| DHT22 Pin | NodeMCU Pin | Wire Color (suggested) | Notes        |
| --------- | ----------- | ---------------------- | ------------ |
| VCC (+)   | 3.3V        | Red                    | Power supply |
| DATA      | D2 (GPIO4)  | Yellow/Orange          | Data signal  |
| GND (-)   | GND         | Black/Brown            | Ground       |

**Optional:** Connect a 10kΩ resistor between VCC and DATA pins for better stability.

---

## 📝 Glossary of Terms

| Term               | Simple Meaning                                       |
| ------------------ | ---------------------------------------------------- |
| **IoT**      | "Internet of Things" - Objects connected to Internet |
| **NodeMCU**  | Small programmable WiFi computer                     |
| **DHT22**    | Temperature and humidity sensor                      |
| **MQTT**     | Messaging system for IoT devices                     |
| **Broker**   | Server that receives and redistributes MQTT messages |
| **Topic**    | "Channel" where messages are published/received      |
| **JSON**     | Text format for exchanging data                      |
| **WiFi**     | Wireless Internet connection                         |
| **Firmware** | Program that controls the device                     |

---

## ❓ Frequently Asked Questions

### Do I need to know programming to use this?

**Not necessarily.** Once configured, the device works automatically. You only need to configure your WiFi and MQTT server once.

### How much does it cost?

**Very little.** The complete hardware costs approximately $10-20. The software and public MQTT servers are free.

### Can I see the data on my phone?

**Yes.** There are free apps like "MQTT Dashboard" that let you see the data in real time.

### Does it work without Internet?

**No.** The device needs WiFi to send data. However, it could be modified to store data locally.

### How often does it send data?

**Every 5 seconds** by default, but this interval is configurable.

---

## 🎓 What You Should Remember

### Key Takeaways About Arduino Programming

1. **Two Functions Rule Everything**: `setup()` runs once, `loop()` runs forever
2. **Libraries Make It Easy**: Don't reinvent the wheel - use existing tools
3. **Clear Structure**: Initialization → Configuration → Continuous Operation
4. **Real-time Operation**: The device is always working, 24/7, automatically
5. **Bidirectional Communication**: It can send data AND receive commands

## 🎓 Final Thoughts: What You Should Remember

1. **IoT is not magic** - These are simple devices that measure, process, and send data over the Internet
2. **Data is valuable** - The ability to automatically collect data 24/7 opens infinite analysis possibilities
3. **Communication is bidirectional** - It not only collects data, but can also receive remote commands
4. **It's scalable** - You can have 1 or 1000 sensors, all reporting to the same system
5. **It's accessible** - With less than $20 you can have your own IoT sensor working
6. **Programming is approachable** - With `setup()` and `loop()`, you have a clear structure to build on
