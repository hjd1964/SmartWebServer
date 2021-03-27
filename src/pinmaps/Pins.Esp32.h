// -------------------------------------------------------------------------------------------------
// Pin map for ESP32
#pragma once

#if defined(ESP32)
  #define PINMAP_STR "ESP32"

  #define AXIS1_ENC_A_PIN 18 // pin# for Axis1 encoder, for A or CW
  #define AXIS1_ENC_B_PIN 19 // pin# for Axis1 encoder, for B or CCW
  #define AXIS2_ENC_A_PIN 22 // pin# for Axis1 encoder, for A or CW
  #define AXIS2_ENC_B_PIN 21 // pin# for Axis1 encoder, for B or CCW

  #ifndef LED_STATUS_ON_STATE
    #define LED_STATUS_ON_STATE HIGH
  #endif
  #define LED_STATUS_PIN   2 // pin 2 is a guess and is probably wrong
                             // should be corrected for the common WeMos D1 Mini footprint ESP32

#endif
