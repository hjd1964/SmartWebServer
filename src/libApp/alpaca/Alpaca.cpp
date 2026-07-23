// -----------------------------------------------------------------------------------
// ASCOM Alpaca REST/JSON device server
#include "Alpaca.h"

#if ALPACA_SERVER == ON

#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

#include "AlpacaError.h"
#include "AlpacaManagement.h"
#include "AlpacaTelescope.h"
#include "AlpacaObservingConditions.h"

void handleNotFound();

#define ALPACA_MAX_SEGMENTS 6

bool alpacaArg(const char *name, String &out) {
  int n = www.args();
  for (int i = 0; i < n; i++) {
    if (www.argName(i).equalsIgnoreCase(name)) { out = www.arg(i); return true; }
  }
  return false;
}

bool alpacaArgBool(const char *name, bool defaultValue) {
  String v;
  if (!alpacaArg(name, v)) return defaultValue;
  return v.equalsIgnoreCase("true") || v.equals("1");
}

bool alpacaArgDouble(const char *name, double &out) {
  String v;
  if (!alpacaArg(name, v)) return false;
  out = atof(v.c_str());
  return true;
}

uint32_t alpacaArgUint32(const char *name, uint32_t defaultValue) {
  String v;
  if (!alpacaArg(name, v)) return defaultValue;
  long l = v.toInt();
  // don't coerce a negative (invalid) value to defaultValue -- when defaultValue is itself a
  // valid enum member (e.g. TrackingRate's 0 = sidereal), that silently accepted bad input
  // instead of rejecting it (confirmed via ASCOM Conform: "No error generated when TrackingRate
  // is set to an invalid value (-1)"). Casting to unsigned instead wraps negative values to a
  // huge number that every caller's existing range/equality check already rejects correctly.
  return (uint32_t)l;
}

// split a URI path into up to ALPACA_MAX_SEGMENTS lower-cased, non-empty '/'-separated parts
static int splitPath(const String &uri, String segments[]) {
  int count = 0;
  int start = 0;
  int len = uri.length();
  while (start <= len && count < ALPACA_MAX_SEGMENTS) {
    int slash = uri.indexOf('/', start);
    if (slash == -1) slash = len;
    if (slash > start) {
      segments[count] = uri.substring(start, slash);
      segments[count].toLowerCase();
      count++;
    }
    start = slash + 1;
  }
  return count;
}

void Alpaca::init() {
  // nothing to do yet: management/device tables are computed live from status.*
}

void Alpaca::handleManagementRequest(String segments[], int count, AlpacaResponse &resp) {
  // segments[0] == "management"
  if (count == 2 && segments[1] == "apiversions") { alpacaManagementApiVersions(resp); return; }
  if (count == 3 && segments[1] == "v1" && segments[2] == "description") { alpacaManagementDescription(resp); return; }
  if (count == 3 && segments[1] == "v1" && segments[2] == "configureddevices") { alpacaManagementConfiguredDevices(resp); return; }

  resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unrecognized management endpoint");
}

void Alpaca::handleApiRequest(String segments[], int count, AlpacaResponse &resp) {
  // segments: "api", "v1", {devicetype}, {devicenumber}, {method}
  if (count != 5 || segments[1] != "v1") {
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Malformed Alpaca device API request");
    return;
  }

  const String &deviceType = segments[2];
  int deviceNumber = segments[3].toInt();
  const String &method = segments[4];
  int httpMethod = www.method();

  if (deviceType == "telescope") {
    alpacaTelescope.handleRequest(deviceNumber, method, httpMethod, resp);
    return;
  }

  if (deviceType == "observingconditions") {
    alpacaObservingConditions.handleRequest(deviceNumber, method, httpMethod, resp);
    return;
  }

  resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unknown or unconfigured Alpaca device type");
}

void Alpaca::handleRequest() {
  String uri = www.uri();

  String segments[ALPACA_MAX_SEGMENTS];
  int count = splitPath(uri, segments);

  uint32_t clientTransactionId = alpacaArgUint32("ClientTransactionID", 0);

  AlpacaResponse resp;
  resp.begin(clientTransactionId);

  if (count == 0) {
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Empty Alpaca request");
  } else if (segments[0] == "management") {
    handleManagementRequest(segments, count, resp);
  } else if (segments[0] == "api") {
    handleApiRequest(segments, count, resp);
  } else {
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Not an Alpaca request");
  }

  // explicit reset (rather than relying on a prior www.send() call somewhere else having
  // already left this in the right state) since an Alpaca client -- unlike a browser -- may
  // well make the very first HTTP request this device sees after boot
  www.setContentLength(CONTENT_LENGTH_NOT_SET);
  www.send(200, "application/json", resp.toJson());
}

Alpaca alpaca;

void alpacaOrNotFound() {
  String uri = www.uri();
  if (uri.startsWith("/api/") || uri.startsWith("/management/") || uri.equals("/management")) {
    alpaca.handleRequest();
  } else {
    handleNotFound();
  }
}

#endif
