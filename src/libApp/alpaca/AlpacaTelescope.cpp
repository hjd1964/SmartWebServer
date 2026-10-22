// -----------------------------------------------------------------------------------
// ASCOM Alpaca ITelescopeV4 implementation, gateway to OnStep via onStep.command()
//
// Command mappings not already exercised by this repo's existing src/pages/mount/*.cpp were
// cross-checked against the OnStepX firmware source (github.com/hjd1964/OnStepX) -- see the
// "confirmed against OnStepX" comments below. That caught three real bugs versus my initial
// LX200-convention guesses: SiteLongitude sign was inverted (OnStepX's ":Gg#"/":Sg#" are
// "east is negative" i.e. +West, the opposite of ASCOM's +East), PulseGuide's ":Mg" direction
// letter must be lowercase (uppercase silently falls through to CE_CMD_UNKNOWN), and ":MS#"'s
// 0-9 goto-result digit is OnStepX's own numbering, not this codebase's CommandErrors ordinal.
// Still not verified against real hardware: none of this has actually been run against a live
// OnStep controller or an Alpaca client/Conform tool.
#include "AlpacaTelescope.h"

#if ALPACA_SERVER == ON

#include "Alpaca.h"
#include "AlpacaCommon.h"
#include "AlpacaError.h"
#include "../cmd/Cmd.h"
#include "../status/Status.h"
#include "../../lib/convert/Convert.h"
#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

// x-sidereal guide rate table, index matches status.guideRate / GuideRatesStr (Status.h)
// and the :R0#..:R9# OnStep commands (src/pages/mount/GuideTile.cpp)
static const double guideRateXSidereal[10] = {0.25, 0.5, 1, 2, 4, 8, 20, 48, 24, 48}; // 8,9 ("1/2 Max","Max") approximated
static const double siderealDegPerSec = 360.0 / 86164.0905; // 0.0041780746.../s

static bool cmdGetHours(const char *cmd, double &out) {
  char temp[80];
  if (!onStep.command(cmd, temp)) return false;
  return convert.hmsToDouble(&out, temp);
}

static bool cmdGetDegrees(const char *cmd, bool signPresent, double &out) {
  char temp[80];
  if (!onStep.command(cmd, temp)) return false;
  return convert.dmsToDouble(&out, temp, signPresent);
}

static bool getRaDec(double &ra, double &dec) {
  #if DISPLAY_HIGH_PRECISION_COORDS == ON
    return cmdGetHours(":GRa#", ra) && cmdGetDegrees(":GDe#", true, dec);
  #else
    return cmdGetHours(":GR#", ra) && cmdGetDegrees(":GD#", true, dec);
  #endif
}

static bool getTargetRaDec(double &ra, double &dec) {
  #if DISPLAY_HIGH_PRECISION_COORDS == ON
    return cmdGetHours(":Gra#", ra) && cmdGetDegrees(":Gde#", true, dec);
  #else
    return cmdGetHours(":Gr#", ra) && cmdGetDegrees(":Gd#", true, dec);
  #endif
}

static bool getAzAlt(double &az, double &alt) {
  #if DISPLAY_HIGH_PRECISION_COORDS == ON
    return cmdGetDegrees(":GZH#", false, az) && cmdGetDegrees(":GAH#", true, alt);
  #else
    return cmdGetDegrees(":GZ#", false, az) && cmdGetDegrees(":GA#", true, alt);
  #endif
}

static bool getSiteLatLon(double &lat, double &lon) {
  // confirmed against OnStepX src/telescope/mount/site/Site.command.cpp: ":Gt#"/":GtH#" are
  // "positive for North" (matches ASCOM SiteLatitude directly), but ":Gg#"/":GgH#" are
  // "east is negative" i.e. +West (the classic Meade LX200 convention) -- the opposite sign
  // of ASCOM's SiteLongitude (+East), so the parsed value must be negated here.
  bool highPrecision = status.getVersionMajor() > 3;
  bool ok1 = cmdGetDegrees(highPrecision ? ":GtH#" : ":Gt#", true, lat);

  // Confirmed against real hardware: longitude's reply is signed AND up to 3-digit degrees
  // (e.g. "+116*23#"), which dmsToDouble()'s signPresent=true mode can't parse -- that mode
  // only reads 2 degree digits (it's designed for <=90 degree quantities like latitude and
  // altitude). So the sign has to be stripped and reapplied manually around a signPresent=false
  // (unsigned, 3-digit) parse, exactly mirroring how OnStepX's own Site.command.cpp parses the
  // ":Sg#" *input* parameter the same way.
  char temp[80];
  bool ok2 = onStep.command(highPrecision ? ":GgH#" : ":Gg#", temp);
  if (ok2) {
    bool negative = (temp[0] == '-');
    char *numPart = (temp[0] == '+' || temp[0] == '-') ? &temp[1] : temp;
    ok2 = convert.dmsToDouble(&lon, numPart, false);
    if (ok2 && negative) lon = -lon;
  }
  if (ok2) lon = -lon; // OnStepX "east is negative" -> ASCOM's +East, per the comment above
  return ok1 && ok2;
}

// converts Alt/Az to RA/Dec so SlewToAltAzAsync can go through the (already hardware-confirmed)
// ":Sr#"/":Sd#"+":MS#" RA/Dec goto path instead of ":Sz#"/":Sa#" -- confirmed against OnStepX
// src/telescope/mount/goto/Goto.cpp's setTarget(): for any mount type other than ALTAZM/ALTALT
// (i.e. every GEM/Fork equatorial mount), ":MS#" ignores whatever ":Sz#"/":Sa#" set and instead
// *overwrites* the goto target's Alt/Az from its existing (possibly stale, e.g. left over from
// the previous test's RA/Dec target) equatorial target -- confirmed via ASCOM Conform against
// real hardware: SlewToAltAzAsync landed ~100 degrees away in Azimuth and ~28 degrees away in
// Altitude, matching where the mount's *previous* RA/Dec target converted to, not the requested
// Alt/Az. The math below mirrors OnStepX's own Transform::horToEqu (src/telescope/mount/
// coordinates/Transform.cpp) exactly, so it round-trips correctly against OnStepX's own
// equToHor() used for the AltAz GET properties -- and works regardless of the mount's physical
// type, since horizontal<->equatorial conversion is invertible either way.
static bool altAzToRaDec(double az, double alt, double &ra, double &dec) {
  double lat, lon, lst;
  if (!getSiteLatLon(lat, lon) || !cmdGetHours(":GS#", lst)) return false;

  const double d2r = 0.017453292519943295;
  double latRad = lat * d2r, azRad = az * d2r, altRad = alt * d2r;
  double cosAz = cos(azRad);
  double sinDec = sin(altRad) * sin(latRad) + cos(altRad) * cos(latRad) * cosAz;
  dec = asin(sinDec) / d2r;

  double t1 = sin(azRad);
  double t2 = cosAz * sin(latRad) - tan(altRad) * cos(latRad);
  double ha = atan2(t1, t2) / d2r;
  ha += 180.0;
  if (ha > 180.0) ha -= 360.0;

  ra = lst - ha / 15.0;
  while (ra < 0) ra += 24.0;
  while (ra >= 24.0) ra -= 24.0;
  return true;
}

// starts a goto via ":MS#" (single-digit reply per Cmd.cpp's shortResponse handling for the
// "S" sub-command). Confirmed against OnStepX src/telescope/mount/goto/Goto.command.cpp: the
// digit is OnStepX's own 0..9 goto-result code (0=possible, 1=below horizon, ... 9=unspecified),
// NOT this codebase's CommandErrors ordinal -- but Cmd.h's enum was deliberately laid out with
// CE_GOTO_ERR_BELOW_HORIZON..CE_GOTO_ERR_UNSPECIFIED as nine consecutive values in that same
// order, so offsetting from CE_GOTO_ERR_BELOW_HORIZON converts correctly.
static bool startGoto(AlpacaResponse &resp) {
  char temp[80];
  if (!onStep.command(":MS#", temp)) {
    resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return false;
  }
  if (temp[0] != 0 && temp[0] != '0') {
    int digit = temp[0] - '0'; // 1..9
    int errEnum = CE_GOTO_ERR_BELOW_HORIZON + (digit - 1);
    resp.setError(ALPACA_ERR_INVALID_OPERATION, onStep.commandErrorToStr(errEnum));
    return false;
  }
  return true;
}

// per the ASCOM spec, AbortSlew/FindHome/MoveAxis/PulseGuide/SyncToCoordinates/SyncToTarget (and,
// defensively, the SlewTo*Async methods -- see startGoto()'s CE_SLEW_ERR_IN_PARK handling, which
// already covers those via OnStep's own rejection) must throw ParkedException while parked --
// confirmed via ASCOM Conform against real hardware: several of these use commandBlind() or a
// "fails silently" OnStep reply (":CS#"), so nothing would otherwise surface OnStep's own
// rejection (if any) as an Alpaca error
static bool rejectIfParked(AlpacaResponse &resp) {
  if (status.parked) {
    resp.setError(ALPACA_ERR_PARKED, "Parked");
    return true;
  }
  return false;
}

// OnStep's ":Gr#"/":Gd#" always return *some* parseable value (its own last/default target),
// so unlike most of this file's "did the command succeed" checks, "has a target ever actually
// been set" has to be tracked here in software -- confirmed via ASCOM Conform: reading
// TargetRightAscension/TargetDeclination before ever writing them must throw
// ValueNotSetException, and previously didn't since OnStep's reply parsed successfully anyway.
static bool targetRaSet = false;
static bool targetDecSet = false;

static void setRaDecTarget(double ra, double dec) {
  char buf[16];
  convert.doubleToHms(buf, ra, false, PM_HIGH);
  char cmd[24];
  snprintf(cmd, sizeof(cmd), ":Sr%s#", buf);
  onStep.commandBool(cmd);
  convert.doubleToDms(buf, dec, false, true, PM_HIGH);
  snprintf(cmd, sizeof(cmd), ":Sd%s#", buf);
  onStep.commandBool(cmd);
  targetRaSet = true;
  targetDecSet = true;
}

// AlignmentModes: algAltAz=0, algPolar=1, algGermanPolar=2
static int alignmentMode() {
  if (status.mountType == MT_GEM) return 2;
  if (status.mountType == MT_FORK || status.mountType == MT_FORKALT) return 1;
  return 0;
}

// PierSide: pierUnknown=-1, pierEast=0, pierWest=1
static int sideOfPier() {
  if (status.pierSide == PierSideEast) return 0;
  if (status.pierSide == PierSideWest) return 1;
  return -1;
}

// DriveRates: driveSidereal=0, driveLunar=1, driveSolar=2, driveKing=3
// index 0=Sidereal,1=Lunar,2=Solar,3=King, matching Alpaca's DriveRate enum and this file's
// trackingrate PUT handler below. Picks the *nearest* of OnStepX's four known rate constants
// (src/lib/Constants.h: SIDEREAL_RATE_HZ=60.16427..., LUNAR_RATE_HZ=57.9, SOLAR_RATE_HZ=60.0,
// KING_RATE_HZ=60.136) instead of fixed +/-0.05 thresholds -- confirmed via ASCOM Conform
// against real hardware: Sidereal and King are only 0.028Hz apart, so a fixed-order threshold
// check matched true Sidereal readings against the King branch first, misreporting the rate
// right after setting it and making Conform think the Sidereal write had failed.
static int trackingRateFromHz(double hz) {
  static const double rateHz[4] = {60.16427456104770, 57.9, 60.0, 60.136};
  int best = 0;
  double bestDiff = fabs(hz - rateHz[0]);
  for (int i = 1; i < 4; i++) {
    double diff = fabs(hz - rateHz[i]);
    if (diff < bestDiff) { bestDiff = diff; best = i; }
  }
  return best;
}

static double guideRateDegPerSec(int index) {
  if (index < 0 || index > 9) index = 2;
  return guideRateXSidereal[index] * siderealDegPerSec;
}

// picks the nearest guide-rate index (0..7, the fixed x-sidereal presets) for a requested deg/s rate
static int nearestGuideRateIndex(double degPerSec) {
  double best = 1e9; int bestIndex = 2;
  for (int i = 0; i <= 7; i++) {
    double d = fabs(guideRateXSidereal[i] * siderealDegPerSec - fabs(degPerSec));
    if (d < best) { best = d; bestIndex = i; }
  }
  return bestIndex;
}

// remembers which continuous-motion direction command is active per axis so MoveAxis(axis,0) knows what to stop
static char axis0Dir = 0; // 'e', 'w', or 0
static char axis1Dir = 0; // 'n', 's', or 0

// live ":GU#" query for goto-in-progress/pulse-guiding state, bypassing status.inGoto/
// status.pulseGuiding, which are only refreshed by the periodic background status poll (up to
// ~1s stale) -- confirmed via ASCOM Conform against real hardware: IsPulseGuiding read
// immediately after PulseGuide() started a 2s pulse returned FALSE, because the cached flag
// hadn't been refreshed yet since the pulse began. Falls back to the cached status.* fields on
// comms failure.
static void queryGotoPulseState(bool &inGoto, bool &pulseGuiding) {
  inGoto = status.inGoto;
  pulseGuiding = status.pulseGuiding;
  char temp[80];
  if (!onStep.command(":GU#", temp)) return;
  inGoto = strstr(temp, "N") == NULL;
  pulseGuiding = strstr(temp, "G") != NULL;
}

// Slewing must be true during MoveAxis-driven jogging, not just during a goto -- confirmed via
// ASCOM Conform against real hardware ("Slewing is not true immediately/after 2 seconds moving").
// The live goto flag only reflects OnStepX's goto engine; the ":Me#"/":Mw#"/":Mn#"/":Ms#" jog
// commands MoveAxis uses are a different subsystem that doesn't set it, so this also has to
// track the jog state kept above.
static bool isSlewing() {
  bool inGoto, pulseGuiding;
  queryGotoPulseState(inGoto, pulseGuiding);
  return inGoto || axis0Dir != 0 || axis1Dir != 0;
}

static String telescopeDeviceStateJson(int deviceNumber) {
  UNUSED(deviceNumber); // single-instance device
  String out;
  bool first = true;
  double ra, dec, az, alt, lst;
  bool haveRaDec = getRaDec(ra, dec);
  bool haveAzAlt = getAzAlt(az, alt);
  bool haveLst = cmdGetHours(":GS#", lst);

  if (haveRaDec) {
    jsonStateValueEntry(out, first, "RightAscension", jsonNumber(ra));
    jsonStateValueEntry(out, first, "Declination", jsonNumber(dec));
  }
  if (haveAzAlt) {
    jsonStateValueEntry(out, first, "Azimuth", jsonNumber(az));
    jsonStateValueEntry(out, first, "Altitude", jsonNumber(alt));
  }
  if (haveLst) jsonStateValueEntry(out, first, "SiderealTime", jsonNumber(lst));
  bool inGoto, pulseGuiding;
  queryGotoPulseState(inGoto, pulseGuiding);
  jsonStateValueEntry(out, first, "AtHome", jsonBool(status.atHome));
  jsonStateValueEntry(out, first, "AtPark", jsonBool(status.parked));
  jsonStateValueEntry(out, first, "Slewing", jsonBool(inGoto || axis0Dir != 0 || axis1Dir != 0));
  jsonStateValueEntry(out, first, "Tracking", jsonBool(status.tracking));
  jsonStateValueEntry(out, first, "SideOfPier", jsonInt(sideOfPier()));
  jsonStateValueEntry(out, first, "IsPulseGuiding", jsonBool(pulseGuiding));
  jsonStateValueEntry(out, first, "TimeStamp", jsonString(""));
  return out;
}

void AlpacaTelescope::handleRequest(int deviceNumber, const String &method, int httpMethod, AlpacaResponse &resp) {
  if (deviceNumber != 0) {
    resp.setError(ALPACA_ERR_INVALID_VALUE, "Invalid Telescope device number");
    return;
  }

  bool connected = status.onStepFound;

  if (alpacaHandleCommonMember(method, httpMethod, resp, "OnStep", "OnStep telescope mount, via SmartWebServer",
                                4, connected, deviceNumber, telescopeDeviceStateJson)) {
    return;
  }

  // every member reaching this point is telescope-specific (the shared "connected" member
  // itself was already handled above), so it's safe to require a live OnStep link here
  if (!connected) {
    resp.setError(ALPACA_ERR_NOT_CONNECTED, "OnStep not found");
    return;
  }

  // ---- read/write properties ----

  if (method == "alignmentmode") { resp.setValue(jsonInt(alignmentMode())); return; }

  if (method == "aperturearea" || method == "aperturediameter" || method == "focallength") {
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Not known to OnStep/SmartWebServer");
    return;
  }

  if (method == "siteelevation") {
    // confirmed against OnStepX src/telescope/mount/site/Site.command.cpp: ":Gv#"/":Sv#" exist
    // (meters, simple signed decimal, no dms/hms conversion needed) -- previously left as
    // NotImplementedException, which turned out to be the root cause of an ASCOM Conform
    // "CheckMethods" abort (some other test apparently needs elevation internally and didn't
    // handle that exception gracefully)
    if (httpMethod == HTTP_GET) {
      char temp[80];
      if (!onStep.command(":Gv#", temp)) { resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure"); return; }
      resp.setValue(jsonNumber(atof(temp)));
    } else {
      // validated against the *intersection* of two different valid ranges, confirmed via ASCOM
      // Conform against real hardware both ways:
      //  - ASCOM's own spec documents SiteElevation as valid from -300 to +10,000m, and Conform
      //    enforces that ceiling regardless of what the hardware can actually do (a value above
      //    10,000 must be rejected here even though OnStep would happily accept it)
      //  - OnStepX's Site::setElevation() (src/telescope/mount/site/Site.cpp) only accepts
      //    [-100, 20000) -- narrower than ASCOM's floor of -300, so a value between -300 and
      //    -100 would pass a check against the ASCOM range alone and then genuinely get
      //    rejected by OnStep (surfaced as a confusing "OnStep rejected value" rather than a
      //    clean InvalidValueException)
      // the intersection (-100 to 10,000) guarantees anything accepted here is valid on both counts
      double v;
      if (!alpacaArgDouble("SiteElevation", v) || v < -100 || v > 10000) { resp.setError(ALPACA_ERR_INVALID_VALUE, "SiteElevation out of range"); return; }
      char cmd[24];
      snprintf(cmd, sizeof(cmd), ":Sv%+.1f#", v);
      if (!onStep.commandBool(cmd)) resp.setError(ALPACA_ERR_INVALID_VALUE, "OnStep rejected value");
    }
    return;
  }

  // per the ASCOM spec these two GETs are mandatory and must return 0 when not settable
  // (CanSetDeclinationRate/CanSetRightAscensionRate are both false) rather than throw --
  // confirmed against real hardware via the ASCOM Conform Universal tool
  if (method == "declinationrate" || method == "rightascensionrate") {
    if (httpMethod == HTTP_GET) resp.setValue(jsonNumber(0));
    else resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Not adjustable");
    return;
  }

  if (method == "altitude") {
    double az, alt;
    if (getAzAlt(az, alt)) resp.setValue(jsonNumber(alt)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }
  if (method == "azimuth") {
    double az, alt;
    if (getAzAlt(az, alt)) resp.setValue(jsonNumber(az)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }
  if (method == "rightascension") {
    double ra, dec;
    if (getRaDec(ra, dec)) resp.setValue(jsonNumber(ra)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }
  if (method == "declination") {
    double ra, dec;
    if (getRaDec(ra, dec)) resp.setValue(jsonNumber(dec)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "targetrightascension") {
    if (httpMethod == HTTP_GET) {
      double ra, dec;
      if (!targetRaSet) { resp.setError(ALPACA_ERR_VALUE_NOT_SET, "Target has not been set"); return; }
      if (getTargetRaDec(ra, dec)) resp.setValue(jsonNumber(ra)); else resp.setError(ALPACA_ERR_VALUE_NOT_SET, "Target not set");
    } else {
      double ra;
      if (!alpacaArgDouble("TargetRightAscension", ra) || ra < 0 || ra >= 24) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad RA"); return; }
      char buf[16]; convert.doubleToHms(buf, ra, false, PM_HIGH);
      char cmd[24]; snprintf(cmd, sizeof(cmd), ":Sr%s#", buf);
      if (!onStep.commandBool(cmd)) resp.setError(ALPACA_ERR_INVALID_VALUE, "OnStep rejected RA"); else targetRaSet = true;
    }
    return;
  }
  if (method == "targetdeclination") {
    if (httpMethod == HTTP_GET) {
      double ra, dec;
      if (!targetDecSet) { resp.setError(ALPACA_ERR_VALUE_NOT_SET, "Target has not been set"); return; }
      if (getTargetRaDec(ra, dec)) resp.setValue(jsonNumber(dec)); else resp.setError(ALPACA_ERR_VALUE_NOT_SET, "Target not set");
    } else {
      double dec;
      if (!alpacaArgDouble("TargetDeclination", dec) || dec < -90 || dec > 90) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad Dec"); return; }
      char buf[16]; convert.doubleToDms(buf, dec, false, true, PM_HIGH);
      char cmd[24]; snprintf(cmd, sizeof(cmd), ":Sd%s#", buf);
      if (!onStep.commandBool(cmd)) resp.setError(ALPACA_ERR_INVALID_VALUE, "OnStep rejected Dec"); else targetDecSet = true;
    }
    return;
  }

  if (method == "sitelatitude" || method == "sitelongitude") {
    if (httpMethod == HTTP_GET) {
      double lat, lon;
      if (!getSiteLatLon(lat, lon)) { resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure"); return; }
      resp.setValue(jsonNumber(method == "sitelatitude" ? lat : lon));
    } else {
      double v;
      if (!alpacaArgDouble(method == "sitelatitude" ? "SiteLatitude" : "SiteLongitude", v)) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Missing value"); return; }
      char buf[16];
      char cmd[24];
      if (method == "sitelatitude") {
        if (v < -90 || v > 90) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad latitude"); return; }
        // PM_HIGHEST (fractional arcseconds), not PM_HIGH -- confirmed via ASCOM Conform that
        // PM_HIGH's whole-second precision was losing the fractional part on round-trip (Conform
        // writes e.g. +44:17:27.4 and expects to read the same value back); OnStepX's ":St#"
        // documents the ".SSS" fractional form as valid input
        convert.doubleToDms(buf, v, false, true, PM_HIGHEST);
        snprintf(cmd, sizeof(cmd), ":St%s#", buf);
      } else {
        if (v < -180 || v > 180) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad longitude"); return; }
        // negate: ASCOM SiteLongitude is +East, OnStepX's ":Sg#" is +West ("east is negative"), see getSiteLatLon()
        // PM_HIGHEST: see the sitelatitude comment above, same round-trip precision fix
        convert.doubleToDms(buf, -v, true, true, PM_HIGHEST);
        snprintf(cmd, sizeof(cmd), ":Sg%s#", buf);
      }
      if (!onStep.commandBool(cmd)) resp.setError(ALPACA_ERR_INVALID_VALUE, "OnStep rejected value");
    }
    return;
  }

  if (method == "siderealtime") {
    double lst;
    if (cmdGetHours(":GS#", lst)) resp.setValue(jsonNumber(lst)); else resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
    return;
  }

  if (method == "equatorialsystem") { resp.setValue(jsonInt(1)); return; } // equTopocentric (JNow)

  if (method == "athome") { resp.setValue(jsonBool(status.atHome)); return; }
  if (method == "atpark") { resp.setValue(jsonBool(status.parked)); return; }
  if (method == "slewing") { resp.setValue(jsonBool(isSlewing())); return; }
  if (method == "ispulseguiding") {
    bool inGoto, pulseGuiding;
    queryGotoPulseState(inGoto, pulseGuiding);
    resp.setValue(jsonBool(pulseGuiding));
    return;
  }
  if (method == "sideofpier") {
    if (httpMethod == HTTP_GET) resp.setValue(jsonInt(sideOfPier()));
    else resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "SideOfPier cannot be set");
    return;
  }

  if (method == "slewsettletime") {
    // OnStepX's goto engine does a final slow-speed "near destination" refine/backlash-takeup
    // pass right before it clears its in-goto flag (src/telescope/mount/goto/Goto.cpp,
    // nearDestinationRefineStages) -- confirmed via ASCOM Conform against real hardware: reading
    // RA/Dec/Az/Alt immediately after Slewing goes false was consistently ~10-25 arcseconds off
    // target. 2s is a heuristic buffer for that settling, not a measured minimum.
    if (httpMethod == HTTP_GET) resp.setValue(jsonInt(2));
    else resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Not adjustable");
    return;
  }

  if (method == "doesrefraction") {
    if (httpMethod == HTTP_GET) resp.setValue(jsonBool(status.rateCompensation != RC_NONE));
    else resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Use tracking rate compensation controls instead");
    return;
  }

  if (method == "tracking") {
    if (httpMethod == HTTP_GET) {
      resp.setValue(jsonBool(status.tracking));
    } else {
      bool on = alpacaArgBool("Tracking", false);
      if (!onStep.commandBool(on ? ":Te#" : ":Td#")) resp.setError(ALPACA_ERR_INVALID_OPERATION, "OnStep rejected tracking change");
    }
    return;
  }

  if (method == "trackingrate") {
    if (httpMethod == HTTP_GET) {
      char temp[80];
      double hz = 0;
      if (onStep.command(":GT#", temp)) hz = atof(temp);
      resp.setValue(jsonInt(trackingRateFromHz(hz)));
    } else {
      uint32_t rate = alpacaArgUint32("TrackingRate", 0);
      const char *cmd = ":TQ#";
      if (rate == 1) cmd = ":TL#"; else if (rate == 2) cmd = ":TS#"; else if (rate == 3) cmd = ":TK#";
      else if (rate != 0) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Unsupported tracking rate"); return; }
      onStep.commandBlind(cmd);
    }
    return;
  }
  if (method == "trackingrates") { resp.setValue("[0,1,2,3]"); return; }

  if (method == "utcdate") {
    if (httpMethod == HTTP_GET) {
      char timeStr[80], dateStr[80];
      if (!onStep.command(":GX80#", timeStr) || !onStep.command(":GX81#", dateStr)) {
        resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure");
        return;
      }
      // confirmed against OnStepX docs/COMMAND_REFERENCE.md: ":GX81#" -> "MM/DD/YY#",
      // ":GX80#" -> "HH:MM:SS.ss#" (fractional seconds are valid trailing digits in ISO 8601)
      int mo = 0, dy = 0, yr = 0;
      sscanf(dateStr, "%d/%d/%d", &mo, &dy, &yr);
      char iso[32];
      snprintf(iso, sizeof(iso), "20%02d-%02d-%02dT%sZ", yr, mo, dy, timeStr);
      resp.setValue(jsonString(iso));
    } else {
      resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Setting the mount clock via Alpaca isn't supported yet, use the web UI");
    }
    return;
  }

  if (method == "guideraterightascension" || method == "guideratedeclination") {
    if (httpMethod == HTTP_GET) {
      resp.setValue(jsonNumber(guideRateDegPerSec(status.guideRate)));
    } else {
      double v;
      const char *argName = (method == "guideraterightascension") ? "GuideRateRightAscension" : "GuideRateDeclination";
      if (!alpacaArgDouble(argName, v)) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Missing value"); return; }
      int idx = nearestGuideRateIndex(v);
      char cmd[8]; snprintf(cmd, sizeof(cmd), ":R%d#", idx);
      onStep.commandBlind(cmd);
    }
    return;
  }

  // ---- capability flags ----
  if (method == "canfindhome") { resp.setValue(jsonBool(true)); return; }
  if (method == "canpark") { resp.setValue(jsonBool(true)); return; }
  if (method == "canunpark") { resp.setValue(jsonBool(true)); return; }
  if (method == "cansetpark") { resp.setValue(jsonBool(true)); return; }
  if (method == "canpulseguide") { resp.setValue(jsonBool(true)); return; }
  if (method == "cansettracking") { resp.setValue(jsonBool(true)); return; }
  if (method == "cansetguiderates") { resp.setValue(jsonBool(true)); return; }
  if (method == "canslew") { resp.setValue(jsonBool(false)); return; } // synchronous slew not implemented, see slewtocoordinates
  if (method == "canslewasync") { resp.setValue(jsonBool(true)); return; }
  if (method == "canslewaltaz") { resp.setValue(jsonBool(false)); return; }
  if (method == "canslewaltazasync") { resp.setValue(jsonBool(true)); return; }
  if (method == "cansync") { resp.setValue(jsonBool(true)); return; }
  if (method == "cansyncaltaz") { resp.setValue(jsonBool(false)); return; }
  if (method == "canmoveaxis") {
    uint32_t axis = alpacaArgUint32("Axis", 0);
    resp.setValue(jsonBool(axis == 0 || axis == 1));
    return;
  }
  if (method == "cansetpierside") { resp.setValue(jsonBool(false)); return; }
  if (method == "cansetdeclinationrate") { resp.setValue(jsonBool(false)); return; }
  if (method == "cansetrightascensionrate") { resp.setValue(jsonBool(false)); return; }

  // ---- methods ----

  if (method == "abortslew") { if (rejectIfParked(resp)) return; onStep.commandBlind(":Q#"); return; }
  if (method == "findhome") { if (rejectIfParked(resp)) return; onStep.commandBlind(":hC#"); return; }
  if (method == "park") { if (!onStep.commandBool(":hP#")) resp.setError(ALPACA_ERR_INVALID_OPERATION, "Park failed"); return; }
  if (method == "unpark") {
    // per the ASCOM spec, unparking an already-unparked scope must be a harmless no-op, not an
    // error -- confirmed via ASCOM Conform: OnStep's ":hR#" isn't idempotent the way ":hP#" is
    // (it fails when there's nothing to unpark), so that has to be handled here instead
    if (!status.parked) return;
    if (!onStep.commandBool(":hR#")) resp.setError(ALPACA_ERR_INVALID_OPERATION, "Unpark failed");
    return;
  }
  if (method == "setpark") { if (!onStep.commandBool(":hQ#")) resp.setError(ALPACA_ERR_INVALID_OPERATION, "SetPark failed"); return; }

  if (method == "slewtocoordinates" || method == "slewtotarget" || method == "slewtoaltaz") {
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Synchronous slewing isn't supported, use the Async method");
    return;
  }

  if (method == "slewtocoordinatesasync") {
    if (rejectIfParked(resp)) return;
    double ra, dec;
    if (!alpacaArgDouble("RightAscension", ra) || !alpacaArgDouble("Declination", dec) ||
        ra < 0 || ra >= 24 || dec < -90 || dec > 90) {
      resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad coordinates");
      return;
    }
    setRaDecTarget(ra, dec);
    startGoto(resp);
    return;
  }
  if (method == "slewtotargetasync") {
    if (rejectIfParked(resp)) return;
    if (!targetRaSet || !targetDecSet) { resp.setError(ALPACA_ERR_VALUE_NOT_SET, "Target has not been set"); return; }
    startGoto(resp);
    return;
  }
  if (method == "slewtoaltazasync") {
    if (rejectIfParked(resp)) return;
    double az, alt;
    if (!alpacaArgDouble("Azimuth", az) || !alpacaArgDouble("Altitude", alt) ||
        az < 0 || az > 360 || alt < -90 || alt > 90) {
      resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad coordinates");
      return;
    }
    // goes via RA/Dec, not OnStepX's ":Sz#"/":Sa#" -- see altAzToRaDec()'s comment for why
    double ra, dec;
    if (!altAzToRaDec(az, alt, ra, dec)) { resp.setError(ALPACA_ERR_DRIVER_COMMS_FAILURE, "Comms failure"); return; }
    setRaDecTarget(ra, dec);
    startGoto(resp);
    return;
  }

  if (method == "synctocoordinates") {
    if (rejectIfParked(resp)) return;
    double ra, dec;
    if (!alpacaArgDouble("RightAscension", ra) || !alpacaArgDouble("Declination", dec) ||
        ra < 0 || ra >= 24 || dec < -90 || dec > 90) {
      resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad coordinates");
      return;
    }
    setRaDecTarget(ra, dec);
    if (!onStep.commandBool(":CS#")) resp.setError(ALPACA_ERR_INVALID_OPERATION, "Sync failed");
    return;
  }
  if (method == "synctotarget") {
    if (rejectIfParked(resp)) return;
    if (!targetRaSet || !targetDecSet) { resp.setError(ALPACA_ERR_VALUE_NOT_SET, "Target has not been set"); return; }
    if (!onStep.commandBool(":CS#")) resp.setError(ALPACA_ERR_INVALID_OPERATION, "Sync failed");
    return;
  }
  if (method == "synctoaltaz") {
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "SyncToAltAz isn't supported by OnStep");
    return;
  }

  if (method == "destinationsideofpier") {
    resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Not available");
    return;
  }

  if (method == "axisrates") {
    uint32_t axis = alpacaArgUint32("Axis", 0);
    if (axis != 0 && axis != 1) { resp.setValue("[]"); return; }
    String out = "[";
    for (int i = 0; i <= 7; i++) {
      if (i) out += ",";
      double r = guideRateXSidereal[i] * siderealDegPerSec;
      out += "{\"Minimum\":" + jsonNumber(r) + ",\"Maximum\":" + jsonNumber(r) + "}";
    }
    out += "]";
    resp.setValue(out);
    return;
  }

  if (method == "moveaxis") {
    if (rejectIfParked(resp)) return;
    uint32_t axis = alpacaArgUint32("Axis", 99);
    double rate;
    if ((axis != 0 && axis != 1) || !alpacaArgDouble("Rate", rate)) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad axis/rate"); return; }

    // per the ASCOM spec, a non-zero Rate outside the range AxisRates() advertises (indices 0..7
    // of guideRateXSidereal, see "axisrates" above) must throw InvalidValueException -- confirmed
    // via ASCOM Conform, which deliberately probes just outside both ends
    if (rate != 0) {
      double minRate = guideRateXSidereal[0] * siderealDegPerSec;
      double maxRate = guideRateXSidereal[7] * siderealDegPerSec;
      // tolerance: the client only ever sees these bounds after they've round-tripped through
      // AxisRates()'s 6-decimal JSON (jsonNumber()), so a client re-sending exactly the
      // advertised boundary value produces a double that differs from this freshly-recomputed
      // maxRate by a tiny rounding amount -- confirmed via ASCOM Conform, which does exactly
      // that and had the true boundary rate spuriously rejected by a strict comparison here
      const double rateEpsilon = 0.0001;
      if (fabs(rate) < minRate - rateEpsilon || fabs(rate) > maxRate + rateEpsilon) {
        resp.setError(ALPACA_ERR_INVALID_VALUE, "Rate outside the range AxisRates() advertises");
        return;
      }
    }

    if (axis == 0) {
      if (axis0Dir) { char c[6]; snprintf(c, sizeof(c), ":Q%c#", axis0Dir); onStep.commandBlind(c); axis0Dir = 0; }
      if (rate != 0) {
        int idx = nearestGuideRateIndex(rate);
        char c[8]; snprintf(c, sizeof(c), ":R%d#", idx); onStep.commandBlind(c);
        axis0Dir = (rate > 0) ? 'e' : 'w';
        snprintf(c, sizeof(c), ":M%c#", axis0Dir); onStep.commandBlind(c);
      }
    } else {
      if (axis1Dir) { char c[6]; snprintf(c, sizeof(c), ":Q%c#", axis1Dir); onStep.commandBlind(c); axis1Dir = 0; }
      if (rate != 0) {
        int idx = nearestGuideRateIndex(rate);
        char c[8]; snprintf(c, sizeof(c), ":R%d#", idx); onStep.commandBlind(c);
        axis1Dir = (rate > 0) ? 'n' : 's';
        snprintf(c, sizeof(c), ":M%c#", axis1Dir); onStep.commandBlind(c);
      }
    }
    return;
  }

  if (method == "pulseguide") {
    if (rejectIfParked(resp)) return;
    // confirmed against OnStepX src/telescope/mount/guide/Guide.command.cpp: ":Mgd[n]#" where d
    // is lowercase 'w','e','n', or 's' (uppercase falls through to CE_CMD_UNKNOWN there) and n is
    // a plain (not zero-padded) millisecond count parsed with convert.atoi2(); "g" (vs. "G") means
    // no reply is sent, matching commandBlind() below, and OnStep self-times/executes it async.
    uint32_t direction = alpacaArgUint32("Direction", 99);
    uint32_t duration = alpacaArgUint32("Duration", 0);
    if (direction > 3) { resp.setError(ALPACA_ERR_INVALID_VALUE, "Bad direction"); return; }
    if (duration > 9999) duration = 9999;
    char dirChar = "nsew"[direction]; // guideNorth=0, guideSouth=1, guideEast=2, guideWest=3
    char cmd[16];
    snprintf(cmd, sizeof(cmd), ":Mg%c%d#", dirChar, (int)duration);
    onStep.commandBlind(cmd);
    return;
  }

  resp.setError(ALPACA_ERR_NOT_IMPLEMENTED, "Unknown Telescope member: " + method);
}

AlpacaTelescope alpacaTelescope;

#endif
