// -----------------------------------------------------------------------------------
// Ambient tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern bool axisTile(int axis, String &data);
extern void axisTileAjax(int axis, String &data);
extern void axisTileGet();

const char html_indexDriverStatus[] PROGMEM = L_DRIVER " " L_STATUS ": <span id='dvr_stat%d' class='c'>%s</span><br />";

const char html_configAdvanced[] PROGMEM =
"<hr>" L_ADV_SET_TITLE " (<i>" L_ADV_SET_HEADER_MSG "</i>)<br /><br />";

const char html_configMountType[] PROGMEM =
" <input style='width: 7em;' value='%d' type='number' name='mountt' min='1' max='3' step='1'>&nbsp; " L_ADV_MOUNT_TYPE "<br /><br />\n";

#if DRIVE_CONFIGURATION == ON
  const char html_configAxisFloat[] PROGMEM =
  " <input style='width: 10em;' value='%s' type='number' name='a%dp%dvalue' min='%ld' max='%ld' step='any'>&nbsp;%s<br />\n";
  const char html_configAxisInt[] PROGMEM =
  " <input style='width: 10em;' value='%ld' type='number' name='a%dp%dvalue' min='%ld' max='%ld' step='1'>&nbsp;%s<br />\n";
  const char html_configAxisSelectStart[] PROGMEM =
  " <select size='1' style='width: 10.6em; margin: 3px; text-align-last: center;' name='a%dp%dvalue' >";
  const char html_configAxisSelectOption[] PROGMEM =
  "<option value='%d'>%s</option>";
  const char html_configAxisSelectOptionSelected[] PROGMEM =
  "<option value='%d' selected>%s</option>";
  const char html_configAxisSelectEnd[] PROGMEM =
  "</select>&nbsp;%s<br />\n";

  const char html_configAxisRevert[] PROGMEM =
  "<button name='revert' value='%d' type='submit'>" L_REVERT "</button>";

  #define AXPN_COUNT 26
  const char html_axpn_1[] PROGMEM = L_AXPN_STEPS_PER_DEG;
  const char html_axpn_2[] PROGMEM = L_AXPN_LIMIT_DEGS_MIN;
  const char html_axpn_3[] PROGMEM = L_AXPN_LIMIT_DEGS_MAX;
  const char html_axpn_4[] PROGMEM = L_AXPN_STEPS_PER_UM;
  const char html_axpn_5[] PROGMEM = L_AXPN_LIMIT_UM_MIN;
  const char html_axpn_6[] PROGMEM = L_AXPN_LIMIT_UM_MAX;
  const char html_axpn_7[] PROGMEM = L_AXPN_REVERSE;
  const char html_axpn_8[] PROGMEM = L_AXPN_MICROSTEPS;
  const char html_axpn_9[] PROGMEM = L_AXPN_MICROSTEPS_GOTO;
  const char html_axpn_10[] PROGMEM = L_AXPN_DECAY_MODE;
  const char html_axpn_11[] PROGMEM = L_AXPN_DECAY_MODE_GOTO;
  const char html_axpn_12[] PROGMEM = L_AXPN_CURRENT_HOLD;
  const char html_axpn_13[] PROGMEM = L_AXPN_CURRENT_RUN;
  const char html_axpn_14[] PROGMEM = L_AXPN_CURRENT_GOTO;
  const char html_axpn_15[] PROGMEM = L_AXPN_INTERPOLATE;
  const char html_axpn_16[] PROGMEM = L_AXPN_PID_P;
  const char html_axpn_17[] PROGMEM = L_AXPN_PID_I;
  const char html_axpn_18[] PROGMEM = L_AXPN_PID_D;
  const char html_axpn_19[] PROGMEM = L_AXPN_PID_SLEWING_P;
  const char html_axpn_20[] PROGMEM = L_AXPN_PID_SLEWING_I;
  const char html_axpn_21[] PROGMEM = L_AXPN_PID_SLEWING_D;
  const char html_axpn_22[] PROGMEM = L_AXPN_RADS_PER_COUNT;
  const char html_axpn_23[] PROGMEM = L_AXPN_STEPS_TO_COUNTS;
  const char html_axpn_24[] PROGMEM = L_AXPN_MAX_ACCEL;
  const char html_axpn_25[] PROGMEM = L_AXPN_MIN_PWR;
  const char html_axpn_26[] PROGMEM = L_AXPN_MAX_PWR;
#endif
