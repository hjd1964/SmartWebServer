// -----------------------------------------------------------------------------------
// Shared ASCOM "common device" members
#include "AlpacaCommon.h"

#if ALPACA_SERVER == ON

#include "Alpaca.h"
#include "AlpacaError.h"
#include "../status/Version.h"
#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

bool alpacaHandleCommonMember(const String &member, int httpMethod, AlpacaResponse &resp,
                               const char *name, const char *description, int interfaceVersion,
                               bool connected, int deviceNumber, String (*deviceStateJson)(int)) {

  if (member == "connected") {
    if (httpMethod == HTTP_GET) {
      resp.setValue(jsonBool(connected));
    } else {
      // accepted but a no-op: this device is a shared always-on gateway to a single OnStep
      // controller, one Alpaca client disconnecting must not affect the web UI, LX200
      // command ports, or any other concurrent Alpaca client
      String v;
      if (!alpacaArg("Connected", v)) resp.setError(ALPACA_ERR_INVALID_VALUE, "Connected value missing");
    }
    return true;
  }

  if (member == "connecting") {
    resp.setValue(jsonBool(false));
    return true;
  }

  if (member == "connect" || member == "disconnect") {
    // async connect/disconnect (ITelescopeV4 etc.) -- resolves immediately, see "connected" above
    return true;
  }

  if (member == "name") { resp.setValue(jsonString(name)); return true; }
  if (member == "description") { resp.setValue(jsonString(description)); return true; }
  if (member == "driverinfo") { resp.setValue(jsonString("SmartWebServer ASCOM Alpaca gateway for OnStep/OnStepX")); return true; }
  if (member == "driverversion") { resp.setValue(jsonString(firmwareVersion.str)); return true; }
  if (member == "interfaceversion") { resp.setValue(jsonInt(interfaceVersion)); return true; }
  if (member == "supportedactions") { resp.setValue("[]"); return true; }

  if (member == "action" || member == "commandblind" || member == "commandbool" || member == "commandstring") {
    resp.setError(ALPACA_ERR_ACTION_NOT_IMPLEMENTED, "Not implemented");
    return true;
  }

  if (member == "devicestate") {
    String out = "[";
    out += deviceStateJson(deviceNumber);
    out += "]";
    resp.setValue(out);
    return true;
  }

  return false;
}

#endif
