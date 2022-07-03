// -----------------------------------------------------------------------------------
// Controller Status tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void statusTile(String &data);
extern void statusTileAjax(String &data);
extern void statusTileGet();

const char html_indexGeneralError[] PROGMEM = L_LAST_GENERAL_ERROR ": <span id='last_err' class='c'>%s</span>\n";

const char html_indexWorkload[] PROGMEM = "<br />" L_WORKLOAD ": <span id='work_load' class='c'>%s</span>\n";

#if DISPLAY_WIFI_SIGNAL_STRENGTH == ON
  const char html_indexSignalStrength[] PROGMEM = "<br />" L_WIRELESS_SIGNAL_STRENGTH ": <span id='signal' class='c'>%s</span>\n";
#endif

const char html_indexMountType[] PROGMEM =
  L_PAGE_MOUNT L_ADV_MOUNT_TYPE ":<br /><input style='width: 7em;' value='%d' type='number' name='mountt' min='1' max='3' step='1'>"
  "<br /><br />\n";

const char html_resetNotes[] PROGMEM =
  "Notes:<ul>"
  "<li>" L_RESET_MSG1 "</li>"
  "<li>" L_DOWN_MESSAGE1 "</li>"
  "</ul>\n";

