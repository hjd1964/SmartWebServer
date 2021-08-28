// -----------------------------------------------------------------------------------
// Cmd channel server
#pragma once

#include "../Common.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  #ifdef ESP8266
    #ifndef ETHERNET_W5500
      #error "The ESP8266 Ethernet option supports the W5500 only"
    #endif
    #include <Ethernet2.h>  // https://github.com/adafruit/Ethernet2
  #else
    #include <Ethernet.h>
  #endif

  #define CMDSERVER_DEBUG OFF

  class CmdServer {
    public:
      void init(uint16_t port, long timeout);
      void handleClient();
      int available();
      char read();
      void print(char s[]);
    private:
      EthernetClient client;
      EthernetServer *cmdserver;

      int thisPort = 0;
      long timeout = 0;
      bool haveClient = false;
      unsigned long lastAccess = 0;
  };

#endif
