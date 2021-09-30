// Platform setup ------------------------------------------------------------------------------------
#pragma once

// We define a more generic symbol, in case more Platform_Name boards based on different lines are supported

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire
#define HAL_WIRE_CLOCK 100000

// Default serial port where OnStep is attached ------------------------------------------------------
#define SERIAL_ONSTEP Serial

// Non-volatile storage ------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#ifndef HAL_INIT
  #define HAL_INIT { nv.init(2048, true, 0, false); }
#endif

//-----------------------------------------------------------------------------------------------------
// Misc. includes and defines to support this processor's operation
