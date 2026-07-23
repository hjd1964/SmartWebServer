// -----------------------------------------------------------------------------------
// ASCOM Alpaca UDP discovery responder (port ALPACA_DISCOVERY_PORT, default 32227)
//
// A single file with a WiFiUDP/EthernetUDP conditional include, following the exact
// pattern already used for NTP (src/lib/tls/ntp/NTP.h) -- unlike WebServer/CmdServer
// (which have substantially different WiFi vs. Ethernet implementations warranting
// separate files per src/lib/wifi/* + src/lib/ethernet/*), the discovery responder's
// logic is identical either way; only the UDP class differs, and WiFiUDP/EthernetUDP
// share the same API.
#pragma once

#include "../../Common.h"

#if ALPACA_SERVER == ON

class AlpacaDiscovery {
  public:
    void init();
    void poll();

  private:
    bool active = false;
};

extern AlpacaDiscovery alpacaDiscovery;

#endif
