#include <Arduino.h>
#include <vector>
#pragma once

/*
* LED LOGIC
*/

enum DigitId : int{
    H1 = 0,
    H2 = 1,
    M1 = 2,
    M2 = 3,
};

struct LedMapEntry {
    DigitId digitNumber;
    int row;
    int column;
    int ledIndex;
};

// the lookup table itself
extern const LedMapEntry ledMap[];
// given a cell in the clock array, returns the led strip index
int getLedIndex(DigitId digit, int row, int col);



/*
* OPERATING THE LED STRIP
*/

static constexpr int NUM_LEDS = 36; // strip length (including dead leds for spacing)

enum class LedAnimation {
    Warmup, // use at startup for quick check
    Test_LEDs, // all leds on and pulsating for prolonged test
    Default, // no animation
    Fade_io
};

void initLedStrip();
void clearLedStrip();
void setLedBrightness(int brightness);
/// Pack an (R,G,B) triple into a NeoPixel-compatible 32-bit color.
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);
// turning on all leds briefly to test the strip
void testStrip();

// wrapper function for calling custom animations
void showLeds(const std::vector<int> on_indices, int color, LedAnimation anim = LedAnimation::Default);

// custom animations
void AnimationWarmup(int colour);
void AnimationTestLEDs();
void AnimationDefault(const std::vector<int> on_indices, int colour);
void AnimationFadeIO(const std::vector<int> on_indices, int colour);