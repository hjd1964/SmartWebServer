// -----------------------------------------------------------------------------------
// Misc functions to help with commands, etc.
#pragma once

#include "../../Common.h"

// convert hex to int, returns -1 on error
int hexToInt(String s);

// convert time to compact byte representation for intervalometer
uint8_t timeToByte(float t);

// convert compact byte representation to time for intervalometer
float byteToTime(uint8_t b);

// return temperature string with proper value and units for this locale
void localeTemperature(char* temperatureStr);

// return pressure string with proper value and units for this locale
void localePressure(char* pressureStr);

// return humidity string with proper value and units for this locale
void localeHumidity(char* humidityStr);
