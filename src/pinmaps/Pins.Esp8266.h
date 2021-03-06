// -------------------------------------------------------------------------------------------------
// Pin map for ESP8266
#pragma once

#if defined(ESP8266)
  #define PINMAP_STR "ESP8266"

  #define BOOT0_PIN        15  // pin D8, GPIO15 to Boot0 of STM32 etc. (no swapped serial if active)

  #define AXIS1_ENC_A_PIN  14  // pin# for Axis1 encoder, for A or CW  or MA/CLK
  #define AXIS1_ENC_B_PIN  12  // pin# for Axis1 encoder, for B or CCW or SLO
  #define AXIS2_ENC_A_PIN  5   // pin# for Axis1 encoder, for A or CW  or MA/CLK
  #define AXIS2_ENC_B_PIN  4   // pin# for Axis1 encoder, for B or CCW or SLO

  #define W5500_RESET_PIN  9   // pin# for controlling the reset of W5500 so it comes up properly

  #define LED_STATUS_PIN   2   // pin GPIO2 is the status led

  #define SD_CARD_CS_PIN   OFF // pin 4 to CS for SD adapter on Arduino Ethernet Shield
#endif
