#include "secrets.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_RST -1 //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_CS D4  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_DC D3  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
WiFiClientSecure net;
MQTTClient client;

String incomingMessage = "";

int8_t TIME_ZONE = 0; //UTC+0
time_t now;
time_t nowish = 1510592825;

void NTPConnect(void)
{
  tft.println("Setting time SNTP");
  Serial.println("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  tft.println("done!");
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.println(asctime(&timeinfo));
}

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  tft.print("Connecting to Wi-Fi : ");
  tft.println(WIFI_SSID);
  Serial.println("Connecting to Wi-Fi : ");
  Serial.println(WIFI_SSID);

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

  tft.println("Connecting to AWS IoT");
  Serial.println("Connecting to AWS IoT");

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

  tft.println("AWS IoT Connected!");
  Serial.println("AWS IoT Connected!");
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

  const char* message = doc["message"];
  incomingMessage = String(message);
  Serial.println(incomingMessage);
}


void setup()
{
  Serial.begin(9600);
  tft.initR(INITR_144GREENTAB);
  tft.setTextWrap(true); // Allow text to run off right edge
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  connectAWS();
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
}

void loop()
{
  client.loop();

  if (!incomingMessage.isEmpty()) {
    if(tft.getCursorY() == 128){
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 0);
    }

    tft.println(incomingMessage);
    incomingMessage = "";
    Serial.println(tft.getCursorY());
  }

  delay(10);
}