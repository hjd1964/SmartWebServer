// Platform setup ------------------------------------------------------------------------------------

// We define a more generic symbol, in case more Teensy boards based on different lines are supported
#define __TEENSYDUINO__

// New symbol for the default I2C port ---------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire
#define HAL_WIRE_CLOCK 100000

// Non-volatile storage ------------------------------------------------------------------------------
#ifdef NV_DEFAULT
  #include "../lib/nv/NV_EEPROM.h"
#endif

// Default serial port where OnStep is attached ------------------------------------------------------
#define Ser Serial

//----------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#define HAL_INIT() { analogReadResolution(10); nv.init(E2END + 1, true, 0, false); }

//-----------------------------------------------------------------------------------------------------
// Misc. includes to support this processor's operation
// #include "imxrt.h"
#define ICACHE_RAM_ATTR
#define FPSTR
