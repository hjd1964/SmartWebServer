// ---------------------------------------------------------------------------------------------------------------------------------
// Configuration for OnStep SmartWebServer infrequently used eXtended options

//                                 *** Most users will not change anything in this file! ***

// ---------------------------------------------------------------------------------------------------------------------------------
// ADJUST THE FOLLOWING TO CONFIGURE YOUR CONTROLLER FEATURES ----------------------------------------------------------------------
// <-Req'd = always must set, <-Often = usually must set, Option = optional, Adjust = adjust as req'd, Infreq = infrequently changed

// DEBUG ------------------------------------------------------------ see https://onstep.groups.io/g/main/wiki/6-Configuration#DEBUG
// Enable additional debugging and/or status messages on the specified SERIAL_DEBUG port
// Note that the SERIAL_DEBUG port cannot be used for normal communication with OnStep

//      Parameter Name              Value        Default  Notes                                                                 Hint
#define DEBUG                      REMOTE //         OFF, Use ON for background error messages only, use VERBOSE for all      Infreq
                                          //              error and status messages, use PROFILER for VT100 task profiler,
                                          //              use REMOTE to forward debug output to OnStep
#define SERIAL_DEBUG               Serial //    SERIAL_A, Or use any h/w serial port. Serial1 or Serial2, etc. as supported.  Option
#define SERIAL_DEBUG_BAUD          115200 //        9600, n. Where n=9600,19200,57600,115200 (common baud rates.)             Option

// ETHERNET SETTINGS ---------------------------------------------------------------------------------------------------------------

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500
//  #include <Ethernet.h>

  // Enter a unique MAC address for your controller if you like:
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

  // The IP addresses below will be dependent on your local network:
  IPAddress ip(192, 168, 1, 55);
  IPAddress myDns(192,168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

//  #include "src/ethernetServers/EthernetServers.h"
//#elif OPERATIONAL_MODE == WIFI
//  #include "src/wifiServers/WifiServers.h"
#endif

// PASSWORD -------------------------------------------------------------------------------------------------------------------------
#define Default_Password       "password" //  "password", Adjust as required, this can be changed at runtime also.             Option

// NON-VOLATILE STORAGE ------------------------------------------------------------------------------------------------------------
#define NV_DEFAULT                     ON //          ON, Uses HAL specified default, for remembering settings when           Infreq
                                          //              powered off.  This is usually done using the microcontrollers
                                          //              built-in EEPROM or FLASH emulation of EEPROM.  Sometimes (Bluepill)
                                          //              it uses an DS3231 RTC's onboard EEPROM AT24C32 chip.
