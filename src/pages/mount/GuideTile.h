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
"var gm={n:0,s:0,e:0,w:0},gq=[],gb=0;"
"function gsend(v){gq.push(v);grun();}"
"function grun(){if(gb||!gq.length)return;gb=1;var v=gq.shift(),x=new XMLHttpRequest(),gd=0;function gf(){if(gd)return;gd=1;gb=0;grun();}x.onreadystatechange=function(){if(x.readyState==4)gf();};x.onerror=x.ontimeout=gf;x.timeout=1000;x.open('GET','mount-ajax-get.txt?dr='+encodeURIComponent(v)+'&x='+new Date().getTime(),true);x.send();}"
"function gstop(a){if(!gm[a])return;gm[a]=0;gsend(a+'0');setTimeout(function(){if(!gm[a])gsend(a+'0')},150);setTimeout(function(){if(!gm[a])gsend(a+'0')},500);}"
"function gstopall(){gstop('n');gstop('s');gstop('e');gstop('w');}"
"function gp(e,v){var a=v.charAt(0);gm[a]=1;if(e.currentTarget.setPointerCapture)try{e.currentTarget.setPointerCapture(e.pointerId)}catch(x){}gsend(v);}"
"function gs(e,v){var a=v.charAt(0);if(e.currentTarget.releasePointerCapture)try{e.currentTarget.releasePointerCapture(e.pointerId)}catch(x){}gstop(a);}"
"window.addEventListener('pointerup',gstopall);"
"window.addEventListener('pointercancel',gstopall);"
"window.addEventListener('blur',gstopall);"
"document.addEventListener('visibilitychange',function(){if(document.hidden)gstopall();});"
"</script>"
"<button class='gb' onpointerdown=\"gp(event,'n1')\" onpointerup=\"gs(event,'n0')\" onpointercancel=\"gs(event,'n0')\" onlostpointercapture=\"gs(event,'n0')\" oncontextmenu=\"gs(event,'n0');return false\" type='button'>" BUTTON_N "</button><br />"
"<button class='gb' onpointerdown=\"gp(event,'e1')\" onpointerup=\"gs(event,'e0')\" onpointercancel=\"gs(event,'e0')\" onlostpointercapture=\"gs(event,'e0')\" oncontextmenu=\"gs(event,'e0');return false\" type='button'>" BUTTON_E "</button>"
"<button class='gb' onpointerdown=\"g('sy')\" title='Sync' type='button'>" BUTTON_SYNC "</button>"
"<button class='gb' onpointerdown=\"gp(event,'w1')\" onpointerup=\"gs(event,'w0')\" onpointercancel=\"gs(event,'w0')\" onlostpointercapture=\"gs(event,'w0')\" oncontextmenu=\"gs(event,'w0');return false\" type='button'>" BUTTON_W "</button><br />"
"<button class='gb' onpointerdown=\"gp(event,'s1')\" onpointerup=\"gs(event,'s0')\" onpointercancel=\"gs(event,'s0')\" onlostpointercapture=\"gs(event,'s0')\" oncontextmenu=\"gs(event,'s0');return false\" type='button'>" BUTTON_S "</button><br />";

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
