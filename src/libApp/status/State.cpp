// scan OnStep state in the background

#include <limits.h>
#include "State.h"
#include "Status.h"
#if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
  #include "../../lib/wifi/WifiManager.h"
#endif
#include "../../lib/tasks/OnTask.h"
#include "../../lib/convert/Convert.h"
#include "../../libApp/misc/Misc.h"
#include "../../libApp/cmd/Cmd.h"
#include "../../libApp/encoders/Encoders.h"
#include "../../locales/Locale.h"

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
  if (status.getVersionMajor() > 3) {
    if (!onStep.command(":GtH#", temp)) strcpy(temp, "?");
  } else {
    if (!onStep.command(":Gt#", temp)) strcpy(temp, "?");
  }
  strncpy(latitudeStr, temp, 9); latitudeStr[9] = 0; Y;
  convert.dmsToDouble(&latitude, latitudeStr, true);

  // Longitude
  if (status.getVersionMajor() > 3) {
    if (!onStep.command(":GgH#", temp)) strcpy(temp, "?");
  } else {
    if (!onStep.command(":Gg#", temp)) strcpy(temp, "?");
  }
  strncpy(longitudeStr, temp, 10); longitudeStr[10] = 0; Y;

  // Ambient conditions
  #if DISPLAY_WEATHER == ON
    if (!onStep.command(":GX9A#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(siteTemperatureStr, temp, 16); siteTemperatureStr[15] = 0; Y;
    if (!onStep.command(":GX9B#", temp)) strcpy(temp, "?"); else localePressure(temp);
    strncpy(sitePressureStr, temp, 16); sitePressureStr[15] = 0; Y;
    if (!onStep.command(":GX9C#", temp)) strcpy(temp, "?"); else localeHumidity(temp);
    strncpy(siteHumidityStr, temp, 16); siteHumidityStr[15] = 0; Y;
    if (!onStep.command(":GX9E#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(siteDewPointStr, temp, 16); siteDewPointStr[15] = 0; Y;
  #endif

  // Focuser/telescope temperature
  if (status.focuserFound) {
    if (!onStep.command(":Ft#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(telescopeTemperatureStr, temp, 16); telescopeTemperatureStr[15] = 0; Y;
  }

  // pier side
  if ((status.pierSide == PierSideFlipWE1) || (status.pierSide == PierSideFlipWE2) || (status.pierSide == PierSideFlipWE3)) strcpy(temp, L_MERIDIAN_FLIP_W_TO_E); else
  if ((status.pierSide == PierSideFlipEW1) || (status.pierSide == PierSideFlipEW2) || (status.pierSide == PierSideFlipEW3)) strcpy(temp, L_MERIDIAN_FLIP_E_TO_W); else
  if (status.pierSide == PierSideWest) strcpy(temp, L_WEST); else
  if (status.pierSide == PierSideEast) strcpy(temp, L_EAST); else
  if (status.pierSide == PierSideNone) strcpy(temp, L_NONE); else strcpy(temp, L_UNKNOWN);
  if (!status.valid) strcpy(temp, "?");
  strncpy(pierSideStr, temp, 10); pierSideStr[9] = 0; Y;

  // meridian flip
  if (status.meridianFlips) {
    strcpy(temp, "On");
    if (status.autoMeridianFlips) strcat(temp, ", " L_AUTO);
  } else strcpy(temp, "Off");
  if (!status.valid) strcpy(temp, "?");
  strncpy(meridianFlipStr, temp, 10); pierSideStr[9] = 0; Y;

  // polar align
  strcpy(alignLrStr, "?");
  strcpy(alignUdStr, "?");
  if (!isnan(latitude) && fabs(latitude) <= 89) {
    long ud = LONG_MIN;
    if (onStep.command(":GX02#", temp)) { ud = strtol(&temp[0], NULL, 10); if (latitude < 0) ud = -ud; }
    long lr = LONG_MIN;
    if (onStep.command(":GX03#", temp)) { lr = strtol(&temp[0], NULL, 10); lr = lr/cos(latitude/57.295); }

    if (ud != LONG_MIN && lr != LONG_MIN) {
      char units = '"';
      if (labs(ud) >= 300 || labs(lr) >= 300) { ud = ud/60; lr = lr/60; units = '\''; }

      char lr_s[12];
      if (lr >= 0) strcpy(lr_s, leftTri); else strcpy(lr_s, rightTri);
      char ud_s[12];
      if (ud >= 0) strcpy(ud_s, upTri); else strcpy(ud_s, downTri);

      sprintf_P(temp, "%s %ld%c", lr_s, labs(lr), units);
      strncpy(alignLrStr, temp, 16); alignLrStr[15] = 0;

      sprintf_P(temp, "%s %ld%c", ud_s, labs(ud), units);
      strncpy(alignUdStr, temp, 16); alignUdStr[15] = 0; Y;
    }
  }

  // align progress
  if (status.aligning && status.alignThisStar >= 0 && status.alignLastStar >= 0) {
    sprintf(temp, L_POINT " %d of %d\n", status.alignThisStar, status.alignLastStar);
  } else {
    if (status.alignThisStar > status.alignLastStar) strcpy(temp, L_COMPLETE "\n"); else strcpy(temp, L_INACTIVE "\n");
  }
  strncpy(alignProgress, temp, 16); alignProgress[15] = 0;

  // Park
  if (status.parked) strcpy(temp, L_PARKED); else strcpy(temp, L_NOT_PARKED);
  if (status.parking) strcpy(temp, L_PARKING); else
  if (status.parkFail) strcpy(temp, L_PARK_FAILED);
  if (status.atHome) strcat(temp, " (" L_AT_HOME ")");
  if (!status.valid) strcpy(temp, "?");
  strncpy(parkStr, temp, 40); parkStr[39] = 0; Y;

  // Tracking
  double r = 0;
  if (status.tracking) {
    if (onStep.command(":GT#", temp)) {
      r = atof(temp);
      sprintF(temp, "%5.3fHz", r);
    } else strcpy(temp, "?");
  } else strcpy(temp, L_OFF);
  if (status.inGoto) strcpy(temp, L_INGOTO);
  if (!status.valid) strcpy(temp, "?");
  trackingSidereal = fabs(r - 60.164) < 0.001;
  trackingLunar = fabs(r - 57.900) < 0.001; 
  trackingSolar = fabs(r - 60.000) < 0.001;
  trackingKing  = fabs(r - 60.136) < 0.001;
  
  strcpy(temp1, "(");
  if (status.ppsSync) strcat(temp1, L_PPS_SYNC ", ");
  if (status.rateCompensation == RC_REFR_RA)   strcat(temp1, L_REFR_COMP_RA ", ");
  if (status.rateCompensation == RC_REFR_BOTH) strcat(temp1, L_REFR_COMP_BOTH ", ");
  if (status.rateCompensation == RC_FULL_RA)   strcat(temp1, L_FULL_COMP_RA ", ");
  if (status.rateCompensation == RC_FULL_BOTH) strcat(temp1, L_FULL_COMP_BOTH ", ");
  if (!status.valid) strcpy(temp1, "?");
  if (temp1[strlen(temp1) - 2] == ',') { temp1[strlen(temp1) - 2] = 0; strcat(temp1, ")"); } else strcpy(temp1, "");
  strcat(temp, " "); strcat(temp, temp1);
  strncpy(trackStr, temp, 40); trackStr[39] = 0; Y;

  // Slew speed
  if (!onStep.command(":GX97#", temp)) strcpy(temp, "?"); else strcat(temp, "&deg;/s");
  strncpy(slewSpeedStr, temp, 16); slewSpeedStr[15] = 0; Y;

  // Driver status
  int numAxes = 2;
  if (status.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++) {
    if (status.driver[axis].valid) {
      strcpy(temp1, "");
      if (status.driver[axis].fault) strcat(temp1, L_DRIVER_FAULT "  ");
      if (status.driver[axis].communicationFailure) strcat(temp1, L_COMMS_FAILURE ", ");
      if (!status.driver[axis].communicationFailure) {
        if (status.driver[axis].standstill) strcat(temp1, L_STANDSTILL ", "); else {
          if (status.driver[axis].outputA.openLoad || status.driver[axis].outputB.openLoad) {
            strcat(temp1, L_OPEN_LOAD " ");
            if (status.driver[axis].outputA.openLoad) strcat(temp1,"A");
            if (status.driver[axis].outputB.openLoad) strcat(temp1,"B");
            strcat(temp1,", ");
          }
        }
        if (status.driver[axis].outputA.shortToGround || status.driver[axis].outputB.shortToGround) {
          strcat(temp1, L_SHORT_GND " ");
          if (status.driver[axis].outputA.shortToGround) strcat(temp1,"A");
          if (status.driver[axis].outputB.shortToGround) strcat(temp1,"B");
          strcat(temp1,", ");
        }
        if (status.driver[axis].overTemperature) strcat(temp1, L_SHUTDOWN_OVER " 150C, ");
        if (status.driver[axis].overTemperaturePreWarning) strcat(temp1, L_PRE_WARNING " &gt;120C, ");
      }
      if (strlen(temp1) > 2) temp1[strlen(temp1) - 2] = 0;
      if (strlen(temp1) == 0) strcpy(temp1, "Ok");

      char *thisDriverStatusStr = driverStatusStr[axis];
      strncpy(thisDriverStatusStr, temp, 40); thisDriverStatusStr[39] = 0; Y;
    }
  }

  // MCU Temperature
  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    if (!onStep.command(":GX9F#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(controllerTemperatureStr, temp, 16); controllerTemperatureStr[15] = 0; Y;
  #endif

  // General Error
  strcpy(temp, "");
  if (status.lastError != ERR_NONE) strcat(temp, "<font class=\"y\">"); 
  status.getLastErrorMessage(temp1);
  if (!status.valid) strcat(temp, "?"); else strcat(temp, temp1);
  if (status.lastError != ERR_NONE) strcat(temp, "</font>"); 
  strncpy(lastErrorStr, temp, 40); lastErrorStr[39] = 0; Y;

  // Loop time
  if (status.getVersionMajor() < 10) {
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

  // update mount status
  status.update(true);

}

void State::pollFast() {
  char temp[80], temp1[80];

  if (status.focuserFound) {
    // identify active focuser
    if (onStep.command(":FA#", temp)) focuserActive = atoi(temp); else focuserActive = 0; Y;

    // focuser position
    if (!onStep.command(":FG#", temp)) strcpy(temp, "?"); else strcat(temp, " microns"); Y;
    strncpy(focuserPositionStr, temp, 20); focuserPositionStr[19] = 0; Y;
  }

  // rotator position
  if (status.rotatorFound) {
    if (onStep.command(":rG#", temp1)) { temp1[4] = 0; strcpy(temp, temp1); strcat(temp, "&deg;"); strcat(temp, &temp1[5]); strcat(temp, "&#39;"); } else strcpy(temp, "?");
    strncpy(rotatorPositionStr, temp, 20); rotatorPositionStr[19] = 0; Y;
  }
}

State state;
