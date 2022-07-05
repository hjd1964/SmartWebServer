// -----------------------------------------------------------------------------------
// Mount status from OnStep

#include <Arduino.h>
#include "../../Constants.h"
#include "../../../Config.h"
#include "../../../Extended.config.h"
#include "../../lib/debug/Debug.h"
#include "../../lib/tasks/OnTask.h"

#include "../../locales/Locale.h"
#include "../cmd/Cmd.h"

#include "Status.h"

bool Status::update(bool all)
{
  char result[80] = "";
  if (!valid) {
    if (!onStep.command(":GVP#", result) || result[0] == 0 || !strstr(result, "On-Step")) { valid = false; return false; } Y;
    if (!onStep.command(":GVN#", result) || result[0] == 0 ) { valid = false; return false; } Y;
    strcpy(id, "OnStep");
    strcpy(ver, result);
    if (strlen(result) > 0) {
      ver_patch = result[strlen(result) - 1];
      result[strlen(result) - 1] = 0;
    }

    char *resultMinor = strchr(result, '.');
    if (resultMinor != NULL) {
      resultMinor[0] = 0;
      resultMinor++;
      ver_maj = atol(result);
      ver_min = atol(resultMinor);
    }

    if (ver_maj < 0 || ver_maj > 99 || ver_min < 0 || ver_min > 99 || ver_patch < 'a' || ver_patch > 'z') {
      ver_maj = -1;
      ver_min = -1;
      ver_patch = 0;
      valid = false;
      return false;
    }
  }

  if (!onStep.command(":GU#", result) || result[0] == 0) { mountFound = false; valid = false; return false; } else mountFound = true; Y;

  if (mountFound) {
    tracking = false;
    inGoto = false;
    if (!strstr(result, "N")) inGoto = true; else tracking = !strstr(result, "n");

    parked      = strstr(result, "P");
    if (strstr(result, "p")) parked = false;
    parking     = strstr(result, "I");
    parkFail    = strstr(result, "F");

    pecRecorded = strstr(result, "R");
    pecIgnore   = strstr(result, "/");
    pecReadyPlay= strstr(result, ",");
    pecPlaying  = strstr(result, "~");
    pecReadyRec = strstr(result, ";");
    pecRecording= strstr(result, "^");
    if (!pecRecording && !pecReadyRec && !pecPlaying && !pecReadyPlay && !pecIgnore && !pecRecorded) pecEnabled = false; else pecEnabled = true;

    syncToEncodersOnly = strstr(result, "e");
    atHome      = strstr(result, "H");
    homing      = strstr(result, "h");
    ppsSync     = strstr(result, "S");
    pulseGuiding= strstr(result, "G");
    guiding     = strstr(result, "g");
    if (pulseGuiding) guiding = true;
    axisFault   = strstr(result, "f");

    if (strstr(result, "r")) { if (strstr(result, "s")) rateCompensation = RC_REFR_RA; else rateCompensation = RC_REFR_BOTH; } else
    if (strstr(result, "t")) { if (strstr(result, "s")) rateCompensation = RC_FULL_RA; else rateCompensation = RC_FULL_BOTH; } else rateCompensation = RC_NONE;

    waitingHome   = strstr(result, "w");
    pauseAtHome   = strstr(result, "u");
    buzzerEnabled = strstr(result, "z");

    if (strstr(result,"E")) mountType = MT_GEM; else
    if (strstr(result,"K")) mountType = MT_FORK; else
    if (strstr(result,"k")) mountType = MT_FORKALT; else
    if (strstr(result,"A")) mountType = MT_ALTAZM; else mountType = MT_UNKNOWN;

    if (mountType == MT_GEM) autoMeridianFlips = strstr(result, "a"); else autoMeridianFlips = false;

    guideRatePulse = result[strlen(result) - 3] - '0';
    if (guideRatePulse < 0) guideRatePulse = 0;
    if (guideRatePulse > 9) guideRatePulse = 9;
    guideRate = result[strlen(result) - 2] - '0';
    if (guideRate < 0) guideRate = 0;
    if (guideRate > 9) guideRate = 9;

    lastError = (Errors)(result[strlen(result) - 1] - '0');

    // get meridian status
    if (!onStep.command(":GX94#", result) || result[0] == 0) { valid = false; return false; } Y;
    meridianFlips = !strstr(result, "N");
    pierSide = strtol(&result[0], NULL, 10);

    // align status
    if (onStep.command(":A?#", result) && strlen(result) == 3) {
      if (result[0] >= '0' && result[0] <= '9') alignMaxStars = result[0] - '0';
      if (result[1] >= '0' && result[1] <= '9') alignThisStar = result[1] - '0';
      if (result[2] >= '0' && result[2] <= '9') alignLastStar = result[2] - '0';
    } else {
      alignMaxStars = 0;
      alignThisStar = 0;
      alignLastStar = 0;
    }
    if (alignThisStar != 0 && alignThisStar <= alignLastStar) aligning = true; else aligning = false;
    Y;
  }

  focuserScan();
  rotatorScan();
  auxiliaryFeatureScan();
  auxiliaryFeatureUpdate();
  axisStatusUpdate();

  valid = true;
  return true;
}

void Status::focuserScan() {
  static bool scanned = false;
  if (!scanned) {
    focuserFound = false;
    focuserCount = 0;
    if (getVersionMajor() >= 10) {
      if (onStep.commandBool(":F1a#")) { focuserPresent[0] = true; focuserCount++; } Y;
      if (onStep.commandBool(":F2a#")) { focuserPresent[1] = true; focuserCount++; } Y;
      if (onStep.commandBool(":F3a#")) { focuserPresent[2] = true; focuserCount++; } Y;
      if (onStep.commandBool(":F4a#")) { focuserPresent[3] = true; focuserCount++; } Y;
      if (onStep.commandBool(":F5a#")) { focuserPresent[4] = true; focuserCount++; } Y;
      if (onStep.commandBool(":F6a#")) { focuserPresent[5] = true; focuserCount++; } Y;
    } else {
      if (onStep.commandBool(":FA#")) { focuserPresent[0] = true; focuserCount++; } Y;
      if (onStep.commandBool(":fA#")) { focuserPresent[1] = true; focuserCount++; } Y;
    }
    if (focuserCount > 0) focuserFound = true;
    scanned = true;
  }
}

void Status::rotatorScan() {
  static bool scanned = false;
  if (!scanned) {
    char temp[80];
    rotatorFound = false;
    derotatorFound = false;
    if (onStep.command(":GX98#", temp)) {
      if (temp[0] == 'R') { rotatorFound = true; derotatorFound = false; }
      if (temp[0] == 'D') { rotatorFound = true; derotatorFound = true; }
    } Y;
    scanned = true;
  }
}

bool Status::auxiliaryFeatureScan() {
  static bool scanned = false;
  bool valid;
  char cmd[40], out[40], present[40];

  if (scanned) return true;

  // check which feature #'s are present
  if (!onStep.command(":GXY0#", present) || present[0] == 0 || strlen(present) != 8) valid = false; else valid = true; Y;

  // try to get the AF presense twice before giving up
  if (!valid) {
    if (!onStep.command(":GXY0#", present) || present[0] == 0 || strlen(present) != 8) valid = false; else valid = true; Y;
    if (!valid) { for (uint8_t j = 0; j < 8; j++) feature[j].purpose = 0; auxiliaryFound = false; scanned = true; return false; }
  }

  // get feature status
  for (uint8_t i = 0; i < 8; i++) {
    char *purpose_str = NULL;

    if (present[i] == '0') continue;

    sprintf(cmd, ":GXY%d#", i+1);
    if (!onStep.command(cmd, out) || out[0] == 0) valid = false; Y;
    if (!valid) { for (uint8_t j = 0; j < 8; j++) feature[j].purpose = 0; auxiliaryFound = false; return false; }

    if (strlen(out) > 1) {
      purpose_str = strstr(out,",");
      if (purpose_str) {
        purpose_str[0] = 0;
        purpose_str++;
      } else valid = false;
      char *name_str = out; if (!name_str) valid = false;
      if (!valid) { for (uint8_t j = 0; j < 8; j++) feature[j].purpose = 0; auxiliaryFound = false; return false; }

      if (strlen(name_str) > 10) name_str[11] = 0;
      strcpy(feature[i].name, name_str);
      if (purpose_str) feature[i].purpose = atoi(purpose_str);
      VF("MSG: AuxFeature, found "); V(name_str); V(" purpose "); VL(feature[i].purpose);

      auxiliaryFound = true;
    }
  }
  scanned = true;
  return true;
}

bool Status::auxiliaryFeatureUpdate(bool all) {
  bool valid;

  // get feature status
  for (uint8_t i = 0; i < 8; i++) {
    char *value1_str = NULL;
    char *value2_str = NULL;
    char *value3_str = NULL;
    char *value4_str = NULL;
    char cmd[40], out[40];

    if (all || (feature[i].purpose == SWITCH || feature[i].purpose == ANALOG_OUTPUT || feature[i].purpose == DEW_HEATER || feature[i].purpose == INTERVALOMETER)) {
      sprintf(cmd,":GXX%d#", i + 1);
      if (!onStep.command(cmd, out) || strlen(out) == 0) valid = false; else valid = true; Y;
      if (!valid) { for (uint8_t j = 0; j < 8; j++) feature[j].purpose = 0; return false; }

      value2_str = strstr(out,",");
      if (value2_str) {
        value2_str[0] = 0;
        value2_str++;
        value3_str = strstr(value2_str, ",");
        if (value3_str) {
          value3_str[0] = 0;
          value3_str++;
          value4_str = strstr(value3_str, ",");
          if (value4_str) {
            value4_str[0] = 0;
            value4_str++;
          }
        }
      }
      value1_str = out;
      if (!value1_str) valid = false;

      if (valid) {
        if (value1_str) feature[i].value1 = atoi(value1_str);
        if (value2_str) feature[i].value2 = atof(value2_str);
        if (value3_str) feature[i].value3 = atof(value3_str);
        if (value4_str) feature[i].value4 = atof(value4_str);
        
      }
    }
  }
  return true;
}

// attempt to get the driver status for all 9 axes, mark the ones that return false so we don't attempt to process again
void Status::axisStatusUpdate() {
  static int driverStatusFailedAttempts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int maxAxes = 2;
  if (status.getVersionMajor() >= 10) maxAxes = 9;
  for (int axis = 0; axis < maxAxes; axis++) {
    if (driverStatusFailedAttempts[axis] < 3) {
      char cmd[40];
      char reply[40];
      sprintf(cmd, ":GXU%d#", axis + 1);
      if (onStep.command(cmd, reply) && reply[0] != '0') {
        driverStatusFailedAttempts[axis] = 0;
        driver[axis].valid = true;
        driver[axis].communicationFailure = strstr(reply, "ST,OA,OB,GA,GB,OT,PW");
        driver[axis].standstill = strstr(reply, "ST");
        driver[axis].outputA.openLoad = strstr(reply, "OA");
        driver[axis].outputB.openLoad = strstr(reply, "OB");
        driver[axis].outputA.shortToGround = strstr(reply, "GA");
        driver[axis].outputB.shortToGround = strstr(reply, "GB");
        driver[axis].overTemperature = strstr(reply, "OT");
        driver[axis].overTemperaturePreWarning = strstr(reply, "PW");
        driver[axis].fault = strstr(reply, "GF");
      } else {
        driverStatusFailedAttempts[axis]++;
        driver[axis].valid = false;
      }
      Y;
    }
  }
}

bool Status::getLastErrorMessage(char message[]) {
  strcpy(message,"");
  if (lastError == ERR_NONE) strcpy(message, L_GE_NONE); else
  if (lastError == ERR_MOTOR_FAULT) strcpy(message, L_GE_MOTOR_FAULT); else
  if (lastError == ERR_ALT_MIN) strcpy(message, L_GE_ALT_MIN); else
  if (lastError == ERR_LIMIT_SENSE) strcpy(message, L_GE_LIMIT_SENSE); else
  if (lastError == ERR_DEC) strcpy(message, L_GE_DEC); else
  if (lastError == ERR_AZM) strcpy(message, L_GE_AZM); else
  if (lastError == ERR_UNDER_POLE) strcpy(message, L_GE_UNDER_POLE); else
  if (lastError == ERR_MERIDIAN) strcpy(message, L_GE_MERIDIAN); else
  if (lastError == ERR_SYNC) strcpy(message, L_GE_SYNC); else
  if (lastError == ERR_PARK) strcpy(message, L_GE_PARK); else
  if (lastError == ERR_GOTO_SYNC) strcpy(message, L_GE_GOTO_SYNC); else
  if (lastError == ERR_UNSPECIFIED) strcpy(message, L_GE_UNSPECIFIED); else
  if (lastError == ERR_ALT_MAX) strcpy(message, L_GE_ALT_MAX); else
  if (lastError == ERR_WEATHER_INIT) strcpy(message, L_GE_WEATHER_INIT); else
  if (lastError == ERR_SITE_INIT) strcpy(message, L_GE_SITE_INIT); else
  if (lastError == ERR_NV_INIT) strcpy(message, L_GE_NV_INIT); else
  sprintf(message, L_GE_OTHER " %d", (int)lastError);
  return message[0];
}

Status status;
