// -----------------------------------------------------------------------------------
// Ethernet Web and Cmd servers

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"
extern NVS nv;
#include "../pinmaps/Models.h"
#include "../debug/Debug.h"

#include <Ethernet.h>

#include "../commands/Commands.h"
#include "CmdServer.h"
#include "WebServer.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  char masterPassword[40] = Default_Password;

  WebServer server;

  #if STANDARD_COMMAND_CHANNEL == ON
    CmdServer cmdSvr;
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON
  #endif

  void idle() {
    server.handleClient();
    #if ENCODERS == ON
      encoders.poll();
    #endif
  }
  
  void ethernetInit(void) {
    if (nv.readI(EE_KEY_HIGH) != NV_KEY_HIGH || nv.readI(EE_KEY_LOW) != NV_KEY_LOW) {
      VLF("WEM: NV key invalid, resetting Ethernet defaults");
      nv.update(EE_TIMEOUT_WEB, (int16_t)webTimeout);
      nv.update(EE_TIMEOUT_CMD, (int16_t)cmdTimeout);

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

    for (int i = 0; i < 4; i++) eth_ip[i] = nv.read(EE_ETH_IP + i);
    for (int i = 0; i < 4; i++) eth_gw[i] = nv.read(EE_ETH_GW + i);
    for (int i = 0; i < 4; i++) eth_dns[i] = nv.read(EE_ETH_GW + i);
    for (int i = 0; i < 4; i++) eth_sn[i] = nv.read(EE_ETH_SN + i);
  }

  void ethernetStart(void) {
    VLF("WEM: Ethernet Server start");

    VF("WEM: Web Channel Timeout ms= "); VL(webTimeout);
    VF("WEM: Cmd Channel Timeout ms= "); VL(cmdTimeout);

    // VF("WEM: Ethernet IP     = "); VL(ip.toString());
    // VF("WEM: Ethernet GATEWAY= "); VL(gateway.toString());
    // VF("WEM: Ethernet SUBNET = "); VL(subnet.toString());

    #if OPERATIONAL_MODE == ETHERNET_W5500
      VF("WEM: Resetting W5500 using BOOT0_PIN ("); V(BOOT0_PIN); VL(")");
      pinMode(BOOT0_PIN, OUTPUT); 
      digitalWrite(BOOT0_PIN, LOW);
      delayMicroseconds(500);
      digitalWrite(BOOT0_PIN, HIGH);
      delayMicroseconds(1000);
      delay(1000);
    #endif
  }

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