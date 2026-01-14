// update controller state
#include "State.h"

#include "Status.h"
#include "../../lib/tasks/OnTask.h"
#include "../../libApp/cmd/Cmd.h"

bool State::updateAuxiliary(bool all, bool now) {
  if (!now && millis() - lastAuxPageLoadTime > 2000) return true;

  bool valid;

  // get feature status
  for (uint8_t i = 0; i < 8; i++) {
    char *value1_str = NULL;
    char *value2_str = NULL;
    char *value3_str = NULL;
    char *value4_str = NULL;
    char *valueV_str = NULL; // monitor voltage
    char *valueI_str = NULL; // monitor current
    char *valueS_str = NULL; // monitor status
    char cmd[20], out[80];

    if (all || (status.feature[i].purpose == SWITCH || status.feature[i].purpose == ANALOG_OUTPUT || status.feature[i].purpose == DEW_HEATER || status.feature[i].purpose == INTERVALOMETER)) {
      sprintf(cmd,":GXX%d#", i + 1);
      if (!onStep.command(cmd, out) || strlen(out) == 0) valid = false; else valid = true; Y;
      if (!valid) {
        status.feature[i].value1 = 0;
        status.feature[i].value2 = NAN;
        status.feature[i].value3 = NAN;
        status.feature[i].value4 = NAN;
        status.feature[i].voltage = NAN;
        status.feature[i].current = NAN;
        status.auxiliaryFound = SD_UNKNOWN;
        return false;
      }

      // for OnStepX version 10.27f and later when the power monitor is enabled
      valueV_str = strstr(out, ";");
      if (valueV_str) {
        valueV_str[0] = 0;
        valueV_str++;
        valueI_str = strstr(valueV_str, ",");
        if (valueI_str) {
          valueI_str[0] = 0;
          valueI_str++;
          valueS_str = strstr(valueI_str, ",");
          if (valueS_str) {
            valueS_str[0] = 0;
            valueS_str++;
          }
        }

        if (valueV_str) status.feature[i].voltage = atof(valueV_str); else status.feature[i].voltage = NAN;
        if (valueI_str) status.feature[i].current = atof(valueI_str); else status.feature[i].current = NAN;
      }

      // normal auxiliary feature state data
      value2_str = strstr(out, ",");
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
        value1_str = out;
        if (value1_str) {
          if (value1_str) status.feature[i].value1 = atoi(value1_str); else status.feature[i].value1 = 0;
          if (value2_str) status.feature[i].value2 = atof(value2_str); else status.feature[i].value2 = NAN;
          if (value3_str) status.feature[i].value3 = atof(value3_str); else status.feature[i].value3 = NAN;
          if (value4_str) status.feature[i].value4 = atof(value4_str); else status.feature[i].value4 = NAN;
        }
      }

    }
  }
  return true;
}
