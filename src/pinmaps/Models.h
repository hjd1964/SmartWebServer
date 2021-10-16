// -------------------------------------------------------------------------------------------------
// Loads pinmap model for current configuration
#pragma once

#include <Arduino.h>
#include "../Constants.h"
#include "../../Config.h"
#include "../../Extended.config.h"

#include "Pins.Esp32.h"
#include "Pins.Esp8266.h"
#include "Pins.M0.h"
#include "Pins.Teensy.h"

#if LED_STATUS_ON_STATE == HIGH
  #define LED_STATUS_OFF_STATE LOW
#else
  #define LED_STATUS_OFF_STATE HIGH
#endif
