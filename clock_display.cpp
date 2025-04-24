#include "WString.h"
#include "HardwareSerial.h"
#include <Arduino.h>
#include <vector>
#include <utility>
#include <string>      // for std::to_string in utils
#include "clock_display.hpp"
#include "utils.hpp"
#include "clock_leds.hpp"

/*
* CONSTRUCTORS
*/
ClockDisplay::ClockDisplay() {
    // initialize clean slate arrays
    ClockDisplay::resetAllArrays();

}


/*
* SETTER & GETTER
*/
void ClockDisplay::setTime(int hour, int minute) {
    this->hour = hour;
    this->minute = minute;
}
void ClockDisplay::setTime(timestamp_t timestamp) {
    this->hour = timestamp.first;
    this->minute = timestamp.second;
}
timestamp_t ClockDisplay::getTime() {
    timestamp_t timestamp = {this->hour, this->minute};
    return timestamp;
}


/*
* CLOCK LOGIC
*/
void ClockDisplay::setAllDigits() {
    int hour = this->hour; int minute = this->minute;
    this->hour_first_digit = firstDigit(hour);
    this->hour_second_digit = secondDigit(hour);
    this->minute_first_digit = firstDigit(minute);
    this->minute_second_digit = secondDigit(minute);
}
void ClockDisplay::resetAllArrays() {
    // zeroing hf array (1x3)
    std::fill_n(this->hour_first_array, 3, 0);
    // zeroing mf array (2x3)
    for (int i = 0; i < 2; i++) {
        std::fill_n(this->minute_first_array[i], 3, 0);
    }
    // zeroing hs and ms arrays (3x3)
    for (int i = 0; i < 3; i++) {
        std::fill_n(this->hour_second_array[i], 3, 0);
        std::fill_n(this->minute_second_array[i], 3, 0);
    }
}
void ClockDisplay::setHourFirstArray() {
    int h1 = this->hour_first_digit;
    coords_array_t coords_array = generateNCoords(h1, 0, 2);
    this->hour_first_coords = coords_array;
    for(coord_t coord : coords_array) {
        int j = coord.second; // no i because this one is 1D array
        this->hour_first_array[j] = 1;
    }
}
void ClockDisplay::setHourSecondArray() {
    int h2 = this->hour_second_digit;
    coords_array_t coords_array = generateNCoords(h2, 2, 2);
    this->hour_second_coords = coords_array;
    for(coord_t coord : coords_array) {
        int i = coord.first; int j = coord.second;
        this->hour_second_array[i][j] = 1;
    }
}
void ClockDisplay::setMinuteFirstArray() {
    int m1 = this->minute_first_digit;
    coords_array_t coords_array = generateNCoords(m1, 1, 2);
    this->minute_first_coords = coords_array;
    for(coord_t coord : coords_array) {
        int i = coord.first; int j = coord.second;
        this->minute_first_array[i][j] = 1;
    }
}
void ClockDisplay::setMinuteScondArray() {
    int m2 = this->minute_second_digit;
    coords_array_t coords_array = generateNCoords(m2, 2, 2);
    this->minute_second_coords = coords_array;
    for(coord_t coord : coords_array) {
        int i = coord.first; int j = coord.second;
        this->minute_second_array[i][j] = 1;
    }
}
void ClockDisplay::setAllArrays() {
    this->resetAllArrays(); // resetting the whole thing before setting it again
    this->setHourFirstArray();
    this->setHourSecondArray();
    this->setMinuteFirstArray();
    this->setMinuteScondArray();
}
void ClockDisplay::refresh() {
    // TODO
    // load new time from clock_timer
    // set hour and minute
    this->setAllDigits();
    this->setAllArrays();
    this->setAllActiveLedsIndices();
}


/*
* LED LOGIC
*/
void ClockDisplay::setAllActiveLedsIndices() {
    std::vector<int> led_indices;
    for(coord_t element : this->hour_first_coords) {
        int index = getLedIndex(H1, element.first, element.second);
        led_indices.push_back(index);
    }
    for(coord_t element : this->hour_second_coords) {
        int index = getLedIndex(H2, element.first, element.second);
        led_indices.push_back(index);
    }
    for(coord_t element : this->minute_first_coords) {
        int index = getLedIndex(M1, element.first, element.second);
        led_indices.push_back(index);
    }
    for(coord_t element : this->minute_second_coords) {
        int index = getLedIndex(M2, element.first, element.second);
        led_indices.push_back(index);
    }

    this->active_led_indices = led_indices;
}
std::vector<int> ClockDisplay::getActiveLedIndices() {
    return this->active_led_indices; // arrow because it's a pointer
}

/*
* CUSTOM DISPLAYING
*/
void ClockDisplay::printTime() {
    timestamp_t timestamp = this->getTime();
    int h = timestamp.first; int m = timestamp.second;
    String s_h = String((h<10)? "0" : "") + h;
    String s_m = String((m<10)? "0" : "") + m;
    Serial.println();
    Serial.print(F("Current time is: "));
    Serial.print(s_h);
    Serial.print(':');
    Serial.println(s_m);
}
void ClockDisplay::showClockInConsole() {
    // top border
    Serial.println(F(" ——————————— "));

    // Hour first digit (1×3)
    Serial.print(F("| "));
    for (int c = 0; c < 3; ++c) {
        Serial.print(hour_first_array[c]);
        Serial.print(F(" | "));
    }
    Serial.println();

    // middle border
    Serial.println(F(" ——————————— "));

    // Hour second digit (3×3)
    for (int r = 0; r < 3; ++r) {
        Serial.print(F("| "));
        for (int c = 0; c < 3; ++c) {
            Serial.print(hour_second_array[r][c]);
            Serial.print(F(" | "));
        }
        Serial.println();
    }

    // border before minutes
    Serial.println(F(" ——————————— "));

    // Minute first digit (2×3)
    for (int r = 0; r < 2; ++r) {
        Serial.print(F("| "));
        for (int c = 0; c < 3; ++c) {
            Serial.print(minute_first_array[r][c]);
            Serial.print(F(" | "));
        }
        Serial.println();
    }

    // border before last digit
    Serial.println(F(" ——————————— "));

    // Minute second digit (3×3)
    for (int r = 0; r < 3; ++r) {
        Serial.print(F("| "));
        for (int c = 0; c < 3; ++c) {
            Serial.print(minute_second_array[r][c]);
            Serial.print(F(" | "));
        }
        Serial.println();
    }

    // bottom border
    Serial.println(F(" ——————————— "));
}
void ClockDisplay::printLedIndices() {
    Serial.println();
    Serial.println(F("Current active LEDs are:\n"));
    for (int index : this->active_led_indices) {
        Serial.print(index);
        Serial.print(' ');
    }
    Serial.println();
}


/*
* TIME SOURCING
*/
void ClockDisplay::machineTimeInput() {
    timestamp_t timestamp = getMachineHoursMinutes();
    this->setTime(timestamp);
}


