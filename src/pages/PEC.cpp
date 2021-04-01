// -----------------------------------------------------------------------------------
// Controls for PEC

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../debug/Debug.h"

#include "../locales/Locale.h"
#include "../commands/Commands.h"
#include "../status/MountStatus.h"
#include "../wifiServers/WifiServers.h"
#include "../ethernetServers/ethernetServers.h"
#include "../encoders/encoders.h"

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#include "PEC.h"

void processPecGet();

#if OPERATIONAL_MODE != WIFI
void handlePec(EthernetClient *client) {
#else
void handlePec() {
#endif
  char temp[120] = "";

  Ser.setTimeout(webTimeout);
  serialRecvFlush();
      
  mountStatus.update();

  processPecGet();

  sendHtmlStart();

  // send a standard http response header
  String data=FPSTR(html_headB);
  data += FPSTR(html_main_cssB);
  data += FPSTR(html_main_css1);
  data += FPSTR(html_main_css2);
  data += FPSTR(html_main_css3);
  data += FPSTR(html_main_css4);
  data += FPSTR(html_main_css5);
  data += FPSTR(html_main_css6);
  sendHtml(data);
  data += FPSTR(html_main_css7);
  data += FPSTR(html_main_css8);
  data += FPSTR(html_main_css_control1);
  data += FPSTR(html_main_css_control2);
  data += FPSTR(html_main_css_control3);
  data += FPSTR(html_main_cssE);
  data += FPSTR(html_headE);
  data += FPSTR(html_bodyB);
  sendHtml(data);

  if (mountStatus.pecEnabled()) {
    // active ajax page is: pecAjax();
    data +="<script>var ajaxPage='pec.txt';</script>\n";
    data +=FPSTR(html_ajax_active);
    data +="<script>auto2Rate=2;</script>";
  }

  // finish the standard http response header
  data += FPSTR(html_onstep_header1); data += "OnStep";
  data += FPSTR(html_onstep_header2);
  if (mountStatus.getVersionStr(temp)) data += temp; else data += "?";
  data += FPSTR(html_onstep_header3);
  data += FPSTR(html_linksStatN);
  data += FPSTR(html_linksCtrlN);
  if (mountStatus.featureFound()) data += FPSTR(html_linksAuxN);
  data += FPSTR(html_linksLibN);
  #if ENCODERS == ON
    data += FPSTR(html_linksEncN);
  #endif
  sendHtml(data);
  data += FPSTR(html_linksPecS);
  data += FPSTR(html_linksSetN);
  data += FPSTR(html_linksCfgN);
  data += FPSTR(html_linksSetupN);
  data += FPSTR(html_onstep_header4);
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data+= FPSTR(html_bad_comms_message); sendHtml(data); sendHtmlDone(data); return; }

  data += FPSTR(html_pec1);

  if (mountStatus.pecEnabled()) {
    data += FPSTR(html_pec2);
    data += FPSTR(html_pecControls0);
    data += FPSTR(html_pecControls1);
    data += FPSTR(html_pecControls2);
    data += FPSTR(html_pecControls3);
    data += FPSTR(html_pecControls4);
    data += FPSTR(html_pecControls5);
    data += "</div></body></html>";
  } else {
    data += L_PEC_NO_PEC_MESSAGE;
    data += "</div><br class='clear' /></body></html>";
  }

  sendHtml(data);
  sendHtmlDone(data);
}

#if OPERATIONAL_MODE != WIFI
void pecAjax(EthernetClient *client) {
#else
void pecAjax() {
#endif
  String data="";
  char temp[80]="";
  
  data += "status|";
  if ((mountStatus.mountType()!=MT_ALTAZM) && (mountStatus.update()) && (command(":$QZ?#",temp))) {
    if (temp[0]=='I') data += L_PEC_IDLE; else
    if (temp[0]=='p') data += L_PEC_WAIT_PLAY; else
    if (temp[0]=='P') data += L_PEC_PLAYING; else
    if (temp[0]=='r') data += L_PEC_WAIT_REC; else
    if (temp[0]=='R') data += L_PEC_RECORDING; else data += L_PEC_UNK;
    if (mountStatus.pecRecording()) data += " (" L_PEC_EEWRITING ")";
  } else { data += "?"; }
  data += "\n";

#if OPERATIONAL_MODE != WIFI
  client->print(data);
#else
  server.send(200, "text/plain",data);
#endif
}

void processPecGet() {
  String v;

  // PEC control
  v=server.arg("pe");
  if (!v.equals(EmptyStr)) {
    if (v == "pl") commandBlind(":$QZ+#"); // play
    if (v == "st") commandBlind(":$QZ-#"); // stop
    if (v == "re") commandBlind(":$QZ/#"); // record
    if (v == "cl") commandBlind(":$QZZ#"); // clear
    if (v == "wr") commandBlind(":$QZ!#"); // write to eeprom
  }
}
