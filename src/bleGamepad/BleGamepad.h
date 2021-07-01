// -----------------------------------------------------------------------------------
// Bluetooth Gamepad variables

#pragma once

#include "../Common.h"

#if BLE_GAMEPAD == ON
  extern void bleInit();
  extern void bleSetup();
  extern void bleTimers();
  extern void bleConnTest();
#endif
