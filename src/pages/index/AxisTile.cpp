// -----------------------------------------------------------------------------------
// Axis tile
#include "AxisTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

bool decodeParameter(char* s, double *value, long *min, long *max, int *type, char *name);

// create the related webpage tile
bool axisTile(int axis, String &data)
{
  bool success = false;
  char temp[256] = "";
  char temp1[80] = "";
  double value;
  long min, max;
  int type;
  char name[40];

  if (axis <= 1 && status.mountFound != SD_TRUE) return success;
  if (axis == 2 && status.rotatorFound != SD_TRUE) return success;
  if (axis >= 3 && !status.focuserPresent[axis - 3]) return success;

  switch (axis) {
    case 0: sprintf(temp1, "Axis%d RA/Azm", 1); break;
    case 1: sprintf(temp1, "Axis%d Dec/Alt", 2); break;
    case 2: sprintf(temp1, "Axis%d Rotator", 3); break;
    case 3: case 4: case 5: case 6: case 7: case 8:
      sprintf(temp1, "Axis%d Focuser%d", axis + 1, axis - 2);
    break;
  }
  sprintf_P(temp, html_tile_text_beg, "22em", "11em", temp1);
  data.concat(temp);
  data.concat(F("<br /><hr>"));
  if (state.driverStatusStr[axis][0] == '?') strcpy(temp1, L_UNKNOWN); else strcpy(temp1, state.driverStatusStr[axis]);
  sprintf_P(temp, html_indexDriverStatus, axis, temp1);
  data.concat(temp);

  data.concat(F("<hr>"));
  www.sendContentAndClear(data);

  #if DRIVE_CONFIGURATION == ON
    if (status.getVersionMajor() > 10 || (status.getVersionMajor() == 10 && status.getVersionMinor() >= 26)) {

      sprintf_P(temp, html_collapsable_beg, L_SETTINGS "...");
      data.concat(temp);

      sprintf_P(temp, html_form_begin, "index.htm");
      data.concat(temp);
      www.sendContentAndClear(data);

      // get axis parameter count
      sprintf(temp, ":GXA%d,0#", axis + 1);
      if (onStep.command(temp, temp1)) {
        int parameterCount = atoi(temp1);

        // show controls
        for (int parameterNumber = 1; parameterNumber <= parameterCount; parameterNumber++) {
          sprintf(temp, ":GXA%d,%d#", axis + 1, parameterNumber);
          if (onStep.command(temp, temp1)) {
            decodeParameter(temp1, &value, &min, &max, &type, name);

            // name lookup, for locale specific strings
            if (name[0] == '$') {
              int i = atoi(&name[1]);
              if (i >= 1 && i <= AXPN_COUNT) {
                switch (i) {
                  case 1: strcpy_P(name, html_axpn_1); break;
                  case 2: strcpy_P(name, html_axpn_2); break;
                  case 3: strcpy_P(name, html_axpn_3); break;
                  case 4: strcpy_P(name, html_axpn_4); break;
                  case 5: strcpy_P(name, html_axpn_5); break;
                  case 6: strcpy_P(name, html_axpn_6); break;
                  case 7: strcpy_P(name, html_axpn_7); break;
                  case 8: strcpy_P(name, html_axpn_8); break;
                  case 9: strcpy_P(name, html_axpn_9); break;
                  case 10: strcpy_P(name, html_axpn_10); break;
                  case 11: strcpy_P(name, html_axpn_11); break;
                  case 12: strcpy_P(name, html_axpn_12); break;
                  case 13: strcpy_P(name, html_axpn_13); break;
                  case 14: strcpy_P(name, html_axpn_14); break;
                  case 15: strcpy_P(name, html_axpn_15); break;
                  case 16: strcpy_P(name, html_axpn_16); break;
                  case 17: strcpy_P(name, html_axpn_17); break;
                  case 18: strcpy_P(name, html_axpn_18); break;
                  case 19: strcpy_P(name, html_axpn_19); break;
                  case 20: strcpy_P(name, html_axpn_20); break;
                  case 21: strcpy_P(name, html_axpn_21); break;
                  case 22: strcpy_P(name, html_axpn_22); break;
                  case 23: strcpy_P(name, html_axpn_23); break;
                  case 24: strcpy_P(name, html_axpn_24); break;
                  case 25: strcpy_P(name, html_axpn_25); break;
                  case 26: strcpy_P(name, html_axpn_26); break;
                }
              }

              // if element 7 (reverse) is present, which it always is, show the Motor/Driver identification string
              if (i == 7) {
                data.concat("<br />" L_ADV_MOTOR ": ");
                sprintf(temp, ":GXA%d,M#", axis + 1);
                if (onStep.command(temp, temp1)) {
                  data.concat(temp1);
                } else {
                  data.concat("?");
                }
                data.concat("<br />");
              }
            }

            if (type == 2 || type == 4 || type == 6) strcat(name, " (<i>i</i>)");

            long valueInt = roundl(value);
            char valueStr[24];
            dtostrf(value, 1, 3, valueStr);
            convert.stripNumericStr(valueStr, true);

            switch (type) {
              // AXP_BOOLEAN, AXP_BOOLEAN_IMMEDIATE
              case 1: case 2: {
                sprintf_P(temp, html_configAxisSelectStart, axis + 1, parameterNumber);
                data.concat(temp);
                int selection[4] = {-1, -2, 0, 1};
                const char *selectionName[5] = {L_OFF, L_ON, L_OFF, L_ON};
                for (int i = 0; i < 4; i++) {
                  if (selection[i] >= min && selection[i] <= max) {
                    sprintf_P(temp, valueInt == selection[i] ? html_configAxisSelectOptionSelected : html_configAxisSelectOption, selection[i], selectionName[i]);
                    data.concat(temp);
                    www.sendContentAndClear(data);
                  }
                }
                sprintf_P(temp, html_configAxisSelectEnd, name);
                data.concat(temp);
              } break;

              // AXP_INTEGER, AXP_INTEGER_IMMEDIATE
              case 3: case 4:
                sprintf_P(temp, html_configAxisInt, valueInt, axis + 1, parameterNumber, min, max, name);
                data.concat(temp);
              break;

              // AXP_FLOAT, AXP_FLOAT_IMMEDIATE
              case 5: case 6:
                sprintf_P(temp, html_configAxisFloat, valueStr, axis + 1, parameterNumber, min, max, name);
                data.concat(temp);
              break;

              // AXP_POW2
              case 9: {
                sprintf_P(temp, html_configAxisSelectStart, axis + 1, parameterNumber);
                data.concat(temp);
                int selection[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
                const char *selectionName[9] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};
                for (int i = 0; i < 9; i++) {
                  if (selection[i] >= min && selection[i] <= max) {
                    sprintf_P(temp, valueInt == selection[i] ? html_configAxisSelectOptionSelected : html_configAxisSelectOption, selection[i], selectionName[i]);
                    data.concat(temp);
                    www.sendContentAndClear(data);
                  }
                }
                sprintf_P(temp, html_configAxisSelectEnd, name);
                data.concat(temp);
              } break;

              // AXP_DECAY
              case 10: {
                sprintf_P(temp, html_configAxisSelectStart, axis + 1, parameterNumber);
                data.concat(temp);
                int selection[5] = {1, 2, 3, 4, 5};
                const char *selectionName[5] = {L_ADV_DECAY_MIXED, L_ADV_DECAY_FAST, L_ADV_DECAY_SLOW, L_ADV_DECAY_SPREADCYCLE, L_ADV_DECAY_STEALTHCHOP};
                for (int i = 0; i < 5; i++) {
                  if (selection[i] >= min && selection[i] <= max) {
                    sprintf_P(temp, valueInt == selection[i] ? html_configAxisSelectOptionSelected : html_configAxisSelectOption, selection[i], selectionName[i]);
                    data.concat(temp);
                    www.sendContentAndClear(data);
                  }
                }
                sprintf_P(temp, html_configAxisSelectEnd, name);
                data.concat(temp);
              } break;
              
            }
          }
        }

        data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button> "));
        www.sendContentAndClear(data);
        sprintf_P(temp, html_configAxisRevert, axis + 1);
        data.concat(temp);
        www.sendContentAndClear(data);

        success = true;
      }

      if (!success) {
        data.concat(L_ADV_SET_AXIS_NO_EDIT "<br />");
      }

      data.concat(FPSTR(html_form_end));

      data.concat(FPSTR(html_collapsable_end));

      www.sendContentAndClear(data);
    }
  #endif

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);

  return success;
}

// use Ajax key/value pairs to pass related data to the web client in the background
void axisTileAjax(int axis, String &data)
{
  char temp[80], temp1[80];

  sprintf(temp, "dvr_stat%d", axis);
  if (state.driverStatusStr[axis][0] == '?') strcpy(temp1, L_UNKNOWN); else strcpy(temp1, state.driverStatusStr[axis]);
  data.concat(keyValueString(temp, temp1));

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void axisTileGet()
{
  if (status.getVersionMajor() < 10 || (status.getVersionMajor() == 10 && status.getVersionMinor() < 26)) return;

  char command[80] = "";
  char response[80] = "";
  char argStr[20];

  #if DRIVE_CONFIGURATION == ON
 
    String ssr = www.arg("revert");
    if (!ssr.equals(EmptyStr)) {
      int axis = ssr.toInt();
      if (axis == 0) {
        strcpy(command, ":SXEM,0#");
        onStep.commandBool(command);
      } else
      if (axis >= 1 && axis <= 9) {
        sprintf(command, ":SXA%d,R#",axis);
        onStep.commandBool(command);
      }
      return;
    }

    // determine what axis is being set
    int axisNumber = 0;
    for (int i = 1; i < 9; i++) {
      sprintf(argStr, "a%dp%dvalue", i, 1);
      if (!www.arg(argStr).equals(EmptyStr)) {
        axisNumber = i;
        break;
      }
    }
    if (axisNumber == 0) return;

    // get axis parameter count
    sprintf(command, ":GXA%d,0#", axisNumber);
    if (!onStep.command(command, response)) return;
    int parameterCount = atoi(response);

    // send axis parameters to OnStepX
    for (int parameterNumber = 1; parameterNumber <= parameterCount; parameterNumber++) {
      sprintf(argStr, "a%dp%dvalue", axisNumber, parameterNumber);
      String parameterValue = www.arg(argStr);
      sprintf(command, ":SXA%d,%d,%s#", axisNumber, parameterNumber, parameterValue.c_str());
      onStep.commandBool(command);
    }
  #endif
}

bool decodeParameter(char* s, double *value, long *min, long *max, int *type, char *name) {
  char *ws = s;
  char *conv_end;
  double f = strtod(ws, &conv_end); 
  if (&s[0] != conv_end) *value = f; else return false;
  ws = strchr(ws, ',');
  if (ws != NULL) {
    ws++;
    *min = atol(ws);
    ws = strchr(ws, ',');
    if (ws != NULL) {
      ws++;
      *max = atol(ws);
      ws = strchr(ws, ',');
      if (ws != NULL) {
        ws++;
        *type = atol(ws);
        ws = strchr(ws, ',');
        if (ws != NULL) {
          ws++;
          sstrcpy(name, ws, 18);
          return true;
        }
      }
    }
  }
  return false;
}
