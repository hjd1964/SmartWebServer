// -----------------------------------------------------------------------------------
// Telescope auxiliary feature related functions 

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../debug/Debug.h"

#include "../locales/Locale.h"
#include "../misc/Misc.h"
#include "../commands/Commands.h"
#include "../status/MountStatus.h"
#include "../wifiServers/WifiServers.h"
#include "../ethernetServers/EthernetServers.h"
#include "../encoders/Encoders.h"

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#include "Auxiliary.h"

void processAuxGet();

#if OPERATIONAL_MODE != WIFI
void handleAux(EthernetClient *client) {
#else
void handleAux() {
#endif
  Ser.setTimeout(webTimeout);
  serialRecvFlush();

  mountStatus.update(true);
    
  char temp[240]="";
  char temp1[80]="";

  processAuxGet();

  sendHtmlStart();
  
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
  data += FPSTR(html_main_css_btns1);
  sendHtml(data);
  data += FPSTR(html_main_css_btns2);
  data += FPSTR(html_main_css_btns3);
  data += FPSTR(html_main_cssE);
  data += FPSTR(html_headE);
  data += FPSTR(html_bodyB);
  sendHtml(data);

  // finish the standard http response header
  data += FPSTR(html_onstep_header1); data += "OnStep";
  data += FPSTR(html_onstep_header2);
  if (mountStatus.getVersionStr(temp)) data += temp; else data += "?";
  data += FPSTR(html_onstep_header3);
  data += FPSTR(html_linksStatN);
  data += FPSTR(html_linksCtrlN);
  if (mountStatus.featureFound()) data += FPSTR(html_linksAuxS);
  data += FPSTR(html_linksLibN);
  #if ENCODERS == ON
    data += FPSTR(html_linksEncN);
  #endif
  sendHtml(data);
  data += FPSTR(html_linksPecN);
  data += FPSTR(html_linksSetN);
  data += FPSTR(html_linksCfgN);
  data += FPSTR(html_linksSetupN);
  data += FPSTR(html_onstep_header4);
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data += FPSTR(html_bad_comms_message); sendHtml(data); sendHtmlDone(data); return; }

  // scripts
  sprintf_P(temp, html_ajaxScript, "auxiliaryA.txt"); data += temp;
  data += FPSTR(html_ajaxScriptShort);

  // active ajax page is: auxAjax();
  data +="<script>var ajaxPage='auxiliary.txt';</script>\n";
  data +=FPSTR(html_ajax_active);
  sendHtml(data);

  // Auxiliary Features --------------------------------------
  int j = 0;
  if (mountStatus.featureFound()) {
    data += FPSTR(html_auxAuxB);

    for (int i=0; i<8; i++) {
      mountStatus.selectFeature(i);

      if (mountStatus.featurePurpose() != 0 && j > 0) {
        data += F("<br/><div style='float: left; width: 26em'><hr></div>\r\n");
      }
      if (mountStatus.featurePurpose() == SWITCH) {
        data += F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;");
        data += mountStatus.featureName();
        data += "&bull;";
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        sprintf_P(temp,html_auxOnSwitch,i+1,i+1); data += temp;
        sprintf_P(temp,html_auxOffSwitch,i+1,i+1); data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        data += "</div>\r\n";
        sendHtml(data);
        j++;
      } else
      if (mountStatus.featurePurpose() == ANALOG_OUTPUT) {
        data += F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;");
        data += mountStatus.featureName();
        data += "&bull;";
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        data += FPSTR(html_auxAnalog);
        sprintf(temp,"%d' onchange=\"sf('x%dv1',this.value)\">",mountStatus.featureValue1(),i+1);
        data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        sprintf(temp,"<span id='x%dv1'>%d</span>%%",i+1,(int)lround((mountStatus.featureValue1()/255.0)*100.0));
        data += temp;
        data += "</div>\r\n";
        sendHtml(data);
        j++;
      } else
      if (mountStatus.featurePurpose() == DEW_HEATER) {
        data += F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;");
        data += mountStatus.featureName();
        data += "&bull;";
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        sprintf_P(temp,html_auxOnSwitch,i+1,i+1); data += temp;
        sprintf_P(temp,html_auxOffSwitch,i+1,i+1); data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        dtostrf(mountStatus.featureValue4(),3,1,temp1);
        sprintf(temp,"&Delta;<span id='x%dv4'>%s</span>&deg;C\r\n",i+1,temp1);
        data += temp;
        data += "</div>\r\n";

        data += F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>");
        data += L_ZERO " (100% " L_POWER ")";
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        data += FPSTR(html_auxHeater);
        sprintf(temp,"%d' onchange=\"sf('x%dv2',this.value)\">",(int)lround(mountStatus.featureValue2()*10.0),i+1);
        data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        dtostrf(mountStatus.featureValue2(),3,1,temp1);
        sprintf(temp,"<span id='x%dv2'>%s</span>&deg;C\r\n",i+1,temp1);
        data += temp;
        data += "</div>\r\n";
        
        data += F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>");
        data += L_SPAN " (0% " L_POWER ")";
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        data += FPSTR(html_auxHeater);
        sprintf(temp,"%d' onchange=\"sf('x%dv3',this.value)\">",(int)lround(mountStatus.featureValue3()*10.0),i+1);
        data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        dtostrf(mountStatus.featureValue3(),3,1,temp1);
        sprintf(temp,"<span id='x%dv3'>%s</span>&deg;C\r\n",i+1,temp1);
        data += temp;
        data += "</div>\r\n";

        sendHtml(data);
        j++;
      } else
      if (mountStatus.featurePurpose() == INTERVALOMETER) {
        data += F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;");
        data += mountStatus.featureName();
        data += "&bull;";
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        data += FPSTR(html_auxStartStop1);
        sprintf(temp,"x%dv1",i+1);
        data += temp;
        data += FPSTR(html_auxStartStop2);
        sprintf(temp,"x%dv1",i+1);
        data += temp;
        data += FPSTR(html_auxStartStop3);
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        sprintf(temp,"<span id='x%dv1'>-</span>\r\n",i+1);
        data += temp;
        data += "</div>\r\n";

        data += F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>");
        data += L_CAMERA_COUNT;
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        data += FPSTR(html_auxCount);
        sprintf(temp,"%d' onchange=\"sf('x%dv4',this.value)\">",(int)mountStatus.featureValue4(),i+1);
        data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        dtostrf(mountStatus.featureValue4(),0,0,temp1);
        sprintf(temp,"<span id='x%dv4'>%s</span> x\r\n",i+1,temp1);
        data += temp;
        data += "</div>\r\n";

        data += F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>");
        data += L_CAMERA_EXPOSURE;
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        data += FPSTR(html_auxExposure);
        sprintf(temp,"%d' onchange=\"sf('x%dv2',this.value)\">",(int)timeToByte(mountStatus.featureValue2()),i+1);
        data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        float v; int d;
        v=mountStatus.featureValue2(); if (v < 1.0) d=3; else if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,temp1);
        sprintf(temp,"<span id='x%dv2'>%s</span> sec\r\n",i+1,temp1);
        data += temp;
        data += "</div>\r\n";

        data += F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>");
        data += L_CAMERA_DELAY;
        data += F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>");
        data += FPSTR(html_auxDelay);
        sprintf(temp,"%d' onchange=\"sf('x%dv3',this.value)\">",(int)timeToByte(mountStatus.featureValue3()),i+1);
        data += temp;
        data += F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>");
        v=mountStatus.featureValue3(); if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,temp1);
        sprintf(temp,"<span id='x%dv3'>%s</span> sec\r\n",i+1,temp1);
        data += temp;
        data += "</div>\r\n";

        sendHtml(data);
        j++;
      }
    }

    data += FPSTR(html_auxAuxE);
  }

  data += FPSTR(html_auxEnd);
  
  data += "</div></body></html>";

  sendHtml(data);
  sendHtmlDone(data);
}

#if OPERATIONAL_MODE != WIFI
void auxAjaxGet(EthernetClient *client) {
#else
void auxAjaxGet() {
#endif
  processAuxGet();
#if OPERATIONAL_MODE != WIFI
  client->print("");
#else
  server.send(200, "text/html","");
#endif
}

#if OPERATIONAL_MODE != WIFI
void auxAjax(EthernetClient *client) {
#else
void auxAjax() {
#endif
  String data="";
  char temp[120]="";

  // update auxiliary feature values
  if (mountStatus.featureFound()) {
    mountStatus.featureUpdate();
    for (int i=0; i<8; i++) {
      mountStatus.selectFeature(i);
      if (mountStatus.featurePurpose() == SWITCH) {
        if (mountStatus.featureValue1() == 0) {
          sprintf(temp,"sw%d_on|%s\n",i+1,"enabled"); data += temp;
          sprintf(temp,"sw%d_off|%s\n",i+1,"disabled"); data += temp;
        } else {
          sprintf(temp,"sw%d_on|%s\n",i+1,"disabled"); data += temp;
          sprintf(temp,"sw%d_off|%s\n",i+1,"enabled"); data += temp;
        }
      } else
      if (mountStatus.featurePurpose() == ANALOG_OUTPUT) {
        sprintf(temp,"x%dv1|%d\n",i+1,(int)lround((mountStatus.featureValue1()/255.0)*100.0)); data += temp;
      } else
      if (mountStatus.featurePurpose() == DEW_HEATER) {
        char s[20];
        if (mountStatus.featureValue1() == 0) {
          sprintf(temp,"sw%d_on|%s\n",i+1,"enabled"); data += temp;
          sprintf(temp,"sw%d_off|%s\n",i+1,"disabled"); data += temp;
        } else {
          sprintf(temp,"sw%d_on|%s\n",i+1,"disabled"); data += temp;
          sprintf(temp,"sw%d_off|%s\n",i+1,"enabled"); data += temp;
        }
        dtostrf(mountStatus.featureValue2(),3,1,s); sprintf(temp,"x%dv2|%s\n",i+1,s); data += temp;
        dtostrf(mountStatus.featureValue3(),3,1,s); sprintf(temp,"x%dv3|%s\n",i+1,s); data += temp;
        dtostrf(mountStatus.featureValue4(),3,1,s); sprintf(temp,"x%dv4|%s\n",i+1,s); data += temp;
      } else
      if (mountStatus.featurePurpose() == INTERVALOMETER) {
        char s[20];
        float v; int d;
        
        v=mountStatus.featureValue1();
        if (abs(v) < 0.001) sprintf(temp,"x%dv1|-\n",i+1); else sprintf(temp,"x%dv1|%d\n",i+1,(int)v); data += temp;
        v=mountStatus.featureValue2(); if (v < 1.0) d=3; else if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); sprintf(temp,"x%dv2|%s\n",i+1,s); data += temp;
        v=mountStatus.featureValue3(); if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); sprintf(temp,"x%dv3|%s\n",i+1,s); data += temp;
        sprintf(temp,"x%dv4|%d\n",i+1,(int)mountStatus.featureValue4()); data += temp;
      }
    }
  }

#if OPERATIONAL_MODE != WIFI
  client->print(data);
#else
  server.send(200, "text/plain",data);
#endif
}

void processAuxGet() {
  String v;
  char temp[40] = "";
  char temp1[40] = "";

  // Auxiliary Feature set Value1 to Value4
  for (char c = '1'; c <= '8'; c++) {
    mountStatus.selectFeature(c - '1');
    sprintf(temp, "x%cv1", c); v=server.arg(temp);
    if (!v.equals(EmptyStr)) { sprintf(temp, ":SXX%c,V%s#", c, v.c_str()); commandBool(temp); }
    if (mountStatus.featurePurpose() == DEW_HEATER) {
      sprintf(temp, "x%cv2", c); v = server.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(v.toFloat()/10.0, 0, 1, temp1); sprintf(temp, ":SXX%c,Z%s#", c, temp1); commandBool(temp); }
      sprintf(temp, "x%cv3", c); v = server.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(v.toFloat()/10.0, 0, 1, temp1); sprintf(temp, ":SXX%c,S%s#", c, temp1); commandBool(temp); }
    } else
    if (mountStatus.featurePurpose() == INTERVALOMETER) {
      sprintf(temp, "x%cv2", c); v = server.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(byteToTime(v.toInt()), 0, 3, temp1); sprintf(temp, ":SXX%c,E%s#", c, temp1); commandBool(temp); }
      sprintf(temp, "x%cv3", c); v = server.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(byteToTime(v.toInt()), 0, 2, temp1); sprintf(temp, ":SXX%c,D%s#", c, temp1); commandBool(temp); }
      sprintf(temp, "x%cv4", c); v = server.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(v.toFloat(), 0, 0, temp1); sprintf(temp, ":SXX%c,C%s#", c, temp1); commandBool(temp); }
    }
  }
}
