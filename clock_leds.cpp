#include <sys/_stdint.h>
#include "core_esp8266_features.h"
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
void showLeds(const std::vector<int> on_indices, int colour, LedAnimation anim, int refresh_time) {
    switch (anim) {
        case LedAnimation::Default: AnimationDefault(on_indices, colour, refresh_time); break;
        case LedAnimation::Fade_io: AnimationFadeIO(on_indices, colour, refresh_time); break;
        default: AnimationDefault(on_indices, colour, refresh_time); break;
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
void AnimationTestLEDs() { // cycle rgb for testing individual pixels
    while(true) {
        
        strip.clear();
        
        const uint32_t red = strip.Color(255, 0, 0);
        const uint32_t green = strip.Color(0, 255, 0);
        const uint32_t blue = strip.Color(0, 0, 255);
        const uint32_t colours[3] = {red, green, blue};
        
        for(const uint32_t colour : colours) {
            for(int index = 0; index < NUM_LEDS; index++) {
                strip.setPixelColor(index, colour);
            }
            strip.show();
            delay(1500);
            strip.clear();
        }
    }
}
void AnimationDefault(const std::vector<int> on_indices, int colour, int refresh_time) {
    strip.clear();
    for(int index : on_indices) {
        if(index >= 0 && index < NUM_LEDS) {
            strip.setPixelColor(index, colour);
        }
    }
    strip.show();
    sleepSeconds(refresh_time);
}
void AnimationFadeIO(const std::vector<int> on_indices, int colour, int refresh_time) {
    strip.clear();

    // parameters
    const uint8_t steps = 40;
    const uint16_t pause_ms = 20;
    
    const int fade_time_ms = 2*steps*pause_ms;
    const int hold_time_ms = (1000*refresh_time) - fade_time_ms;

    // 1) Extract R, G, B from your 24-bit colour
    uint8_t r = (colour >> 16) & 0xFF;
    uint8_t g = (colour >> 8 ) & 0xFF;
    uint8_t b =  colour        & 0xFF;

    // 2) Fade IN
    for (uint8_t s = 0; s <= steps; ++s) {
        float frac = (float)s / steps;
        strip.clear();
        for (int idx : on_indices) {
            if (idx < 0 || idx >= NUM_LEDS) continue;
            // scale each channel by frac
            strip.setPixelColor(idx, strip.Color(r * frac, g * frac, b * frac));
        }
        strip.show();
        delay(pause_ms);
    }

    // 3) hold at full brightness
    delay(1000*refresh_time);

    // 4) Fade OUT
    for (int s = steps; s >= 0; --s) {
        float frac = (float)s / steps;
        strip.clear();
        for (int idx : on_indices) {
            if (idx < 0 || idx >= NUM_LEDS) continue;
            strip.setPixelColor(idx, strip.Color(r * frac, g * frac, b * frac));
        }
        strip.show();
        delay(pause_ms);
    }
}
