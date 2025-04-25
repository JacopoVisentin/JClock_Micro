#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "clock_display.hpp"
#include "clock_leds.hpp"
#include "utils.hpp"


/*
* RUNNING PARAMETERS
*/
const char* SSID = "Proximus-Home-DEB8";
const char* PASS = "007007007";



ClockDisplay my_clock;

void setup() {
  Serial.begin(115200);
  while (!Serial) {} // waiting for serial if needed

  // wifi protocol
  WiFi.begin(SSID, PASS);
  Serial.print(F("Connecting to WiFi..."));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println(F("\nWiFi connected successfully!"));

  initTimeClient(); // must call after wifi is up

  initLedStrip();
  setLedBrightness(50); // out of 256
}

void loop() {
  my_clock.machineTimeInput(); // using NTPClient
  my_clock.refresh();

  // vector of led indices on
  auto on = my_clock.getActiveLedIndices();

  // choose colour
  int my_colour = makeColor(235, 149, 61);

  showLeds(on, my_colour);
  
  sleepSeconds(5);
}