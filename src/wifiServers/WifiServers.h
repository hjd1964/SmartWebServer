// -----------------------------------------------------------------------------------
// Wifi Web and Cmd servers
#pragma once

#include "../SmartWebServer.h"

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

  #ifndef LEGACY_TRANSMIT_ON
    // macros to help with sending webpage data, chunked
    #define sendHtmlStart() server.setContentLength(CONTENT_LENGTH_UNKNOWN); server.sendHeader("Cache-Control","no-cache"); server.send(200, "text/html", String());
    #define sendHtml(x) server.sendContent(x); x = ""
    #define sendHtmlDone(x) server.sendContent("");
  #else
    // macros to help with sending webpage data, normal method
    #define sendHtmlStart()
    #define sendHtml(x)
    #define sendHtmlDone(x) server.send(200, "text/html", x)
  #endif

#endif
