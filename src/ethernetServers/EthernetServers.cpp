// -----------------------------------------------------------------------------------
// Ethernet Web and Cmd servers

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"
#include "../pinmaps/Models.h"
#include "../debug/Debug.h"

#include <Ethernet.h>

#include "../commands/Commands.h"
#include "CmdServer.h"
#include "WebServer.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  char masterPassword[40] = Default_Password;

  #if STANDARD_COMMAND_CHANNEL == ON
    WebServer server;
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
  
  void ethernetStart(void) {
    VF("WEM: Ethernet Addon "); V(FirmwareVersionMajor); V("."); V(FirmwareVersionMinor); VL(FirmwareVersionPatch);
    VF("WEM: MCU = "); VLF(MCU_STR);

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
      cmdBuffer[cmdBufferPos] = b; cmdBufferPos++;
      if (cmdBufferPos > 39) cmdBufferPos = 39; cmdBuffer[cmdBufferPos] = 0;

      if (b == '#' || (strlen(cmdBuffer) == 1 && b == (char)6)) {
        char result[40] = "";
        processCommand(cmdBuffer, result, cmdTimeout);               // send cmd to OnStep, pickup response
        if (strlen(result) > 0) { cmdSvr.print(result); delay(2); }  // return any response to client
        cmdBuffer[0] = 0; cmdBufferPos = 0;
      } else idle();
    }
  }

#endif