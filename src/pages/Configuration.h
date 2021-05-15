// -----------------------------------------------------------------------------------
// Configuration
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

// Misc
const char html_configFormBegin[] PROGMEM = "<div class='content'><br />\r\n<form method='get' action='/configuration.htm'>";
const char html_configFormEnd[] PROGMEM = "\r\n</form><br />\r\n</div><br />\r\n";

// Site
const char html_configLongDeg[] PROGMEM =
"<input style='width: 4em;' value='%s' type='number' name='g1' min='-180' max='180'>&nbsp;&deg;&nbsp;";
const char html_configLongMin[] PROGMEM =
"<input style='width: 3.5em;' value='%s' type='number' name='g2' min='0' max='60'>&nbsp;'&nbsp;";
const char html_configLongSec[] PROGMEM =
"<input style='width: 3.5em;' value='%s' type='number' name='g3' min='0' max='60'>&nbsp;\"";
const char html_configLongMsg[] PROGMEM =
"&nbsp;&nbsp; &nbsp;" L_LOCATION_LONG "<br />\r\n";
const char html_configLatDeg[] PROGMEM =
"<input style='width: 4em;' value='%s' type='number' name='t1' min='-90' max='90'>&nbsp;&deg;&nbsp;";
const char html_configLatMin[] PROGMEM =
"<input style='width: 3.5em;' value='%s' type='number' name='t2' min='0' max='60'>&nbsp;'&nbsp;";
const char html_configLatSec[] PROGMEM =
"<input style='width: 3.5em;' value='%s' type='number' name='t3' min='0' max='60'>&nbsp;\"";
const char html_configLatMsg[] PROGMEM =
"&nbsp;&nbsp; &nbsp;" L_LOCATION_LAT "<br />\r\n";
const char html_configOffsetHrs[] PROGMEM =
"<input style='width: 4em;' value='%s' type='number' name='u1' min='-14' max='12'>&nbsp;h&nbsp;";
const char html_configOffsetMin[] PROGMEM =
"<select name='u2'><option value='0' %s>00</option><option value='30' %s>30</option><option value='45' %s>45</option></select>&nbsp;m&nbsp;"
"&nbsp;&nbsp;" L_LOCATION_RANGE_UTC_OFFSET "<br />" L_LOCATION_MESSAGE_UTC_OFFSET "<br /><br />";

// Overhead and Horizon limits
const char html_configMinAlt[] PROGMEM =
"<input value='%d' type='number' name='hl' min='-30' max='30'>&nbsp;" L_LIMITS_RANGE_HORIZON "<br />\r\n";
const char html_configMaxAlt[] PROGMEM =
"<input value='%d' type='number' name='ol' min='60' max='90'>&nbsp;" L_LIMITS_RANGE_OVERHEAD "<br /><br />";

// Axis1
const char html_configBlAxis1[] PROGMEM =
"<input value='%d' type='number' name='b1' min='0' max='3600'>&nbsp;" L_BACKLASH_RANGE_AXIS1 "<br />\r\n";
const char html_configPastMerE[] PROGMEM =
"<input value='%d' type='number' name='el' min='-270' max='270'>&nbsp;" L_LIMITS_RANGE_MERIDIAN_E "<br />\r\n";
const char html_configPastMerW[] PROGMEM =
"<input value='%d' type='number' name='wl' min='-270' max='270'>&nbsp;" L_LIMITS_RANGE_MERIDIAN_W "<br /><br />";

// Axis2
const char html_configBlAxis2[] PROGMEM =
"<input value='%d' type='number' name='b2' min='0' max='3600'>&nbsp;" L_BACKLASH_RANGE_AXIS2 "<br /><br />";

// Axis3
const char html_configBlAxis3[] PROGMEM =
"<input value='%d' type='number' name='b3' min='0' max='32767'>&nbsp;" L_BACKLASH_RANGE_AXIS345 "<br /><br />";

// Axis4
const char html_configBlAxis4[] PROGMEM =
"<input value='%d' type='number' name='b4' min='0' max='32767'>&nbsp;" L_BACKLASH_RANGE_AXIS345 "<br /><br />";
const char html_configTcfEnAxis4[] PROGMEM =
"<input value='%d' type='number' name='en4' min='0' max='1' step='1'>&nbsp;" L_TCF_COEF_EN_AXIS45 "<br />";
const char html_configDbAxis4[] PROGMEM =
"<input value='%d' type='number' name='d4' min='1' max='32767'>&nbsp;" L_DEADBAND_RANGE_AXIS45 "<br />";
const char html_configTcfCoefAxis4[] PROGMEM =
"<input value='%s' type='number' name='tc4' min='-999.0' max='999.0' step='any'>&nbsp;" L_TCF_COEF_RANGE_AXIS45 "<br /><br />";

// Axis5
const char html_configBlAxis5[] PROGMEM =
"<input value='%d' type='number' name='b5' min='0' max='32767'>&nbsp;" L_BACKLASH_RANGE_AXIS345 "<br /><br />";
const char html_configTcfEnAxis5[] PROGMEM =
"<input value='%d' type='number' name='en5' min='0' max='1' step='1'>&nbsp;" L_TCF_COEF_EN_AXIS45 "<br />";
const char html_configDbAxis5[] PROGMEM =
"<input value='%d' type='number' name='d5' min='1' max='32767'>&nbsp;" L_DEADBAND_RANGE_AXIS45 "<br />";
const char html_configTcfCoefAxis5[] PROGMEM =
"<input value='%s' type='number' name='tc5' min='-999.0' max='999.0' step='any'>&nbsp;" L_TCF_COEF_RANGE_AXIS45 "<br /><br />";

const char html_configAdvanced[] PROGMEM =
"<hr>" L_ADV_SET_TITLE " (<i>" L_ADV_SET_HEADER_MSG "</i>)<br /><br />";
const char html_configMountType[] PROGMEM =
" <input style='width: 7em;' value='%d' type='number' name='mountt' min='1' max='3' step='1'>&nbsp; " L_ADV_MOUNT_TYPE "<br /><br />\r\n";

#if DRIVE_CONFIGURATION == ON
  const char html_configAxisSpwr[] PROGMEM =
  " <input style='width: 7em;' value='%ld' type='number' name='a%dspwr' min='%d' max='%ld' step='1'>&nbsp; " L_ADV_SET_SPWR "<br />\r\n";
  const char html_configAxisSpd[] PROGMEM =
  " <input style='width: 7em;' value='%s' type='number' name='a%dspd' min='%d' max='%ld' step='any'>&nbsp; " L_ADV_SET_SPD "<br />\r\n";
  const char html_configAxisSpu[] PROGMEM =
  " <input style='width: 7em;' value='%s' type='number' name='a%dspu' min='0.01' max='10' step='any'>&nbsp; " L_ADV_SET_SPM "<br />\r\n";
  const char html_configAxisMicroSteps[] PROGMEM =
  " <input style='width: 7em;' value='%d' type='number' name='a%dustp' min='1' max='256' step='1'>&nbsp; " L_ADV_SET_us "<br />\r\n";
  const char html_configAxisCurrent[] PROGMEM =
  " <input style='width: 7em;' value='%d' type='number' name='a%dI' min='0' max='%d' step='1'>&nbsp; " L_ADV_SET_ma "<br />\r\n";
  const char html_configAxisReverse[] PROGMEM =
  " <input style='width: 7em;' value='%d' type='number' name='a%drev' min='0' max='1' step='1'>&nbsp; " L_ADV_SET_REV "<br />\r\n";
  const char html_configAxisMin[] PROGMEM =
  " <input style='width: 7em;' value='%d' type='number' name='a%dmin' min='%d' max='%d' step='1'>&nbsp;%s&nbsp; " L_ADV_SET_MIN "<br />\r\n";
  const char html_configAxisMax[] PROGMEM =
  " <input style='width: 7em;' value='%d' type='number' name='a%dmax' min='%d' max='%d' step='1'>&nbsp;%s&nbsp; " L_ADV_SET_MAX "<br /><br />\r\n";
  const char html_configAxesNotes[] PROGMEM =
  "<br />Notes:<ul>"
  "<li>" L_ADV_SET_FOOTER_MSG1 "</li>"
  "<li>" L_ADV_SET_FOOTER_MSG2 "</li>"
  #if DRIVE_MAIN_AXES_CURRENT == ON
    "<li>" L_ADV_SET_FOOTER_MSG3 "</li>"
  #endif
  "<li>" L_ADV_SET_FOOTER_MSG4 "</li>"
  "</ul>";
#endif

const char html_resetNotes[] PROGMEM =
"<br />Notes:<ul>"
"<li>" L_RESET_MSG1 "</li>"
"<li>" L_DOWN_MESSAGE1 "</li>"
"</ul>";
