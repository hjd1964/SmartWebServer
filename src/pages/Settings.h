// -----------------------------------------------------------------------------------
// Settings
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

const char html_settingsStart[] PROGMEM =
"<form method='get' action='/settings.htm'>";

const char html_settingsSlewSpeed1[] PROGMEM =
L_MAX_SLEW_SPEED ": <br />"
"<button id='sr_vf' class='btns_right' onpointerdown=\"s('ss','vf')\" type='button' disabled>" L_VFAST "</button>"
"<button id='sr_f'  class='btns_mid'   onpointerdown=\"s('ss','f')\"  type='button' disabled>" L_FAST "</button>"
"<button id='sr_n'  class='btns_mid'   onpointerdown=\"s('ss','n')\"  type='button' disabled>" L_NORMAL "</button>";
const char html_settingsSlewSpeed2[] PROGMEM =
"<button id='sr_s'  class='btns_mid'   onpointerdown=\"s('ss','s')\"  type='button' disabled>" L_SLOW "</button>"
"<button id='sr_vs' class='btns_left'  onpointerdown=\"s('ss','vs')\" type='button' disabled>" L_VSLOW "</button>"
"<br /><br />\r\n";

const char html_settingsTrackComp1[] PROGMEM =
L_TRK_COMP ": <br />";
const char html_settingsTrackComp2[] PROGMEM =
"<button id='ot_on'  class='btns_right' onpointerdown=\"s('rr','otk')\"  type='button' disabled>" L_TRK_FULL "</button>"
"<button id='ot_ref' class='btns_mid'   onpointerdown=\"s('rr','on')\"   type='button' disabled>" L_TRK_REFR "</button>"
"<button id='ot_off' class='btns_left'  onpointerdown=\"s('rr','off')\"  type='button' disabled>" L_OFF "</button><br />\r\n";
const char html_settingsTrackComp3[] PROGMEM =
"<button id='ot_dul' class='btns_right' onpointerdown=\"s('rr','don')\"  type='button' disabled>" L_TRK_DUAL "</button>"
"<button id='ot_sgl' class='btns_left'  onpointerdown=\"s('rr','doff')\" type='button' disabled>" L_TRK_SINGLE "</button>"
"<br /><br />\r\n";

const char html_settingsPark1[] PROGMEM =
L_PARK ": <br />"
"<button onpointerdown=\"s('pk','s')\" type='button'>" L_SET_PARK "</button>"
"<br /><br />\r\n";

const char html_settingsTrack1[] PROGMEM =
L_TRACKING_RATE ": <br />";
const char html_settingsTrack2[] PROGMEM =
"<button onpointerdown=\"s('tk','f')\" type='button'>+ (0.02Hz " L_TRK_FASTER ")</button>"
"<button onpointerdown=\"s('tk','-')\" type='button'>- (0.02Hz " L_TRK_SLOWER ")</button>"
"<button onpointerdown=\"s('tk','r')\" type='button'>" L_TRK_RESET "</button>"
"<br /><br />\r\n";

const char html_settingsBuzzer1[] PROGMEM =
L_BUZZER ": <br />";
const char html_settingsBuzzer2[] PROGMEM =
"<button id='bzr_on'  class='btns_right' onpointerdown=\"s('ab','on')\"  type='button' disabled>" L_ON "</button>"
"<button id='bzr_off' class='btns_left'  onpointerdown=\"s('ab','off')\" type='button' disabled>" L_OFF "</button>"
"<br /><br />\r\n";

const char html_settingsMFAuto1[] PROGMEM =
L_MERIDIAN_FLIP_AUTO ":<br />";
const char html_settingsMFAuto2[] PROGMEM =
"<button id='mfa_on' class='btns_right' onpointerdown=\"s('ma','on')\"  type='button' disabled>" L_ON "</button>"
"<button id='mfa_off' class='btns_left' onpointerdown=\"s('ma','off')\" type='button' disabled>" L_OFF "</button>&nbsp;&nbsp;"
"<button name='ma' value='now' type='submit'>" L_MERIDIAN_FLIP_NOW "</button>"
"<br /><br />\r\n";

const char html_settingsMFPause1[] PROGMEM =
L_MERIDIAN_FLIP_PAUSE ": <br />";
const char html_settingsMFPause2[] PROGMEM =
"<button id='mfp_on' class='btns_right' onpointerdown=\"s('mp','on')\"  type='button' disabled>" L_ON "</button>"
"<button id='mfp_off' class='btns_left' onpointerdown=\"s('mp','off')\" type='button' disabled>" L_OFF "</button>"
"<br /><br />\r\n";

const char html_settingsEnd[] PROGMEM =
"</form>\r\n";
