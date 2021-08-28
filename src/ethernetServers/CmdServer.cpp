// -----------------------------------------------------------------------------------
// Cmd channel server

#include "CmdServer.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  // t is the timeout in ms, 0 for never
  void CmdServer::init(uint16_t port, long timeout) {
    cmdserver = new EthernetServer(port);
    thisPort = port;
    cmdserver->begin();
    this->timeout = timeout;
    #if CMDSERVER_DEBUG == ON
      VF("WEM: Ethernet IP     = "); V(Ethernet.localIP()); VF(":"); VL(thisPort);
    #endif
  }

  void CmdServer::handleClient() {
  if (thisPort == 0) return;
    if (!haveClient) {
      // new client connect
      client = cmdserver->available();
      if (client) {
        lastAccess = millis();
        haveClient = true;
        #if CMDSERVER_DEBUG == ON
          VLF("WEM: Client connected");
        #endif
      }
    } else {
      // client disconnect
      if (!client.connected() || (timeout != 0 && (long)(millis() - lastAccess) > timeout)) {
        client.stop();
        client = EthernetClient();
        haveClient = false;
        #if CMDSERVER_DEBUG == ON
          VF("WEM: Client disconnected");
          if ((long)(millis() - lastAccess) > timeout) VLF(" (timed out)"); else VLF("");
        #endif
      }
    }
  }

  int CmdServer::available() {
    if (thisPort == 0) return 0;
    if (!haveClient) return 0;
    if (!client.connected()) return 0;
    lastAccess = millis();
    return client.available();
  }

  char CmdServer::read() {
    if (thisPort == 0) return 0;
    lastAccess = millis();
    return client.read();
  }

  void CmdServer::print(char s[]) {
    if (thisPort == 0) return;
    if (!client.connected()) return;
    lastAccess = millis();
    client.write(s);
  }
#endif