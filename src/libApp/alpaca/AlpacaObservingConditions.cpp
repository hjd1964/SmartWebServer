// -----------------------------------------------------------------------------------
// ASCOM Alpaca IObservingConditionsV2 implementation, gateway to OnStep via onStep.command()
#include "AlpacaObservingConditions.h"

#if ALPACA_SERVER == ON

#include "Alpaca.h"
#include "AlpacaCommon.h"
#include "AlpacaError.h"
#include "../cmd/Cmd.h"
#include "../status/Status.h"
#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

struct ObsCondSensor {
  const char *key;         // lower-case: matches both the URL property name and the SensorName argument
  const char *displayName; // PascalCase: used for the DeviceState JSON "Name" field
  const char *cmd;         // OnStep command
  const char *description; // for SensorDescription()
};

// confirmed against OnStepX docs/COMMAND_REFERENCE.md + src/telescope/Telescope.command.cpp:
// ":GX9A#" temperature C, ":GX9B#" pressure mbar, ":GX9C#" relative humidity %, ":GX9E#" dew
// point C -- all reply "+/-n.n#" (src/libApp/weather/Weather.h). Every other ASCOM-recognized
// weather sensor (cloud cover, rain rate, sky quality/brightness/temperature, wind, star FWHM)
// has no OnStepX data source and reports NotImplementedException, the standard/expected ASCOM
// behavior for a partial ObservingConditions implementation.
static const ObsCondSensor sensors[] = {
  {"temperature", "Temperature", ":GX9A#", "OnStep ambient temperature sensor (BME280/BMP280)"},
  {"pressure",    "Pressure",    ":GX9B#", "OnStep barometric pressure sensor (BME280/BMP280)"},
  {"humidity",    "Humidity",    ":GX9C#", "OnStep relative humidity sensor (BME280)"},
  {"dewpoint",    "DewPoint",    ":GX9E#", "OnStep calculated dew point (from temperature and humidity)"},
};
static const int sensorCount = sizeof(sensors) / sizeof(sensors[0]);

// listed explicitly so they get a proper NotImplementedException instead of falling through
// to the generic "unknown member" error
static const char *unsupportedSensors[] = {
  "cloudcover", "rainrate", "skybrightness", "skyquality", "skytemperature",
  "starfwhm", "winddirection", "windgust", "windspeed"
};
static const int unsupportedSensorCount = sizeof(unsupportedSensors) / sizeof(unsupportedSensors[0]);

// returns false only on a comms failure; a successful read of "nan" -- no physical sensor
// present, see OnStepX's Weather class defaulting every reading to NAN when WS_NONE -- is
// reported back to the caller via isnan(out), not as a failure here
static bool cmdGetSensor(const char *cmd, double &out) {
  char temp[80];
  if (!onStep.command(cmd, temp)) return false;
  out = atof(temp);
  return true;
}

static int findSensor(const String &key) {
  for (int i = 0; i < sensorCount; i++) if (key == sensors[i].key) return i;
  return -1;
}

// per the ASCOM spec, a sensor's Value, SensorDescription, and TimeSinceLastUpdate must be
// consistently all-implemented or all-not-implemented (confirmed via ASCOM Conform: on this rig,
// with no physical BME280/BMP280 attached, Value already correctly threw NotImplementedException
// via the isnan() check below, but SensorDescription/TimeSinceLastUpdate only checked the static
// name table and succeeded anyway -- this does the same live check those two need too)
static bool sensorAvailable(int index) {
  double v;
  return cmdGetSensor(sensors[index].cmd, v) && !isnan(v);
}

static String obsCondDeviceStateJson(int deviceNumber) {
  UNUSED(deviceNumber); // single-instance device
  String out;
  bool first = true;
  for (int i = 0; i < sensorCount; i++) {
    double v;
    if (cmdGetSensor(sensors[i].cmd, v) && !isnan(v)) {
      jsonStateValueEntry(out, first, sensors[i].displayName, jsonNumber(v));
    }
  }
  jsonStateValueEntry(out, first, "TimeStamp", jsonString(""));
  return out;
}

void AlpacaObservingConditions::handleRequest(int deviceNumber, const String &method, int httpMethod, AlpacaResponse &resp) {
  if (deviceNumber != 0) {
    resp.setError(ALPACA_ERR_INVALID_VALUE, "Invalid ObservingConditions device number");
    return;
  }

  bool connected = status.onStepFound;

  if (alpacaHandleCommonMember(method, httpMethod, resp, "OnStep", "OnStep ambient weather sensors, via SmartWebServer",
                                2, connected, deviceNumber, obsCondDeviceStateJson)) {
    return;
  }

  if (!connected) {
    resp.setError(ALPACA_ERR_NOT_CONNECTED, "OnStep not found");
    return;
  }

  int sensorIndex = findSensor(method);
  if (sensorIndex >= 0) {
    double v;
    if (!cmdGetSensor(sensors[sensorIndex].cmd, v)) { resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure"); return; }
    if (isnan(v)) { resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "No sensor present for this property"); return; }
    resp.setValue(jsonNumber(v));
    return;
  }

  for (int i = 0; i < unsupportedSensorCount; i++) {
    if (method == unsupportedSensors[i]) {
      resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Not available from OnStepX");
      return;
    }
  }

  if (method == "averageperiod") {
    if (httpMethod == HTTP_GET) {
      resp.setValue(jsonNumber(0));
    } else {
      double v;
      if (!alpacaArgDouble("AveragePeriod", v)) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Missing value"); return; }
      if (v != 0) resp.setError(ALPACA_ERR_INVALID_VALUE, "Only AveragePeriod=0 (current value) is supported");
    }
    return;
  }

  if (method == "refresh") return; // no-op: every property above already reads OnStep live

  if (method == "sensordescription") {
    String sensorName;
    if (!alpacaArg("SensorName", sensorName)) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Missing SensorName"); return; }
    sensorName.toLowerCase();
    int i = findSensor(sensorName);
    if (i < 0 || !sensorAvailable(i)) { resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unknown or unsupported sensor"); return; }
    resp.setValue(jsonString(sensors[i].description));
    return;
  }

  if (method == "timesincelastupdate") {
    String sensorName;
    alpacaArg("SensorName", sensorName); // optional -- "" means "most recently updated sensor"
    sensorName.toLowerCase();
    if (sensorName.length() > 0) {
      int i = findSensor(sensorName);
      if (i < 0 || !sensorAvailable(i)) { resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unknown or unsupported sensor"); return; }
    }
    resp.setValue(jsonNumber(0)); // always freshly read from OnStep, per request
    return;
  }

  resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unknown ObservingConditions member: " + method);
}

AlpacaObservingConditions alpacaObservingConditions;

#endif
