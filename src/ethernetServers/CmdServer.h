// -----------------------------------------------------------------------------------
// Cmd channel server

#pragma once

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"

#include <Ethernet.h>

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  #define CMDSERVER_DEBUG OFF

  class CmdServer {
    public:
      void init(int port, long t);
      void handleClient();
      int available();
      char read();
      void print(char s[]);
    private:
      EthernetClient client;

      int thisPort = 0;
      long timeout = 0;
      bool haveClient = false;
      unsigned long lastAccess = 0;
  };

#endif
