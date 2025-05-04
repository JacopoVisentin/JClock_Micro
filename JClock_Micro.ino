#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "clock_display.hpp"
#include "clock_leds.hpp"
#include "utils.hpp"


/*
* RUNNING PARAMETERS
*/

// WIFI
constexpr char* SSID = "Proximus-Home-DEB8";
constexpr char* PASS = "007007007";

// DISPLAY
constexpr int LED_BRIGHTNESS = 50; // /256
constexpr int LED_REFRESH_TIME = 5; // seconds



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

  // LED strip setup
  initLedStrip();
  setLedBrightness(LED_BRIGHTNESS);
}

void loop() {
  my_clock.machineTimeInput(); // using NTPClient
  my_clock.refresh();

  // vector of led indices on
  auto on = my_clock.getActiveLedIndices();

  int my_colour = makeColor(235, 149, 61);
  showLeds(on, my_colour);
  
  sleepSeconds(LED_REFRESH_TIME);
}