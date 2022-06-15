// -------------------------------------------------------------------------------------------------
// Pin map for ESP8266
#pragma once

#define BOOT0_PIN          15  // pin D8, GPIO15 to Boot0 of STM32 etc. (no swapped serial if active)

#define AXIS1_ENC_A_PIN    14  // pin# for Axis1 encoder, for A or CW  or MA/CLK
#define AXIS1_ENC_B_PIN    12  // pin# for Axis1 encoder, for B or CCW or SLO
#define AXIS2_ENC_A_PIN    5   // pin# for Axis2 encoder, for A or CW  or MA/CLK
#define AXIS2_ENC_B_PIN    4   // pin# for Axis2 encoder, for B or CCW or SLO

#if AXIS1_ENC > 0 && AXIS2_ENC > 0
  #define VGPIO_PIN_0      OFF
  #define VGPIO_PIN_1      OFF
  #define VGPIO_PIN_2      OFF
  #define VGPIO_PIN_3      OFF
#else
  #define VGPIO_PIN_0      AXIS1_ENC_A_PIN
  #define VGPIO_PIN_1      AXIS1_ENC_B_PIN
  #define VGPIO_PIN_2      AXIS2_ENC_A_PIN
  #define VGPIO_PIN_3      AXIS2_ENC_B_PIN
#endif
#define VGPIO_PIN_4        OFF
#define VGPIO_PIN_5        OFF
#define VGPIO_PIN_6        OFF
#define VGPIO_PIN_7        OFF

#define ETHERNET_RESET_PIN D2  // pin# for controlling the reset of W5500 so it comes up properly
#define ETHERNET_CS_PIN    D1  // pin# for W5500 SPI CS

#define LED_STATUS_PIN     2   // pin GPIO2 is the status led

#define SD_CARD_CS_PIN     OFF // pin 4 to CS for SD adapter on Arduino Ethernet Shield
