// -----------------------------------------------------------------------------------
// Ethernet Web and Cmd servers
#pragma once

#include "../Common.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  #if defined(__TEENSYDUINO__) || defined(__M0__) || defined(ESP8266)
    #include "CmdServer.h"
    #include "WebServer.h"
    extern WebServer server;
  #else
    #error "Configuration (Config.h): No Ethernet support is present for this device"
  #endif

  void ethernetInit(void);
  void ethernetStart(void);
  extern WebServer server;

  #if STANDARD_COMMAND_CHANNEL == ON
    extern CmdServer cmdSvr;
    void ethernetCommandChannel(void);
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON
    extern CmdServer cmdSvrPersistant;
    void ethernetPersistantCommandChannel(void);
  #endif

  // macros to help with sending webpage data
  #define sendHtmlStart()
  #define sendHtml(x) client->print(x); x = ""
  #define sendHtmlDone(x) client->print(x); x = ""

#endif
