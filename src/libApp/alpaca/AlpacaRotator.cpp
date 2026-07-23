// -----------------------------------------------------------------------------------
// ASCOM Alpaca IRotatorV4 implementation, gateway to OnStep via onStep.command()
//
// Command mappings confirmed against OnStepX src/telescope/rotator/local/Rotator.command.cpp
// (fetched directly, not just the docs table -- see AlpacaTelescope.cpp's header comment for
// why that mattered elsewhere in this session).
#include "AlpacaRotator.h"

#if ALPACA_SERVER == ON

#include "Alpaca.h"
#include "AlpacaCommon.h"
#include "AlpacaError.h"
#include "../cmd/Cmd.h"
#include "../status/Status.h"
#include "../../lib/convert/Convert.h"
#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

// ":rG#" replies "sDDD*MM#" (signed, 3-digit degrees, no seconds -- PM_LOW). dmsToDouble's
// signPresent=true mode only reads 2 degree digits (matches AlpacaTelescope.cpp's
// getSiteLatLon() longitude fix -- the same OnStep decode gap), so the sign has to be stripped
// and reapplied manually around an unsigned parse. Result normalized to ASCOM's required 0-360
// range (OnStepX can report a small negative angle after a CCW move through zero).
static bool getPositionDeg(double &out) {
  char temp[80];
  if (!onStep.command(":rG#", temp)) return false;
  bool negative = (temp[0] == '-');
  char *numPart = (temp[0] == '+' || temp[0] == '-') ? &temp[1] : temp;
  if (!convert.dmsToDouble(&out, numPart, false)) return false;
  if (negative) out = -out;
  if (out < 0) out += 360;
  if (out >= 360) out -= 360;
  return true;
}

static bool getIsMoving(bool &out) {
  char temp[80];
  if (!onStep.command(":rT#", temp)) return false;
  out = strchr(temp, 'M') != NULL;
  return true;
}

// OnStepX has no "get target" rotator command (only the internal axis3.getTargetCoordinate()
// used by ":rr#"), so -- matching how AlpacaTelescope.cpp tracks TargetRightAscension/
// TargetDeclination -- TargetPosition is tracked here in software, throwing ValueNotSetException
// if Move()/MoveAbsolute()/MoveMechanical() was never called.
static bool targetPositionSet = false;
static double targetPositionValue = 0;

static String rotatorDeviceStateJson(int deviceNumber) {
  UNUSED(deviceNumber); // single-instance device
  String out;
  bool first = true;
  double pos;
  bool moving;
  if (getPositionDeg(pos)) jsonStateValueEntry(out, first, "Position", jsonNumber(pos));
  if (getIsMoving(moving)) jsonStateValueEntry(out, first, "IsMoving", jsonBool(moving));
  jsonStateValueEntry(out, first, "TimeStamp", jsonString(""));
  return out;
}

void AlpacaRotator::handleRequest(int deviceNumber, const String &method, int httpMethod, AlpacaResponse &resp) {
  if (deviceNumber != 0) {
    resp.setError(ALPACA_ERR_INVALID_VALUE, "Invalid Rotator device number");
    return;
  }

  // rotator presence is decided once at boot by Status::rotatorScan() (Status.cpp), which probes
  // ":GX98#" and sets status.rotatorFound -- gate live "connected" state on it the same way
  // AlpacaManagement.cpp gates whether the device is advertised at all
  bool connected = status.onStepFound && status.rotatorFound == SD_TRUE;

  if (alpacaHandleCommonMember(method, httpMethod, resp, "OnStep", "OnStep rotator, via SmartWebServer",
                                4, connected, deviceNumber, rotatorDeviceStateJson)) {
    return;
  }

  if (!connected) {
    resp.setError(ALPACA_ERR_NOT_CONNECTED, "OnStep or rotator not found");
    return;
  }

  if (method == "canreverse") { resp.setValue(jsonBool(false)); return; }

  if (method == "reverse") {
    if (httpMethod == HTTP_GET) resp.setValue(jsonBool(false));
    // OnStepX's ":rR#" only reverses the optional alt-az derotator sub-feature, not the rotator's
    // own sense of direction -- there's no general "flip which way Position increases" primitive
    else resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Not adjustable");
    return;
  }

  if (method == "ismoving") {
    bool moving;
    if (getIsMoving(moving)) resp.setValue(jsonBool(moving)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "position" || method == "mechanicalposition") {
    // OnStepX's rotator doesn't expose a separate raw/mechanical vs. sky-adjusted position via
    // LX200 (no sync-to-arbitrary-value command exists either, see "sync" below), so both ASCOM
    // properties map to the same ":rG#" reading
    double pos;
    if (getPositionDeg(pos)) resp.setValue(jsonNumber(pos)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "stepsize") {
    char temp[80];
    if (!onStep.command(":rD#", temp)) { resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure"); return; }
    resp.setValue(jsonNumber(atof(temp)));
    return;
  }

  if (method == "targetposition") {
    if (!targetPositionSet) { resp.setError(ALPACA_ERR_VALUE_NOT_SET, "Target has not been set"); return; }
    resp.setValue(jsonNumber(targetPositionValue));
    return;
  }

  if (method == "halt") { onStep.commandBlind(":rQ#"); return; }

  if (method == "move") {
    double relDeg;
    if (!alpacaArgDouble("Position", relDeg)) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Missing Position"); return; }
    // OnStepX's ":rr#" parses its parameter with dmsToDouble(signPresent=true), which -- like
    // ":rG#" above -- only reads 2 degree digits, capping the relative offset at +/-99 degrees
    if (relDeg <= -100 || relDeg >= 100) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Relative move must be within +/-99 degrees"); return; }
    double base;
    if (targetPositionSet) base = targetPositionValue; else if (!getPositionDeg(base)) { resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure"); return; }
    char buf[16], cmd[24];
    convert.doubleToDms(buf, relDeg, false, true, PM_LOW);
    snprintf(cmd, sizeof(cmd), ":rr%s#", buf);
    onStep.commandBlind(cmd);
    targetPositionValue = base + relDeg;
    targetPositionSet = true;
    return;
  }

  if (method == "moveabsolute" || method == "movemechanical") {
    // no separate mechanical-position command exists (see "position" above), so MoveMechanical
    // is implemented identically to MoveAbsolute
    double pos;
    if (!alpacaArgDouble("Position", pos) || pos < 0 || pos >= 360) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Position must be 0 <= Position < 360"); return; }
    char buf[16], cmd[24];
    convert.doubleToDms(buf, pos, true, true, PM_LOW);
    snprintf(cmd, sizeof(cmd), ":rS%s#", buf);
    if (!onStep.commandBool(cmd)) { resp.setError(ALPACA_ERR_INVALID_OPERATION, "OnStep rejected the move"); return; }
    targetPositionValue = pos;
    targetPositionSet = true;
    return;
  }

  if (method == "sync") {
    // no general "set current position to this arbitrary value" primitive exists in OnStepX's
    // rotator LX200 command set (only ":rZ#" sync-to-zero and ":rF#" sync-to-half-travel do)
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Sync to an arbitrary position isn't supported by OnStepX");
    return;
  }

  resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unknown Rotator member: " + method);
}

AlpacaRotator alpacaRotator;

#endif
