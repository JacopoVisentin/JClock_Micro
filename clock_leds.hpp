#include <Arduino.h>
#include <vector>
#pragma once


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
extern const size_t ledMapSize;

// given a cell in the clock array, returns the led strip index
int getLedIndex(DigitId digit, int row, int col);


/*
* OPERATING THE LED STRIP
*/
void initLedStrip();
void clearLedStrip();
void setLedBrightness(int brightness);
/// Pack an (R,G,B) triple into a NeoPixel-compatible 32-bit color.
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);
void showLeds(std::vector<int> on_indices, int color);