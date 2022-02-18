// scan OnStep state in the background

#include "State.h"
#include <limits.h>
#if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
  #include "..\lib\wifi\WifiManager.h"
#endif
#include "..\lib\tasks\OnTask.h"
#include "..\lib\convert\Convert.h"
#include "..\libApp\misc\Misc.h"
#include "..\libApp\cmd\Cmd.h"
#include "..\libApp\status\MountStatus.h"
#include "..\libApp\encoders\Encoders.h"
#include "..\locales\Locale.h"

void pollState() { state.poll(); }
void pollStateSlow() { state.pollSlow(); }
void pollStateFast() { state.pollFast(); }

void State::init() {
  //doc = new DynamicJsonDocument(2000);

  // start polling tasks
  VF("MSG: State, start polling task (priority 6)... ");
  if (tasks.add(STATE_POLLING_RATE_MS, 0, true, 6, pollState, "StaPoll")) { VLF("success"); } else { VLF("FAILED!"); }

  VF("MSG: State, start slow polling task (priority 7)... ");
  if (tasks.add(STATE_SLOW_POLLING_RATE_MS, 0, true, 7, pollStateSlow, "StSPoll")) { VLF("success"); } else { VLF("FAILED!"); }

  VF("MSG: State, start fast polling task (priority 5)... ");
  if (tasks.add(STATE_FAST_POLLING_RATE_MS, 0, true, 5, pollStateFast, "StFPoll")) { VLF("success"); } else { VLF("FAILED!"); }
}

void State::poll() {
  char temp[80];

  // UTC Time and Date
  if (!onStep.command(":GX80#", temp)) strcpy(temp, "?");
  strncpy(timeStr, temp, 8); timeStr[9] = 0;
  if (strcmp(timeStr, "00:00:00") ||
     (strlen(dateStr) == 0 && !strcmp(timeStr, "23:59:59"))) {
    if (!onStep.command(":GX81#", temp)) strcpy(temp, "?");
    if (temp[0] == '0') strcpy(&temp[0], &temp[1]);
    strncpy(dateStr, temp, 10); dateStr[9] = 0;
  }
  Y;

  // LST
  if (!onStep.command(":GS#", temp)) strcpy(temp, "?");
  strncpy(lastStr, temp, 8); lastStr[9] = 0; Y;

  #if DISPLAY_HIGH_PRECISION_COORDS == ON
    // RA,Dec current
    if (!onStep.command(":GRa#", temp)) strcpy(temp, "?");
    strncpy(indexRaStr, temp, 14); indexRaStr[13] = 0; Y;
    if (!onStep.command(":GDe#", temp)) strcpy(temp, "?");
    strncpy(indexDecStr, temp, 14); indexDecStr[13] = 0; Y;

    // RA,Dec target
    if (!onStep.command(":Gra#", temp)) strcpy(temp, "?");
    strncpy(targetRaStr, temp, 14); targetRaStr[13] = 0; Y;
    if (!onStep.command(":Gde#", temp)) strcpy(temp, "?");
    strncpy(targetDecStr, temp, 14); targetDecStr[13] = 0; Y;
  #else
    // RA,Dec Current
    if (!onStep.command(":GR#", temp)) strcpy(temp, "?");
    strncpy(indexRaStr, temp, 14); indexRaStr[13] = 0; Y;
    if (!onStep.command(":GD#", temp)) strcpy(temp, "?");
    strncpy(indexDecStr, temp, 14); indexDecStr[13] = 0; Y;

    // RA,Dec Target
    if (!onStep.command(":Gr#", temp)) strcpy(temp, "?");
    strncpy(targetRaStr, temp, 14); targetRaStr[13] = 0; Y;
    if (!onStep.command(":Gd#", temp)) strcpy(temp, "?");
    strncpy(targetDecStr, temp, 14); targetDecStr[13] = 0; Y;
  #endif

  #if ENCODERS == ON
    // RA,Dec OnStep and Encoder positions
    double f;
    f = encoders.getOnStepAxis1(); convert.doubleToDms(temp, f, true, true, PM_HIGH);
    strncpy(angleAxis1Str, temp, 14); angleAxis1Str[13] = 0;
    if (encoders.validAxis1()) { f = encoders.getAxis1(); convert.doubleToDms(temp, f, true, true, PM_HIGH); } else strcpy(temp," ** " L_FAULT " ** ");
    strncpy(encAngleAxis1Str, temp, 14); encAngleAxis1Str[13] = 0; Y;

    f = encoders.getOnStepAxis2(); convert.doubleToDms(temp, f, true, true, PM_HIGH);
    strncpy(angleAxis2Str, temp, 14); angleAxis2Str[13] = 0;
    if (encoders.validAxis2()) { f = encoders.getAxis2(); convert.doubleToDms(temp, f, true, true, PM_HIGH); } else strcpy(temp," ** " L_FAULT " ** ");
    strncpy(encAngleAxis2Str, temp, 14); encAngleAxis2Str[13] = 0; Y;
  #endif

}

void State::pollSlow() {
  char temp[80], temp1[80];

  // Latitude
  if (mountStatus.getVersionMajor() > 3) {
    if (!onStep.command(":GtH#", temp)) strcpy(temp, "?");
  } else {
    if (!onStep.command(":Gt#", temp)) strcpy(temp, "?");
  }
  strncpy(latitudeStr, temp, 9); latitudeStr[9] = 0; Y;
  convert.dmsToDouble(&latitude, latitudeStr, true);

  // Longitude
  if (mountStatus.getVersionMajor() > 3) {
    if (!onStep.command(":GgH#", temp)) strcpy(temp, "?");
  } else {
    if (!onStep.command(":Gg#", temp)) strcpy(temp, "?");
  }
  strncpy(longitudeStr, temp, 10); longitudeStr[10] = 0; Y;

  // Ambient conditions
  #if DISPLAY_WEATHER == ON
    if (!onStep.command(":GX9A#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(siteTemperatureStr, temp, 10); siteTemperatureStr[9] = 0; Y;
    if (!onStep.command(":GX9B#", temp)) strcpy(temp, "?"); else localePressure(temp);
    strncpy(sitePressureStr, temp, 10); sitePressureStr[9] = 0; Y;
    if (!onStep.command(":GX9C#", temp)) strcpy(temp, "?"); else localeHumidity(temp);
    strncpy(siteHumidityStr, temp, 10); siteHumidityStr[9] = 0; Y;
    if (!onStep.command(":GX9E#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(siteDewPointStr, temp, 10); siteDewPointStr[9] = 0; Y;
  #endif

  // Focuser/telescope temperature
  if (mountStatus.focuserPresent()) {
    if (!onStep.command(":Ft#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(telescopeTemperatureStr, temp, 10); telescopeTemperatureStr[9] = 0; Y;
  }

  // pier side
  if ((mountStatus.pierSide() == PierSideFlipWE1) || (mountStatus.pierSide() == PierSideFlipWE2) || (mountStatus.pierSide() == PierSideFlipWE3)) strcpy(temp, L_MERIDIAN_FLIP_W_TO_E); else
  if ((mountStatus.pierSide() == PierSideFlipEW1) || (mountStatus.pierSide() == PierSideFlipEW2) || (mountStatus.pierSide() == PierSideFlipEW3)) strcpy(temp, L_MERIDIAN_FLIP_E_TO_W); else
  if (mountStatus.pierSide() == PierSideWest) strcpy(temp, L_WEST); else
  if (mountStatus.pierSide() == PierSideEast) strcpy(temp, L_EAST); else
  if (mountStatus.pierSide() == PierSideNone) strcpy(temp, L_NONE); else strcpy(temp, L_UNKNOWN);
  if (!mountStatus.valid()) strcpy(temp, "?");
  strncpy(pierSideStr, temp, 10); pierSideStr[9] = 0; Y;

  // meridian flip
  if (mountStatus.meridianFlips()) {
    strcpy(temp, "On");
    if (mountStatus.autoMeridianFlips()) strcat(temp, ", " L_AUTO);
  } else strcpy(temp, "Off");
  if (!mountStatus.valid()) strcpy(temp, "?");
  strncpy(meridianFlipStr, temp, 10); pierSideStr[9] = 0; Y;

  // polar align
  strcpy(alignLrStr, "?");
  strcpy(alignUdStr, "?");
  if (!isnan(latitude) && abs(latitude) <= 89) {
    long ud = LONG_MIN;
    if (onStep.command(":GX02#", temp)) { ud = strtol(&temp[0], NULL, 10); if (latitude < 0) ud = -ud; }
    long lr = LONG_MIN;
    if (onStep.command(":GX03#", temp)) { lr = strtol(&temp[0], NULL, 10); lr = lr/cos(latitude/57.295); }

    if (ud != LONG_MIN && lr != LONG_MIN) {
      char units = '"';
      if (abs(ud) >= 300 || abs(lr) >= 300) { ud = ud/60; lr = lr/60; units = '\''; }

      char lr_s[2];
      if (lr >= 0) strcpy(lr_s, leftTri); else strcpy(lr_s, rightTri);
      char ud_s[2];
      if (ud >= 0) strcpy(ud_s, upTri); else strcpy(ud_s, downTri);

      sprintf_P(temp, "%s %ld%c", lr_s, (long)(abs(lr)), units);
      strncpy(alignLrStr, temp, 10); alignLrStr[9] = 0;

      sprintf_P(temp, "%s %ld%c", ud_s, (long)(abs(ud)), units);
      strncpy(alignUdStr, temp, 10); alignUdStr[9] = 0; Y;
    }
  }

  // Park
  if (mountStatus.parked()) strcpy(temp, L_PARKED); else strcpy(temp, L_NOT_PARKED);
  if (mountStatus.parking()) strcpy(temp, L_PARKING); else
  if (mountStatus.parkFail()) strcpy(temp, L_PARK_FAILED);
  if (mountStatus.atHome()) strcat(temp, " (" L_AT_HOME ")");
  if (!mountStatus.valid()) strcpy(temp, "?");
  strncpy(parkStr, temp, 40); parkStr[39] = 0; Y;

  // Tracking
  if (mountStatus.tracking()) {
    if (onStep.command(":GT#", temp)) {
      double tr = atof(temp);
      sprintF(temp, "%5.3fHz", tr);
    } else strcpy(temp, "?");
  } else strcpy(temp, L_OFF);
  if (mountStatus.inGoto()) strcpy(temp, L_INGOTO);
  if (!mountStatus.valid()) strcpy(temp, "?");
  
  strcpy(temp1, "(");
  if (mountStatus.ppsSync()) strcat(temp1, L_PPS_SYNC ", ");
  if (mountStatus.rateCompensation() == RC_REFR_RA)   strcat(temp1, L_REFR_COMP_RA ", ");
  if (mountStatus.rateCompensation() == RC_REFR_BOTH) strcat(temp1, L_REFR_COMP_BOTH ", ");
  if (mountStatus.rateCompensation() == RC_FULL_RA)   strcat(temp1, L_FULL_COMP_RA ", ");
  if (mountStatus.rateCompensation() == RC_FULL_BOTH) strcat(temp1, L_FULL_COMP_BOTH ", ");
  if (!mountStatus.valid()) strcpy(temp1, "?");
  if (temp1[strlen(temp1) - 2] == ',') { temp1[strlen(temp1) - 2] = 0; strcat(temp1, ")"); } else strcpy(temp1, "");
  strcat(temp, " "); strcat(temp, temp1);
  strncpy(trackStr, temp, 40); trackStr[39] = 0; Y;

  // Slew speed
  if (!onStep.command(":GX97#", temp)) strcpy(temp, "?"); else strcat(temp, "&deg;/s");
  strncpy(slewSpeedStr, temp, 16); slewSpeedStr[15] = 0; Y;

  // Driver status
  int numAxes = 2;
  if (mountStatus.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++) {
    if (mountStatus.driver[axis].valid) {
      strcpy(temp1, "");
      if (mountStatus.driver[axis].fault) strcat(temp1, L_DRIVER_FAULT "  ");
      if (mountStatus.driver[axis].communicationFailure) strcat(temp1, L_COMMS_FAILURE ", ");
      if (!mountStatus.driver[axis].communicationFailure) {
        if (mountStatus.driver[axis].standstill) strcat(temp1, L_STANDSTILL ", "); else {
          if (mountStatus.driver[axis].outputA.openLoad || mountStatus.driver[axis].outputB.openLoad) {
            strcat(temp1, L_OPEN_LOAD " ");
            if (mountStatus.driver[axis].outputA.openLoad) strcat(temp1,"A");
            if (mountStatus.driver[axis].outputB.openLoad) strcat(temp1,"B");
            strcat(temp1,", ");
          }
        }
        if (mountStatus.driver[axis].outputA.shortToGround || mountStatus.driver[axis].outputB.shortToGround) {
          strcat(temp1, L_SHORT_GND " ");
          if (mountStatus.driver[axis].outputA.shortToGround) strcat(temp1,"A");
          if (mountStatus.driver[axis].outputB.shortToGround) strcat(temp1,"B");
          strcat(temp1,", ");
        }
        if (mountStatus.driver[axis].overTemperature) strcat(temp1, L_SHUTDOWN_OVER " 150C, ");
        if (mountStatus.driver[axis].overTemperaturePreWarning) strcat(temp1, L_PRE_WARNING " &gt;120C, ");
      }
      if (strlen(temp1) > 2) temp1[strlen(temp1) - 2] = 0;
      if (strlen(temp1) == 0) strcpy(temp1, "Ok");

      char *thisDriverStatusStr = driverStatusStr[axis];
      strncpy(thisDriverStatusStr, temp, 40); thisDriverStatusStr[39] = 0; Y;
    }
  }

  // MCU Temperature
  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    if (!onStep.command(":GX9F#", temp1)) strcpy(temp1, "?"); else localeTemperature(temp1);
    strncpy(controllerTemperatureStr, temp, 10); controllerTemperatureStr[9] = 0; Y;
  #endif

  // General Error
  strcpy(temp, "");
  if (mountStatus.lastError() != ERR_NONE) strcat(temp, "<font class=\"y\">"); 
  mountStatus.getLastErrorMessage(temp1);
  if (!mountStatus.valid()) strcat(temp, "?"); else strcat(temp, temp1);
  if (mountStatus.lastError() != ERR_NONE) strcat(temp, "</font>"); 
  strncpy(lastErrorStr, temp, 40); lastErrorStr[39] = 0; Y;

  // Loop time
  if (mountStatus.getVersionMajor() < 10) {
    if (!onStep.command(":GXFA#", temp)) strcpy(temp, "?%");
    strncpy(workLoadStr, temp, 20); workLoadStr[19] = 0; Y;
  }

  // wifi signal strength
  #if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
    long signal_strength_dbm = WiFi.RSSI();
    long signal_strength_qty = 2*(signal_strength_dbm + 100);
    if (signal_strength_qty > 100) signal_strength_qty = 100; 
    else if (signal_strength_qty < 0) signal_strength_qty = 0;
    sprintf(temp, "%lddBm (%ld%%)", signal_strength_dbm, signal_strength_qty);
    strncpy(signalStrengthStr, temp, 20); signalStrengthStr[19] = 0; Y;
  #endif
}

void State::pollFast() {
}

State state;
