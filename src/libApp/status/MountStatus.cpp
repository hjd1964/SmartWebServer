// -----------------------------------------------------------------------------------
// Mount status from OnStep

#include <Arduino.h>
#include "../../Constants.h"
#include "../../../Config.h"
#include "../../../Extended.config.h"
#include "../../lib/debug/Debug.h"

#include "../../locales/Locale.h"
#include "../cmd/Cmd.h"

#include "MountStatus.h"

bool MountStatus::update(bool all) {
  char result[80] = "";
  if (!_valid) {
    if (!onStep.command(":GVP#", result) || result[0] == 0 || (!strstr(result, "On-Step") && !strstr(result, "OnStepX"))) { _valid = false; return false; }
    if (!onStep.command(":GVN#", result) || result[0] == 0 ) { _valid = false; return false; }
    strcpy(_id, "OnStep");
    strcpy(_ver, result);
    if (strlen(result) > 0) {
      _ver_patch = result[strlen(result) - 1];
      result[strlen(result) - 1] = 0;
    }

    char *resultMinor = strchr(result, '.');
    if (resultMinor != NULL) {
      resultMinor[0] = 0;
      resultMinor++;
      _ver_maj = atol(result);
      _ver_min = atol(resultMinor);
    }

    if (_ver_maj < 0 || _ver_maj > 99 || _ver_min < 0 || _ver_min > 99 || _ver_patch < 'a' || _ver_patch > 'z') {
      _ver_maj = -1;
      _ver_min = -1;
      _ver_patch = 0;
      _valid = false;
      return false;
    }
  }

  if (!onStep.command(":GU#", result) || result[0] == 0) { _valid = false; return false; }

  _tracking = false; _inGoto = false;
  if (!strstr(result, "N")) _inGoto = true; else _tracking = !strstr(result, "n");

  _parked      = strstr(result, "P");
  if (strstr(result, "p")) _parked = false;
  _parking     = strstr(result, "I");
  _parkFail    = strstr(result, "F");

  _pecRecorded = strstr(result, "R");
  _pecIgnore   = strstr(result, "/");
  _pecReadyPlay= strstr(result, ",");
  _pecPlaying  = strstr(result, "~");
  _pecReadyRec = strstr(result, ";");
  _pecRecording= strstr(result, "^");
  if (!_pecRecording && !_pecReadyRec && !_pecPlaying && !_pecReadyPlay && !_pecIgnore && !_pecRecorded) _pecEnabled = false; else _pecEnabled = true;

  _toEncOnly   = strstr(result, "e");
  _atHome      = strstr(result, "H");
  _ppsSync     = strstr(result, "S");
  _pulseGuiding= strstr(result, "G");
  _guiding     = strstr(result, "g");
  if (_pulseGuiding) _guiding = true;
  _axisFault   = strstr(result, "f");

  if (strstr(result, "r")) { if (strstr(result, "s")) _rateCompensation = RC_REFR_RA; else _rateCompensation = RC_REFR_BOTH; } else
  if (strstr(result, "t")) { if (strstr(result, "s")) _rateCompensation = RC_FULL_RA; else _rateCompensation = RC_FULL_BOTH; } else _rateCompensation = RC_NONE;

  _waitingHome   = strstr(result, "w");
  _pauseAtHome   = strstr(result, "u");
  _buzzerEnabled = strstr(result, "z");

  if (strstr(result,"E")) _mountType = MT_GEM; else
  if (strstr(result,"K")) _mountType = MT_FORK; else
  if (strstr(result,"k")) _mountType = MT_FORKALT; else
  if (strstr(result,"A")) _mountType = MT_ALTAZM; else _mountType = MT_UNKNOWN;

  if (_mountType == MT_GEM) _autoMeridianFlips = strstr(result, "a"); else _autoMeridianFlips = false;

  _guideRatePulse = (Errors)(result[strlen(result) - 3] - '0');
  _guideRate = (Errors)(result[strlen(result) - 2] - '0');

  _lastError = (Errors)(result[strlen(result) - 1] - '0');

  // get a list of auxiliary features (runs once)
  featureScan();

  if (all) {
    // get meridian status
    if (!onStep.command(":GX94#", result) || result[0] == 0) { _valid = false; return false; }
    _meridianFlips = !strstr(result, "N");
    _pierSide = strtol(&result[0], NULL, 10);

    // update auxiliary features
    featureUpdate();

    // attempt to get the driver status for all 9 axes, mark the ones that return false so we don't attempt to process again
    static int driverStatusFailedAttempts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int maxAxes = 2;
    if (mountStatus.getVersionMajor() >= 10) maxAxes = 9;
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
      }
    }

    if (_alignMaxStars == -1) {
      _alignMaxStars = 3;
      if (!onStep.command(":A?#", result) || result[0] != 0) {
        if (result[0] > '0' && result[0] <= '9') _alignMaxStars = result[0] - '0';
      }
    }
  }

  _valid = true;
  return true;
}

bool MountStatus::getId(char id[]) { if (!_valid) return false; else { strcpy(id, _id); return true; } }
bool MountStatus::getVersionStr(char ver[]) { if (!_valid) return false; else { strcpy(ver, _ver); return true; } }
int  MountStatus::getVersionMajor() { return _ver_maj; }
int  MountStatus::getVersionMinor() { return _ver_min; }
char MountStatus::getVersionPatch() { return _ver_patch; }
bool MountStatus::valid() { return _valid; }
bool MountStatus::aligning() {
  char s[20] = "";
  if (onStep.command(":A?#", s) && strlen(s) == 3 && s[1] <= s[2] && s[1] != '0') return true; else return false;
}
bool MountStatus::tracking() { return _tracking; }
bool MountStatus::inGoto() { return _inGoto; }
bool MountStatus::parked() { return _parked; }
bool MountStatus::parking() { return _parking; }
bool MountStatus::parkFail() { return _parkFail; }
bool MountStatus::pecEnabled() { return _pecEnabled; }
bool MountStatus::pecIgnore() { return _pecIgnore; }
bool MountStatus::pecReadyPlay() { return _pecReadyPlay; }
bool MountStatus::pecPlaying() { return _pecPlaying; }
bool MountStatus::pecReadyRec() { return _pecReadyRec; }
bool MountStatus::pecRecorded() { return _pecRecorded; }
bool MountStatus::pecRecording() { return _pecRecording; }
bool MountStatus::syncToEncodersOnly() { return _toEncOnly; }
bool MountStatus::atHome() { return _atHome; }
bool MountStatus::ppsSync() { return _ppsSync; }
bool MountStatus::pulseGuiding() { return _pulseGuiding; }
bool MountStatus::guiding() { return _guiding; }
bool MountStatus::guideRate() { return _guideRate; }
bool MountStatus::guideRatePulse() { return _guideRatePulse; }
bool MountStatus::focuserPresent() {
  static int p = -1;
  if (p == -1) p = onStep.commandBool(":FA#");
  return p;
}

bool MountStatus::axisFault() { return _axisFault; }
bool MountStatus::waitingHome() { return _waitingHome; }
bool MountStatus::pauseAtHome() { return _pauseAtHome; }
bool MountStatus::buzzerEnabled() { return _buzzerEnabled; }
MountTypes MountStatus::mountType() { return _mountType; }
RateCompensation MountStatus::rateCompensation() { return _rateCompensation; }
bool MountStatus::meridianFlips() { return _meridianFlips; }
bool MountStatus::autoMeridianFlips() { return _autoMeridianFlips; }
byte MountStatus::pierSide() { return _pierSide; }
int MountStatus::alignMaxStars() { return _alignMaxStars; }

bool MountStatus::featureFound() { return _featureFound; }
void MountStatus::selectFeature(int f) { _featureSelected = f; }
char* MountStatus::featureName() { return _feature[_featureSelected].name; }
int MountStatus::featurePurpose() { return _feature[_featureSelected].purpose; }
int MountStatus::featureValue1() { return _feature[_featureSelected].value1; }
float MountStatus::featureValue2() { return _feature[_featureSelected].value2; }
float MountStatus::featureValue3() { return _feature[_featureSelected].value3; }
float MountStatus::featureValue4() { return _feature[_featureSelected].value4; }

bool MountStatus::featureScan() {
  static bool scanned = false;
  bool valid;
  char cmd[40], out[40], present[40];

  // check which feature #'s are present
  if (!onStep.command(":GXY0#", present) || present[0] == 0 || strlen(present) != 8) valid = false; else valid = true;
  if (!valid) { for (uint8_t j = 0; j < 8; j++) _feature[j].purpose = 0; _featureFound = false; return false; }

  // get feature status
  for (uint8_t i = 0; i < 8; i++) {
    char *purpose_str = NULL;

    if (present[i] == '0') continue;

    if (!scanned) {
      sprintf(cmd, ":GXY%d#", i+1);
      if (!onStep.command(cmd, out) || out[0] == 0) valid = false;
      if (!valid) { for (uint8_t j = 0; j < 8; j++) _feature[j].purpose = 0; _featureFound = false; return false; }

      if (strlen(out) > 1) {
        purpose_str = strstr(out,",");
        if (purpose_str) {
          purpose_str[0] = 0;
          purpose_str++;
        } else valid = false;
        char *name_str = out; if (!name_str) valid = false;
        if (!valid) { for (uint8_t j = 0; j < 8; j++) _feature[j].purpose = 0; _featureFound = false; return false; }

        if (strlen(name_str) > 10) name_str[11] = 0;
        strcpy(_feature[i].name, name_str);
        if (purpose_str) _feature[i].purpose = atoi(purpose_str);

        _featureFound = true;
      }
    }
  }
  scanned = true;
  return true;
}

bool MountStatus::featureUpdate(bool all) {
  bool valid;

  // get feature status
  for (uint8_t i = 0; i < 8; i++) {
    char *value1_str = NULL;
    char *value2_str = NULL;
    char *value3_str = NULL;
    char *value4_str = NULL;
    char cmd[40], out[40];

    if (all || (_feature[i].purpose == SWITCH || _feature[i].purpose == ANALOG_OUTPUT || _feature[i].purpose == DEW_HEATER || _feature[i].purpose == INTERVALOMETER)) {
      sprintf(cmd,":GXX%d#", i + 1);
      if (!onStep.command(cmd, out) || strlen(out) == 0) valid = false; else valid = true;
      if (!valid) { for (uint8_t j = 0; j < 8; j++) _feature[j].purpose = 0; return false; }

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
        if (value1_str) _feature[i].value1 = atoi(value1_str);
        if (value2_str) _feature[i].value2 = atof(value2_str);
        if (value3_str) _feature[i].value3 = atof(value3_str);
        if (value4_str) _feature[i].value4 = atof(value4_str);
      }
    }
  }
  return true;
}

Errors MountStatus::lastError() { return _lastError; }

bool MountStatus::getLastErrorMessage(char message[]) {
  strcpy(message,"");
  if (_lastError == ERR_NONE) strcpy(message, L_GE_NONE); else
  if (_lastError == ERR_MOTOR_FAULT) strcpy(message, L_GE_MOTOR_FAULT); else
  if (_lastError == ERR_ALT_MIN) strcpy(message, L_GE_ALT_MIN); else
  if (_lastError == ERR_LIMIT_SENSE) strcpy(message, L_GE_LIMIT_SENSE); else
  if (_lastError == ERR_DEC) strcpy(message, L_GE_DEC); else
  if (_lastError == ERR_AZM) strcpy(message, L_GE_AZM); else
  if (_lastError == ERR_UNDER_POLE) strcpy(message, L_GE_UNDER_POLE); else
  if (_lastError == ERR_MERIDIAN) strcpy(message, L_GE_MERIDIAN); else
  if (_lastError == ERR_SYNC) strcpy(message, L_GE_SYNC); else
  if (_lastError == ERR_PARK) strcpy(message, L_GE_PARK); else
  if (_lastError == ERR_GOTO_SYNC) strcpy(message, L_GE_GOTO_SYNC); else
  if (_lastError == ERR_UNSPECIFIED) strcpy(message, L_GE_UNSPECIFIED); else
  if (_lastError == ERR_ALT_MAX) strcpy(message, L_GE_ALT_MAX); else
  if (_lastError == ERR_WEATHER_INIT) strcpy(message, L_GE_WEATHER_INIT); else
  if (_lastError == ERR_SITE_INIT) strcpy(message, L_GE_SITE_INIT); else
  if (_lastError == ERR_NV_INIT) strcpy(message, L_GE_NV_INIT); else
  sprintf(message, L_GE_OTHER " %d", (int)_lastError);
  return message[0];
}

MountStatus mountStatus;
