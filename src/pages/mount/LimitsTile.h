// -----------------------------------------------------------------------------------
// Limits and Backlash tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void limitsTile(String &data);
extern void limitsTileAjax(String &data);
extern void limitsTileGet();

// Misc
const char html_configFormBegin[] PROGMEM = "<div class='content'><br />\n<form method='get' action='/configuration.htm'>";
const char html_configFormEnd[] PROGMEM = "\n</form><br />\n</div><br />\n";

// Overhead and Horizon limits
const char html_configMinAlt[] PROGMEM =
  "<input value='%d' type='number' name='hl' min='-30' max='30'>&nbsp;" L_LIMITS_RANGE_HORIZON "<br />\n";

const char html_configMaxAlt[] PROGMEM =
  "<input value='%d' type='number' name='ol' min='60' max='90'>&nbsp;" L_LIMITS_RANGE_OVERHEAD "<br />\n";

const char html_configBlAxis1[] PROGMEM =
  "<input value='%d' type='number' name='b1' min='0' max='3600'>&nbsp;" L_BACKLASH_RANGE_AXIS1 "<br />\n";

const char html_configBlAxis2[] PROGMEM =
  "<input value='%d' type='number' name='b2' min='0' max='3600'>&nbsp;" L_BACKLASH_RANGE_AXIS2 "<br />\n";

const char html_configPastMerE[] PROGMEM =
  "<input value='%d' type='number' name='el' min='-270' max='270'>&nbsp;" L_LIMITS_RANGE_MERIDIAN_E "<br />\n";

const char html_configPastMerW[] PROGMEM =
  "<input value='%d' type='number' name='wl' min='-270' max='270'>&nbsp;" L_LIMITS_RANGE_MERIDIAN_W "<br />\n";

// Runtime sync limits
const char html_limitsMeridianSummary[] PROGMEM =
  "Limit Past Meridian, East <span id='limMerE' class='c'>%d</span>&deg;, West <span id='limMerW' class='c'>%d</span>&deg;<br />";
const char html_limitsPolarSummary[] PROGMEM =
  L_SYNC_LIMIT_SUMMARY_POLAR " <span id='limSyncPolar' class='c'>%s</span>&deg;<br />";
const char html_limitsOffsetSummary[] PROGMEM =
  L_SYNC_LIMIT_SUMMARY_OFFSET1 " <span id='limSync1' class='c'>%s</span>&deg;, "
  L_SYNC_LIMIT_SUMMARY_OFFSET2 " <span id='limSync2' class='c'>%s</span>&deg;<br />";
const char html_syncLimitLabel[] PROGMEM =
  "&nbsp;<label for='%s'>";
const char html_syncLimitUnavailable[] PROGMEM =
  "<input id='%s' type='number' disabled> ";
const char html_syncLimitInput[] PROGMEM =
  "<input id='%s' name='%s' type='number' min='%d' max='%d' step='any' required value='%s'>";

