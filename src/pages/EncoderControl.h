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