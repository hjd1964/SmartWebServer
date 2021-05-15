// -----------------------------------------------------------------------------------
// Bluetooth Gamepad variables

#pragma once

#include "../SmartWebServer.h"

#if BLE_GAMEPAD == ON
  extern void bleInit();
  extern void bleSetup();
  extern void bleTimers();
  extern void bleConnTest();
#endif
