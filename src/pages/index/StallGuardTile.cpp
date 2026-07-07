// -----------------------------------------------------------------------------------
// StallGuard monitor tile
#include "StallGuardTile.h"

#if DISPLAY_STALLGUARD_MONITOR == ON

#include "../KeyValue.h"
#include "../Pages.common.h"

int _stallguard_axis = 0;

// create the related webpage tile
void stallGuardTile(String &data)
{
  char temp[240] = "";

  data.concat(FPSTR(html_stallGuardScript));
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_tile_beg, "28em", "13em", "StallGuard <span class='c'>Axis</span><span id='sgA' class='c'>?</span> Monitor");
  data.concat(temp);
  data.concat(F("<div style='float: right; text-align: right;'>"));
  data.concat(F("SG <span id='sgV' class='c'>?</span>, "));
  data.concat(F("Trip <span id='sgT' class='c'>?</span>"));
  data.concat(F("</div><br /><hr>"));

  data.concat(F("<div>"));
  snprintf_P(temp, sizeof(temp), html_stallGuardSelect, 0, 0, 'x');
  data.concat(temp);
  for (int i = 1; i <= 9; i++) {
    char command[12];
    snprintf(command, sizeof(command), ":GXSG%d#", i);
    if (onStep.command(command, temp)) {
      snprintf_P(temp, sizeof(temp), html_stallGuardSelect, i, i, '0' + i);
      data.concat(temp);
    }
  }
  data.concat(F("</div><br />"));

  data.concat(F("Bad <span id='sgB' class='c'>?</span> ms, "));
  data.concat(F("<span id='sgS' class='c'>?</span>"));
  data.concat(F("<br />"));
  data.concat(FPSTR(html_stallGuardGraph));

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void stallGuardTileAjax(String &data)
{
  char temp[80] = "", command[12];

  if (_stallguard_axis == 0) strcpy(temp, "sgA|?\n"); else snprintf(temp, sizeof(temp), "sgA|%d\n", _stallguard_axis);
  data.concat(temp);

  snprintf(command, sizeof(command), ":GXSG%d#", _stallguard_axis);
  if (_stallguard_axis >= 1 && _stallguard_axis <= 9 && onStep.command(command, temp)) {
    int sg, trip, badMs, armed, latched;
    if (sscanf(temp, "%d,%d,%d,%d,%d", &sg, &trip, &badMs, &armed, &latched) == 5) {
      snprintf(temp, sizeof(temp), "sgV|%d\n", sg); data.concat(temp);
      snprintf(temp, sizeof(temp), "sgT|%d\n", trip); data.concat(temp);
      snprintf(temp, sizeof(temp), "sgB|%d\n", badMs); data.concat(temp);
      data.concat(F("sgS|"));
      data.concat(latched ? F("Latched") : armed ? F("Armed") : F("Idle"));
      data.concat('\n');
    } else {
      data.concat(F("sgV|?\nsgT|?\nsgB|?\nsgS|?\n"));
    }
  } else {
    data.concat(F("sgV|?\nsgT|?\nsgB|?\nsgS|?\n"));
    _stallguard_axis = 0;
  }

  data.concat(keyValueBoolEnabled("sgax1", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax2", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax3", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax4", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax5", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax6", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax7", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax8", _stallguard_axis == 0));
  data.concat(keyValueBoolEnabled("sgax9", _stallguard_axis == 0));

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void stallGuardTileGet()
{
  String v = www.arg("sgax");
  if (!v.equals(EmptyStr)) {
    int axis = v.toInt();
    if (axis >= 0 && axis <= 9) _stallguard_axis = axis;
  }
}

#endif
