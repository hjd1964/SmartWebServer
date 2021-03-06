// -----------------------------------------------------------------------------------
// Web server

#include "WebServer.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500

  #define SD_CARD OFF

  // SD CARD support, simply enable and provide a webserver.on("filename.htm") to serve each file
  #if SD_CARD == ON
    #include <SD.h>
  #endif

  // Enter a unique MAC address for your controller if you like:
  byte eth_mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

  // The IP addresses below will be dependent on your local network:
  IPAddress eth_ip = IPAddress STA_IP_ADDR;
  IPAddress eth_dns= IPAddress STA_GW_ADDR;
  IPAddress eth_gw = IPAddress STA_GW_ADDR;
  IPAddress eth_sn = IPAddress STA_SN_MASK;

  EthernetServer _server(80);

  const char http_defaultHeader[] PROGMEM = "HTTP/1.1 200 OK\r\n" "Content-Type: text/html\r\n" "Connection: close\r\n" "\r\n";

  void WebServer::init() {
    // start the Ethernet connection and the server:
    setResponseHeader(http_defaultHeader);

    Ethernet.begin(eth_mac, eth_ip, eth_dns, eth_gw, eth_sn);
    _server.begin();

    WF("SWS: WWW Server started at = "); WL(Ethernet.localIP());

    #if SD_CARD == ON
      SDfound = SD.begin(SD_CARD_CS_PIN);
    #else
      // disable the SDCARD CS pin
      if (SD_CARD_CS_PIN != OFF) {
        pinMode(SD_CARD_CS_PIN, OUTPUT);
        digitalWrite(SD_CARD_CS_PIN, HIGH);
      }
    #endif

    handler_count = 0;
  }

  void WebServer::handleClient() {
    EthernetClient client;
    client = _server.available();
    if (client) {
      WL("SWS: Webserver New client");

      parameter_count = 0;
      String line = "";
      int currentSection = 1;
      int handler_number = -1;
      bool isGet = false;
      bool isPost = false;

      unsigned long to = millis() + WEB_SOCKET_TIMEOUT;
      while (client.connected() && (long)(millis() - to) < 0 && currentSection <= 2) {
        if (client.available()) {
          // read in a char
          char c = client.read(); if (c == '\r') continue;

          // build up ea. line
          if (line.length() <= 128) line += c;

          // loop until an entire line is present
          if (c != '\n' && client.available()) continue;

          // look for end of sections
          if (line.equals("\n")) { line = ""; currentSection++; continue; }

          // scan the header
          if (currentSection == 1) {
            #if SD_CARD == ON
              if (!modifiedSinceFound && line.indexOf("If-Modified-Since:") >= 0) modifiedSinceFound = true;
            #endif
            if (!isGet && !isPost) {
              int index = line.indexOf("GET ");
              if (index >= 0) {
                isGet = true;
                line = line.substring(index + 4);
                handler_number = getHandler(&line);
                if (handler_number >= 0) processGet(&line);
              } else {
                index = line.indexOf("POST ");
                if (index >= 0) {
                  isPost = true;
                  line = line.substring(index + 5);
                  handler_number = getHandler(&line);
                }
              }
            }
          } else

          // scan the request
          if (currentSection == 2 && isPost) {
            if (handler_number >= 0) processPost(&line);
          }

          line = "";
        } else break;
      }

      // process get or post request
      bool handlerFound = false;
      if (handler_number >= 0) {
        if (handlers[handler_number] != NULL) {
          WF("SWS: Webserver Running handler# "); WL(handler_number);
          client.print(responseHeader);
          (*handlers[handler_number])(&client);
          handlerFound = true;
        } else {
          #if SD_CARD == ON
            if (modifiedSinceFound) {
              WLF("SWS: Webserver Sending js304Header");
              char temp[255]; strcpy_P(temp, http_js304Header); _client.print(temp);
              handlerFound = true;
            } else {
              if (handlers_fn[handler].indexOf(".js") > 0) {
                WLF("SWS: Webserver Sending jsHeader");
                char temp[255]; strcpy_P(temp, http_jsHeader); _client.print(temp); 
              } else client.print(responseHeader);
              WLF("SWS: Webserver Sending SD file");
              sdPage(handlers_fn[handler], &client);
              handlerFound = true;
            }
          #endif
        }
      }
      
      // handle 404 page not found
      if (!handlerFound && notFoundHandler != NULL) (*notFoundHandler)(&client);

      // give the web browser time to receive the data
      delay(1);
  
      // close the connection:
      client.stop();
  
      #if SD_CARD == ON
        modifiedSinceFound = false;
      #endif
  
      WL("SWS: Webserver Client disconnected");
    }
  }

  int WebServer::getHandler(String* line) {
    int url_end = line->indexOf("HTTP/");
    if (url_end <= 0) return -1;

    WLF("SWS: Webserver processing header GET/POST handler");
    WF("SWS: Webserver ["); W(line->substring(0, 8)); WL("...]");

    // isolate the content
    *line = line->substring(0, url_end);

    for (int i = 0; i < handler_count; i++) {
      int j = line->indexOf(handlers_fn[i]);
      if (j >= 0) {
        // success, isolate any parameters and return
        WF("SWS: Webserver found handler "); W(i); W(" ["); W(handlers_fn[i]); WL("]");
        *line = line->substring(j + handlers_fn[i].length());
        line->trim();
        return i;
      }
    }
    return -1;
  }

  void WebServer::processGet(String* line) {
    WLF("SWS: Webserver processing header GET parameters");
    WF("SWS: Webserver ["); W(line->substring(0, 8)); WL("...]");

    // isolate any parameters, get their values
    // look for form "?a=1&" or "&a=1"
    while ((*line)[0] == '?' || (*line)[0] == '&') {
      *line = line->substring(1);
      int j  = line->indexOf('='); if (j == -1) j = line->length(); if (j == -1) break; // invalid formatting
      int j1 = line->indexOf('&'); if (j1 == -1) j1 = line->length() + 1;
      String thisArg = line->substring(0, j);
      String thisVal = line->substring(j + 1, j1);
      if (thisArg != "") {
        if (++parameter_count > PARAMETER_COUNT_MAX) parameter_count = PARAMETER_COUNT_MAX;
        parameters[parameter_count - 1] = thisArg;
        values[parameter_count - 1] = thisVal;
      }
      if ((int)line->length() > j1) *line = line->substring(j1); else *line = "";

      WF("SWS: Webserver param. "); W(thisArg); W(" = "); WL(thisVal);
    }
  }

  void WebServer::processPost(String* line) {
    WLF("SWS: Webserver processing header POST parameter");
    WF("SWS: Webserver ["); W(line->substring(0, 8)); WL("...]");

    // make all tokens start with '&'
    *line = "&" + *line;

    // isolate any parameters, get their values
    // look for form "&a=1"
    while ((*line)[0] == '&') {
      *line = line->substring(1);
      int j  = line->indexOf('='); if (j == -1) j = line->length(); if (j == -1) break; // invalid formatting
      int j1 = line->indexOf('&'); if (j1 == -1) j1 = line->length() + 1;
      String thisArg = line->substring(0, j);
      String thisVal = line->substring(j + 1, j1);
      if (thisArg != "") {
        if (++parameter_count > PARAMETER_COUNT_MAX) parameter_count = PARAMETER_COUNT_MAX;
        parameters[parameter_count - 1] = thisArg;
        values[parameter_count - 1] = thisVal;
      }
      if ((int)line->length() > j1) *line = line->substring(j1); else *line = "";

      WF("SWS: Webserver param. "); W(thisArg); W(" = "); WL(thisVal);
    }
  }

  void WebServer::setResponseHeader(const char* str) {
    if (!str) return;
    strcpy_P(responseHeader,str);
  }
  
  void WebServer::on(String fn, webFunction handler) {
    handler_count++; if (handler_count > HANDLER_COUNT_MAX) { handler_count = HANDLER_COUNT_MAX; return; }
    handlers[handler_count - 1] = handler;
    handlers_fn[handler_count - 1] = fn;
  }
  
  void WebServer::onNotFound(webFunction handler) {
    notFoundHandler = handler;
  }
  
  String WebServer::arg(String id) {
    for (int i = 0; i < parameter_count; i++) {
      if (id == parameters[i]) return values[i];
    }
    return EmptyStr;
  }
  
  #if SD_CARD == ON
    void WebServer::on(String fn) {
      handler_count++; if (handler_count > HANDLER_COUNT_MAX) { handler_count = HANDLER_COUNT_MAX; return; }
      handlers[handler_count - 1] = NULL;
      handlers_fn[handler_count - 1] = fn;
    }
    
    void WebServer::sdPage(String fn, EthernetClient *client) {
      char temp[256] = "";
      int n;
    
      // open the sdcard file
      if (SDfound) {
        File dataFile = SD.open(fn, FILE_READ);
        if (dataFile) {
          do {
            n=dataFile.available(); if (n > 256) n = 256;
            if (n > 0) { dataFile.read(temp, n); client->write(temp, n); }
          } while (n > 0);
          dataFile.close();
        }
      }
    }
  #endif

#endif