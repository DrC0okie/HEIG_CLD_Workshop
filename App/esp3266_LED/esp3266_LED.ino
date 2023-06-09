#include "secrets.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <Adafruit_NeoPixel.h>

#define PIN   D4
#define LED_NUM 7
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp8266_LED/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266_LED/sub"

WiFiClientSecure net;
MQTTClient client;

String incomingMessage = "";

int8_t TIME_ZONE = 2; //UTC+2
time_t now;
time_t nowish = 1510592825;

void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

    NTPConnect();

  // Create a secure client
  BearSSL::WiFiClientSecure *clientSecure = new BearSSL::WiFiClientSecure;

  // Load the certificates
  BearSSL::X509List *certificate = new BearSSL::X509List(AWS_CERT_CRT);
  BearSSL::PrivateKey *privateKey = new BearSSL::PrivateKey(AWS_CERT_PRIVATE);
  BearSSL::X509List *caCert = new BearSSL::X509List(AWS_CERT_CA);

  clientSecure->setClientRSACert(certificate, privateKey);
  clientSecure->setTrustAnchors(caCert);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, *clientSecure);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IoT");

  while (!client.connect(THINGNAME)) {
    Serial.println(client.lastError());
    delay(1000);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(A0);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(String &topic, String &payload)
{
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println("Failed to parse JSON message");
    return;
  }

  // As the message now contains an object, we parse the JSON object into the individual R, G, and B values.
  JsonObject message = doc["message"];
  uint8_t r = message["r"];
  uint8_t g = message["g"];
  uint8_t b = message["b"];
  
  // We now call the led_set function with the RGB values we've just retrieved.
  led_set(r, g, b);
}

void led_set(uint8 R, uint8 G, uint8 B) {
  for (int i = 0; i < LED_NUM; i++) {
    leds.setPixelColor(i, leds.Color(R, G, B));
    leds.show();
    delay(50);
  }
}


void setup()
{
  Serial.begin(9600);
  leds.begin(); // This initializes the NeoPixel library.
  led_set(0, 0, 0);
  connectAWS();
}

void loop()
{
  publishMessage();
  client.loop();
  delay(1000);
}