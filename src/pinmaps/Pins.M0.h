// -------------------------------------------------------------------------------------------------
// Pin map for Arduino M0
#pragma once

#if defined(__M0__)
  #define PINMAP_STR "ARDUINO M0"

  #define AXIS1_ENC_A_PIN 5  // pin# for Axis1 encoder, for A or CW
  #define AXIS1_ENC_B_PIN 6  // pin# for Axis1 encoder, for B or CCW
  #define AXIS2_ENC_A_PIN 7  // pin# for Axis2 encoder, for A or CW
  #define AXIS2_ENC_B_PIN 8  // pin# for Axis2 encoder, for B or CCW

  #define LED_STATUS_PIN  13 // pin 13 is probably the status led, a guess
#endif
