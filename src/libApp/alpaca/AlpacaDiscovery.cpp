// -----------------------------------------------------------------------------------
// ASCOM Alpaca UDP discovery responder
#include "AlpacaDiscovery.h"

#if ALPACA_SERVER == ON

#if OPERATIONAL_MODE == WIFI
  #include <WiFiUdp.h>
  WiFiUDP alpacaUdp;
#elif OPERATIONAL_MODE == ETHERNET_TEENSY41
  #include <NativeEthernetUdp.h>
  EthernetUDP alpacaUdp;
#else
  #include <EthernetUdp.h>
  EthernetUDP alpacaUdp;
#endif

// the ASCOM Alpaca spec fixes this exact request string and JSON reply shape
#define ALPACA_DISCOVERY_REQUEST "alpacadiscovery1"

// the port src/lib/wifi/webServer + src/lib/ethernet/webServer listen HTTP on (SmartWebServer.ino: www.begin())
#define ALPACA_HTTP_PORT 80

void AlpacaDiscovery::init() {
  active = alpacaUdp.begin(ALPACA_DISCOVERY_PORT);
}

void AlpacaDiscovery::poll() {
  if (!active) return;

  int size = alpacaUdp.parsePacket();
  if (size <= 0) return;

  char buf[32];
  int len = alpacaUdp.read(buf, sizeof(buf) - 1);
  if (len < 0) len = 0;
  buf[len] = 0;

  if (len == (int)strlen(ALPACA_DISCOVERY_REQUEST) && strcmp(buf, ALPACA_DISCOVERY_REQUEST) == 0) {
    char reply[24];
    snprintf(reply, sizeof(reply), "{\"AlpacaPort\":%d}", ALPACA_HTTP_PORT);
    alpacaUdp.beginPacket(alpacaUdp.remoteIP(), alpacaUdp.remotePort());
    alpacaUdp.write((const uint8_t *)reply, strlen(reply));
    alpacaUdp.endPacket();
  }
}

AlpacaDiscovery alpacaDiscovery;

#endif
