// -----------------------------------------------------------------------------------
// Alpaca Management API
#include "AlpacaManagement.h"

#if ALPACA_SERVER == ON

#include "../status/Version.h"
#include "../status/Status.h"

void alpacaManagementApiVersions(AlpacaResponse &resp) {
  resp.setValue("[1]");
}

void alpacaManagementDescription(AlpacaResponse &resp) {
  String out = "{";
  out += "\"ServerName\":" + jsonString(HOST_NAME) + ",";
  out += "\"Manufacturer\":" + jsonString("Howard Dutton") + ",";
  out += "\"ManufacturerVersion\":" + jsonString(firmwareVersion.str) + ",";
  out += "\"Location\":" + jsonString("");
  out += "}";
  resp.setValue(out);
}

static void appendDevice(String &out, bool &first, const char *deviceName, const char *deviceType, int deviceNumber) {
  if (!first) out += ",";
  out += "{";
  out += "\"DeviceName\":" + jsonString(deviceName) + ",";
  out += "\"DeviceType\":" + jsonString(deviceType) + ",";
  out += "\"DeviceNumber\":" + jsonInt(deviceNumber) + ",";
  out += "\"UniqueID\":" + jsonString(String(HOST_NAME) + "-" + deviceType + "-" + String(deviceNumber));
  out += "}";
  first = false;
}

void alpacaManagementConfiguredDevices(AlpacaResponse &resp) {
  String out = "[";
  bool first = true;

  if (status.mountFound == SD_TRUE) {
    appendDevice(out, first, "OnStep", "Telescope", 0);
  }

  // advertised whenever OnStep itself is reachable, even if no physical weather sensor is
  // attached -- individual sensor properties report NotImplementedException in that case
  // (see AlpacaObservingConditions.cpp), which is standard/expected ASCOM behavior
  if (status.onStepFound) {
    appendDevice(out, first, "OnStep Weather", "ObservingConditions", 0);
  }

  // a rotator is fundamentally an axis 3 configured at compile time on the OnStepX side, and
  // its presence is decided once at boot by Status::rotatorScan() (Status.cpp), which probes
  // ":GX98#" -- reuse that same decision here rather than probing again
  if (status.rotatorFound == SD_TRUE) {
    appendDevice(out, first, "OnStep", "Rotator", 0);
  }

  out += "]";
  resp.setValue(out);
}

#endif
