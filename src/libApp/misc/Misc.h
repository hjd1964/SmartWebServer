// -----------------------------------------------------------------------------------
// Misc functions to help with commands, etc.
#pragma once

#include "../../Common.h"

typedef struct AxisSettings {
   double stepsPerMeasure;
   int8_t reverse;
   int16_t min;
   int16_t max;
   int16_t microsteps;
   int16_t IRUN;
   int16_t IGOTO;
   float p, i, d;
   double param1;
   double param2;
   double param3;
   bool isServo;
} AxisSettings;

// remove leading and trailing 0's
void stripNum(char* s);

// convert hex to int, returns -1 on error
int hexToInt(String s);

// convert time to compact byte representation for intervalometer
uint8_t timeToByte(float t);

// convert compact byte representation to time for intervalometer
float byteToTime(uint8_t b);

// convert axis settings string into numeric form
bool decodeAxisSettings(char* s, AxisSettings* a);

// convert axis settings string into numeric form
bool decodeAxisSettingsX(char* s, AxisSettings* a);

// validate axis settings for a given axis and mount type
bool validateAxisSettings(int axisNum, bool altAz, AxisSettings a);

// validate axis settings for a given axis and mount type
bool validateAxisSettingsX(int axisNum, bool altAz, AxisSettings a);

// return temperature string with proper value and units for this locale
void localeTemperature(char* temperatureStr, char* units);

// return pressure string with proper value and units for this locale
void localePressure(char* pressureStr, char* units);
