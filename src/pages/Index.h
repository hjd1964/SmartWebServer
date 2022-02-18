// -----------------------------------------------------------------------------------
// The home page, status information
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

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
"<script>update_date_time();</script>"
"<br />\n";

const char html_indexDate[] PROGMEM = "&nbsp;&nbsp;<span id='date_ut' class='c'>%s</span>";
const char html_indexTime[] PROGMEM = "&nbsp;<span id='time_ut' class='c'>%s</span>&nbsp;" L_UT;
const char html_indexSidereal[] PROGMEM = "&nbsp;(<span id='time_lst' class='c'>%s</span>&nbsp; " L_LST ")<br />";
const char html_indexSite[] PROGMEM =
"&nbsp;&nbsp;" L_LONG " <span id='site_long' class='c'>%s</span>, " L_LAT " <span id='site_lat' class='c'>%s</span><br />";

const char html_indexPosition[] PROGMEM =
"&nbsp;&nbsp;" L_CURRENT ": " Axis1 " <span id='idx_a1' class='c'>%s</span>, " Axis2 " <span id='idx_a2' class='c'>%s</span><br />";
const char html_indexTarget[] PROGMEM =
"&nbsp;&nbsp;" L_TARGET ": " Axis1 " <span id='tgt_a1' class='c'>%s</span>, " Axis2 " <span id='tgt_a2' class='c'>%s</span><br />";
#if ENCODERS == ON
  const char html_indexEncoder1[] PROGMEM =
  "&nbsp;&nbsp;OnStep: Ax1 <span id='raw_a1' class='c'>%s</span>, Ax2 <span id='raw_a2' class='c'>%s</span><br />";
  const char html_indexEncoder2[] PROGMEM =
  "&nbsp;&nbsp;" L_ENCODER ": Ax1 <span id='enc_a1' class='c'>%s</span>, Ax2 <span id='enc_a2' class='c'>%s</span><br />";
#endif
const char html_indexPier[] PROGMEM =
"&nbsp;&nbsp;" L_PIER_SIDE ": <span id='pier_side' class='c'>%s</span> (" L_MERIDIAN_FLIPS " <span id='mdn_flip' class='c'>%s</span>)<br />";

const char html_indexCorPolar[] PROGMEM =
"&nbsp;&nbsp;<span id='align_lr' class='c'>%s</span> &nbsp;&amp;&nbsp; <span id='align_ud' class='c'>%s</span> &nbsp;(" L_ALIGN_MESSAGE " %s)<br />";

const char html_indexPark[] PROGMEM = "&nbsp;&nbsp;<span id='park' class='c'>%s</span><br />";
const char html_indexTracking[] PROGMEM = "&nbsp;&nbsp;" L_TRACKING ": <span id='track' class='c'>%s</span><br />";
const char html_indexTrackingRate[] PROGMEM = "&nbsp;&nbsp;" L_TRACKING ": <span id='track_rate' class='c'>%s %s</span><br />";
const char html_indexMaxSpeed[] PROGMEM = "&nbsp;&nbsp;" L_CURRENT_MAXSLEW ": <span id='mr_slew' class='c'>%s</span><br />";

const char html_indexTPHD[] PROGMEM = "&nbsp;&nbsp;%s <span id='tphd_%c' class='c'>%s</span><br />";

const char html_indexDriverStatus[] PROGMEM = "&nbsp;&nbsp;Axis%d " L_DRIVER_STATUS ": <span id='dvr_stat%d' class='c'>%s</span><br />";
const char html_indexGeneralError[] PROGMEM = "&nbsp;&nbsp;" L_LAST_GENERAL_ERROR ": <span id='last_err' class='c'>%s</span><br />";
const char html_indexWorkload[] PROGMEM = "&nbsp;&nbsp;" L_WORKLOAD ": <span id='work_load' class='c'>%s</span><br />";
#if DISPLAY_WIFI_SIGNAL_STRENGTH == ON
  const char html_indexSignalStrength[] PROGMEM = "&nbsp;&nbsp;" L_WIRELESS_SIGNAL_STRENGTH ": <span id='signal' class='c'>%s</span><br />";
#endif
