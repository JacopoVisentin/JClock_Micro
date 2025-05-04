#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "clock_display.hpp"
#include "clock_leds.hpp"
#include "utils.hpp"

/*
* RUNNING PARAMETERS
*/

// uncomment for testing modes
#define CONSOLE_DISPLAY
//#define RGB_TEST
#define WARMUP_TEST



// WIFI
constexpr const char* SSID = "Proximus-Home-DEB8";
constexpr const char* PASS = "007007007";

// DISPLAY
constexpr int LED_BRIGHTNESS = 50; // /256
constexpr int LED_REFRESH_TIME = 5; // seconds
constexpr uint8_t RGB[3] = {235, 149, 61};
const int my_colour = makeColor(RGB[0], RGB[1], RGB[2]);

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

  // TESTS
  // led check at startup
  #ifdef WARMUP_TEST
    AnimationWarmup(my_colour);
  #endif
  // rgb test
  #ifdef RGB_TEST
    AnimationTestLEDs();
  #endif
}

void loop() {
  my_clock.machineTimeInput(); // using NTPClient
  my_clock.refresh();


  // displaying clock in console for debugging
  #ifdef CONSOLE_DISPLAY
    my_clock.showClockInConsole();
    my_clock.printTime();
    my_clock.printLedIndices();
    clearTerminal();
  #endif


  // vector of led indices on
  auto on_indices = my_clock.getActiveLedIndices();

  showLeds(on_indices, my_colour, LedAnimation::Fade_io, LED_REFRESH_TIME);

}