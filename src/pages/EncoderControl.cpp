// -----------------------------------------------------------------------------------
// Encoders

#include "EncoderControl.h"

#if ENCODERS == ON
  #include "../lib/convert/Convert.h"

  extern NVS nv;

  void processEncodersGet();

  void handleEncoders() {
    char temp[400] = "";
    char temp1[80] = "";
    char temp2[40] = "";

    SERIAL_ONSTEP.setTimeout(webTimeout);
    onStep.serialRecvFlush();
    
    mountStatus.update();

    processEncodersGet();

    sendHtmlStart();

    String data=FPSTR(html_headB);

    // active ajax page is: encAjax();
    data.concat("<script>var ajaxPage='enc.txt';</script>\n");
    data.concat(FPSTR(html_ajax_active));
    data.concat("<script>auto2Rate=2;</script>");
    sendHtml(data);

    // scripts
    sprintf_P(temp, html_ajaxScript, "encA.txt"); data.concat(temp);
    sendHtml(data);

    // send a standard http response header
    data.concat(FPSTR(html_main_cssB));
    data.concat(FPSTR(html_main_css1));
    data.concat(FPSTR(html_main_css2));
    data.concat(FPSTR(html_main_css3));
    data.concat(FPSTR(html_main_css4));
    data.concat(FPSTR(html_main_css5));
    sendHtml(data);
    data.concat(FPSTR(html_main_css6));
    data.concat(FPSTR(html_main_css7));
    data.concat(FPSTR(html_main_css8));
    data.concat(FPSTR(html_main_css_btns1));
    data.concat(FPSTR(html_main_css_btns2));
    data.concat(FPSTR(html_main_css_btns3));
    sendHtml(data);
    data.concat(FPSTR(html_main_css_collapse1));
    data.concat(FPSTR(html_main_css_collapse2));
    data.concat(FPSTR(html_main_cssE));
    data.concat(FPSTR(html_headE));
    data.concat(FPSTR(html_bodyB));
    sendHtml(data);

    // finish the standard http response header
    data.concat(FPSTR(html_onstep_header1)); data.concat("OnStep");
    data.concat(FPSTR(html_onstep_header2)); data.concat(firmwareVersion.str);
    data.concat(" (OnStep");
    if (mountStatus.getVersionStr(temp2)) data.concat(temp2); else data.concat("?");
    data.concat(FPSTR(html_onstep_header3));

    data.concat(FPSTR(html_linksStatN));
    data.concat(FPSTR(html_linksCtrlN));
    if (mountStatus.featureFound()) data.concat(FPSTR(html_linksAuxN));
    data.concat(FPSTR(html_linksLibN));
    data.concat(FPSTR(html_linksEncS));
    sendHtml(data);
    if (mountStatus.pecEnabled()) data.concat(FPSTR(html_linksPecN));
    data.concat(FPSTR(html_linksSetN));
    data.concat(FPSTR(html_linksCfgN));
    data.concat(FPSTR(html_linksSetupN));
    data.concat(FPSTR(html_onstep_header4));
    sendHtml(data);

    // OnStep wasn't found, show warning and info.
    if (!mountStatus.valid()) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(); return; }

    data.concat("<div style='width: 35em;'>");

    data.concat(FPSTR(html_syncOnStepNow));
    data.concat(FPSTR(html_syncEncodersNow));
    #ifdef ENC_HAS_ABSOLUTE
      data.concat(FPSTR(html_zeroEncodersNow));
    #endif
    data.concat("</form>");

    // Autosync
    data.concat(FPSTR(html_encEn1));
    data.concat(FPSTR(html_encEn2));
    sendHtml(data);

    // Encoder configuration section
    data.concat(L_ENC_CONF ":<br />");

    // Axis1 RA/Azm
    data.concat("<button type='button' class='collapsible'>Axis1 RA/Azm</button>");
    data.concat(FPSTR(html_encFormBegin));
    dtostrf(encoders.settings.axis1.ticksPerDeg,1,3,temp1); stripNum(temp1);
    sprintf_P(temp,html_encAxisTpd,temp1,1,1,100000L); data.concat(temp);         // Encoder counts per degree
    sprintf_P(temp,html_encAxisReverse,encoders.settings.axis1.reverse==ON?1:0,1); data.concat(temp); // Encode reverse count
    sprintf_P(temp,html_encMxAxis1,(long)encoders.settings.axis1.diffTo); data.concat(temp);      // Encoder sync thresholds
    sendHtml(data);
    data.concat("<button type='submit'>" L_UPLOAD "</button>\r\n");
    data.concat("<button name='revert' value='1' type='submit'>" L_REVERT "</button>");
    data.concat(FPSTR(html_encFormEnd));
    sendHtml(data);

    // Axis2 Dec/Alt
    data.concat("<button type='button' class='collapsible'>Axis2 Dec/Alt</button>");
    data.concat(FPSTR(html_encFormBegin));
    dtostrf(encoders.settings.axis2.ticksPerDeg,1,3,temp1); stripNum(temp1);
    sprintf_P(temp,html_encAxisTpd,temp1,2,1,100000L); data.concat(temp);         // Encoder counts per degree
    sprintf_P(temp,html_encAxisReverse,encoders.settings.axis2.reverse==ON?1:0,2); data.concat(temp); // Encode reverse count
    sprintf_P(temp,html_encMxAxis2,(long)encoders.settings.axis2.diffTo); data.concat(temp);      // Encoder sync thresholds
    sendHtml(data);
    data.concat("<button type='submit'>" L_UPLOAD "</button>\r\n");
    data.concat("<button name='revert' value='2' type='submit'>" L_REVERT "</button>");
    data.concat(FPSTR(html_encFormEnd));
    sendHtml(data);
    
    // end of page
    data.concat("<br />");
    
    // collapsible script
    data.concat(FPSTR(html_collapseScript));
    
    strcpy(temp,"</div></div></body></html>");
    data.concat(temp);

    sendHtml(data);
    sendHtmlDone();
  }

  void encAjax() {
    String data = "";
    
    data.concat("eas_on|");  if (encoders.autoSync) data.concat("disabled\n"); else data.concat("enabled\n");
    data.concat("eas_off|"); if (encoders.autoSync) data.concat("enabled\n"); else data.concat("disabled\n");

    sendHtml(data);
    sendHtmlDone();
  }

  void encAjaxGet() {
    processEncodersGet();

    #if OPERATIONAL_MODE != WIFI
      www.sendContent("");
    #else
      www.send(200, "text/html", "");
    #endif
  }

  void processEncodersGet() {
    String v;

    v = www.arg("ms");
    if (!v.equals(EmptyStr)) {
      if (v.equals("ons")) encoders.syncToOnStep();
      if (v.equals("enc")) encoders.syncFromOnStep();
      #ifdef ENC_HAS_ABSOLUTE
        if (v.equals("zro")) encoders.zeroFromOnStep();
      #endif
    }
    
    bool updateNV = false;

    // Autosync
    v = www.arg("as");
    if (!v.equals(EmptyStr)) {
      if (v.equals("on")) {
        encoders.autoSync = true;
        updateNV = true;
      }
      if (v.equals("off")) {
        encoders.autoSync = false;
        updateNV = true;
      }
    }

    // Max. limits
    v = www.arg("d1");
    if (!v.equals(EmptyStr)) {
      int16_t i;
      if (convert.atoi2((char*)v.c_str(), &i) && (i >= 0 && i <= 9999)) { 
        encoders.settings.axis1.diffTo = i;
        updateNV = true;
      }
    }
    v = www.arg("d2");
    if (!v.equals(EmptyStr)) {
      int16_t i;
      if (convert.atoi2((char*)v.c_str(), &i) && (i >= 0 && i <= 9999)) { 
        encoders.settings.axis2.diffTo=i;
        updateNV = true;
      }
    }

    // Counts per degree
    v = www.arg("a1cpd");
    if (!v.equals(EmptyStr)) {
      double d = v.toFloat();
      if (d >= 1.0 && d <= 1000000.0) { 
        encoders.settings.axis1.ticksPerDeg = d;
        updateNV = true;
      }
    }
    v = www.arg("a2cpd");
    if (!v.equals(EmptyStr)) {
      double d = v.toFloat();
      if (d >= 1.0 && d <= 1000000.0) { 
        encoders.settings.axis2.ticksPerDeg = d;
        updateNV = true;
      }
    }

    v = www.arg("a1rev");
    if (!v.equals(EmptyStr)) {
      if (v.equals("0")) {
        encoders.settings.axis1.reverse = OFF;
        updateNV = true;
      }
      if (v.equals("1")) {
        encoders.settings.axis1.reverse=ON;
        updateNV = true;
      }
    }

    v = www.arg("a2rev");
    if (!v.equals(EmptyStr)) {
      if (v.equals("0")) {
        encoders.settings.axis2.reverse = OFF;
        updateNV = true;
      }
      if (v.equals("1")) {
        encoders.settings.axis2.reverse=ON;
        updateNV = true;
      }
    }

    v = www.arg("revert");
    if (!v.equals(EmptyStr)) { 
      if (v.equals("1")) {
        encoders.settings.axis1.ticksPerDeg = AXIS1_ENC_TICKS_DEG;
        encoders.settings.axis1.reverse = AXIS1_ENC_REVERSE;
        encoders.settings.axis1.diffTo = AXIS1_ENC_DIFF_LIMIT_TO;
        updateNV = true;
      }
      if (v.equals("2")) {
        encoders.settings.axis2.ticksPerDeg = AXIS2_ENC_TICKS_DEG;
        encoders.settings.axis2.reverse = AXIS1_ENC_REVERSE;
        encoders.settings.axis2.diffTo = AXIS2_ENC_DIFF_LIMIT_TO;
        updateNV = true;
      }
    }

    if (updateNV) {
      nv.writeBytes(NV_ENCODER_SETTINGS_BASE, &encoders.settings, sizeof(EncoderSettings));
    }
  }
#endif
