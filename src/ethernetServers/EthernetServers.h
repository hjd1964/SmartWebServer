// -----------------------------------------------------------------------------------
// Ethernet Web and Cmd servers
#pragma once

#include "../Common.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

#include <Ethernet.h>

#include "CmdServer.h"
#include "WebServer.h"

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

  void ethernetInit(void);
  void ethernetStart(void);
  void ethernetCommandChannel(void);

  // macros to help with sending webpage data
  #define sendHtmlStart()
  #define sendHtml(x) client->print(x); x = ""
  #define sendHtmlDone(x) client->print(x); x = ""

#endif
