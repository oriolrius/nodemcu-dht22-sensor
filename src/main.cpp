#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESPDateTime.h>
#include "secrets.h"
#include "DHT.h"

#define MQTT_VERSION MQTT_VERSION_3_1_1

// Function declarations
void processCommand(String command, String source);
void blinkLED();
 
#define DHTPIN 4        // GPIO 4 (D2 pin on NodeMCU) connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302)
#define LED_BUILTIN 2   // Built-in LED on NodeMCU (GPIO2/D4)
 
DHT dht(DHTPIN, DHTTYPE);
 
float h ;
float t;
bool sensorActive = true;  // Control flag for sensor readings

WiFiClient net;
PubSubClient client(net);
 
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(THINGNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("IP GW: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
}

void NTPConnect(void) {
  DateTime.setServer("ntp.ubuntu.com");
  DateTime.setTimeZone(TIME_ZONE);
  DateTime.begin();
  delay(3000);
  if (!DateTime.isTimeValid()) {
    Serial.println("NTP server: fail.");
  } else {
    Serial.printf("Now is %s\n", DateTime.toISOString().c_str());
  }
}

void blinkLED() {
  digitalWrite(LED_BUILTIN, LOW);   // Turn LED ON (LOW is ON for built-in LED)
  delay(1000);                       // Keep ON for 1 second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn LED OFF (HIGH is OFF for built-in LED)
}

void messageReceived(char *topic, byte *payload, unsigned int length) {
  // Convert payload to string
  String command = "";
  for (unsigned int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  command.trim();
  command.toLowerCase();
  
  Serial.print("Received MQTT [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(command);
  
  // Process MQTT command same as serial commands
  processCommand(command, "MQTT");
}

void showHelp() {
  Serial.println(F("=== DHT22 Sensor Control Commands ==="));
  Serial.println(F("help or h    - Show this help message"));
  Serial.println(F("stop         - Stop sensor readings and MQTT publishing"));
  Serial.println(F("start        - Start/resume sensor readings and MQTT publishing"));
  Serial.println(F("status       - Show current sensor status"));
  Serial.println(F("Commands work via Serial Console AND MQTT"));
  Serial.println(F("========================================"));
}

void processCommand(String command, String source) {
  if (command == "help" || command == "h") {
    showHelp();
  }
  else if (command == "stop") {
    sensorActive = false;
    String msg = "Sensor readings STOPPED via " + source + ". MQTT publishing disabled.";
    Serial.println(msg);
    
    // Send acknowledgment via MQTT if command came from MQTT
    if (source == "MQTT" && client.connected()) {
      client.publish(PUBLISH_TOPIC, "{\"status\":\"stopped\",\"message\":\"Sensor readings stopped\"}");
    }
  }
  else if (command == "start") {
    sensorActive = true;
    String msg = "Sensor readings STARTED via " + source + ". MQTT publishing enabled.";
    Serial.println(msg);
    
    // Send acknowledgment via MQTT if command came from MQTT
    if (source == "MQTT" && client.connected()) {
      client.publish(PUBLISH_TOPIC, "{\"status\":\"started\",\"message\":\"Sensor readings started\"}");
    }
  }
  else if (command == "status") {
    Serial.print(F("Status requested via "));
    Serial.println(source);
    Serial.print(F("Sensor status: "));
    Serial.println(sensorActive ? F("ACTIVE") : F("STOPPED"));
    Serial.print(F("WiFi status: "));
    Serial.println(WiFi.status() == WL_CONNECTED ? F("CONNECTED") : F("DISCONNECTED"));
    Serial.print(F("MQTT status: "));
    Serial.println(client.connected() ? F("CONNECTED") : F("DISCONNECTED"));
    
    // Send status via MQTT if command came from MQTT
    if (source == "MQTT" && client.connected()) {
      StaticJsonDocument<300> statusDoc;
      statusDoc["sensor"] = sensorActive ? "ACTIVE" : "STOPPED";
      statusDoc["wifi"] = WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED";
      statusDoc["mqtt"] = client.connected() ? "CONNECTED" : "DISCONNECTED";
      statusDoc["message"] = "Status report";
      
      char statusBuffer[350];
      serializeJson(statusDoc, statusBuffer);
      client.publish(PUBLISH_TOPIC, statusBuffer);
    }
  }
  else if (command.length() > 0) {
    String msg = "Unknown command '" + command + "' received via " + source + ". Type 'help' for available commands.";
    Serial.println(msg);
    
    // Send error via MQTT if command came from MQTT
    if (source == "MQTT" && client.connected()) {
      client.publish(PUBLISH_TOPIC, "{\"error\":\"unknown_command\",\"message\":\"Unknown command. Send 'help' for available commands\"}");
    }
  }
}

void handleSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();
    
    // Process command using the unified function
    processCommand(command, "Serial");
  }
}

void connectMQTT() {
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(messageReceived);
 
  Serial.println("Connecting to MQTT");

  while (!client.connect(THINGNAME)) {
    //Serial.print("ERROR CODE (PubSubClient.h): ");
    //Serial.println(client.state());
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("MQTT Timeout!");
    return;
  }
  client.subscribe(SUBSCRIBE_TOPIC);
 
  Serial.println("MQTT Connected!");
}
 
void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["time"] = DateTime.toISOString();
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(PUBLISH_TOPIC, jsonBuffer);
  blinkLED();  // Blink LED for 1 second after publishing
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n=== DHT22 NodeMCU Sensor Starting ==="));
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Initialize LED as OFF
  
  connectWiFi();
  NTPConnect();
  connectMQTT();
  dht.begin();
  
  Serial.println(F("\nSystem ready! Type 'help' for available commands."));
  showHelp();
}
 
void loop() {
  // Always handle serial commands
  handleSerialCommands();
  
  // Always maintain MQTT connection
  if (!client.connected()) {
    connectMQTT();
  } else {
    client.loop();
  }
  
  // Only read and publish sensor data if active
  if (sensorActive) {
    h = dht.readHumidity();
    t = dht.readTemperature();
   
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      delay(2000);
      return;
    }
   
    Serial.print(DateTime.toISOString().c_str());
    Serial.print(F(" - Humidity: "));
    Serial.print(h);
    Serial.print(F("%  - Temperature: "));
    Serial.print(t);
    Serial.println(F("°C "));
   
    publishMessage();
    delay(FREQ);
  } else {
    // When stopped, just wait a short time to keep the system responsive
    delay(100);
  }
}