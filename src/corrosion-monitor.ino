/*
 * Corrosion Monitor
 * @author Thais Watanabe
 */
#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

Adafruit_INA219 ina219;

void setup() {
  Serial.begin(9600);
  ina219.begin();
}

void loop() {
  Serial.println("Measuring current...");
  float current_mA = 0;
  current_mA = ina219.getCurrent_mA();
  Serial.print("Current: ");
  Serial.print(current_mA);
  Serial.println(" mA");
  Serial.println("");

  delay(10000);
}
