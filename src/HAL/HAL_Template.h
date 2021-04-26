// Platform setup ------------------------------------------------------------------------------------
#pragma once

// We define a more generic symbol, in case more Platform_Name boards based on different lines are supported
#define __Platform_Name__

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire
#define HAL_WIRE_CLOCK 100000

// Default serial port where OnStep is attached ------------------------------------------------------
#define SERIAL_ONSTEP Serial

// Non-volatile storage ------------------------------------------------------------------------------
#ifdef NV_DEFAULT
  #include "../lib/nv/NV.h"
#endif

//----------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#define HAL_INIT() { nv.init(0, false, 0, false); }

//-----------------------------------------------------------------------------------------------------
// Misc. includes and defines to support this processor's operation
#define IRAM_ATTR
#define ICACHE_RAM_ATTR
#define FPSTR
