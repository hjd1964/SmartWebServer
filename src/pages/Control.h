// -----------------------------------------------------------------------------------
// Control
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

const char html_controlQuick1[] PROGMEM =
"<div style='text-align: center; width: 30em'>"
"<form style='display: inline;' method='get' action='/control.htm'>"
"<button name='qb' class='bb' value='st' onpointerdown='SetDateTime();' title='" L_HINT_CLOCK "' type='submit'>" CLOCK_CH "</button>"
"&nbsp;&nbsp;";
const char html_controlQuick1a[] PROGMEM =
"<input id='dm' type='hidden' name='dm'><input id='dd' type='hidden' name='dd'><input id='dy' type='hidden' name='dy'>"
"<input id='th' type='hidden' name='th'><input id='tm' type='hidden' name='tm'><input id='ts' type='hidden' name='ts'>";
const char html_controlQuick2[] PROGMEM =
"</form>"
"<button id='c_goto' class='bb' onpointerdown=\"g('qc')\" title='" L_HINT_CONTINUE_GOTO "' type='button' disabled>" ARROW_R2 HOME_CH ARROW_R2 "</button>";
const char html_controlQuick3[] PROGMEM =
"&nbsp;&nbsp;"
"<button class='bb' onpointerdown=\"g('qh')\" title='" L_HINT_FIND_HOME "' type='button'>" ARROW_DR HOME_CH "</button>"
"<button class='bb' onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) g('qr')\" title='" L_HINT_RESET_HOME "' type='button'>" CAUTION_CH HOME_CH "</button>";
const char html_controlQuick4[] PROGMEM =
"&nbsp;&nbsp;"
"<button id='unpark' class='bb' onpointerdown=\"g('pu')\" title='" L_HINT_UNPARK "' type='button' disabled>P" ARROW_UR "</button>"
"<button id='park' class='bb' onpointerdown=\"g('pk')\" title='" L_HINT_PARK "' type='button' disabled>" ARROW_DR "P</button>";
const char html_controlQuick5[] PROGMEM =
"&nbsp;&nbsp;"
"<button class='bb' style=\"height: 2.2em; color: White;\" onpointerdown=\"g('qq')\" title='" L_HINT_EMERGENCY_STOP "' type='button'>" L_STOP "!</button>"
"</div><br class='clear' />\r\n";

const char html_controlTrack1[] PROGMEM = 
"<div class='b1' style='width: 8em'>"
"<div align='left'>" L_TRACKING ":</div>"
"<button id='trk_on' class='btns_right' onpointerdown=\"s('dr','T1')\" type='button' disabled>" L_ON "</button>";
const char html_controlTrack2[] PROGMEM =
"<button id='trk_off' class='btns_left' onpointerdown=\"s('dr','T0')\" type='button' disabled>" L_OFF "</button><br />"
"<button class='bbh' style='width: 2em' onpointerdown=\"g('Ts')\" title='" L_HINT_RATE_SIDEREAL "' type='button'>" SIDEREAL_CH "</button>";
const char html_controlTrack3[] PROGMEM =
"<button class='bbh' style='width: 2em' onpointerdown=\"g('Tl')\" title='" L_HINT_RATE_LUNAR "' type='button'>" LUNAR_CH "</button>";
const char html_controlTrack4[] =
"<button class='bbh' style='width: 2em' onpointerdown=\"g('Th')\" title='" L_HINT_RATE_SOLAR "' type='button'>" SOLAR_CH "</button>"
"</div>";

const char html_controlAlign1[] PROGMEM = 
"<div class='b1' style='width: 16.2em'>"
"<div align='left'>" L_ALIGN ":</div>"
"<form method='get' action='/control.htm'>";
const char html_controlAlign2[] PROGMEM =
"<button id='alg%d' class='bbh' onpointerdown=\"s('al','%d')\" type='button' disabled>%d%s</button>";
const char html_controlAlign3[] PROGMEM =
"&nbsp;&nbsp;&nbsp;<button id='alga' class='bbh' onpointerdown=\"s('al','n')\" type='button' disabled>" L_ACCEPT "</button>";
const char html_controlAlign4[] PROGMEM =
"<br /><button class='collapsible' style='padding-top: 4px; text-align: center;' type='button'>" L_REFINE_PA "</button>"
"<div class='content' style='padding: 5px'>" L_REFINE_MESSAGE1 L_REFINE_MESSAGE2 L_REFINE_MESSAGE3
"<br /><button id='rpa' onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) s('rp','a')\" type='button'>" L_REFINE_PA "</button>"
"<br /></div>";

const char html_controlTrack5[] PROGMEM =
"</form></div><br class='clear' />\r\n";

const char html_controlGuide1[] PROGMEM =
"<div class='b1' style='width: 27em'>"
"<div align='left'>" L_GUIDE ":</div>"
"<button class='gb' onpointerdown=\"g('n1')\" onpointerup=\"g('n0')\" type='button'>" BUTTON_N "</button><br />";
const char html_controlGuide2[] PROGMEM =
"<button class='gb' onpointerdown=\"g('e1')\" onpointerup=\"g('e0')\" type='button'>" BUTTON_E "</button>";
const char html_controlGuide3[] PROGMEM =
"<button class='gb' onpointerdown=\"g('sy')\" title='Sync' type='button'>" BUTTON_SYNC "</button>"
"<button class='gb' onpointerdown=\"g('w1')\" onpointerup=\"g('w0')\" type='button'>" BUTTON_W "</button><br />";
const char html_controlGuide4[] PROGMEM =
"<button class='gb' onpointerdown=\"g('s1')\" onpointerup=\"g('s0')\" type='button'>" BUTTON_S "</button><br /><br />";
const char html_controlGuide5[] PROGMEM =
"<form method='get' action=\"/control.htm\">"
"<button class='bbh' onpointerdown=\"g('R0')\" type='button'>0.25</button>"
"<button class='bbh' onpointerdown=\"g('R1')\" type='button'>0.5</button>"
"<button class='bbh' onpointerdown=\"g('R2')\" type='button'>1x</button>";
const char html_controlGuide6[] PROGMEM =
"<button class='bbh' onpointerdown=\"g('R5')\" type='button'>" L_CENTER_RATE "</button>"
"<button class='bbh' onpointerdown=\"g('R6')\" type='button'>" L_FIND_RATE "</button>";
const char html_controlGuide7[] PROGMEM =
"<button class='bbh' onpointerdown=\"g('R7')\" type='button'>" L_FAST_RATE "</button>"
"<button class='bbh' onpointerdown=\"g('R8')\" type='button'>" L_VFAST_RATE "</button>"
"</div><br class='clear' />\r\n";

const char html_controlFocusBeg[] PROGMEM =
"<div class='b1' style='width: 27em'>";
const char html_selectFocuser1[] PROGMEM =
"<button id='foc1_sel' class='bbh' onpointerdown=\"gf('F1')\" title='" L_HINT_FOC_SEL " 1' type='button' disabled>1</button>";
const char html_selectFocuser2[] PROGMEM =
"<button id='foc2_sel' class='bbh' onpointerdown=\"gf('F2')\" title='" L_HINT_FOC_SEL " 2' type='button' disabled>2</button>";
const char html_selectFocuser3[] PROGMEM =
"<button id='foc3_sel' class='bbh' onpointerdown=\"gf('F3')\" title='" L_HINT_FOC_SEL " 3' type='button' disabled>3</button>";
const char html_selectFocuser4[] PROGMEM =
"<button id='foc4_sel' class='bbh' onpointerdown=\"gf('F4')\" title='" L_HINT_FOC_SEL " 4' type='button' disabled>4</button>";
const char html_selectFocuser5[] PROGMEM =
"<button id='foc5_sel' class='bbh' onpointerdown=\"gf('F5')\" title='" L_HINT_FOC_SEL " 5' type='button' disabled>5</button>";
const char html_selectFocuser6[] PROGMEM =
"<button id='foc6_sel' class='bbh' onpointerdown=\"gf('F6')\" title='" L_HINT_FOC_SEL " 6' type='button' disabled>6</button>";
const char html_setFocus1[] PROGMEM =
"&nbsp;&nbsp;&nbsp;&nbsp;<span style='border: 1px solid #551111; background-color: #181818; text-align: center; margin: 1px; padding: 0.5em 0.1em 0.5em 0.1em;'>";
const char html_setFocus2[] PROGMEM =
"<input id='foc_set' style='width: 6em;' value='0' type='number' min='-500000' max='500000'>&nbsp;&nbsp;"
"<button onpointerdown=\"s('fs',document.getElementById('foc_set').value)\" type='button'>" L_SET "</button></span>";
const char html_controlFocus1[] PROGMEM =
"<button class='bbh' onpointerdown=\"gf('Fh')\" title='" L_HINT_FOC_FIND_HOME "' type='button'>" ARROW_DR HOME_CH "</button>"
"<button class='bbh' onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) gf('FH')\" title='" L_HINT_FOC_RESET_HOME "' type='button'>" CAUTION_CH HOME_CH "</button>&nbsp;&nbsp;&nbsp;&nbsp;";
const char html_controlFocus2[] PROGMEM =
"<button class='bbh' onpointerdown=\"gf('FI')\" onpointerup=\"g('Fq');\" type='button'>" ARROW_DD "</button>"
"<button class='bbh' style='width: 2em' onpointerdown=\"gf('Fi')\" onpointerup=\"g('Fq')\" type='button'>" ARROW_D "</button>";
const char html_controlFocus3[] PROGMEM =
"<button class='bbh' style='width: 2em' onpointerdown=\"gf('Fo')\" onpointerup=\"g('Fq')\" type='button'>" ARROW_U "</button>"
"<button class='bbh' onpointerdown=\"gf('FO')\" onpointerup=\"g('Fq')\" type='button'>" ARROW_UU "</button>";
const char html_controlFocusEnd[] PROGMEM =
"</div><br class='clear' />\r\n";

const char html_controlRotateBeg[] PROGMEM =
"<div class='b1' style='width: 27em'>";
const char html_setRotate1[] PROGMEM =
"<span style='border: 1px solid #551111; background-color: #181818; text-align: center; margin: 1px; padding: 0.5em 0.1em 0.5em 0.1em;'>";
const char html_setRotate2[] PROGMEM =
"<input id='rot_set' style='width: 6em;' value='0.0' type='number' min='-360' max='360' step='any'>&nbsp;&nbsp;"
"<button onpointerdown=\"s('rs',document.getElementById('rot_set').value)\" type='button'>" L_SET "</button></span>";
const char html_controlRotate1[] PROGMEM =
"<button class='bbh' style='height: 2.1em' onpointerdown=\"gf('rh')\" title='" L_HINT_ROT_FIND_HOME "' type='button'>" ARROW_DR HOME_CH "</button>"
"<button class='bbh' style='height: 2.1em' onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) gf('rH')\" title='" L_HINT_ROT_RESET_HOME "' type='button'>" CAUTION_CH HOME_CH "</button>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
const char html_controlRotate2[] PROGMEM =
"<button class='bbh' style='height: 2.1em' onpointerdown=\"gf('rB')\" onpointerup=\"g('rq')\" type='button'>" ARROW_LL "</button>"
"<button class='bbh' style='width: 2em' onpointerdown=\"gf('rr')\" onpointerup=\"g('rq')\" type='button'>" ARROW_L "</button>";
const char html_controlRotate3[] PROGMEM =
"<button class='bbh' style='width: 2em' onpointerdown=\"gf('rf')\" onpointerup=\"g('rq')\" type='button'>" ARROW_R "</button>"
"<button class='bbh' style='height: 2.1em' onpointerdown=\"gf('rF')\" onpointerup=\"g('rq')\" type='button'>" ARROW_RR "</button><br />";
const char html_controlDeRotate1[] PROGMEM =
"<button onpointerdown=\"gf('r1')\" type='button'>" L_HINT_DEROTATE_ON "</button>&nbsp;&nbsp;&nbsp;"
"<button onpointerdown=\"gf('rR')\" title='" L_HINT_ROTATOR_REV "' type='button'>Rev</button>";
const char html_controlDeRotate2[] PROGMEM =
"<button onpointerdown=\"gf('rp')\" title='" L_HINT_ROTATOR_PAR "' type='button'>P</button>&nbsp;&nbsp;&nbsp;"
"<button onpointerdown=\"gf('r0')\" type='button'>" L_HINT_DEROTATE_OFF "</button>";
const char html_controlRotateEnd[] PROGMEM =
"</div><br class='clear' />\r\n";

const char html_controlEnd[] PROGMEM = 
"<br />\r\n";
