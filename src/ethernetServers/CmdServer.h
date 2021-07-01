// -----------------------------------------------------------------------------------
// Cmd channel server
#pragma once

#include "../Common.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

#include <Ethernet.h>

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
