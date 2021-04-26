// -----------------------------------------------------------------------------------
// Bluetooth Gamepad variables

#pragma once

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"

#if BLE_GAMEPAD == ON
  extern void bleInit();
  extern void bleSetup();
  extern void bleTimers();
  extern void bleConnTest();
#endif
