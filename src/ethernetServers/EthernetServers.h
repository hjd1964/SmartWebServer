// -----------------------------------------------------------------------------------
// Ethernet Web and Cmd servers

#pragma once

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"

#include <Ethernet.h>

#include "CmdServer.h"
#include "WebServer.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  #if defined(__TEENSYDUINO__) || defined(__M0__)
    #include <Ethernet.h>
    extern WebServer server;
    #include "CmdServer.h"
    #include "WebServer.h"
  #else
    #error "Configuration (Config.h): No Ethernet support is present for this device"
  #endif

  #if STANDARD_COMMAND_CHANNEL == ON
    extern WebServer server;
    extern CmdServer cmdSvr;
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON
  #endif

  void ethernetStart(void);
  void ethernetCommandChannel(void);

  // macros to help with sending webpage data
  #define sendHtmlStart()
  #define sendHtml(x) client->print(x); x = ""
  #define sendHtmlDone(x) client->print(x); x = ""

#endif
