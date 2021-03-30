// -----------------------------------------------------------------------------------
// Web server

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"
#include "../debug/Debug.h"
#include "../pinmaps/Models.h"

#include <Ethernet.h>

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
  IPAddress eth_ip = IPAddress IP_ADDRESS;
  IPAddress eth_dns= IPAddress GATEWAY_ADDRESS;
  IPAddress eth_gw = IPAddress GATEWAY_ADDRESS;
  IPAddress eth_sn = IPAddress SUBNET;

  EthernetServer _server(80);

  const char http_defaultHeader[] PROGMEM = "HTTP/1.1 200 OK\r\n" "Content-Type: text/html\r\n" "Connection: close\r\n" "\r\n";

  void WebServer::init() {
    // start the Ethernet connection and the server:
    setResponseHeader(http_defaultHeader);

    Ethernet.begin(eth_mac, eth_ip, eth_dns, eth_gw, eth_sn);
    _server.begin();

    WF("WEM: WWW Server started at = "); WL(Ethernet.localIP());

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
    // listen for incoming clients
    EthernetClient _client;
    _client = _server.available();
    if (_client) {
      WL("New client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      unsigned long to = millis() + WebSocketTimeOut;
      while (_client.connected() && (long)(millis() - to) < 0) {
      if (_client.available()) {
          char c = _client.read(); // W(c);
          if (inputBuffer.length() < 128) inputBuffer += c;
  
          // if you've gotten to the end of the line (received a newline character) and the line is blank, the http request has ended, send a reply
          if (c == '\n' && currentLineIsBlank) {
  
          // grab the URL
          int url_start = inputBuffer.indexOf("GET ");
          int url_end = inputBuffer.indexOf("HTTP/");
          if (url_start != -1 && url_end != -1) {
            String command = inputBuffer.substring(url_start,url_end);
            WL(command);
	    
            #if SD_CARD == ON
              // watch for cache requests
              if (!modifiedSinceFound && command.indexOf("If-Modified-Since:") >= 0) modifiedSinceFound = true;
            #endif
	    
            // pass to handlers
            bool handlerFound=false;
            for (int i = 0; i < handler_count; i++) {
              if (command.indexOf(handlers_fn[i]) >= 0) {
                // but first, isolate any get parameters and their values
                command = command.substring(command.indexOf(handlers_fn[i]) + handlers_fn[i].length());
                W(command.length()); W("  *"); W(command); WL("*");
	    
                // trim white space
                while (command.length() > 0 && command[0] == ' ') { if (command.length() > 1) command = command.substring(1); else command = ""; }
                while (command.length() > 0 && command[command.length() - 1] == ' ') command = command.substring(0, command.length() - 1);
	    
                W(command.length()); W("  *"); W(command); WL("*");
	    
                if (handlers[i] != NULL) {
                  // check to see if there's a ?a=1& or &a=1
                  parameter_count = 0;
                  while (command[0] == '?' || command[0] == '&') {
                    command = command.substring(1);
                    int j  = command.indexOf('='); if (j == -1) j = command.length(); if (j == -1) break; // invalid formatting
                    int j1 = command.indexOf('&'); if (j1 == -1) j1 = command.length() + 1;
                    String thisArg = command.substring(0, j);
                    String thisVal = command.substring(j+1, j1);
                    if (thisArg != "") {
                      parameter_count++;
                      parameters[parameter_count - 1] = thisArg;
                     values[parameter_count-1] = thisVal;
                    }
                    if (int(command.length()) > j1) command = command.substring(j1); else command = "";
                    W(thisArg); W(" = "); WL(thisVal);
                    W("Handler = "); WL(i);
                  }
                  _client.print(responseHeader);
                  (*handlers[i])(&_client); // send page content
                  handlerFound = true;
                  break;
                } else {
                  #if SD_CARD == ON
                    // send a 304 header
                    if (modifiedSinceFound && true ) {
                     char temp[255]; strcpy_P(temp, http_js304Header); _client.print(temp);
                     handlerFound = true;
                     break;
                    } else {
                      if (handlers_fn[i].indexOf(".js") > 0) {
                        char temp[255]; strcpy_P(temp, http_jsHeader); _client.print(temp); 
                      } else client.print(responseHeader);
                      sdPage(handlers_fn[i], &client);
                      handlerFound = true;
                      break;
                    }
                    #endif
                  }
                }
              }
              // handle not found
              if (!handlerFound && notFoundHandler != NULL) (*notFoundHandler)(&_client);
	    
            } else {
              WL("Invalid response");
            }
            inputBuffer = "";
            break;
          }
          if (c == '\n') {
            // starting a new line
            currentLineIsBlank = true;
          } else if (c != '\r') {
            // have a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
  
      // close the connection:
      _client.stop();
  
      // clear the input buffer
      inputBuffer = "";
  
      #if SD_CARD == ON
      modifiedSinceFound = false;
      #endif
  
      WL("Client disconnected");
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