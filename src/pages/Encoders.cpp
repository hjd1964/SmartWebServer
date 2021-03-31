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
  #include "../ethernetServers/ethernetServers.h"

  #include "htmlHeaders.h"
  #include "htmlMessages.h"
  #include "htmlScripts.h"

  void processEncodersGet();

  #if OPERATIONAL_MODE != WIFI
  void handleEncoders(EthernetClient *client) {
  #else
  void handleEncoders() {
  #endif
    char temp[400] = "";
    char temp1[80] = "";

    Ser.setTimeout(webTimeout);
    serialRecvFlush();
    
    mountStatus.update();

    processEncodersGet();

    sendHtmlStart();

    String data=FPSTR(html_headB);

    // active ajax page is: encAjax();
    data +="<script>var ajaxPage='enc.txt';</script>\n";
    data +=FPSTR(html_ajax_active);
    data +="<script>auto2Rate=2;</script>";
    sendHtml(data);

    // scripts
    sprintf_P(temp, html_ajaxScript, "encA.txt"); data += temp;
    data += html_encScript1;
    sendHtml(data);

    #if AXIS1_ENC_RATE_CONTROL == ON
      data += html_encScript2;
      sendHtml(data);
    #endif

    // send a standard http response header
    data += FPSTR(html_main_cssB);
    data += FPSTR(html_main_css1);
    data += FPSTR(html_main_css2);
    data += FPSTR(html_main_css3);
    data += FPSTR(html_main_css4);
    data += FPSTR(html_main_css5);
    sendHtml(data);
    data += FPSTR(html_main_css6);
    data += FPSTR(html_main_css7);
    data += FPSTR(html_main_css8);
    data += FPSTR(html_main_css_btns1);
    data += FPSTR(html_main_css_btns2);
    data += FPSTR(html_main_css_btns3);
    sendHtml(data);
    data += FPSTR(html_main_css_collapse1);
    data += FPSTR(html_main_css_collapse2);
    data += FPSTR(html_main_cssE);
    data += FPSTR(html_headE);
    data += FPSTR(html_bodyB);
    sendHtml(data);

    // finish the standard http response header
    {
      char temp1[40]="";
      data += FPSTR(html_onstep_header1); data += "OnStep";
      data += FPSTR(html_onstep_header2);
      if (mountStatus.getVersionStr(temp1)) data += temp1; else data += "?";
      data += FPSTR(html_onstep_header3);
    }
    data += FPSTR(html_linksStatN);
    data += FPSTR(html_linksCtrlN);
    if (mountStatus.featureFound()) data += FPSTR(html_linksAuxN);
    data += FPSTR(html_linksLibN);
    data += FPSTR(html_linksEncS);
    sendHtml(data);
    data += FPSTR(html_linksPecN);
    data += FPSTR(html_linksSetN);
    data += FPSTR(html_linksCfgN);
    data += FPSTR(html_linksSetupN);
    data += FPSTR(html_onstep_header4);
    sendHtml(data);

    // OnStep wasn't found, show warning and info.
    if (!mountStatus.valid()) { data+= FPSTR(html_bad_comms_message); sendHtml(data); sendHtmlDone(data); return; }

    data+="<div style='width: 35em;'>";

    data += FPSTR(html_syncOnStepNow);
    data += FPSTR(html_syncEncodersNow);
    #ifdef ENC_HAS_ABSOLUTE
      data += FPSTR(html_zeroEncodersNow);
    #endif
    data += "</form>";

    // Autosync
    data += FPSTR(html_encEn1);
    data += FPSTR(html_encEn2);
    sendHtml(data);

    #if AXIS1_ENC_RATE_CONTROL == ON
      // OnStep rate control
      data+="<br />";
      data += FPSTR(html_encRateEn1);
      data += FPSTR(html_encRateEn2);

      // Encoder averaging (integration) samples
      sprintf_P(temp,html_encStaAxis1,Axis1EncStaSamples);
      data += temp;
      sprintf_P(temp,html_encLtaAxis1,Axis1EncLtaSamples);
      data += temp;
      sendHtml(data);

      // Encoder poportional response
      sprintf_P(temp,html_encPropAxis1,Axis1EncProp);
      data += temp;

      // Encoder minimum guide
      sprintf_P(temp,html_encMinGuideAxis1,Axis1EncMinGuide);
      data += temp;

      // Encoder rate compensation
      #if AXIS1_ENC_RATE_AUTO == OFF
        long l=round(axis1EncRateComp*1000000.0);
        sprintf_P(temp,html_encErc2Axis1,l);
        data += temp;
      #endif

      #if AXIS1_ENC_INTPOL_COS == ON
        // Encoder interpolation compensation
        sprintf_P(temp,html_encIntPolPhaseAxis1,Axis1EncIntPolPhase);
        data += temp;

        sprintf_P(temp,html_encIntPolMagAxis1,Axis1EncIntPolMag);
        data += temp;
      #endif
      sendHtml(data);

      // Encoder status display
      sprintf(temp,L_ENC_STAT_RATE_AXIS1 ":<br />");
      data += temp;
      sprintf(temp,"&nbsp; OnStep = <span id='stO'>?</span><br />");
      data += temp;
      #if AXIS1_ENC_INTPOL_COS == ON
        sprintf(temp,"&nbsp; " L_ENC_STAT_INTPOL_COMP " = <span id='ipC'>?</span><br />");
        data += temp;
        sprintf(temp,"&nbsp; " L_ENC_STAT_INTPOL_PHASE " = <span id='ipP'>?</span><br />");
        data += temp;
      #endif
      #if AXIS1_ENC_RATE_AUTO > 0
        sprintf(temp,"&nbsp; " L_ENCODER " ARC = <span id='erA'>?</span><br />");
        data += temp;
      #endif
      sprintf(temp,"&nbsp; " L_ENCODER " STA = <span id='stS'>?</span> x<br />");
      data += temp;
      sprintf(temp,"&nbsp; " L_ENCODER " LTA = <span id='stL'>?</span> x<br />");
      data += temp;
      sprintf(temp,"&nbsp; " L_DELTA " &nbsp;= <span id='stD'>?</span><br />");
      data += temp;
      sprintf(temp,"&nbsp; " L_GUIDE " &nbsp;= <span id='rtF'>?</span><br />");
      data += temp;

      sendHtml(data);

      data += "<br /><canvas id='myCanvas' width='600' height='300' style='margin-left: -2px; border:2px solid #999999;'></canvas>";
      data += "&nbsp; " L_CENTER ": OnStep " L_RATE ", " L_BLUE ": STA (" L_RANGE " &#xb1;0.1), " L_GREEN ": LTA (" L_RANGE " &#xb1;0.01)<br />";

      data += FPSTR(html_encSweepEn1);
      data += FPSTR(html_encSweepEn2);

      encoders.clearAverages();
    #endif

    // Encoder configuration section
    data += L_ENC_CONF ":<br />";

    // Axis1 RA/Azm
    data += "<button type='button' class='collapsible'>Axis1 RA/Azm</button>";
    data += FPSTR(html_encFormBegin);
    dtostrf(Axis1EncTicksPerDeg,1,3,temp1); stripNum(temp1);
    sprintf_P(temp,html_encAxisTpd,temp1,1,1,100000L); data += temp;         // Encoder counts per degree
    sprintf_P(temp,html_encAxisReverse,Axis1EncRev==ON?1:0,1); data += temp; // Encode reverse count
    sprintf_P(temp,html_encMxAxis1,(long)Axis1EncDiffTo); data += temp;      // Encoder sync thresholds
    sendHtml(data);
    data += "<button type='submit'>" L_UPLOAD "</button>\r\n";
    data += "<button name='revert' value='1' type='submit'>" L_REVERT "</button>";
    data += FPSTR(html_encFormEnd);
    sendHtml(data);

    // Axis2 Dec/Alt
    data += "<button type='button' class='collapsible'>Axis2 Dec/Alt</button>";
    data += FPSTR(html_encFormBegin);
    dtostrf(Axis2EncTicksPerDeg,1,3,temp1); stripNum(temp1);
    sprintf_P(temp,html_encAxisTpd,temp1,2,1,100000L); data += temp;         // Encoder counts per degree
    sprintf_P(temp,html_encAxisReverse,Axis2EncRev==ON?1:0,2); data += temp; // Encode reverse count
    sprintf_P(temp,html_encMxAxis2,(long)Axis2EncDiffTo); data += temp;      // Encoder sync thresholds
    sendHtml(data);
    data += "<button type='submit'>" L_UPLOAD "</button>\r\n";
    data += "<button name='revert' value='2' type='submit'>" L_REVERT "</button>";
    data += FPSTR(html_encFormEnd);
    sendHtml(data);
    
    // end of page
    data+="<br />";
    
    // collapsible script
    data += FPSTR(html_collapseScript);
    
    strcpy(temp,"</div></div></body></html>");
    data += temp;
    sendHtml(data);

    sendHtmlDone(data);
  }

  #if OPERATIONAL_MODE != WIFI
  void encAjax(EthernetClient *client) {
  #else
  void encAjax() {
  #endif
    String data="";
    
    #if AXIS1_ENC_RATE_CONTROL == ON
      char temp[20]="";
      data += "stO|"; sprintf(temp,"%+1.4f\n",axis1Rate); data += temp;
      data += "stD|"; sprintf(temp,"%+1.4f\n",axis1Rate-axis1EncRateSta); data += temp;
      data += "stS|"; sprintf(temp,"%+1.4f\n",axis1EncRateSta); data += temp;
      data += "stL|"; sprintf(temp,"%+1.4f\n",axis1EncRateLta); data += temp;
      #if AXIS1_ENC_INTPOL_COS == ON
        data += "ipC|"; sprintf(temp,"%+1.4f\n",intpolComp); data += temp;
        data += "ipP|"; sprintf(temp,"%d\n",(int)intpolPhase); data += temp;
      #endif
      #if AXIS1_ENC_RATE_AUTO > 0
        data += "erA|"; sprintf(temp,"%+1.5f\n",axis1EncRateComp); data += temp;
      #endif

      if (guideCorrectionMillis==0) {
        data += "rtF|"; sprintf(temp,L_NONE "\n"); data += temp;
      } else
      if (guideCorrectionMillis>0) {
        data += "rtF|"; sprintf(temp,L_WEST " %ld ms\n",guideCorrectionMillis); data += temp;
      } else
      if (guideCorrectionMillis<0) {
        data += "rtF|"; sprintf(temp,L_EAST " %ld ms\n",-guideCorrectionMillis); data += temp;
      }

      data += "orc|"; if (encRateControl) data+=L_ON "\n"; else data+=L_OFF "\n";
      data += "osc|"; if (encSweep) data+=L_ON "\n"; else data+=L_OFF "\n";
    #endif

    data += "eas_on|";  if (encAutoSync) data+="disabled"; else data+="enabled"; data+="\n";
    data += "eas_off|"; if (encAutoSync) data+="enabled"; else data+="disabled"; data+="\n";

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
      if (v=="ons") encoders.syncToOnStep();
      if (v=="enc") encoders.syncFromOnStep();
  #ifdef ENC_HAS_ABSOLUTE
      if (v=="zro") encoders.zeroFromOnStep();
  #endif
    }

    // Autosync
    v=server.arg("as");
    if (!v.equals(EmptyStr)) {
      if (v=="on") { 
        encAutoSync=true;
        if (ENC_AUTO_SYNC_MEMORY == ON) nv.update(EE_ENC_AUTO_SYNC, encAutoSync);
      }
      if (v=="off") { 
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
    v=server.arg("a1cpd");
    if (!v.equals(EmptyStr)) {
      double d = v.toFloat();
      if (d >= 1.0 && d <= 10000.0) { 
        Axis1EncTicksPerDeg=d;
        nv.update(EE_ENC_A1_TICKS, Axis1EncTicksPerDeg);
      }
    }
    v=server.arg("a2cpd");
    if (!v.equals(EmptyStr)) {
      double d = v.toFloat();
      if (d >= 1.0 && d <= 10000.0) { 
        Axis2EncTicksPerDeg=d;
        nv.update(EE_ENC_A2_TICKS, Axis2EncTicksPerDeg);
      }
    }

    v=server.arg("a1rev");
    if (!v.equals(EmptyStr)) {
      if (v == "0") {
        Axis1EncRev=OFF;
        nv.update(EE_ENC_A1_REV, Axis1EncRev);
      }
      if (v == "1") {
        Axis1EncRev=ON;
        nv.update(EE_ENC_A1_REV, Axis1EncRev);
      }
    }

    v=server.arg("a2rev");
    if (!v.equals(EmptyStr)) {
      if (v == "0") {
        Axis2EncRev=OFF;
        nv.update(EE_ENC_A2_REV, Axis2EncRev);
      }
      if (v == "1") {
        Axis2EncRev=ON;
        nv.update(EE_ENC_A2_REV, Axis2EncRev);
      }
    }

    v=server.arg("revert");
    if (!v.equals(EmptyStr)) { 
      if (v == "1") {
        Axis1EncTicksPerDeg=AXIS1_ENC_TICKS_DEG;
        nv.update(EE_ENC_A1_TICKS, Axis1EncTicksPerDeg);
        Axis1EncRev=AXIS1_ENC_REVERSE;
        nv.update(EE_ENC_A1_REV, Axis1EncRev);
        Axis1EncDiffTo=AXIS1_ENC_DIFF_LIMIT_TO;
        nv.update(EE_ENC_A1_DIFF_TO, (int32_t)Axis1EncDiffTo);
      }
      if (v == "2") {
        Axis2EncTicksPerDeg=AXIS2_ENC_TICKS_DEG;
        nv.update(EE_ENC_A2_TICKS, Axis2EncTicksPerDeg);
        Axis2EncRev=AXIS1_ENC_REVERSE;
        nv.update(EE_ENC_A2_REV, Axis2EncRev);
        Axis2EncDiffTo=AXIS2_ENC_DIFF_LIMIT_TO;
        nv.update(EE_ENC_A2_DIFF_TO, (int32_t)Axis2EncDiffTo);
      }
    }

    #if AXIS1_ENC_RATE_CONTROL == ON
      // OnStep rate control
      v=server.arg("rc");
      if (!v.equals(EmptyStr)) {
        if (v=="on") encRateControl=true;
        if (v=="off") encRateControl=false;
      }

      // Encoder averaging samples
      v=server.arg("sa");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=1) && (i<=999))) { 
          Axis1EncStaSamples=i;
          nv.update(EE_ENC_RC_STA, (int32_t)Axis1EncStaSamples);
        }
      }
      v=server.arg("la");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=1) && (i<=999))) { 
          Axis1EncLtaSamples=i;
          nv.update(EE_ENC_RC_LTA, (int32_t)Axis1EncLtaSamples);
        }
      }

      // Encoder proportional response
      v=server.arg("pr");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=50) && (i<=5000))) { 
          Axis1EncProp=i;
          nv.update(EE_ENC_RC_PROP, (int32_t)Axis1EncProp);
        }
      }
      
      // Encoder minimum guide
      v=server.arg("mg");
      if (!v.equals(EmptyStr)) {
        int i;
        if ( (atoi2((char*)v.c_str(),&i)) && ((i>=25) && (i<=1000))) { 
          Axis1EncMinGuide=i;
          nv.update(EE_ENC_MIN_GUIDE, (int32_t)Axis1EncMinGuide);
        }
      }

      // Encoder rate compensation
      v=server.arg("er");
      if (!v.equals(EmptyStr)) {
        int l=0;
        l=strtol(v.c_str(),NULL,10);
        if ((l>=-99999) && (l<=99999)) {
          axis1EncRateComp=(float)l/1000000.0;
          nv.update(EE_ENC_RC_RCOMP, (int32_t)l);
        }
      }

      #if AXIS1_ENC_INTPOL_COS == ON
        // Encoder interpolation compensation
        v=server.arg("ip"); // phase
        if (!v.equals(EmptyStr)) {
          int i;
          if ( (atoi2((char*)v.c_str(),&i)) && ((i>=0) && (i<=255))) { 
            Axis1EncIntPolPhase=i;
            nv.update(EE_ENC_RC_INTP_P, (int32_t)i);
          }
        }
        v=server.arg("im"); // magnitude
        if (!v.equals(EmptyStr)) {
          int i;
          if ( (atoi2((char*)v.c_str(),&i)) && ((i>=0) && (i<=29000))) { 
            Axis1EncIntPolMag=i;
            nv.update(EE_ENC_RC_INTP_M, (int32_t)i);
          }
        }
      #endif

      // Sweep control
      v=server.arg("sw");
      if (!v.equals(EmptyStr)) {
        if (v=="on") encSweep=true;
        if (v=="off") encSweep=false;
      }
    #endif
  }
#endif
