// Platform setup ------------------------------------------------------------------------------------
// Arduino M0
#pragma once

// We define a more generic symbol, in case more Teensy boards based on different lines are supported
#define __M0__

#include <avr/dtostrf.h>

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
#define SERIAL_ONSTEP Serial1

// Non-volatile storage ------------------------------------------------------------------------------
#if NV_DRIVER == NV_DEFAULT
  // Library FlashStorage defaults to 1024 bytes https://github.com/cmaglie/FlashStorage
  #define E2END 1023
  #include "../lib/nv/NV_M0.h"
  #define HAL_NV_INIT() { nv.init(E2END + 1, false, 1000, false); }
#endif

//----------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#define HAL_INIT() { ; }

//-----------------------------------------------------------------------------------------------------
// Misc. includes and defines to support this processor's operation

// stand-in for delayNanoseconds()
#define delayNanoseconds(ns) delayMicroseconds(ceilf(ns/1000.0F))
