// -----------------------------------------------------------------------------------
// StallGuard monitor tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

#if DISPLAY_STALLGUARD_MONITOR == ON

extern void stallGuardTile(String &data);
extern void stallGuardTileAjax(String &data);
extern void stallGuardTileGet();

const char html_stallGuardSelect[] PROGMEM = "<button id='sgax%d' onpointerdown=\"s('sgax','%d')\" type='button' class='bb'>%c</button>";

const char html_stallGuardGraph[] PROGMEM = "<canvas id='stallGuardCanvas' width='420' height='220' style='border:1px solid #000000;'></canvas> \n";

const char html_stallGuardScript_1[] PROGMEM =
  "<script>\n"
  "var sgu=setInterval(updateStallGuard,500);"
  "var sgr=false;"
  "var sgtm=0;"
  "var sgData=[];"
  "var sgTrip=[];"
  "var sgw=210;"
  "var sgh=220;"
  "var sgx=0;"
  "var sgy=0;"
  "var sgty=0;"
  "var sglastTrip=-1;\n"

  "function updateStallGuard(){"
    "var canvas=document.getElementById('stallGuardCanvas');"
    "var ctx=canvas.getContext('2d');"
    "var sgText=document.getElementById('sgV').innerText;"
    "var tripText=document.getElementById('sgT').innerText;"
    "var sg=Number(sgText);"
    "var trip=Number(tripText);"
    "var sgValid=(sgText!='?'&&!isNaN(sg)&&sg>=0);"
    "var tripValid=(tripText!='?'&&!isNaN(trip)&&trip>=0);"
    "if(!sgr){for(i=0;i<sgw;i++){sgData[i]=-1;sgTrip[i]=-1;}sgr=true;sgtm=0;sgx=0;sgy=sgh;sgty=sgh;sglastTrip=-1;clearStallGuard(ctx);}"
    "if(tripValid){sglastTrip=trip;}else{trip=sglastTrip;tripValid=(trip>=0);}"
    "sgtm++;"
    "var x=sgtm;";

const char html_stallGuardScript_2[] PROGMEM =
    "if(x>=sgw-1){x=sgw-1;for(i=1;i<=x;i++){sgData[i-1]=sgData[i];sgTrip[i-1]=sgTrip[i];}sgtm=x;clearStallGuard(ctx);sgx=x-1;if(sgData[sgx]>=0){sgy=sgh-sgData[sgx]*(sgh/1023);}if(sgTrip[sgx]>=0){sgty=sgh-sgTrip[sgx]*(sgh/1023);}}"
    "sgData[x]=sgValid?sg:-1;"
    "sgTrip[x]=tripValid?trip:-1;"
    "if(sgValid){var y=sgh-sg*(sgh/1023);if(sgData[sgx]>=0){ctx.beginPath();ctx.moveTo(sgx*2,sgy);ctx.lineTo(x*2,y);ctx.strokeStyle='yellow';ctx.lineWidth=1;ctx.stroke();}sgy=y;}"
    "if(tripValid){var ty=sgh-trip*(sgh/1023);if(sgTrip[sgx]>=0){ctx.beginPath();ctx.moveTo(sgx*2,sgty);ctx.lineTo(x*2,ty);ctx.strokeStyle='orange';ctx.lineWidth=1;ctx.stroke();}sgty=ty;}"
    "sgx=x;"
  "}\n"

  "function clearStallGuard(ctx){"
    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_1 "';"
    "ctx.fillRect(0,0,sgw*2,sgh);"
    "ctx.strokeStyle='" COLOR_SERVO_PEN_1 "';"
    "ctx.beginPath();"
    "for(i=60;i<sgw*2;i+=60){ctx.moveTo(i,0);ctx.lineTo(i,sgh);}"
    "ctx.stroke();"
    "ctx.strokeStyle='" COLOR_SERVO_PEN_3 "';"
    "ctx.beginPath();ctx.moveTo(0,sgh/2);ctx.lineTo(sgw*2,sgh/2);ctx.stroke();"
    "ctx.strokeStyle='" COLOR_SERVO_PEN_2 "';";

const char html_stallGuardScript_3[] PROGMEM =
    "ctx.beginPath();ctx.moveTo(0,sgh*0.25);ctx.lineTo(sgw*2,sgh*0.25);ctx.moveTo(0,sgh*0.75);ctx.lineTo(sgw*2,sgh*0.75);ctx.stroke();"
    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_3 "';"
    "ctx.font='bold 12px Arial';"
    "ctx.textBaseline='top';"
    "ctx.fillText('1023',2,2);"
    "ctx.fillText('768',2,sgh*0.25+2);"
    "ctx.fillText('512',2,sgh/2+2);"
    "ctx.fillText('256',2,sgh*0.75+2);"
    "ctx.fillText('SG',sgw*2-52,2);"
    "ctx.fillText('Trip',sgw*2-52,18);"
    "ctx.fillStyle='yellow';ctx.fillRect(sgw*2-72,6,14,2);"
    "ctx.fillStyle='orange';ctx.fillRect(sgw*2-72,21,14,2);"
    "ctx.strokeStyle='yellow';ctx.lineWidth=1;"
    "ctx.beginPath();"
    "for(i=1;i<=sgtm;i++){if(sgData[i-1]>=0&&sgData[i]>=0){ctx.moveTo((i-1)*2,sgh-sgData[i-1]*(sgh/1023));ctx.lineTo(i*2,sgh-sgData[i]*(sgh/1023));}}ctx.stroke();"
    "ctx.strokeStyle='orange';ctx.lineWidth=1;"
    "ctx.beginPath();"
    "for(i=1;i<=sgtm;i++){if(sgTrip[i-1]>=0&&sgTrip[i]>=0){ctx.moveTo((i-1)*2,sgh-sgTrip[i-1]*(sgh/1023));ctx.lineTo(i*2,sgh-sgTrip[i]*(sgh/1023));}}ctx.stroke();ctx.lineWidth=1;"
  "}\n"
  "</script>\n";

#endif
