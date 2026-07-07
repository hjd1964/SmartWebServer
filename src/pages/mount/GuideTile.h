// -----------------------------------------------------------------------------------
// Guide tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void guideTile(String &data);
extern void guideTileAjax(String &data);
extern void guideTileGet();

const char html_guidePad[] PROGMEM =
"<script>"
"var gmc=motionControls({p:'mount-ajax-get.txt',k:'dr',s:function(a){return a+'0';},a:'q'});"
"function gstopall(){gmc.stopAll();}"
"function gt(e,v){gmc.t(e,v,v.charAt(0));}"
"function gu(e,v){gmc.u(e,v.charAt(0));}"
"function gp(e,v){gmc.p(e,v,v.charAt(0));}"
"function gs(e,v){gmc.r(e,v.charAt(0));}"
"function gc(e){return gmc.c(e);}"
"</script>"
"<button class='gb' ontouchstart=\"gt(event,'n1')\" ontouchend=\"gu(event,'n0')\" ontouchcancel=\"gu(event,'n0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'n1')\" onpointerup=\"gs(event,'n0')\" onpointercancel=\"gs(event,'n0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_N "</button><br />"
"<button class='gb' ontouchstart=\"gt(event,'e1')\" ontouchend=\"gu(event,'e0')\" ontouchcancel=\"gu(event,'e0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'e1')\" onpointerup=\"gs(event,'e0')\" onpointercancel=\"gs(event,'e0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_E "</button>"
"<button class='gb' onpointerdown=\"g('sy')\" title='Sync' type='button'>" BUTTON_SYNC "</button>"
"<button class='gb' ontouchstart=\"gt(event,'w1')\" ontouchend=\"gu(event,'w0')\" ontouchcancel=\"gu(event,'w0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'w1')\" onpointerup=\"gs(event,'w0')\" onpointercancel=\"gs(event,'w0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_W "</button><br />"
"<button class='gb' ontouchstart=\"gt(event,'s1')\" ontouchend=\"gu(event,'s0')\" ontouchcancel=\"gu(event,'s0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'s1')\" onpointerup=\"gs(event,'s0')\" onpointercancel=\"gs(event,'s0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_S "</button><br />";

const char html_guidePulseRates[] PROGMEM =
"<button id='guide_r0' class='btns_right' onpointerdown=\"g('R0')\" type='button'>0.25</button>"
"<button id='guide_r1' class='btns_mid' onpointerdown=\"g('R1')\" type='button'>0.5</button>"
"<button id='guide_r2' class='btns_left' onpointerdown=\"g('R2')\" type='button'>1x</button><br />";

const char html_guideRates[] PROGMEM =
"<button id='guide_r3' class='btns_right' onpointerdown=\"g('R3')\" type='button'>2x</button>"
"<button id='guide_r4' class='btns_mid' onpointerdown=\"g('R4')\" type='button'>4x</button>"
"<button id='guide_r5' class='btns_mid' onpointerdown=\"g('R5')\" type='button'>8x</button>"
"<button id='guide_r6' class='btns_mid' onpointerdown=\"g('R6')\" type='button'>20x</button>"
"<button id='guide_r7' class='btns_mid' onpointerdown=\"g('R7')\" type='button'>48x</button>"
"<button id='guide_r8' class='btns_mid' onpointerdown=\"g('R8')\" type='button'>VF</button>"
"<button id='guide_r9' class='btns_left' onpointerdown=\"g('R9')\" type='button'>VVF</button><br />";
