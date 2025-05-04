#include "utils.hpp"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // led library
#include <vector>
#include "clock_leds.hpp"

/*
* LED INDEX LOGIC
*/
const LedMapEntry ledMap[] = {
    // first digit
    {H1, 0, 0, 0}, {H1, 0, 1, 23}, {H1, 0, 2, 24},
    // second digit
    {H2, 0, 0, 2}, {H2, 0, 1, 21}, {H2, 0, 2, 26},
    {H2, 1, 0, 3}, {H2, 1, 1, 20}, {H2, 1, 2, 27},
    {H2, 2, 0, 4}, {H2, 2, 1, 19}, {H2, 2, 2, 28},
    // third digit
    {M1, 0, 0, 6}, {M1, 0, 1, 17}, {M1, 0, 2, 30},
    {M1, 1, 0, 7}, {M1, 1, 1, 16}, {M1, 1, 2, 31},
    // fourth digit
    {M2, 0, 0, 9}, {M2, 0, 1, 14}, {M2, 0, 2, 33},
    {M2, 1, 0, 10}, {M2, 1, 1, 13}, {M2, 1, 2, 34},
    {M2, 2, 0, 11}, {M2, 2, 1, 12}, {M2, 2, 2, 35},
};


int getLedIndex(DigitId digit, int row, int column) {
    for (const LedMapEntry &entry : ledMap) {
        if(entry.digitNumber == digit && entry.row == row && entry.column == column) {
            return entry.ledIndex;
        }
    }
    return -1; // not found
}


/*
* OPERATING THE LED STRIP
*/

#define PIN        D4

// creating the led strip object
static Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void initLedStrip() {
    strip.begin();
    strip.setBrightness(128);  // default mid-level
    strip.show();
}
void clearLedStrip() {
    strip.clear();
    strip.show();
}
void setLedBrightness(int brightness) {
  strip.setBrightness(brightness);
  strip.show();
}
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b) {
    return strip.Color(r, g, b);
}
void showLeds(const std::vector<int> on_indices, int colour, LedAnimation anim) {
    switch (anim) {
        case LedAnimation::Warmup: AnimationWarmup(colour); break;
        case LedAnimation::Test_LEDs: AnimationTestLEDs(); break;
        case LedAnimation::Default: AnimationDefault(on_indices, colour); break;
        case LedAnimation::Fade_io: AnimationFadeIO(on_indices, colour); break;
        default: AnimationDefault(on_indices, colour); break;
    }
}

// various animation functions
void AnimationWarmup(int colour) { // turn on all leds for 2s before showing time
    strip.clear();
    for(int index = 0; index < NUM_LEDS; index++) {
        strip.setPixelColor(index, colour);
    }
    strip.show();
    sleepSeconds(2);
    clearLedStrip();
    sleepSeconds(1);
}
void AnimationTestLEDs() {
}
void AnimationDefault(const std::vector<int> on_indices, int colour) {
    strip.clear();
    for(int index : on_indices) {
        if(index >= 0 && index < NUM_LEDS) {
            strip.setPixelColor(index, colour);
        }
    }
    strip.show();
}
void AnimationFadeIO(const std::vector<int> on_indices, int colour) {
}
