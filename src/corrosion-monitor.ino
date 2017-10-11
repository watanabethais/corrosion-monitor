/*
 * Corrosion Current Monitor
 * @author Thais Watanabe
 */
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

Adafruit_INA219 ina219;

// router ssid & pass
const char* ssid = "";
const char* password = "";

// clinet
WiFiClient client;

// ThingSpeak Settings
const int channelID = x;
const char* server = "api.thingspeak.com";
const int postingInterval = 60 * 1000; // post data every 20 seconds
String writeAPIKey = "x"; // write API key for your ThingSpeak Channel

// working with esp-wroom
extern "C" {
#include "user_interface.h"
}

void setup() {
  uint32_t currentFrequency;

  Serial.begin(9600);
  connectingToWiFi();
  ina219.begin();
}

void loop() {
  read_INA219_values();
  delay(60000);
}

/*
 * Make connection with wifi
 */
void connectingToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.print("Connected with IP address: ");
  Serial.println(WiFi.localIP());
}

/*
 * Sends data to ThingSpeak cloud
 */
void sendDataToThingSpeak(float current) {
  if (client.connect(server, 80)) {

    // Construct API request body
    String body = "field1=";
           body += String(current);

    Serial.print("Current: ");
    Serial.println(current);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(body.length());
    client.print("\n\n");
    client.print(body);
    client.print("\n\n");

  }
  client.stop();

  // wait and then post again
  //delay(postingInterval);
}

/*
 * Read values from the INA 219 current sensor
 */
void read_INA219_values() {
  Serial.println("Measuring current from INA219...");

  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.println("");

  sendDataToThingSpeak(current_mA);
}
