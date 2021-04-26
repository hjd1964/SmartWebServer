// -----------------------------------------------------------------------------------
// Encoders

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"
extern NVS nv;
#include "../debug/Debug.h"

#include "../encoders/Encoders.h"
#if ENCODERS == ON
  #include "../locales/Locale.h"
  #include "../misc/Misc.h"
  #include "../commands/Commands.h"
  #include "../status/MountStatus.h"
  #include "../wifiServers/WifiServers.h"
  #include "../ethernetServers/EthernetServers.h"

  #include "htmlHeaders.h"
  #include "htmlMessages.h"
  #include "htmlScripts.h"

  #include "EncoderControl.h"

  void processEncodersGet();

  #if OPERATIONAL_MODE != WIFI
  void handleEncoders(EthernetClient *client) {
  #else
  void handleEncoders() {
  #endif
    char temp[400] = "";
    char temp1[80] = "";

    SERIAL_ONSTEP.setTimeout(webTimeout);
    serialRecvFlush();
    
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

    #if AXIS1_ENC_RATE_CONTROL == ON
      data.concat(html_encScript2);
      sendHtml(data);
    #endif

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
    {
      char temp1[40]="";
      data.concat(FPSTR(html_onstep_header1)); data.concat("OnStep");
      data.concat(FPSTR(html_onstep_header2));
      if (mountStatus.getVersionStr(temp1)) data.concat(temp1); else data.concat("?");
      data.concat(FPSTR(html_onstep_header3));
    }
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
    if (!mountStatus.valid()) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(data); return; }

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

    #if AXIS1_ENC_RATE_CONTROL == ON
      // OnStep rate control
      data.concat("<br />");
      data.concat(FPSTR(html_encRateEn1));
      data.concat(FPSTR(html_encRateEn2));

      // Encoder averaging (integration) samples
      sprintf_P(temp, html_encStaAxis1, Axis1EncStaSamples);
      data.concat(temp);
      sprintf_P(temp, html_encLtaAxis1, Axis1EncLtaSamples);
      data.concat(temp);
      sendHtml(data);

      // Encoder poportional response
      sprintf_P(temp, html_encPropAxis1, Axis1EncProp);
      data.concat(temp);

      // Encoder minimum guide
      sprintf_P(temp, html_encMinGuideAxis1, Axis1EncMinGuide);
      data.concat(temp);

      // Encoder rate compensation
      #if AXIS1_ENC_RATE_AUTO == OFF
        long l=round(axis1EncRateComp*1000000.0);
        sprintf_P(temp,html_encErc2Axis1,l);
        data.concat(temp);
      #endif

      #if AXIS1_ENC_INTPOL_COS == ON
        // Encoder interpolation compensation
        sprintf_P(temp,html_encIntPolPhaseAxis1,Axis1EncIntPolPhase);
        data.concat(temp;

        sprintf_P(temp,html_encIntPolMagAxis1,Axis1EncIntPolMag);
        data.concat(temp;
      #endif
      sendHtml(data);

      // Encoder status display
      sprintf(temp,L_ENC_STAT_RATE_AXIS1 ":<br />");
      data.concat(temp);
      sprintf(temp,"&nbsp; OnStep = <span id='stO'>?</span><br />");
      data.concat(temp);
      #if AXIS1_ENC_INTPOL_COS == ON
        sprintf(temp,"&nbsp; " L_ENC_STAT_INTPOL_COMP " = <span id='ipC'>?</span><br />");
        data.concat(temp;
        sprintf(temp,"&nbsp; " L_ENC_STAT_INTPOL_PHASE " = <span id='ipP'>?</span><br />");
        data.concat(temp;
      #endif
      #if AXIS1_ENC_RATE_AUTO > 0
        sprintf(temp,"&nbsp; " L_ENCODER " ARC = <span id='erA'>?</span><br />");
        data.concat(temp;
      #endif
      sprintf(temp,"&nbsp; " L_ENCODER " STA = <span id='stS'>?</span> x<br />");
      data.concat(temp);
      sprintf(temp,"&nbsp; " L_ENCODER " LTA = <span id='stL'>?</span> x<br />");
      data.concat(temp);
      sprintf(temp,"&nbsp; " L_DELTA " &nbsp;= <span id='stD'>?</span><br />");
      data.concat(temp);
      sprintf(temp,"&nbsp; " L_GUIDE " &nbsp;= <span id='rtF'>?</span><br />");
      data.concat(temp);

      sendHtml(data);

      data.concat("<br /><canvas id='myCanvas' width='600' height='300' style='margin-left: -2px; border:2px solid #999999;'></canvas>");
      data.concat("&nbsp; " L_CENTER ": OnStep " L_RATE ", " L_BLUE ": STA (" L_RANGE " &#xb1;0.1), " L_GREEN ": LTA (" L_RANGE " &#xb1;0.01)<br />");

      data.concat(FPSTR(html_encSweepEn1));
      data.concat(FPSTR(html_encSweepEn2));

      encoders.clearAverages();
    #endif

    // Encoder configuration section
    data.concat(L_ENC_CONF ":<br />");

    // Axis1 RA/Azm
    data.concat("<button type='button' class='collapsible'>Axis1 RA/Azm</button>");
    data.concat(FPSTR(html_encFormBegin));
    dtostrf(Axis1EncTicksPerDeg,1,3,temp1); stripNum(temp1);
    sprintf_P(temp,html_encAxisTpd,temp1,1,1,100000L); data.concat(temp);         // Encoder counts per degree
    sprintf_P(temp,html_encAxisReverse,Axis1EncRev==ON?1:0,1); data.concat(temp); // Encode reverse count
    sprintf_P(temp,html_encMxAxis1,(long)Axis1EncDiffTo); data.concat(temp);      // Encoder sync thresholds
    sendHtml(data);
    data.concat("<button type='submit'>" L_UPLOAD "</button>\r\n");
    data.concat("<button name='revert' value='1' type='submit'>" L_REVERT "</button>");
    data.concat(FPSTR(html_encFormEnd));
    sendHtml(data);

    // Axis2 Dec/Alt
    data.concat("<button type='button' class='collapsible'>Axis2 Dec/Alt</button>");
    data.concat(FPSTR(html_encFormBegin));
    dtostrf(Axis2EncTicksPerDeg,1,3,temp1); stripNum(temp1);
    sprintf_P(temp,html_encAxisTpd,temp1,2,1,100000L); data.concat(temp);         // Encoder counts per degree
    sprintf_P(temp,html_encAxisReverse,Axis2EncRev==ON?1:0,2); data.concat(temp); // Encode reverse count
    sprintf_P(temp,html_encMxAxis2,(long)Axis2EncDiffTo); data.concat(temp);      // Encoder sync thresholds
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

    sendHtmlDone(data);
  }

  #if OPERATIONAL_MODE != WIFI
  void encAjax(EthernetClient *client) {
  #else
  void encAjax() {
  #endif
    String data = "";
    
    #if AXIS1_ENC_RATE_CONTROL == ON
      char temp[20]="";
      data.concat("stO|"); sprintf(temp,"%+1.4f\n",axis1Rate); data.concat(temp);
      data.concat("stD|"); sprintf(temp,"%+1.4f\n",axis1Rate - axis1EncRateSta); data.concat(temp);
      data.concat("stS|"); sprintf(temp,"%+1.4f\n",axis1EncRateSta); data.concat(temp);
      data.concat("stL|"); sprintf(temp,"%+1.4f\n",axis1EncRateLta); data.concat(temp);
      #if AXIS1_ENC_INTPOL_COS == ON
        data.concat("ipC|"); sprintf(temp,"%+1.4f\n",intpolComp); data.concat(temp);
        data.concat("ipP|"); sprintf(temp,"%d\n",(int)intpolPhase); data.concat(temp);
      #endif
      #if AXIS1_ENC_RATE_AUTO > 0
        data.concat("erA|"); sprintf(temp,"%+1.5f\n",axis1EncRateComp); data.concat(temp);
      #endif

      if (guideCorrectionMillis == 0) {
        data.concat("rtF|"); sprintf(temp,L_NONE "\n"); data.concat(temp);
      } else
      if (guideCorrectionMillis > 0) {
        data.concat("rtF|"); sprintf(temp,L_WEST " %ld ms\n",guideCorrectionMillis); data.concat(temp);
      } else
      if (guideCorrectionMillis < 0) {
        data.concat("rtF|"); sprintf(temp,L_EAST " %ld ms\n",-guideCorrectionMillis); data.concat(temp);
      }

      data.concat("orc|"); if (encRateControl) data.concat(L_ON "\n"); else data.concat(L_OFF "\n");
      data.concat("osc|"); if (encSweep) data.concat(L_ON "\n"); else data.concat(L_OFF "\n");
    #endif

    data.concat("eas_on|");  if (encAutoSync) data.concat("disabled\n"); else data.concat("enabled\n");
    data.concat("eas_off|"); if (encAutoSync) data.concat("enabled\n"); else data.concat("disabled\n");

  #if OPERATIONAL_MODE != WIFI
    client->print(data);
  #else
    server.send(200, "text/plain",data);
  #endif
  }

  #if OPERATIONAL_MODE != WIFI
  void encAjaxGet(EthernetClient *client) {
  #else
  void encAjaxGet() {
  #endif
    processEncodersGet();
  #if OPERATIONAL_MODE != WIFI
    client->print("");
  #else
    server.send(200, "text/html","");
  #endif
  }

  void processEncodersGet() {
    String v;

    v=server.arg("ms");
    if (!v.equals(EmptyStr)) {
      if (v.equals("ons")) encoders.syncToOnStep();
      if (v.equals("enc")) encoders.syncFromOnStep();
      #ifdef ENC_HAS_ABSOLUTE
        if (v.equals("zro")) encoders.zeroFromOnStep();
      #endif
    }

    // Autosync
    v=server.arg("as");
    if (!v.equals(EmptyStr)) {
      if (v.equals("on")) { 
        encAutoSync=true;
        if (ENC_AUTO_SYNC_MEMORY == ON) nv.update(EE_ENC_AUTO_SYNC, encAutoSync);
      }
      if (v.equals("off")) { 
        encAutoSync=false;
        if (ENC_AUTO_SYNC_MEMORY == ON) nv.update(EE_ENC_AUTO_SYNC, encAutoSync);
      }
    }

    // Max. limits
    v=server.arg("d1");
    if (!v.equals(EmptyStr)) {
      int i;
      if ( (atoi2((char*)v.c_str(),&i)) && ((i>=0) && (i<=9999))) { 
        Axis1EncDiffTo=i;
        nv.update(EE_ENC_A1_DIFF_TO, (int32_t)Axis1EncDiffTo);
      }
    }
    v=server.arg("d2");
    if (!v.equals(EmptyStr)) {
      int i;
      if ( atoi2((char*)v.c_str(),&i) && (i >= 0 && i <= 9999)) { 
        Axis2EncDiffTo=i;
        nv.update(EE_ENC_A2_DIFF_TO, (int32_t)Axis2EncDiffTo);
      }
    }

    // Counts per degree
    v = server.arg("a1cpd");
    if (!v.equals(EmptyStr)) {
      double d = v.toFloat();
      if (d >= 1.0 && d <= 1000000.0) { 
        Axis1EncTicksPerDeg = d;
        nv.update(EE_ENC_A1_TICKS, Axis1EncTicksPerDeg);
      }
    }
    v = server.arg("a2cpd");
    if (!v.equals(EmptyStr)) {
      double d = v.toFloat();
      if (d >= 1.0 && d <= 1000000.0) { 
        Axis2EncTicksPerDeg = d;
        nv.update(EE_ENC_A2_TICKS, Axis2EncTicksPerDeg);
      }
    }

    v = server.arg("a1rev");
    if (!v.equals(EmptyStr)) {
      if (v.equals("0")) {
        Axis1EncRev = OFF;
        nv.update(EE_ENC_A1_REV, Axis1EncRev);
      }
      if (v.equals("1")) {
        Axis1EncRev=ON;
        nv.update(EE_ENC_A1_REV, Axis1EncRev);
      }
    }

    v = server.arg("a2rev");
    if (!v.equals(EmptyStr)) {
      if (v.equals("0")) {
        Axis2EncRev = OFF;
        nv.update(EE_ENC_A2_REV, Axis2EncRev);
      }
      if (v.equals("1")) {
        Axis2EncRev=ON;
        nv.update(EE_ENC_A2_REV, Axis2EncRev);
      }
    }

    v = server.arg("revert");
    if (!v.equals(EmptyStr)) { 
      if (v.equals("1")) {
        Axis1EncTicksPerDeg = AXIS1_ENC_TICKS_DEG;
        nv.update(EE_ENC_A1_TICKS, Axis1EncTicksPerDeg);
        Axis1EncRev=AXIS1_ENC_REVERSE;
        nv.update(EE_ENC_A1_REV, Axis1EncRev);
        Axis1EncDiffTo = AXIS1_ENC_DIFF_LIMIT_TO;
        nv.update(EE_ENC_A1_DIFF_TO, (int32_t)Axis1EncDiffTo);
      }
      if (v.equals("2")) {
        Axis2EncTicksPerDeg = AXIS2_ENC_TICKS_DEG;
        nv.update(EE_ENC_A2_TICKS, Axis2EncTicksPerDeg);
        Axis2EncRev = AXIS1_ENC_REVERSE;
        nv.update(EE_ENC_A2_REV, Axis2EncRev);
        Axis2EncDiffTo = AXIS2_ENC_DIFF_LIMIT_TO;
        nv.update(EE_ENC_A2_DIFF_TO, (int32_t)Axis2EncDiffTo);
      }
    }

    #if AXIS1_ENC_RATE_CONTROL == ON
      // OnStep rate control
      v = server.arg("rc");
      if (!v.equals(EmptyStr)) {
        if (v.equals("on")) encRateControl = true;
        if (v.equals("off")) encRateControl = false;
      }

      // Encoder averaging samples
      v = server.arg("sa");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=1) && (i<=999))) { 
          Axis1EncStaSamples = i;
          nv.update(EE_ENC_RC_STA, (int32_t)Axis1EncStaSamples);
        }
      }
      v = server.arg("la");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=1) && (i<=999))) { 
          Axis1EncLtaSamples = i;
          nv.update(EE_ENC_RC_LTA, (int32_t)Axis1EncLtaSamples);
        }
      }

      // Encoder proportional response
      v = server.arg("pr");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=50) && (i<=5000))) { 
          Axis1EncProp = i;
          nv.update(EE_ENC_RC_PROP, (int32_t)Axis1EncProp);
        }
      }
      
      // Encoder minimum guide
      v = server.arg("mg");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=25) && (i<=1000))) { 
          Axis1EncMinGuide = i;
          nv.update(EE_ENC_MIN_GUIDE, (int32_t)Axis1EncMinGuide);
        }
      }

      // Encoder rate compensation
      v=server.arg("er");
      if (!v.equals(EmptyStr)) {
        int l=0;
        l=strtol(v.c_str(),NULL,10);
        if (l >= -99999 && l <= 99999) {
          axis1EncRateComp = (float)l/1000000.0;
          nv.update(EE_ENC_RC_RCOMP, (int32_t)l);
        }
      }

      #if AXIS1_ENC_INTPOL_COS == ON
        // Encoder interpolation compensation
        v = server.arg("ip"); // phase
        if (!v.equals(EmptyStr)) {
          int i;
          if ( atoi2((char*)v.c_str(), &i) && (i >= 0 && i <= 255)) { 
            Axis1EncIntPolPhase = i;
            nv.update(EE_ENC_RC_INTP_P, (int32_t)i);
          }
        }
        v = server.arg("im"); // magnitude
        if (!v.equals(EmptyStr)) {
          int i;
          if ( atoi2((char*)v.c_str(), &i) && (i >= 0 && i <= 29000)) { 
            Axis1EncIntPolMag = i;
            nv.update(EE_ENC_RC_INTP_M, (int32_t)i);
          }
        }
      #endif

      // Sweep control
      v = server.arg("sw");
      if (!v.equals(EmptyStr)) {
        if (v.equals("on")) encSweep = true;
        if (v.equals("off")) encSweep = false;
      }
    #endif
  }
#endif
