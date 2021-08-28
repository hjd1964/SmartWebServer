// -----------------------------------------------------------------------------------
// Ethernet Web and Cmd servers

#include "EthernetServers.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  extern NVS nv;

  #include "../commands/Commands.h"

  char masterPassword[40] = PASSWORD_DEFAULT;

  WebServer server;

  void idle() {
    server.handleClient();
    #if ENCODERS == ON
      encoders.poll();
    #endif
  }
  
  void ethernetInit(void) {
    if (nv.readI(EE_KEY_HIGH) != NV_KEY_HIGH || nv.readI(EE_KEY_LOW) != NV_KEY_LOW) {
      VLF("WEM: bad NV key, reset Eth defaults");
      nv.update(EE_TIMEOUT_WEB, (int16_t)webTimeout);
      nv.update(EE_TIMEOUT_CMD, (int16_t)cmdTimeout);

      nv.updateBytes(EE_PASSWORD, masterPassword, -40);
      for (int i = 0; i < 4; i++) nv.update(EE_ETH_IP + i, eth_ip[i]);
      for (int i = 0; i < 4; i++) nv.update(EE_ETH_GW + i, eth_gw[i]);
      for (int i = 0; i < 4; i++) nv.update(EE_ETH_SN + i, eth_sn[i]);
    }

    VLF("WEM: NV reading Ethernet settings");
    webTimeout = nv.readI(EE_TIMEOUT_WEB);
    if (webTimeout > 300) webTimeout = 300;
    if (webTimeout < 100) webTimeout = 100;
    cmdTimeout = nv.readI(EE_TIMEOUT_CMD);
    if (cmdTimeout > 300) cmdTimeout = 300;
    if (cmdTimeout < 100) cmdTimeout = 100;

    nv.readBytes(EE_PASSWORD, masterPassword, -40);
    for (int i = 0; i < 4; i++) eth_ip[i] = nv.read(EE_ETH_IP + i);
    for (int i = 0; i < 4; i++) eth_gw[i] = nv.read(EE_ETH_GW + i);
    for (int i = 0; i < 4; i++) eth_dns[i] = nv.read(EE_ETH_GW + i);
    for (int i = 0; i < 4; i++) eth_sn[i] = nv.read(EE_ETH_SN + i);
  }

  void ethernetStart(void) {
    VF("WEM: Master Pwd = "); VL(masterPassword);

    VF("WEM: Web Ch Timeout ms = "); VL(webTimeout);
    VF("WEM: Cmd Ch Timeout ms = "); VL(cmdTimeout);

    VF("WEM: Eth IP = "); V(eth_ip[0]); V("."); V(eth_ip[1]); V("."); V(eth_ip[2]); V("."); VL(eth_ip[3]);
    VF("WEM: Eth GW = "); V(eth_gw[0]); V("."); V(eth_gw[1]); V("."); V(eth_gw[2]); V("."); VL(eth_gw[3]);
    VF("WEM: Eth SN = "); V(eth_sn[0]); V("."); V(eth_sn[1]); V("."); V(eth_sn[2]); V("."); VL(eth_sn[3]);

    #if OPERATIONAL_MODE == ETHERNET_W5500
      VF("WEM: Resetting W5500 using W5500_RESET_PIN ("); V(W5500_RESET_PIN); VL(")");
      pinMode(W5500_RESET_PIN, OUTPUT); 
      digitalWrite(W5500_RESET_PIN, LOW);
      delayMicroseconds(500);
      digitalWrite(W5500_RESET_PIN, HIGH);
      delayMicroseconds(1000);
      delay(1000);
    #endif
    server.init();
  }

  #if STANDARD_COMMAND_CHANNEL == ON
    CmdServer cmdSvr;

    void ethernetCommandChannel(void) {
      cmdSvr.handleClient();

      // check clients for data, if found get the command, send cmd and pickup the response, then return the response
      static char cmdBuffer[40] = "";
      static int cmdBufferPos = 0;
      while (cmdSvr.available()) {
        byte b = cmdSvr.read();
        cmdBuffer[cmdBufferPos] = b;
        cmdBufferPos++;
        if (cmdBufferPos > 39) cmdBufferPos = 39;
        cmdBuffer[cmdBufferPos] = 0;

        if (b == '#' || (strlen(cmdBuffer) == 1 && b == (char)6)) {
          char result[40] = "";
          processCommand(cmdBuffer, result, cmdTimeout);               // send cmd to OnStep, pickup response
          if (strlen(result) > 0) { cmdSvr.print(result); delay(2); }  // return any response to client
          cmdBuffer[0] = 0; cmdBufferPos = 0;
        } else idle();
      }
    }
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON
    CmdServer cmdSvrPersistant;

    void ethernetPersistantCommandChannel(void) {
      cmdSvrPersistant.handleClient();

      // check clients for data, if found get the command, send cmd and pickup the response, then return the response
      static char cmdBuffer[40] = "";
      static int cmdBufferPos = 0;
      while (cmdSvrPersistant.available()) {
        byte b = cmdSvrPersistant.read();
        cmdBuffer[cmdBufferPos] = b;
        cmdBufferPos++;
        if (cmdBufferPos > 39) cmdBufferPos = 39;
        cmdBuffer[cmdBufferPos] = 0;

        if (b == '#' || (strlen(cmdBuffer) == 1 && b == (char)6)) {
          char result[40] = "";
          processCommand(cmdBuffer, result, cmdTimeout);                         // send cmd to OnStep, pickup response
          if (strlen(result) > 0) { cmdSvrPersistant.print(result); delay(2); }  // return any response to client
          cmdBuffer[0] = 0; cmdBufferPos = 0;
        } else idle();
      }
    }
  #endif

#endif