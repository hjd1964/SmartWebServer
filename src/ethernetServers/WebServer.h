// -----------------------------------------------------------------------------------
// Web server
#pragma once

#include "../Common.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  #ifdef ESP8266
    #include <Ethernet2.h>  // https://github.com/adafruit/Ethernet2
  #else
    #include <Ethernet.h>
  #endif

  // Turn ON to allow webserver debug messages
  #define WEBSERVER_DEBUG OFF
  
  #if WEBSERVER_DEBUG == ON
    #define W(x) V(x)
    #define WF(x) VF(x)
    #define WL(x) VL(x)
    #define WLF(x) VLF(x)
  #else
    #define W(x)
    #define WF(x)
    #define WL(x)
    #define WLF(x)
  #endif

  // Enter a unique MAC address for your controller if you like:
  extern byte eth_mac[];

  // The IP addresses below will be dependent on your local network:
  extern IPAddress eth_ip;
  extern IPAddress eth_dns;
  extern IPAddress eth_gw;
  extern IPAddress eth_sn;

  // misc.
  #define WEB_SOCKET_TIMEOUT    10000
  #define HANDLER_COUNT_MAX     25
  #define PARAMETER_COUNT_MAX   20
  
  typedef void (* webFunction) (EthernetClient *client);
  
  class WebServer {
    public:
      void init();
      void handleClient();
      void setResponseHeader(const char *str);
      void on(String fn, webFunction handler);
      #if SD_CARD == ON
        void on(String fn);
      #endif
      void onNotFound(webFunction handler);
      String arg(String id);
  
      bool SDfound = false;

    private:
      int  getHandler(String* line);
      void processGet(String* line);
      void processPost(String* line);
 
      #if SD_CARD == ON
        void sdPage(String fn, EthernetClient* client);
      #endif
  
      char responseHeader[200] = "";
      #if SD_CARD == ON
        bool modifiedSinceFound=false;
      #endif
  
      webFunction notFoundHandler = NULL;
      webFunction handlers[HANDLER_COUNT_MAX];
      String handlers_fn[HANDLER_COUNT_MAX];
      int handler_count;
      
      String parameters[PARAMETER_COUNT_MAX];
      String values[PARAMETER_COUNT_MAX];
      int parameter_count;
  };

#endif
