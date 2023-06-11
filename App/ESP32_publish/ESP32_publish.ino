#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

// The MQTT topics that this device should publish
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);
bool lastButtonState = false;

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);
  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("");

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  Serial.println("AWS IoT Connected!");
}

void publishMessage(bool buttonState) {
  StaticJsonDocument<200> doc;
  doc["button"] = buttonState;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  pinMode(0, INPUT);
  Serial.begin(115200);
  connectAWS();
}

void loop() {

  // Read the current state (pull-up resistor)
  bool currentButtonState = !digitalRead(0);

  // If the button state has changed, publish a message
  if (currentButtonState != lastButtonState) {
    publishMessage(currentButtonState);
    // Update the last known button state
    lastButtonState = currentButtonState;
  }

  client.loop();
  delay(1000);
}