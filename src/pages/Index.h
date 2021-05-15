// -----------------------------------------------------------------------------------
// The home page, status information
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#define leftTri  "&#x25c4;"
#define rightTri "&#x25ba;"
#define upTri    "&#x25b2;"
#define downTri  "&#x25bc;"

#if DISPLAY_ADVANCED_CHARS == ON
  #define Axis1 "&alpha;"
  #define Axis1A "&alpha;"
  #define Axis2 "&delta;"
#else
  #define Axis1 "RA"
  #define Axis1A "RA"
  #define Axis2 "DEC"
#endif

const char html_settingsBrowserTime[] PROGMEM =
"&nbsp;&nbsp;<span id=\"datetime\"></span> " L_TIME_BROWSER_UT
"<script> "
"function pad(num, size) { var s = '000000000' + num; return s.substr(s.length-size); }"
"var now = new Date(); document.getElementById('datetime').innerHTML = (now.getUTCMonth()+1)+'/'+"
"pad(now.getUTCDate().toString(),2)+'/'+pad(now.getUTCFullYear().toString().substr(-2),2)+"
"' '+pad(now.getUTCHours().toString(),2)+':'+pad(now.getUTCMinutes().toString(),2)+':'+pad(now.getUTCSeconds().toString(),2); "
"</script><br />\r\n";

const char html_indexDate[] PROGMEM = "&nbsp;&nbsp;<font class='c'>%s</font>";
const char html_indexTime[] PROGMEM = "&nbsp;<font class='c'>%s</font>&nbsp;" L_UT;
const char html_indexSidereal[] PROGMEM = "&nbsp;(<font class='c'>%s</font>&nbsp; " L_LST ")<br />";
const char html_indexSite[] PROGMEM =
"&nbsp;&nbsp;" L_LONG " <font class='c'>%s</font>, " L_LAT " <font class='c'>%s</font><br />";

const char html_indexPosition[] PROGMEM =
"&nbsp;&nbsp;" L_CURRENT ": " Axis1 " <font class='c'>%s</font>, " Axis2 " <font class='c'>%s</font><br />";
const char html_indexTarget[] PROGMEM =
"&nbsp;&nbsp;" L_TARGET ": " Axis1 " <font class='c'>%s</font>, " Axis2 " <font class='c'>%s</font><br />";
#if ENCODERS == ON
  const char html_indexEncoder1[] PROGMEM =
  "&nbsp;&nbsp;OnStep: Ax1 <font class='c'>%s</font>, Ax2 <font class='c'>%s</font><br />";
  const char html_indexEncoder2[] PROGMEM =
  "&nbsp;&nbsp;" L_ENCODER ": Ax1 <font class='c'>%s</font>, Ax2 <font class='c'>%s</font><br />";
#endif
const char html_indexPier[] PROGMEM =
"&nbsp;&nbsp;" L_PIER_SIDE ": <font class='c'>%s</font> (" L_MERIDIAN_FLIPS " <font class='c'>%s</font>)<br />";

const char html_indexCorPolar[] PROGMEM =
"&nbsp;&nbsp;%s <font class='c'>%ld</font>%c &nbsp; %s <font class='c'>%ld</font>%c &nbsp;(" L_ALIGN_MESSAGE " %s)<br />";

const char html_indexPark[] PROGMEM = "&nbsp;&nbsp;" L_PARKING ": <font class='c'>%s</font><br />";
const char html_indexTracking[] PROGMEM = "&nbsp;&nbsp;" L_TRACKING ": <font class='c'>%s %s</font><br />";
const char html_indexMaxRate[] PROGMEM =
"&nbsp;&nbsp;" L_CURRENT_MAXRATE ": <font class='c'>%ld</font> (" L_DEFAULT_MAXRATE ": <font class='c'>%ld</font>)<br />";
const char html_indexMaxSpeed[] PROGMEM = "&nbsp;&nbsp;" L_CURRENT_MAXSLEW ": <font class='c'>%s</font>&deg;/s<br />";

const char html_indexTPHD[] PROGMEM = "&nbsp;&nbsp;%s <font class='c'>%s</font>%s<br />";

const char html_indexDriverStatus[] PROGMEM = " " L_DRIVER_STATUS ": <font class='c'>%s</font><br />";
const char html_indexGeneralError[] PROGMEM = "&nbsp;&nbsp;" L_LAST_GENERAL_ERROR ": <font class='c'>%s</font><br />";
const char html_indexCmdErrorLog[] PROGMEM = "&nbsp;&nbsp;&nbsp;&nbsp;%s %s<br />";
const char html_indexWorkload[] PROGMEM = "&nbsp;&nbsp;" L_WORKLOAD ": <font class='c'>%s</font><br />";
#if DISPLAY_WIFI_SIGNAL_STRENGTH == ON
  const char html_indexSignalStrength[] PROGMEM = "&nbsp;&nbsp;" L_WIRELESS_SIGNAL_STRENGTH ": <font class=\"c\">%s</font><br />";
#endif
