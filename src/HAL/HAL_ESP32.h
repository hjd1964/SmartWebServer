// Platform setup ------------------------------------------------------------------------------------
// ESP32
#pragma once

// We define a more generic symbol, in case more Platform_Name boards based on different lines are supported

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire
#ifndef HAL_WIRE_CLOCK
  #define HAL_WIRE_CLOCK 100000
#endif

// Default serial port where OnStep is attached ------------------------------------------------------
#define SERIAL_ONSTEP Serial1

// Non-volatile storage ------------------------------------------------------------------------------
#if NV_DRIVER == NV_DEFAULT
  #define E2END 1023
  #define NV_ENDURANCE NVE_LOW
  #include "../lib/nv/NV_ESP.h"
  #define HAL_NV_INIT() { nv.init(E2END + 1, false, 1000, false); }
#endif

//--------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#define HAL_INIT() { ; }

//-----------------------------------------------------------------------------------------------------
// Misc. includes and defines to support this processor's operation

// MCU reset
#define HAL_RESET() ESP.restart()

#ifdef ARDUINO_ESP32C3_DEV
  // stand-in for delayNanoseconds(), assumes 80MHz clock
  #define delayNanoseconds(ns) { unsigned int c = ESP.getCycleCount() + ns/12.5F; do {} while ((int)(ESP.getCycleCount() - c) < 0); }
#else
  // stand-in for delayNanoseconds(), assumes 240MHz clock
  #include "xtensa/core-macros.h"
  #define delayNanoseconds(ns) { unsigned int c = xthal_get_ccount() + ns/4.166F; do {} while ((int)(xthal_get_ccount() - c) < 0); }
#endif
