#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <string>
#include <TimeLib.h>     // from “Time” library
#include <Timezone.h>    // from “Timezone” library
#include "utils.hpp"



/*
* DIGIT RESHAPING
*/
int firstDigit(int x) {
    std::string s = std::to_string(x);
    char c; int value;
    if(s.length() == 1) {value = 0;}
    else{
        c = s[0];
        value = c - '0';
    }
    return value;
}
int secondDigit(int x) {
    std::string s = std::to_string(x);
    char c; int value;
    if(s.length() == 1) {
        c = s[0];
        value = c - '0';
    }
    else{
        c = s[1];
        value = c - '0';
    }
    return value;
}


/*
* COORDINATE HANDLING
*/
coords_array_t generateNCoords(int n, int max_n, int max_m) {
    int total_grid_spaces = (max_n + 1) * (max_m + 1);
    if (n > total_grid_spaces) {
        Serial.println(F("Error: requested more coordinates than grid capacity"));
        return coords_array_t();  // return empty array
    }

    // build all coords
    static coord_t all_coords_storage[9];
    int idx = 0;
    for (int i = 0; i <= max_n; ++i) {
        for (int j = 0; j <= max_m; ++j) {
            all_coords_storage[idx++] = coord_t{i, j};
        }
    }

    // Fisher–Yates shuffle in-place
    for (int i = idx - 1; i > 0; --i) {
        int j = random(i + 1);  // random between 0 and i
        auto tmp = all_coords_storage[i];
        all_coords_storage[i] = all_coords_storage[j];
        all_coords_storage[j] = tmp;
    }

    // copy first n elements to a vector (if you still want a vector)
    coords_array_t result;
    result.reserve(n);
    for (int k = 0; k < n; ++k) {
        result.push_back(all_coords_storage[k]);
    }
    return result;
}


/*
* TIME RELATIVE
*/

//––– NTPClient setup –––
static WiFiUDP ntpUDP;
// pool.ntp.org, adjust to your favorite NTP pool
// UTC offset in seconds (e.g. CET = +1h → 3600) here it's 0 because handled by DST script,
// 30 min update interval → 30*60*1000 ms
static NTPClient timeClient(ntpUDP, "be.pool.ntp.org", /*utcOffset*/0, /*interval*/1800000);
// — define CET/CEST rules —
//   Last Sun Mar 02:00 → switch _into_ CEST (UTC+2h)
//   Last Sun Oct 03:00 → switch _into_ CET  (UTC+1h)
static TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};
static TimeChangeRule CET  = {"CET",  Last, Sun, Oct, 3, 60};
static Timezone      CE(CEST, CET);

void initTimeClient() {
  timeClient.begin();
  // do an initial sync immediately
  timeClient.update();
}
timestamp_t getMachineHoursMinutes() {
  timeClient.update(); // inop until 30 min have passed
  // get raw UTC epoch
  time_t utc = timeClient.getEpochTime();
  // convert to local UTC+1/UTC+2 based on DST rules
  time_t local = CE.toLocal(utc);
  // extract hour/minute via TimeLib
  return { hour(local), minute(local) };
}


/*
* SYSTEM UTILS
*/
void clearTerminal() {
    // prints a bunch of empty lines because there is no actual way to clear terminal
    for(int i = 0; i < 10; i++) {Serial.println();}
}
void sleepSeconds(unsigned int seconds) {
    delay(1000*seconds);
}
