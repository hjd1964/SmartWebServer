// Platform setup ------------------------------------------------------------------------------------
// ESP8266
#pragma once

// We define a more generic symbol, in case more Platform_Name boards based on different lines are supported

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire
#define HAL_WIRE_CLOCK 100000

// Default serial port where OnStep is attached ------------------------------------------------------
#include <Arduino.h>
#define SERIAL_ONSTEP Serial

// Non-volatile storage ------------------------------------------------------------------------------
#if NV_DRIVER == NV_DEFAULT
  #define E2END 1023
  #define NV_ENDURANCE NVE_LOW
  #include "../lib/nv/NV_ESP.h"
  #define HAL_NV_INIT() { nv.init(E2END + 1, false, 5000, false); }
#endif

//--------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#define HAL_INIT() { ; }

//-----------------------------------------------------------------------------------------------------
// Misc. includes and defines to support this processor's operation
