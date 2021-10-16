// -----------------------------------------------------------------------------------
// Telescope auxiliary feature related functions 

#include "Auxiliary.h"

void processAuxGet();

void handleAux() {
  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  mountStatus.update(true);

  char temp[240] = "";
  char temp1[80] = "";

  processAuxGet();

  sendHtmlStart();
  
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
  data.concat(FPSTR(html_main_css_btns1));
  sendHtml(data);
  data.concat(FPSTR(html_main_css_btns2));
  data.concat(FPSTR(html_main_css_btns3));
  data.concat(FPSTR(html_main_cssE));
  data.concat(FPSTR(html_headE));
  data.concat(FPSTR(html_bodyB));
  sendHtml(data);

  // finish the standard http response header
  data.concat(FPSTR(html_onstep_header1)); data.concat("OnStep");
  data.concat(FPSTR(html_onstep_header2)); data.concat(firmwareVersion.str);
  data.concat(" (OnStep");
  if (mountStatus.getVersionStr(temp)) data.concat(temp); else data.concat("?");
  data.concat(FPSTR(html_onstep_header3));
  data.concat(FPSTR(html_linksStatN));
  data.concat(FPSTR(html_linksCtrlN));
  if (mountStatus.featureFound()) data.concat(FPSTR(html_linksAuxS));
  data.concat(FPSTR(html_linksLibN));
  #if ENCODERS == ON
    data.concat(FPSTR(html_linksEncN));
  #endif
  sendHtml(data);
  if (mountStatus.pecEnabled()) data.concat(FPSTR(html_linksPecN));
  data.concat(FPSTR(html_linksSetN));
  data.concat(FPSTR(html_linksCfgN));
  data.concat(FPSTR(html_linksSetupN));
  data.concat(FPSTR(html_onstep_header4));
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(data); return; }

  // scripts
  sprintf_P(temp, html_ajaxScript, "auxiliaryA.txt"); data.concat(temp);
  data.concat(FPSTR(html_ajaxScriptShort));

  // active ajax page is: auxAjax();
  data +="<script>var ajaxPage='auxiliary.txt';</script>\n";
  data +=FPSTR(html_ajax_active);
  sendHtml(data);

  // Auxiliary Features --------------------------------------
  int j = 0;
  if (mountStatus.featureFound()) {
    data.concat(FPSTR(html_auxAuxB));

    for (int i=0; i<8; i++) {
      mountStatus.selectFeature(i);

      if (mountStatus.featurePurpose() != 0 && j > 0) {
        data.concat(F("<br/><div style='float: left; width: 26em'><hr></div>\r\n"));
      }
      if (mountStatus.featurePurpose() == SWITCH) {
        data.concat(F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;"));
        data.concat(mountStatus.featureName());
        data.concat("&bull;");
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        sprintf_P(temp,html_auxOnSwitch,i+1,i+1); data.concat(temp);
        sprintf_P(temp,html_auxOffSwitch,i+1,i+1); data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        data.concat("</div>\r\n");
        sendHtml(data);
        j++;
      } else
      if (mountStatus.featurePurpose() == ANALOG_OUTPUT) {
        data.concat(F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;"));
        data.concat(mountStatus.featureName());
        data.concat("&bull;");
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        data.concat(FPSTR(html_auxAnalog));
        sprintf(temp,"%d' onchange=\"sf('x%dv1',this.value)\">",mountStatus.featureValue1(),i+1);
        data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        sprintf(temp,"<span id='x%dv1'>%d</span>%%",i+1,(int)lround((mountStatus.featureValue1()/255.0)*100.0));
        data.concat(temp);
        data.concat("</div>\r\n");
        sendHtml(data);
        j++;
      } else
      if (mountStatus.featurePurpose() == DEW_HEATER) {
        data.concat(F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;"));
        data.concat(mountStatus.featureName());
        data.concat("&bull;");
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        sprintf_P(temp,html_auxOnSwitch,i+1,i+1); data.concat(temp);
        sprintf_P(temp,html_auxOffSwitch,i+1,i+1); data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        dtostrf(mountStatus.featureValue4(),3,1,temp1);
        sprintf(temp,"&Delta;<span id='x%dv4'>%s</span>&deg;C\r\n",i+1,temp1);
        data.concat(temp);
        data.concat("</div>\r\n");

        data.concat(F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>"));
        data.concat(L_ZERO " (100% " L_POWER ")");
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        data.concat(FPSTR(html_auxHeater));
        sprintf(temp,"%d' onchange=\"sf('x%dv2',this.value)\">",(int)lround(mountStatus.featureValue2()*10.0),i+1);
        data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        dtostrf(mountStatus.featureValue2(),3,1,temp1);
        sprintf(temp,"<span id='x%dv2'>%s</span>&deg;C\r\n",i+1,temp1);
        data.concat(temp);
        data.concat("</div>\r\n");
        
        data.concat(F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>"));
        data.concat(L_SPAN " (0% " L_POWER ")");
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        data.concat(FPSTR(html_auxHeater));
        sprintf(temp,"%d' onchange=\"sf('x%dv3',this.value)\">",(int)lround(mountStatus.featureValue3()*10.0),i+1);
        data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        dtostrf(mountStatus.featureValue3(),3,1,temp1);
        sprintf(temp,"<span id='x%dv3'>%s</span>&deg;C\r\n",i+1,temp1);
        data.concat(temp);
        data.concat("</div>\r\n");

        sendHtml(data);
        j++;
      } else
      if (mountStatus.featurePurpose() == INTERVALOMETER) {
        data.concat(F("<div style='float: left; width: 8em; height: 2em; line-height: 2em'>&bull;"));
        data.concat(mountStatus.featureName());
        data.concat("&bull;");
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        data.concat(FPSTR(html_auxStartStop1));
        sprintf(temp,"x%dv1",i+1);
        data.concat(temp);
        data.concat(FPSTR(html_auxStartStop2));
        sprintf(temp,"x%dv1",i+1);
        data.concat(temp);
        data.concat(FPSTR(html_auxStartStop3));
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        sprintf(temp,"<span id='x%dv1'>-</span>\r\n",i+1);
        data.concat(temp);
        data.concat("</div>\r\n");

        data.concat(F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>"));
        data.concat(L_CAMERA_COUNT);
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        data.concat(FPSTR(html_auxCount));
        sprintf(temp,"%d' onchange=\"sf('x%dv4',this.value)\">",(int)mountStatus.featureValue4(),i+1);
        data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        dtostrf(mountStatus.featureValue4(),0,0,temp1);
        sprintf(temp,"<span id='x%dv4'>%s</span> x\r\n",i+1,temp1);
        data.concat(temp);
        data.concat("</div>\r\n");

        data.concat(F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>"));
        data.concat(L_CAMERA_EXPOSURE);
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        data.concat(FPSTR(html_auxExposure));
        sprintf(temp,"%d' onchange=\"sf('x%dv2',this.value)\">",(int)timeToByte(mountStatus.featureValue2()),i+1);
        data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        float v; int d;
        v=mountStatus.featureValue2(); if (v < 1.0) d=3; else if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,temp1);
        sprintf(temp,"<span id='x%dv2'>%s</span> sec\r\n",i+1,temp1);
        data.concat(temp);
        data.concat("</div>\r\n");

        data.concat(F("<div style='float: left; text-align: right; width: 8em; height: 2em; line-height: 2em'>"));
        data.concat(L_CAMERA_DELAY);
        data.concat(F("</div><div style='float: left; width: 14em; height: 2em; line-height: 2em'>"));
        data.concat(FPSTR(html_auxDelay));
        sprintf(temp,"%d' onchange=\"sf('x%dv3',this.value)\">",(int)timeToByte(mountStatus.featureValue3()),i+1);
        data.concat(temp);
        data.concat(F("</div><div style='float: left; width: 4em; height: 2em; line-height: 2em'>"));
        v=mountStatus.featureValue3(); if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,temp1);
        sprintf(temp,"<span id='x%dv3'>%s</span> sec\r\n",i+1,temp1);
        data.concat(temp);
        data.concat("</div>\r\n");

        sendHtml(data);
        j++;
      }
    }

    data.concat(FPSTR(html_auxAuxE));
  }

  data.concat(FPSTR(html_auxEnd));
  
  data.concat("</div></body></html>");

  sendHtml(data);
  sendHtmlDone(data);
}

void auxAjaxGet() {
  processAuxGet();
  #if OPERATIONAL_MODE != WIFI
    www.sendContent("");
  #else
    www.send(200, "text/html", "");
  #endif
}

void auxAjax() {
  String data="";
  char temp[120]="";

  // update auxiliary feature values
  if (mountStatus.featureFound()) {
    mountStatus.featureUpdate();
    for (int i=0; i<8; i++) {
      mountStatus.selectFeature(i);
      if (mountStatus.featurePurpose() == SWITCH) {
        if (mountStatus.featureValue1() == 0) {
          sprintf(temp,"sw%d_on|%s\n",i+1,"enabled"); data.concat(temp);
          sprintf(temp,"sw%d_off|%s\n",i+1,"disabled"); data.concat(temp);
        } else {
          sprintf(temp,"sw%d_on|%s\n",i+1,"disabled"); data.concat(temp);
          sprintf(temp,"sw%d_off|%s\n",i+1,"enabled"); data.concat(temp);
        }
      } else
      if (mountStatus.featurePurpose() == ANALOG_OUTPUT) {
        sprintf(temp,"x%dv1|%d\n",i+1,(int)lround((mountStatus.featureValue1()/255.0)*100.0)); data.concat(temp);
      } else
      if (mountStatus.featurePurpose() == DEW_HEATER) {
        char s[40];
        if (mountStatus.featureValue1() == 0) {
          sprintf(temp,"sw%d_on|%s\n",i+1,"enabled"); data.concat(temp);
          sprintf(temp,"sw%d_off|%s\n",i+1,"disabled"); data.concat(temp);
        } else {
          sprintf(temp,"sw%d_on|%s\n",i+1,"disabled"); data.concat(temp);
          sprintf(temp,"sw%d_off|%s\n",i+1,"enabled"); data.concat(temp);
        }
        dtostrf(mountStatus.featureValue2(),3,1,s); sprintf(temp,"x%dv2|%s\n",i+1,s); data.concat(temp);
        dtostrf(mountStatus.featureValue3(),3,1,s); sprintf(temp,"x%dv3|%s\n",i+1,s); data.concat(temp);
        dtostrf(mountStatus.featureValue4(),3,1,s); sprintf(temp,"x%dv4|%s\n",i+1,s); data.concat(temp);
      } else
      if (mountStatus.featurePurpose() == INTERVALOMETER) {
        char s[40];
        float v; int d;
        
        v=mountStatus.featureValue1();
        if (abs(v) < 0.001) sprintf(temp,"x%dv1|-\n",i+1); else sprintf(temp,"x%dv1|%d\n",i+1,(int)v); data.concat(temp);
        v=mountStatus.featureValue2(); if (v < 1.0) d=3; else if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); sprintf(temp,"x%dv2|%s\n",i+1,s); data.concat(temp);
        v=mountStatus.featureValue3(); if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); sprintf(temp,"x%dv3|%s\n",i+1,s); data.concat(temp);
        sprintf(temp,"x%dv4|%d\n",i+1,(int)mountStatus.featureValue4()); data.concat(temp);
      }
    }
  }

#if OPERATIONAL_MODE != WIFI
  sendHtmlDone(data);
#else
  www.send(200, "text/plain",data);
#endif
}

void processAuxGet() {
  String v;
  char temp[80] = "";
  char temp1[40] = "";

  // Auxiliary Feature set Value1 to Value4
  for (char c = '1'; c <= '8'; c++) {
    mountStatus.selectFeature(c - '1');
    sprintf(temp, "x%cv1", c); v = www.arg(temp);
    if (!v.equals(EmptyStr)) { sprintf(temp, ":SXX%c,V%s#", c, v.c_str()); onStep.commandBool(temp); }
    if (mountStatus.featurePurpose() == DEW_HEATER) {
      sprintf(temp, "x%cv2", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(v.toFloat()/10.0, 0, 1, temp1); sprintf(temp, ":SXX%c,Z%s#", c, temp1); onStep.commandBool(temp); }
      sprintf(temp, "x%cv3", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(v.toFloat()/10.0, 0, 1, temp1); sprintf(temp, ":SXX%c,S%s#", c, temp1); onStep.commandBool(temp); }
    } else
    if (mountStatus.featurePurpose() == INTERVALOMETER) {
      sprintf(temp, "x%cv2", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(byteToTime(v.toInt()), 0, 3, temp1); sprintf(temp, ":SXX%c,E%s#", c, temp1); onStep.commandBool(temp); }
      sprintf(temp, "x%cv3", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(byteToTime(v.toInt()), 0, 2, temp1); sprintf(temp, ":SXX%c,D%s#", c, temp1); onStep.commandBool(temp); }
      sprintf(temp, "x%cv4", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) { dtostrf(v.toFloat(), 0, 0, temp1); sprintf(temp, ":SXX%c,C%s#", c, temp1); onStep.commandBool(temp); }
    }
  }
}
