// -------------------------------------------------------------------------------------------------
// Pin map for Teensy 3.0, 3.1, 3.2, 3.5, 3.6, 4.0
#pragma once

#if defined(__TEENSYDUINO__)
  #define PINMAP_STR "TEENSY"
  
  #define BOOT0_PIN        4  // pin 4, to Boot0 of STM32 etc.

  #define AXIS1_ENC_A_PIN  5   // pin# for Axis1 encoder, for A or CW
  #define AXIS1_ENC_B_PIN  6   // pin# for Axis1 encoder, for B or CCW
  #define AXIS2_ENC_A_PIN  7   // pin# for Axis2 encoder, for A or CW
  #define AXIS2_ENC_B_PIN  8   // pin# for Axis2 encoder, for B or CCW

  #define ETH_RESET_PIN    9   // pin# for controlling the reset of W5500 so it comes up properly

  #define LED_STATUS_PIN   11  // pin 11 is the status led

  #define SD_CARD_CS_PIN   OFF // pin 4 to CS for SD adapter on Arduino Ethernet Shield
#endif
