// -------------------------------------------------------------------------------------------------
// Pin map for Arduino M0
#pragma once

#if defined(__M0__)
  #define PINMAP_STR "ARDUINO M0"

  #define BOOT0_PIN          10  // pin 10 to Boot0 of STM32 etc.

  #define AXIS1_ENC_A_PIN    5   // pin# for Axis1 encoder, for A or CW  or MA/CLK
  #define AXIS1_ENC_B_PIN    6   // pin# for Axis1 encoder, for B or CCW or SLO
  #define AXIS2_ENC_A_PIN    7   // pin# for Axis2 encoder, for A or CW  or MA/CLK
  #define AXIS2_ENC_B_PIN    8   // pin# for Axis2 encoder, for B or CCW or SLO

  #define ETHERNET_RESET_PIN 9   // pin# for controlling the reset of W5500 so it comes up properly

  #define LED_STATUS_PIN     13  // pin 13 is probably the status led, a guess

  #define SD_CARD_CS_PIN     4   // pin 4 to CS for SD adapter on Arduino Ethernet Shield
#endif
