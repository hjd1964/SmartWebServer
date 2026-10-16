// -----------------------------------------------------------------------------------
// Limits and Backlash tile
#include "LimitsTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#include <cmath>
#include <stdlib.h>

namespace {
const char syncLimitArgs[3][10] PROGMEM = {"syncPolar", "syncAxis1", "syncAxis2"};
const uint8_t syncLimitMin[] = {60, 0, 0};
const uint8_t syncLimitMax[] = {90, 30, 30};

// Runtime limits use degrees: polar 90 and axis 0 disable the respective limit.
bool parseSyncLimit(int index, const char* text, double &value) {
  char* end;
  value = strtod(text, &end);
  if (end == text || !std::isfinite(value)) return false;
  return *end == 0 && value >= syncLimitMin[index] && value <= syncLimitMax[index];
}

bool readSyncLimit(int index, double &value) {
  if (!status.hasSyncLimits()) return false;
  char command[16], reply[120] = "";
  snprintf_P(command, sizeof(command), PSTR(":GXE%c#"), 'H' + index);
  return onStep.command(command, reply) && parseSyncLimit(index, reply, value);
}

void formatSyncLimit(double value, char* valueText, size_t valueTextSize) {
  snprintf_P(valueText, valueTextSize, PSTR("%.3f"), value == 0.0 ? 0.0 : value);
  char* end = valueText + strlen(valueText) - 1;
  while (*end == '0') *end-- = 0;
  if (*end == '.') *end = 0;
}

void showSyncLimit(String &data, int index, bool supported, const char* valueText) {
  char temp[240];
  char arg[10];
  strcpy_P(arg, syncLimitArgs[index]);
  if (supported) snprintf_P(temp, sizeof(temp), html_syncLimitInput, arg, arg, syncLimitMin[index], syncLimitMax[index], valueText);
  else snprintf_P(temp, sizeof(temp), html_syncLimitUnavailable, arg);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_syncLimitLabel, arg);
  data.concat(temp);
  if (index == 0) data.concat(F(L_SYNC_LIMIT_POLAR));
  else if (index == 1) data.concat(F(L_SYNC_LIMIT_AXIS1));
  else data.concat(F(L_SYNC_LIMIT_AXIS2));
  data.concat(F("</label><br />"));
  www.sendContentAndClear(data);
}
}

// create the related webpage tile
void limitsTile(String &data)
{
  char temp[240] = "";
  char reply[120] = "";

  snprintf_P(temp, sizeof(temp), html_tile_text_beg, "22em", "15em", "Backlash and Limits");
  data.concat(temp);

  data.concat(F("<br /><hr>"));

  // get backlash and limits
  if (!onStep.command(":%BR#", reply)) strcpy(reply, "0");
  int backlashAxis1 = (int)strtol(&reply[0], NULL, 10);

  if (!onStep.command(":%BD#", reply)) strcpy(reply, "0");
  int backlashAxis2 = (int)strtol(&reply[0], NULL, 10);

   if (!onStep.command(":Gh#", reply)) strcpy(reply, "0");
  int minAlt = (int)strtol(&reply[0], NULL, 10);

  if (!onStep.command(":Go#", reply)) strcpy(reply, "0");
  int maxAlt = (int)strtol(&reply[0], NULL, 10);

  if (!onStep.command(":GXE9#", reply)) strcpy(reply, "0");
  int degPastMerE = (int)strtol(&reply[0], NULL, 10);
  degPastMerE = round((degPastMerE * 15.0) / 60.0);

  if (!onStep.command(":GXEA#", reply)) strcpy(reply, "0");
  int degPastMerW = (int)strtol(&reply[0], NULL, 10);
  degPastMerW = round((degPastMerW * 15.0) / 60.0);

  // Share the mount values between the summary and settings controls.
  char syncLimitValues[3][16];
  bool syncLimitSupported[3] = {false, false, false};
  if (status.hasSyncLimits()) {
    for (int i = 0; i < 3; i++) {
      double value;
      syncLimitSupported[i] = readSyncLimit(i, value);
      if (syncLimitSupported[i]) formatSyncLimit(value, syncLimitValues[i], sizeof(syncLimitValues[i]));
      else strcpy_P(syncLimitValues[i], PSTR("?"));
    }
  }

  // show values
  snprintf(temp, sizeof(temp), "Backlash, Axis1 <span id='limBl1' class='c'>%d</span> arc-sec<br />", backlashAxis1);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "Backlash, Axis2 <span id='limBl2' class='c'>%d</span> arc-sec<br /><br />", backlashAxis2);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "Limit Horizon, min altitude <span id='limAltMin' class='c'>%d</span>&deg;<br />", minAlt);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "Limit Overhead, max altitude <span id='limAltMax' class='c'>%d</span>&deg;<br />", maxAlt);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_limitsMeridianSummary, degPastMerE, degPastMerW);
  data.concat(temp);
  if (status.hasSyncLimits()) {
    snprintf_P(temp, sizeof(temp), html_limitsPolarSummary, syncLimitValues[0]);
    data.concat(temp);
    snprintf_P(temp, sizeof(temp), html_limitsOffsetSummary, syncLimitValues[1], syncLimitValues[2]);
    data.concat(temp);
  }

  data.concat(F("<hr>"));

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS "...");
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_form_begin, "mount.htm");
  data.concat(temp);

  // Backlash
  data.concat(F("Backlash:<br />"));

  snprintf_P(temp, sizeof(temp), html_configBlAxis1, backlashAxis1);
  data.concat(temp);
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_configBlAxis2, backlashAxis2);
  data.concat(temp);

  data.concat(F("<br />General Limits:<br />"));

  // Overhead and Horizon Limits
  snprintf_P(temp, sizeof(temp), html_configMinAlt, minAlt);
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_configMaxAlt, maxAlt);
  data.concat(temp);

  // Meridian Limits
  if (status.mountType == MT_GEM)
  {
    snprintf_P(temp, sizeof(temp), html_configPastMerE, degPastMerE);
    data.concat(temp);

    snprintf_P(temp, sizeof(temp), html_configPastMerW, degPastMerW);
    data.concat(temp);
  }

  if (status.hasSyncLimits()) {
    data.concat(F("<br />" L_SYNC_LIMITS_TITLE ":<br />"));
    for (int i = 0; i < 3; i++) showSyncLimit(data, i, syncLimitSupported[i], syncLimitValues[i]);
  }

  data.concat(F("<br />" L_SYNC_LIMIT_POLAR_OFF));
  if (status.hasSyncLimits()) data.concat(F("; " L_SYNC_LIMIT_OFFSET_OFF));
  data.concat(F("<br /><br />\n"));
  data.concat(F("<button type='submit'>" L_UPLOAD "</button>\n"));

  data.concat(FPSTR(html_form_end));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void limitsTileAjax(String &data)
{
  UNUSED(data);
}

// pass related data back to OnStep
extern void limitsTileGet()
{
  String v;
  char temp[80];

  for (int i = 0; i < 3; i++) {
    if (!status.hasSyncLimits()) continue;
    char arg[10];
    strcpy_P(arg, syncLimitArgs[i]);
    v = www.arg(arg);
    if (v.equals(EmptyStr)) continue;
    double value;
    if (!parseSyncLimit(i, v.c_str(), value)) continue;
    double current;
    if (!readSyncLimit(i, current) || current == value) continue;
    // Preserve the validated decimal text rather than rounding small fractions to zero.
    String command = String(F(":SXE")) + char('H' + i) + ',' + v + '#';
    onStep.commandBool(command.c_str());
  }

  // Overhead limit
  v = www.arg("ol");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 60 && v.toInt() <= 90)
    {
      snprintf(temp, sizeof(temp), ":So%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Horizon limit
  v = www.arg("hl");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -30 && v.toInt() <= 30)
    {
      snprintf(temp, sizeof(temp), ":Sh%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Meridian limit E
  v = www.arg("el");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -270 && v.toInt() <= 270)
    {
      snprintf(temp, sizeof(temp), ":SXE9,%d#", (int16_t)round((v.toInt() * 60.0) / 15.0));
      onStep.commandBool(temp);
    }
  }

  // Meridian limit W
  v = www.arg("wl");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -270 && v.toInt() <= 270)
    {
      snprintf(temp, sizeof(temp), ":SXEA,%d#", (int16_t)round((v.toInt() * 60.0) / 15.0));
      onStep.commandBool(temp);
    }
  }

  // Backlash
  v = www.arg("b1");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 0 && v.toInt() <= 3600)
    {
      snprintf(temp, sizeof(temp), ":$BR%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  v = www.arg("b2");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 0 && v.toInt() <= 3600)
    {
      snprintf(temp, sizeof(temp), ":$BD%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }
}
