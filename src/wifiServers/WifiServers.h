#pragma once

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"

#if OPERATIONAL_MODE == WIFI
  #if defined(ESP32)
    #include <WiFi.h>
    #include <WiFiClient.h>
    #include <WiFiAP.h>
    #include <WebServer.h>
    extern WebServer server;
  #elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <WiFiClient.h>
    #include <ESP8266WiFiAP.h>
    #include <ESP8266WebServer.h>
    extern ESP8266WebServer server;
  #else
    #error "Configuration (Config.h): No Wifi support is present for this device"
  #endif

  #if STANDARD_COMMAND_CHANNEL == ON
    extern WiFiServer cmdSvr;
    extern WiFiClient cmdSvrClient;
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON
    extern WiFiServer persistentCmdSvr;
    extern WiFiClient persistentCmdSvrClient;
  #endif

  void wifiInit(void);
  void wifiStart(void);
  void wifiCommandChannel(void);
  void wifiPersistantCommandChannel(void);

#endif
