// -----------------------------------------------------------------------------------
// Encoders
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#if ENCODERS == ON

  #include "../locales/Locale.h"

  const char html_encScript2[] PROGMEM =
  "<script>"
    "var autoG=setInterval(autoRunG,1000);"
    "var x=0;"
    "function autoRunG() {"
      "x=x+1; if (x>599) x=0;"
      "var c = document.getElementById('myCanvas');"
      "var ctx = c.getContext('2d');"
      
      "ctx.beginPath();"
      "ctx.strokeStyle='#666666';"
      "ctx.moveTo(0, 150); ctx.lineTo(600, 150);"
      "ctx.stroke();"

      "if (document.getElementById('osc').innerText=='On') {"
        "ctx.beginPath();"
        "ctx.strokeStyle='#111111';"
        "x1=x+1; if (x1>599) x1=0;"
        "ctx.moveTo(x1, 0); ctx.lineTo(x1, 300);"
        "ctx.stroke();"
      "};"

      "var stO=document.getElementById('stO').innerText;"

      "var Ylta=document.getElementById('stL').innerText;"
      "Ylta=(stO-Ylta)*10000.0;"
      "ctx.beginPath();"
      "ctx.fillStyle='#66ff66';"
      "ctx.fillRect(x,Ylta+150,3,3);"
      "ctx.stroke();"

      "var Ysta=document.getElementById('stS').innerText;"
      "Ysta=(stO-Ysta)*1000.0;"
      "ctx.beginPath();"
      "ctx.fillStyle='#6666ff';"
      "ctx.fillRect(x,Ysta+150,3,3);"
      "ctx.stroke();"

    "};"
  "</script>\n";

  const char html_syncOnStepNow[] PROGMEM =
  L_ENC_SYNC_TO_ENC ": <br />"
  "<form method='get' action='/enc.htm'>"
  "<button name='ms' value='ons' type='submit'>" L_ENC_SYNC_NOW "</button>"
  "<br />";

  const char html_syncEncodersNow[] PROGMEM =
  L_ENC_SYNC_TO_ONS ": <br />"
  "<button name='ms' value='enc' type='submit'>" L_ENC_SYNC_NOW "</button>"
  "<br />";

  #ifdef ENC_HAS_ABSOLUTE
    const char html_zeroEncodersNow[] PROGMEM =
    L_ENC_ZERO_TO_ONS ": <br />"
    "<button name='ms' value='zro' type='submit'>" L_ENC_SYNC_NOW "</button>"
    "<br />";
  #endif

  const char html_encEn1[] PROGMEM = "<br />"
  L_ENC_AUTO_SYNC ": <br />";
  const char html_encEn2[] PROGMEM =
  "<button id='eas_on'  class='btns_right' onpointerdown=\"s('as','on')\"  type='button' disabled>" L_ON "</button>"
  "<button id='eas_off' class='btns_left'  onpointerdown=\"s('as','off')\" type='button' disabled>" L_OFF "</button>"
  "<br /><br />";

  // rate control
  const char html_encRateEn1[] PROGMEM =
  L_ENC_AUTO_RATE " (<span id='orc'>?</span>):<br />";
  const char html_encRateEn2[] PROGMEM =
  "<button type='button' onpointerdown=\"s('rc','on')\" >" L_ON "</button>"
  "<button type='button' onpointerdown=\"s('rc','off')\" >" L_OFF "</button>"
  "<br /><br />";

  const char html_encStaAxis1[] PROGMEM =
  #if AXIS1_ENC_BIN_AVG > 0
    L_ENC_BINNED_SAMPLES ": <br />"
  #else
    L_ENC_SAMPLES ": <br />"
  #endif
  "<form method='get' action='/enc.htm'>"
  " <input value='%ld' type='number' name='sa' min='1' max='999'>"
  "<button type='submit'>" L_UPLOAD "</button>"
  " (STA 1 to 999)"
  "</form>";
  const char html_encLtaAxis1[] PROGMEM = 
  "<form method='get' action='/enc.htm'>"
  " <input value='%ld' type='number' name='la' min='1' max='999'>"
  "<button type='submit'>" L_UPLOAD "</button>"
  " (LTA 1 to 999)"
  "</form><br />";

  const char html_encPropAxis1[] PROGMEM = 
  L_ENC_PROPORTIONAL ": <br />"
  "<form method='get' action='/enc.htm'>"
  " <input 'width: 75px' value='%ld' type='number' name='pr' min='50' max='5000'>"
  "<button type='submit'>" L_UPLOAD "</button>"
  " (P 50 to 5000&#x25;)"
  "</form><br />";

  const char html_encMinGuideAxis1[] PROGMEM = 
  L_ENC_MIN_GUIDE ": <br />"
  "<form method='get' action='/enc.htm'>"
  " <input 'width: 75px' value='%ld' type='number' name='mg' min='25' max='1000'>"
  "<button type='submit'>" L_UPLOAD "</button>"
  " (25 to 1000mS)"
  "</form><br />";

  #if AXIS1_ENC_RATE_AUTO == OFF
    const char html_encErc2Axis1[] PROGMEM = 
    L_ENC_RATE_COMP ": <br />"
    "<form method='get' action='/enc.htm'>"
    " <input style='width: 75px' value='%ld' type='number' name='er' min='-99999' max='99999'>"
    "<button type='submit'>" L_UPLOAD "</button>"
    " (ERC &#xb1;99999 PPM, &#xb1;10&#x25;)"
    "</form><br />";
  #endif

  #if AXIS1_ENC_INTPOL_COS == ON
    const char html_encIntPolPhaseAxis1[] PROGMEM = 
    "<form method='get' action='/enc.htm'>"
    " <input value='%ld' type='number' name='ip' min='0' max='255'>"
    "<button type='submit'>" L_UPLOAD "</button>"
    " (" L_ENC_PHASE " 1 to 256 " L_ENC_COUNTS ")"
    "</form>";

    const char html_encIntPolMagAxis1[] PROGMEM = 
    "<form method='get' action='/enc.htm'>"
    " <input style='width: 75px' value='%ld' type='number' name='im' min='0' max='29999'>"
    "<button type='submit'>" L_UPLOAD "</button>"
    " (" L_ENC_MAGN " 0 to 29999 PPM, 0 to 3&#x25;)"
    "</form><br />";
  #endif

  const char html_encSweepEn1[] PROGMEM =
  "<br />&nbsp; Sweep (<span id='osc'>?</span>):<br />";
  const char html_encSweepEn2[] PROGMEM =
  "&nbsp; <button type='button' onpointerdown=\"s('sw','on')\" >" L_ON "</button>"
  "<button type='button' onpointerdown=\"s('sw','off')\" >" L_OFF "</button>"
  "<br /><br />";

  // encoder configuration
  const char html_encFormBegin[] PROGMEM = "<div class='content'><br />\r\n<form method='get' action='/enc.htm'>";
  const char html_encFormEnd[] PROGMEM = "\r\n</form><br />\r\n</div><br />\r\n";

  const char html_encAxisTpd[] PROGMEM =
  " <input style='width: 7em;' value='%s' type='number' name='a%dcpd' min='%d' max='%ld' step='any'>&nbsp; " L_ENC_SET_TPD "<br />\r\n";

  const char html_encAxisReverse[] PROGMEM =
  " <input style='width: 7em;' value='%d' type='number' name='a%drev' min='0' max='1' step='1'>&nbsp; " L_ADV_SET_REV "<br />\r\n";

  const char html_encMxAxis1[] PROGMEM =
  " <input style='width: 7em;' value='%ld' type='number' name='d1' min='0' max='9999'>"
  "&nbsp; " L_ENC_MAX_ANGLE ", " L_ENC_MAX_ANGLE_AXIS1 "<br/>";

  const char html_encMxAxis2[] PROGMEM =
  " <input style='width: 7em;' value='%ld' type='number' name='d2' min='0' max='9999'>"
  "&nbsp; " L_ENC_MAX_ANGLE ", " L_ENC_MAX_ANGLE_AXIS2 "<br/>";
#endif