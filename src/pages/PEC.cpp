// -----------------------------------------------------------------------------------
// Controls for PEC

#include "PEC.h"

void processPecGet();

void handlePec() {
  char temp[120] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();
      
  mountStatus.update();

  processPecGet();

  sendHtmlStart();

  // send a standard http response header
  String data=FPSTR(html_headB);
  data.concat(FPSTR(html_main_cssB));
  data.concat(FPSTR(html_main_css1));
  data.concat(FPSTR(html_main_css2));
  data.concat(FPSTR(html_main_css3));
  data.concat(FPSTR(html_main_css4));
  data.concat(FPSTR(html_main_css5));
  data.concat(FPSTR(html_main_css6));
  sendHtml(data);
  data.concat(FPSTR(html_main_css7));
  data.concat(FPSTR(html_main_css8));
  data.concat(FPSTR(html_main_css_control1));
  data.concat(FPSTR(html_main_css_control2));
  data.concat(FPSTR(html_main_css_control3));
  data.concat(FPSTR(html_main_cssE));
  data.concat(FPSTR(html_headE));
  data.concat(FPSTR(html_bodyB));
  sendHtml(data);

  if (mountStatus.pecEnabled()) {
    // active ajax page is: pecAjax();
    data.concat("<script>var ajaxPage='pec.txt';</script>\n");
    data.concat(FPSTR(html_ajax_active));
    data.concat("<script>auto2Rate=2;</script>");
  }

  // finish the standard http response header
  data.concat(FPSTR(html_onstep_header1)); data.concat("OnStep");
  data.concat(FPSTR(html_onstep_header2)); data.concat(firmwareVersion.str);
  data.concat(" (OnStep");
  if (mountStatus.getVersionStr(temp)) data.concat(temp); else data.concat("?");
  data.concat(FPSTR(html_onstep_header3));
  data.concat(FPSTR(html_linksStatN));
  data.concat(FPSTR(html_linksCtrlN));
  if (mountStatus.featureFound()) data.concat(FPSTR(html_linksAuxN));
  data.concat(FPSTR(html_linksLibN));
  #if ENCODERS == ON
    data.concat(FPSTR(html_linksEncN));
  #endif
  sendHtml(data);
  if (mountStatus.pecEnabled()) data.concat(FPSTR(html_linksPecS));
  data.concat(FPSTR(html_linksSetN));
  data.concat(FPSTR(html_linksCfgN));
  data.concat(FPSTR(html_linksSetupN));
  data.concat(FPSTR(html_onstep_header4));
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data.concat( FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(data); return; }

  data.concat(FPSTR(html_pec1));

  if (mountStatus.pecEnabled()) {
    data.concat(FPSTR(html_pec2));
    data.concat(FPSTR(html_pecControls0));
    data.concat(FPSTR(html_pecControls1));
    data.concat(FPSTR(html_pecControls2));
    data.concat(FPSTR(html_pecControls3));
    data.concat(FPSTR(html_pecControls4));
    data.concat(FPSTR(html_pecControls5));
    data.concat("</div></body></html>");
  } else {
    data.concat(L_PEC_NO_PEC_MESSAGE);
    data.concat("</div><br class='clear' /></body></html>");
  }

  sendHtml(data);
  sendHtmlDone(data);
}

void pecAjax() {
  String data="";
  char temp[80]="";
  
  data.concat("status|");
  if ((mountStatus.mountType() != MT_ALTAZM) && (mountStatus.update()) && (onStep.command(":$QZ?#",temp))) {
    if (temp[0] == 'I') data.concat(L_PEC_IDLE); else
    if (temp[0] == 'p') data.concat(L_PEC_WAIT_PLAY); else
    if (temp[0] == 'P') data.concat(L_PEC_PLAYING); else
    if (temp[0] == 'r') data.concat(L_PEC_WAIT_REC); else
    if (temp[0] == 'R') data.concat(L_PEC_RECORDING); else data.concat(L_PEC_UNK);
    if (mountStatus.pecRecording()) data.concat(" (" L_PEC_EEWRITING ")");
  } else { data.concat("?"); }
  data.concat("\n");

#if OPERATIONAL_MODE != WIFI
  sendHtmlDone(data);
#else
  www.send(200, "text/plain",data);
#endif
}

void processPecGet() {
  String v;

  // PEC control
  v = www.arg("pe");
  if (!v.equals(EmptyStr)) {
    if (v.equals("pl")) onStep.commandBlind(":$QZ+#"); // play
    if (v.equals("st")) onStep.commandBlind(":$QZ-#"); // stop
    if (v.equals("re")) onStep.commandBlind(":$QZ/#"); // record
    if (v.equals("cl")) onStep.commandBlind(":$QZZ#"); // clear
    if (v.equals("wr")) onStep.commandBlind(":$QZ!#"); // write to eeprom
  }
}
