// -----------------------------------------------------------------------------------
// ASCOM Alpaca REST/JSON device server
//
// Dispatched from the existing www.onNotFound() handler (see SmartWebServer.ino) since
// Alpaca's /api/v1/{devicetype}/{devicenumber}/{method} URLs are dynamic and the shared
// WebServer wrapper (src/lib/wifi/webServer, src/lib/ethernet/webServer) only supports a
// small fixed table of exact-match routes.
#pragma once

#include "../../Common.h"

#if ALPACA_SERVER == ON

#include "AlpacaJson.h"

// case-insensitive request parameter lookup (Alpaca requires parameter name matching to
// be case-insensitive; real client libraries are inconsistent about e.g. ClientID vs
// clientid) -- built only on the cross-platform www.args()/argName()/arg() accessors.
bool alpacaArg(const char *name, String &out);
bool alpacaArgBool(const char *name, bool defaultValue = false);
bool alpacaArgDouble(const char *name, double &out);
uint32_t alpacaArgUint32(const char *name, uint32_t defaultValue = 0);

class Alpaca {
  public:
    void init();

    // entry point for any request under /api/ or /management/
    void handleRequest();

  private:
    void handleManagementRequest(String segments[], int count, AlpacaResponse &resp);
    void handleApiRequest(String segments[], int count, AlpacaResponse &resp);
};

extern Alpaca alpaca;

// registered as the www onNotFound handler when ALPACA_SERVER == ON; routes /api/ and
// /management/ requests to Alpaca, everything else falls through to the normal 404 page
void alpacaOrNotFound();

#endif
