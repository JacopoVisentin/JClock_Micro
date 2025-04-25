#pragma once
#include <vector>
#include "utils.hpp"


/*
* CLOCK DISPLAY CLASS
*/
class ClockDisplay {
private:

    // time numbers
    int hour;
    int minute;

    // time digits
    int hour_first_digit;
    int hour_second_digit;
    int minute_first_digit;
    int minute_second_digit;

    // led matrices
    int hour_first_array[3];
    int hour_second_array[3][3];
    int minute_first_array[2][3];
    int minute_second_array[3][3];

    // coordinates for led index translation
    coords_array_t hour_first_coords;
    coords_array_t hour_second_coords;
    coords_array_t minute_first_coords;
    coords_array_t minute_second_coords;
    
    // the current active led indices
    std::vector<int> active_led_indices;


public:

    ClockDisplay(); // constructor

    // set and get time
    void setTime(int hour, int minute);
    void setTime(timestamp_t timestamp); // overloaded
    timestamp_t getTime();

    // setting all time digits from current time numbers
    void setAllDigits();

    // resetting led arrays to zeros
    void resetAllArrays();

    // setting 1s on the led arrays
    void setHourFirstArray();
    void setHourSecondArray();
    void setMinuteFirstArray();
    void setMinuteSecondArray();

    // setting led arrays
    void setAllArrays();
    std::vector<int> getActiveLedIndices();

    // refreshing all led arrays
    void refresh();

    // get lit led indices translation
    void setAllActiveLedsIndices();

    // printing the led arrays in console
    void printTime();
    void showClockInConsole();
    void printLedIndices();

    // automated time input
    void machineTimeInput();

};
