#pragma once
#include <vector>
#include <utility>



/*
* CUSTOM TYPE DEFINITIONS
*/
typedef std::pair<int, int> coord_t;
typedef std::pair<int, int> timestamp_t;
typedef std::vector<coord_t> coords_array_t;


/*
* DIGIT RESHAPING
*/
int firstDigit(int x);
int secondDigit(int x);


/*
* COORDINATE HANDLING
*/
coords_array_t generateNCoords(int n, int max_n, int max_m);


/*
* TIME RELATIVE
*/
void initTimeClient();
timestamp_t getMachineHoursMinutes();


/*
* SYSTEM UTILS
*/
void clearTerminal();
void sleepSeconds(unsigned int seconds);
