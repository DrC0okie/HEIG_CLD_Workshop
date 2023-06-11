#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

// Define constants for the MQTT topics that this device should publish
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define BUTTON_PIN 0

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);
bool lastButtonState = false;

// This function connects the ESP32 to AWS IoT Core over MQTT
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for Wi-Fi to connect
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected!");

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Wait for MQTT broker to connect
  Serial.print("Connecting to AWS IOT");
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  Serial.println("");
  Serial.println("AWS IoT Connected!");
}

// This function publishes a message to AWS IoT Core
void publishMessage(bool buttonState) {
  StaticJsonDocument<200> doc;
  doc["button"] = buttonState;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

// Runs once when the board is powered
void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
  connectAWS();
}

void loop() {
  // Read the current state of the button
  bool currentButtonState = !digitalRead(BUTTON_PIN);

  // If the button state has changed, publish a message
  if (currentButtonState != lastButtonState) {
    publishMessage(currentButtonState);
    lastButtonState = currentButtonState;
  }

  client.loop();
  delay(20);
}
