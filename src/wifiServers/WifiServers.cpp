// -----------------------------------------------------------------------------------
// Wifi Web and Cmd servers

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"
extern NVS nv;
#include "../debug/Debug.h"

#if OPERATIONAL_MODE == WIFI
  #include "../commands/Commands.h"

  #include "WifiServers.h"

  #if defined(ESP32)
    WebServer server(80);
  #elif defined(ESP8266)
    ESP8266WebServer server(80);
  #endif

  char masterPassword[40] = PASSWORD_DEFAULT;

  bool accessPointEnabled = AP_ENABLED;
  bool stationEnabled     = STA_ENABLED;
  bool stationDhcpEnabled = STA_DHCP_ENABLED;

  char wifi_sta_ssid[40] = STA_SSID;
  char wifi_sta_pwd[40] = STA_PASSWORD;

  IPAddress wifi_sta_ip = IPAddress STA_IP_ADDR;
  IPAddress wifi_sta_gw = IPAddress STA_GW_ADDR;
  IPAddress wifi_sta_sn = IPAddress STA_SN_MASK;

  char wifi_ap_ssid[40] = AP_SSID;
  char wifi_ap_pwd[40] = AP_PASSWORD;
  byte wifi_ap_ch = AP_CHANNEL;

  IPAddress wifi_ap_ip = IPAddress AP_IP_ADDR;
  IPAddress wifi_ap_gw = IPAddress AP_GW_ADDR;
  IPAddress wifi_ap_sn = IPAddress AP_SN_MASK;

  #if STANDARD_COMMAND_CHANNEL == ON
    WiFiServer cmdSvr(9999);
    WiFiClient cmdSvrClient;
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON
    WiFiServer persistentCmdSvr(9998);
    WiFiClient persistentCmdSvrClient;
  #endif

  void idle() {
    server.handleClient(); 
    #if ENCODERS == ON
      encoders.poll();
    #endif
  }

  void wifiInit(void) {
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);

    if (nv.readI(EE_KEY_HIGH) != NV_KEY_HIGH || nv.readI(EE_KEY_LOW) != NV_KEY_LOW) {
      VLF("WEM: bad NV key, reset Wifi defaults");
      nv.update(EE_AP_EN, (int16_t)accessPointEnabled);
      nv.update(EE_STA_EN, (int16_t)stationEnabled);
      nv.update(EE_DHCP_EN, (int16_t)stationDhcpEnabled);

      nv.update(EE_TIMEOUT_WEB, (int16_t)webTimeout);
      nv.update(EE_TIMEOUT_CMD, (int16_t)cmdTimeout);

      nv.updateBytes(EE_STA_SSID, wifi_sta_ssid, -40);
      nv.updateBytes(EE_STA_PWD, wifi_sta_pwd, -40);
      nv.updateBytes(EE_PASSWORD, masterPassword, -40);
      for (int i = 0; i < 4; i++) nv.update(EE_STA_IP + i, wifi_sta_ip[i]);
      for (int i = 0; i < 4; i++) nv.update(EE_STA_GW + i, wifi_sta_gw[i]);
      for (int i = 0; i < 4; i++) nv.update(EE_STA_SN + i, wifi_sta_sn[i]);

      nv.updateBytes(EE_AP_SSID, wifi_ap_ssid, -40);
      nv.updateBytes(EE_AP_PWD, wifi_ap_pwd, -40);
      nv.update(EE_AP_CH, (int16_t)wifi_ap_ch);
      for (int i = 0; i < 4; i++) nv.update(EE_AP_IP + i, wifi_ap_ip[i]);
      for (int i = 0; i < 4; i++) nv.update(EE_AP_GW + i, wifi_ap_gw[i]);
      for (int i = 0; i < 4; i++) nv.update(EE_AP_SN + i, wifi_ap_sn[i]);
    }

    VLF("WEM: NV reading Wifi settings");
    accessPointEnabled = nv.readI(EE_AP_EN);
    stationEnabled = nv.readI(EE_STA_EN);
    if (!accessPointEnabled && !stationEnabled) accessPointEnabled = true;
    stationDhcpEnabled = nv.readI(EE_DHCP_EN);

    webTimeout = nv.readI(EE_TIMEOUT_WEB);
    if (webTimeout > 300) webTimeout = 300;
    if (webTimeout < 100) webTimeout = 100;
    cmdTimeout = nv.readI(EE_TIMEOUT_CMD);
    if (cmdTimeout > 300) cmdTimeout = 300;
    if (cmdTimeout < 100) cmdTimeout = 100;

    nv.readBytes(EE_STA_SSID, wifi_sta_ssid, -40);
    nv.readBytes(EE_STA_PWD, wifi_sta_pwd, -40);
    nv.readBytes(EE_PASSWORD, masterPassword, -40);
    for (int i = 0; i < 4; i++) wifi_sta_ip[i] = nv.read(EE_STA_IP + i);
    for (int i = 0; i < 4; i++) wifi_sta_gw[i] = nv.read(EE_STA_GW + i);
    for (int i = 0; i < 4; i++) wifi_sta_sn[i] = nv.read(EE_STA_SN + i);

    nv.readBytes(EE_AP_SSID, wifi_ap_ssid, -40);
    nv.readBytes(EE_AP_PWD, wifi_ap_pwd, -40);
    wifi_ap_ch = nv.readI(EE_AP_CH);
    for (int i = 0; i < 4; i++) wifi_ap_ip[i] = nv.read(EE_AP_IP + i);
    for (int i = 0; i < 4; i++) wifi_ap_gw[i] = nv.read(EE_AP_GW + i);
    for (int i = 0; i < 4; i++) wifi_ap_sn[i] = nv.read(EE_AP_SN + i);
  }

  void wifiStart(void) {
    VF("WEM: Master Pwd = "); VL(masterPassword);

    VF("WEM: Web Ch Timeout ms = "); VL(webTimeout);
    VF("WEM: Cmd Ch Timeout ms = "); VL(cmdTimeout);

    VF("WEM: WiFi AP Enabled  = "); VL(accessPointEnabled);
    VF("WEM: WiFi Sta Enabled = "); VL(stationEnabled);
    VF("WEM: WiFi Sta DHCP En = "); VL(stationDhcpEnabled);

    VF("WEM: WiFi STA SSID    = "); VL(wifi_sta_ssid);
    VF("WEM: WiFi STA PWD     = "); VL(wifi_sta_pwd);
    VF("WEM: WiFi STA IP      = "); VL(wifi_sta_ip.toString());
    VF("WEM: WiFi STA GATEWAY = "); VL(wifi_sta_gw.toString());
    VF("WEM: WiFi STA SN      = "); VL(wifi_sta_sn.toString());

    VF("WEM: WiFi AP SSID     = "); VL(wifi_ap_ssid);
    VF("WEM: WiFi AP PWD      = "); VL(wifi_ap_pwd);
    VF("WEM: WiFi AP CH       = "); VL(wifi_ap_ch);
    VF("WEM: WiFi AP IP       = "); VL(wifi_ap_ip.toString());
    VF("WEM: WiFi AP GATEWAY  = "); VL(wifi_ap_gw.toString());
    VF("WEM: WiFi AP SN       = "); VL(wifi_ap_sn.toString());

  TryAgain:
    if (accessPointEnabled && !stationEnabled) {
      VLF("WEM: Starting WiFi Soft AP");
      WiFi.softAP(wifi_ap_ssid, wifi_ap_pwd, wifi_ap_ch);
      WiFi.mode(WIFI_AP);
    } else
    if (!accessPointEnabled && stationEnabled) {
      VLF("WEM: Starting WiFi Station");
      WiFi.begin(wifi_sta_ssid, wifi_sta_pwd);
      WiFi.mode(WIFI_STA);
    } else
    if (accessPointEnabled && stationEnabled) {
      VLF("WEM: Starting WiFi Soft AP");
      WiFi.softAP(wifi_ap_ssid, wifi_ap_pwd, wifi_ap_ch);
      VLF("WEM: Starting WiFi Station");
      WiFi.begin(wifi_sta_ssid, wifi_sta_pwd);
      WiFi.mode(WIFI_AP_STA);
    }

    delay(100);
    if ((stationEnabled) && (!stationDhcpEnabled)) WiFi.config(wifi_sta_ip, wifi_sta_gw, wifi_sta_sn);
    if (accessPointEnabled) WiFi.softAPConfig(wifi_ap_ip, wifi_ap_gw, wifi_ap_sn);

    // wait for connection in station mode, if it fails fall back to access-point mode
    if (!accessPointEnabled && stationEnabled) {
      for (int i = 0; i < 8; i++) if (WiFi.status() != WL_CONNECTED) delay(1000); else break;
      if (WiFi.status() != WL_CONNECTED) {
        VLF("WEM: Starting WiFi Station, failed");
        WiFi.disconnect(); delay(3000);
        VLF("WEM: Switching to WiFi Soft AP mode");
        stationEnabled = false;
        accessPointEnabled = true;
        goto TryAgain;
      }
    }
  }

  void wifiCommandChannel(void) {
    #if STANDARD_COMMAND_CHANNEL == ON
      // Standard IP connections on port 9999

      // disconnect client
      static unsigned long clientTime = 0;
      if (cmdSvrClient && !cmdSvrClient.connected()) cmdSvrClient.stop();
      if (cmdSvrClient && (long)(clientTime-millis()) < 0) cmdSvrClient.stop();

      // new client
      if (!cmdSvrClient && cmdSvr.hasClient()) {
        // find free/disconnected spot
        cmdSvrClient = cmdSvr.available();
        clientTime = millis() + 2000UL;
      }

      // check clients for data, if found get the command, pass to OnStep and pickup the response, then return the response to client
      while (cmdSvrClient && cmdSvrClient.connected() && cmdSvrClient.available() > 0) {
        static char cmdBuffer[40] = "";
        static int cmdBufferPos = 0;

        // get the data
        byte b = cmdSvrClient.read();
        cmdBuffer[cmdBufferPos] = b; cmdBufferPos++; if (cmdBufferPos > 39) cmdBufferPos = 39; cmdBuffer[cmdBufferPos] = 0;

        // send cmd and pickup the response
        if (b == '#' || (strlen(cmdBuffer) == 1 && b == (char)6)) {
          char result[40]="";
          // send cmd to OnStep, pickup response
          processCommand(cmdBuffer, result, cmdTimeout);
          if (strlen(result) > 0) {
            if (cmdSvrClient && cmdSvrClient.connected()) {
              cmdSvrClient.print(result);
              delay(2);
            }
          }
          cmdBuffer[0] = 0; cmdBufferPos = 0;
        } else idle();
      }
    #endif
  }

  void wifiPersistantCommandChannel(void) {
    #if PERSISTENT_COMMAND_CHANNEL == ON
      // Persistent IP connections on port 9998

      // disconnect client
      static unsigned long persistentClientTime = 0;
      if (persistentCmdSvrClient && !persistentCmdSvrClient.connected()) persistentCmdSvrClient.stop();
      if (persistentCmdSvrClient && (long)(persistentClientTime - millis()) < 0) persistentCmdSvrClient.stop();

      // new client
      if (!persistentCmdSvrClient && persistentCmdSvr.hasClient()) {
        // find free/disconnected spot
        persistentCmdSvrClient = persistentCmdSvr.available();
        persistentClientTime = millis() + 120000UL;
      }

      // check clients for data, if found get the command, pass to OnStep and pickup the response, then return the response to client
      while (persistentCmdSvrClient && persistentCmdSvrClient.connected() && persistentCmdSvrClient.available() > 0) {
        static char cmdBuffer[40] = "";
        static int cmdBufferPos = 0;

        // still active? push back disconnect by 2 minutes
        persistentClientTime = millis() + 120000UL;

        // get the data
        byte b = persistentCmdSvrClient.read();
        cmdBuffer[cmdBufferPos] = b; cmdBufferPos++; if (cmdBufferPos > 39) cmdBufferPos = 39; cmdBuffer[cmdBufferPos] = 0;

        // send cmd and pickup the response
        if (b == '#' || (strlen(cmdBuffer) == 1 && b == (char)6)) {
          char result[40] = "";
          // send cmd to OnStep, pickup response
          processCommand(cmdBuffer, result, cmdTimeout);
          if (strlen(result) > 0) { if (persistentCmdSvrClient && persistentCmdSvrClient.connected()) {
            persistentCmdSvrClient.print(result);
            delay(2);
            }
          }
          cmdBuffer[0] = 0; cmdBufferPos = 0;
        } else idle();
      }
    #endif
  }

#endif