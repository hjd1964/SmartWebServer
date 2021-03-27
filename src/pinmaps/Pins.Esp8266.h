// -------------------------------------------------------------------------------------------------
// Pin map for ESP8266
#pragma once

#if defined(ESP8266)
  #define PINMAP_STR "ESP8266"

  #if DISPLAY_RESET_CONTROLS == FWU
    #define BOOT0_PIN     13 // pin D7, GPIO13 to Boot0 of STM32 (no swapped serial if active)
  #endif

  #define AXIS1_ENC_A_PIN 14 // pin# for Axis1 encoder, for A or CW
  #define AXIS1_ENC_B_PIN 12 // pin# for Axis1 encoder, for B or CCW
  #define AXIS2_ENC_A_PIN 5  // pin# for Axis1 encoder, for A or CW
  #define AXIS2_ENC_B_PIN 4  // pin# for Axis1 encoder, for B or CCW

  #define LED_STATUS_PIN  2  // pin GPIO2 is the status led
#endif
