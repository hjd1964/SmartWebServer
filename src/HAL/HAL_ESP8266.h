// Platform setup ------------------------------------------------------------------------------------
// ESP8266
#pragma once

// We define a more generic symbol, in case more Platform_Name boards based on different lines are supported

// Analog read and write
#ifndef ANALOG_READ_RANGE
  #define ANALOG_READ_RANGE 1023
#endif
#ifndef ANALOG_WRITE_RANGE
  #define ANALOG_WRITE_RANGE 255
#endif
#ifndef ANALOG_WRITE_PWM_BITS
  #define ANALOG_WRITE_PWM_BITS 8
#endif

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire
#ifndef HAL_WIRE_CLOCK
  #define HAL_WIRE_CLOCK 100000
#endif

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
#define HAL_INIT() { analogWriteRange(ANALOG_WRITE_PWM_BITS); }

//-----------------------------------------------------------------------------------------------------
// Misc. includes and defines to support this processor's operation

// MCU reset
#define HAL_RESET() ESP.restart()

// stand-in for delayNanoseconds(), assumes 80MHz clock
#define delayNanoseconds(ns) { unsigned int c = ESP.getCycleCount() + ns/12.5F; do {} while ((int)(ESP.getCycleCount() - c) < 0); }
