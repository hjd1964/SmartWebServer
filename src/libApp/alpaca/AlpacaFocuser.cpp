// -----------------------------------------------------------------------------------
// ASCOM Alpaca IFocuserV4 implementation, gateway to OnStep via onStep.command()
//
// Command mappings confirmed against OnStepX src/telescope/focuser/local/Focuser.command.cpp
// (fetched directly, not just the docs table -- see AlpacaTelescope.cpp's header comment for
// why that mattered elsewhere in this session).
//
// OnStep supports up to 6 focusers (OnStepX axes 4-9); Alpaca device number N maps directly to
// OnStep focuser slot N+1 (device 0 = focuser 1/axis4, ... device 5 = focuser 6/axis9), matching
// how status.focuserPresent[6] (Status.cpp's Status::focuserScan(), probed once at boot) already
// tracks presence per slot -- device numbers for absent slots simply aren't advertised, rather
// than compacting the numbering.
//
// Every command below directly addresses its focuser (":F<n><cmd>...#", n = 1-6) rather than
// using the stateful ":FA<n>#" select-then-act pattern the existing web UI uses (see
// src/pages/focuser/SelectTile.cpp) -- OnStepX's own command parser supports this (it rewrites
// ":F<n><cmd>#" into an internally-selected command before dispatch), and it avoids a
// select/act race between concurrent Alpaca requests for different focusers entirely.
#include "AlpacaFocuser.h"

#if ALPACA_SERVER == ON

#include "Alpaca.h"
#include "AlpacaCommon.h"
#include "AlpacaError.h"
#include "../cmd/Cmd.h"
#include "../status/Status.h"
#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

// all commands use the UPPERCASE sub-command letter (micron units, matching the existing web
// UI) except temperature/TCF-enable, which OnStepX only exposes in one (lowercase) form
static bool cmdGetLong(int deviceNumber, char subCmd, long &out) {
  char cmd[8], temp[80];
  snprintf(cmd, sizeof(cmd), ":F%d%c#", deviceNumber + 1, subCmd);
  if (!onStep.command(cmd, temp)) return false;
  out = atol(temp);
  return true;
}

static bool cmdSetLong(int deviceNumber, char subCmd, long value) {
  char cmd[24];
  snprintf(cmd, sizeof(cmd), ":F%d%c%ld#", deviceNumber + 1, subCmd, value);
  return onStep.commandBool(cmd);
}

static bool getIsMoving(int deviceNumber, bool &out) {
  char cmd[8], temp[80];
  snprintf(cmd, sizeof(cmd), ":F%dT#", deviceNumber + 1);
  if (!onStep.command(cmd, temp)) return false;
  out = temp[0] == 'M';
  return true;
}

static bool getTemperature(int deviceNumber, double &out) {
  char cmd[8], temp[80];
  snprintf(cmd, sizeof(cmd), ":F%dt#", deviceNumber + 1);
  if (!onStep.command(cmd, temp)) return false;
  out = atof(temp);
  return true;
}

static String focuserDeviceStateJson(int deviceNumber) {
  String out;
  bool first = true;
  long pos;
  bool moving;
  double temperature;
  if (cmdGetLong(deviceNumber, 'G', pos)) jsonStateValueEntry(out, first, "Position", jsonInt(pos));
  if (getIsMoving(deviceNumber, moving)) jsonStateValueEntry(out, first, "IsMoving", jsonBool(moving));
  if (getTemperature(deviceNumber, temperature)) jsonStateValueEntry(out, first, "Temperature", jsonNumber(temperature));
  jsonStateValueEntry(out, first, "TimeStamp", jsonString(""));
  return out;
}

void AlpacaFocuser::handleRequest(int deviceNumber, const String &method, int httpMethod, AlpacaResponse &resp) {
  // presence is decided once at boot by Status::focuserScan() (Status.cpp), which probes
  // ":F1a#".."F6a#" -- reuse that decision (and AlpacaManagement.cpp only ever advertises
  // device numbers where it's true) rather than probing again per-request
  bool present = deviceNumber >= 0 && deviceNumber < 6 && status.focuserPresent[deviceNumber];
  bool connected = status.onStepFound && present;

  if (alpacaHandleCommonMember(method, httpMethod, resp, "OnStep", "OnStep focuser, via SmartWebServer",
                                4, connected, deviceNumber, focuserDeviceStateJson)) {
    return;
  }

  if (!connected) {
    resp.setError(ALPACA_ERR_NOT_CONNECTED, present ? "OnStep not found" : "No focuser at this device number");
    return;
  }

  // both stepper (true absolute position/limit switches) and DC-motor (":Fp#" reports "pseudo
  // absolute", no limit switches) focusers still track and report a position and accept
  // absolute-position moves the same way through ":FS#", so both are reported as Absolute here
  if (method == "absolute") { resp.setValue(jsonBool(true)); return; }

  if (method == "ismoving") {
    bool moving;
    if (getIsMoving(deviceNumber, moving)) resp.setValue(jsonBool(moving)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "maxstep" || method == "maxincrement") {
    // no separate "maximum single move" limit is exposed, so MaxIncrement (largest move allowed
    // in one Move() call) is reported the same as MaxStep (the full travel range), allowing
    // full-range moves in a single command -- a common, reasonable default for this property
    long maxPos;
    if (cmdGetLong(deviceNumber, 'M', maxPos)) resp.setValue(jsonInt(maxPos)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "position") {
    long pos;
    if (cmdGetLong(deviceNumber, 'G', pos)) resp.setValue(jsonInt(pos)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "stepsize") {
    // Position above is reported in the same microns ":FG#" itself returns (matching the
    // existing web UI, src/pages/focuser/*.cpp), not raw motor steps -- so StepSize must be
    // 1.0 here to stay internally consistent (Position * StepSize must equal real microns).
    // Reporting OnStepX's actual microns-per-step ratio (":Fu#") instead would silently corrupt
    // any client math (e.g. autofocus routines) that combines the two.
    resp.setValue(jsonNumber(1.0));
    return;
  }

  if (method == "temperature") {
    double t;
    if (getTemperature(deviceNumber, t)) resp.setValue(jsonNumber(t)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "tempcompavailable") { resp.setValue(jsonBool(true)); return; }

  if (method == "tempcomp") {
    if (httpMethod == HTTP_GET) {
      char cmd[8];
      snprintf(cmd, sizeof(cmd), ":F%dc#", deviceNumber + 1);
      resp.setValue(jsonBool(onStep.commandBool(cmd)));
    } else {
      bool on = alpacaArgBool("TempComp", false);
      char cmd[8];
      snprintf(cmd, sizeof(cmd), ":F%dc%d#", deviceNumber + 1, on ? 1 : 0);
      if (!onStep.commandBool(cmd)) resp.setError(ALPACA_ERR_INVALID_OPERATION, "OnStep rejected the change");
    }
    return;
  }

  if (method == "halt") {
    char cmd[8];
    snprintf(cmd, sizeof(cmd), ":F%dQ#", deviceNumber + 1);
    onStep.commandBlind(cmd);
    return;
  }

  if (method == "move") {
    // Absolute is always true above, so per the ASCOM spec Move(Position) always targets an
    // absolute position (never a relative offset)
    double posD;
    if (!alpacaArgDouble("Position", posD)) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Missing Position"); return; }
    long pos = lround(posD);
    long minPos, maxPos;
    if (!cmdGetLong(deviceNumber, 'I', minPos) || !cmdGetLong(deviceNumber, 'M', maxPos)) {
      resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
      return;
    }
    if (pos < minPos || pos > maxPos) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Position outside the focuser's travel range"); return; }
    if (!cmdSetLong(deviceNumber, 'S', pos)) resp.setError(ALPACA_ERR_INVALID_OPERATION, "OnStep rejected the move");
    return;
  }

  resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unknown Focuser member: " + method);
}

AlpacaFocuser alpacaFocuser;

#endif
