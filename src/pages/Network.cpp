// -----------------------------------------------------------------------------------
// Setup Wifi and Ethernet

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../HAL/HAL.h"
extern NVS nv;
#include "../debug/Debug.h"

#include "../locales/Locale.h"
#include "../commands/Commands.h"
#include "../misc/Misc.h"
#include "../status/MountStatus.h"
#include "../wifiServers/WifiServers.h"
#include "../ethernetServers/ethernetServers.h"

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

extern int webTimeout;
extern int cmdTimeout;

extern char masterPassword[40];

void processNetworkGet();

const char htmL_NETWORKSerial[] PROGMEM =
"<b>" L_NETWORK_PERFORMANCE ":</b><br/>"
"<form method='post' action='/net.htm'>"
L_NETWORK_CMD_TO ": <input style='width:4em' name='ccto' value='%d' type='number' min='100' max='300'> ms<br/>"
L_NETWORK_WWW_TO ": <input style='width:4em' name='wcto' value='%d' type='number' min='100' max='300'> ms<br/>"
"<button type='submit'>" L_UPLOAD "</button></form><br />\r\n";

const char htmL_NET_MAC[] PROGMEM =
"MAC: <input style='width:10em' name='%s' type='text' value='%s' maxlength='17' disabled><br/>";

const char htmL_NET_IP[] PROGMEM =
"<table><tr><td>" L_IP_ADDRESS ": </td><td>"
"<input name='%sip1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip4' value='%d' type='number' min='0' max='255'></td>";

const char htmL_NET_GW[] PROGMEM =
"<tr><td>" L_GATEWAY ": </td><td>"
"<input name='%sgw1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw4' value='%d' type='number' min='0' max='255'></td>";

const char htmL_NET_SN[] PROGMEM =
"<tr><td>" L_SUBNET ": </td><td>"
"<input name='%ssn1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn4' value='%d' type='number' min='0' max='255'></td></tr></table>";

#if OPERATIONAL_MODE == WIFI
  extern bool accessPointEnabled;
  extern bool stationEnabled;
  extern bool stationDhcpEnabled;

  extern char wifi_sta_ssid[40];
  extern char wifi_sta_pwd[40];

  extern IPAddress wifi_sta_ip;
  extern IPAddress wifi_sta_gw;
  extern IPAddress wifi_sta_sn;

  extern char wifi_ap_ssid[40];
  extern char wifi_ap_pwd[40];
  extern byte wifi_ap_ch;

  extern IPAddress wifi_ap_ip;
  extern IPAddress wifi_ap_gw;
  extern IPAddress wifi_ap_sn;

  const char htmL_NETWORKSSID1[] PROGMEM =
  "<br/><b>" L_NETWORK_STA_TITLE ":</b><br/>"
  "<form method='post' action='/net.htm'>"
  "SSID: <input style='width:6em' name='stssid' type='text' value='%s' maxlength='32'>&nbsp;&nbsp;&nbsp;"
  L_NETWORK_PWD ": <input style='width:8em' name='stpwd' type='password' value='%s' minlength='8' maxlength='39'> (" L_NETWORK_PWD_MSG ")<br/>";

  const char htmL_NETWORKSSID2[] PROGMEM =
  L_NETWORK_EN_DHCP ": <input type='checkbox' name='stadhcp' value='1' %s> (" L_NETWORK_EN_DHCP_MSG ")<br/>"
  L_NETWORK_EN_STA ": <input type='checkbox' name='staen' value='1' %s><br/>"
  "<button type='submit'>" L_UPLOAD "</button></form><br />\r\n";

  const char htmL_NETWORKSSID3A[] PROGMEM =
  "<br/><b>" L_NETWORK_AP ":</b><br/>"
  "<form method='post' action='/net.htm'>"
  "SSID: <input style='width:6em' name='apssid' type='text' ";

  const char htmL_NETWORKSSID3B[] PROGMEM =
  "value='%s' maxlength='32'>&nbsp;&nbsp;&nbsp;"
  L_NETWORK_PWD ": <input style='width:8em' name='appwd' type='password' value='%s' minlength='8' maxlength='39'> " L_NETWORK_PWD_MSG "&nbsp;&nbsp;&nbsp;"
  L_NETWORK_CHA ": <input style='width:3em' name='apch' value='%d' type='number' min='1' max='11'><br/>";

  const char htmL_NETWORKSSID7[] PROGMEM =
  L_NETWORK_EN_AP_MODE ": <input type='checkbox' name='apen' value='1' %s> (Note: auto-enabled if Station Mode fails to connect)<br/>"
  "<button type='submit'>" L_UPLOAD "</button></form><br />\r\n";
#else
  const char htmL_NETWORK_ETH_BEG[] PROGMEM =
  "<br/><b>Ethernet:</b><br/>"
  "<form method='post' action='/net.htm'><br />\r\n";

  const char htmL_NETWORK_ETH_END[] PROGMEM =
  "<button type='submit'>" L_UPLOAD "</button></form><br />\r\n";
#endif

const char html_logout[] PROGMEM =
"<br/><b>" L_NETWORK_CONFIG_SECURITY ":</b><br/>"
"<form method='post' action='/net.htm'>"
L_NETWORK_PWD ": <input style='width:8em' name='webpwd' type='password' minlength='3' maxlength='39'> "
"<button type='submit'>" L_UPLOAD "</button></form>"
"<form method='post' action='/net.htm'>"
"<button type='submit' name='logout' value='1'>" L_LOGOUT "</button></form><br />\r\n";

const char html_reboot[] PROGMEM =
"<br/><br/><br/><br/><br/><form method='post' action='/net.htm'>"
"<b>" L_NETWORK_RESTART_MSG "</b><br/><br/>"
"<button type='submit'>" L_CONTINUE "</button>"
"</form><br/><br/><br/><br/>"
"\r\n";

const char html_login[] PROGMEM =
"<br/><form method='post' action='/net.htm'>"
"<br/>" L_NETWORK_TITLE "<br />"
"<input style='width:8em' name='login' type='password' minlength='3' maxlength='39'>"
"<button type='submit'>" L_OK "</button>"
"</form><br/><br/><br/>"
#if OPERATIONAL_MODE == WIFI
  L_NETWORK_ADVICE1 L_NETWORK_ADVICE2 L_NETWORK_ADVICE3 L_NETWORK_ADVICE4 L_NETWORK_ADVICE5 L_NETWORK_ADVICE6 L_NETWORK_ADVICE7 L_NETWORK_ADVICE8 L_NETWORK_ADVICE9 
  "<br/><br/>"
#endif
"\r\n";

bool restartRequired = false;
bool loginRequired = true;
byte temp_ip[4] = {0,0,0,0};
byte temp_sn[4] = {0,0,0,0};
byte temp_gw[4] = {0,0,0,0};

#if OPERATIONAL_MODE != WIFI
void handleNetwork(EthernetClient *client) {
#else
void handleNetwork() {
#endif
  char temp[420]  = "";
  char temp1[140] = "";

  Ser.setTimeout(webTimeout);
  serialRecvFlush();
  
  mountStatus.update(true);
  
  processNetworkGet();

  sendHtmlStart();

  // send a standard http response header
  String data=FPSTR(html_headB);
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
  data += FPSTR(html_main_cssE);
  data += FPSTR(html_headE);
  data += FPSTR(html_bodyB);
  sendHtml(data);

  // finish the standard http response header
  data += FPSTR(html_onstep_header1); data += "OnStep";
  data += FPSTR(html_onstep_header2);
  if (mountStatus.getVersionStr(temp1)) data += temp1; else data += "?";
  data += FPSTR(html_onstep_header3);
  data += FPSTR(html_linksStatN);
  data += FPSTR(html_linksCtrlN);
  if (mountStatus.featureFound()) data += FPSTR(html_linksAuxN);
  data += FPSTR(html_linksLibN);
  #if ENCODERS == ON
    data += FPSTR(html_linksEncN);
  #endif
  sendHtml(data);
  data += FPSTR(html_linksPecN);
  data += FPSTR(html_linksSetN);
  data += FPSTR(html_linksCfgN);
  data += FPSTR(html_linksSetupS);
  data += FPSTR(html_onstep_header4);
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data += FPSTR(html_bad_comms_message); sendHtml(data); sendHtmlDone(data); return; }

  data+="<div>";

  if (restartRequired) {
    restartRequired = false;
    data += FPSTR(html_reboot);
  } else
  if (loginRequired) {
    restartRequired = false;
    data += FPSTR(html_login);
  } else {
    sprintf_P(temp, htmL_NETWORKSerial, cmdTimeout, webTimeout); data += temp;
    
    #if OPERATIONAL_MODE == WIFI
      sprintf_P(temp, htmL_NETWORKSSID1, wifi_sta_ssid, ""); data += temp;
      nv.readStr(EE_STA_SSID, wifi_sta_ssid, 40);
      nv.readStr(EE_STA_PWD, wifi_sta_pwd, 40);

      uint8_t macsta[6] = {0,0,0,0,0,0}; WiFi.macAddress(macsta); temp1[0] = 0;
      for (int i = 0; i < 6; i++) { sprintf(temp1, "%s%02x:", temp1, macsta[i]); } temp1[strlen(temp1) - 1] = 0;

      sprintf_P(temp,htmL_NET_MAC,"sta", temp1); data += temp;
      sendHtml(data);
      sprintf_P(temp,htmL_NET_IP, "sta", wifi_sta_ip[0], "sta", wifi_sta_ip[1], "sta", wifi_sta_ip[2], "sta", wifi_sta_ip[3]); data += temp;
      sprintf_P(temp,htmL_NET_GW, "sta", wifi_sta_gw[0], "sta", wifi_sta_gw[1], "sta", wifi_sta_gw[2], "sta", wifi_sta_gw[3]); data += temp;
      sprintf_P(temp,htmL_NET_SN, "sta", wifi_sta_sn[0], "sta", wifi_sta_sn[1], "sta", wifi_sta_sn[2], "sta", wifi_sta_sn[3]); data += temp;

      sprintf_P(temp,htmL_NETWORKSSID2, stationDhcpEnabled?"checked":"",stationEnabled?"checked":""); data += temp;
      data += FPSTR(htmL_NETWORKSSID3A);
      sprintf_P(temp,htmL_NETWORKSSID3B, wifi_ap_ssid, "", wifi_ap_ch); data += temp;
      sendHtml(data);
    
      uint8_t macap[6] = {0,0,0,0,0,0}; WiFi.softAPmacAddress(macap); temp1[0] = 0;
      for (int i = 0; i < 6; i++) { sprintf(temp1, "%s%02x:", temp1, macap[i]); } temp1[strlen(temp1) - 1] = 0;
      
      sprintf_P(temp,htmL_NET_MAC,"ap", temp1); data += temp;
      sendHtml(data);
      sprintf_P(temp,htmL_NET_IP, "ap", wifi_sta_ip[0], "ap", wifi_sta_ip[1], "ap", wifi_sta_ip[2], "ap", wifi_sta_ip[3]); data += temp;
      sprintf_P(temp,htmL_NET_GW, "ap", wifi_sta_gw[0], "ap", wifi_sta_gw[1], "ap", wifi_sta_gw[2], "ap", wifi_sta_gw[3]); data += temp;
      sprintf_P(temp,htmL_NET_SN, "ap", wifi_sta_sn[0], "ap", wifi_sta_sn[1], "ap", wifi_sta_sn[2], "ap", wifi_sta_sn[3]); data += temp;

      sprintf_P(temp,htmL_NETWORKSSID7, accessPointEnabled?"checked":""); data += temp;
    #else
      nv.readBytes(EE_ETH_IP, temp_ip, 4);
      nv.readBytes(EE_ETH_SN, temp_sn, 4);
      nv.readBytes(EE_ETH_GW, temp_gw, 4);

      data += FPSTR(htmL_NETWORK_ETH_BEG); temp1[0] = 0;
      for (int i = 0; i < 6; i++) { sprintf(temp1, "%s%02x:", temp1, eth_mac[i]); } temp1[strlen(temp1) - 1] = 0;

      sprintf_P(temp,htmL_NET_MAC,"eth", temp1); data += temp;
      sendHtml(data);
      sprintf_P(temp,htmL_NET_IP, "eth", temp_ip[0], "eth", temp_ip[1], "eth", temp_ip[2], "eth", temp_ip[3]); data += temp;
      sprintf_P(temp,htmL_NET_GW, "eth", temp_gw[0], "eth", temp_gw[1], "eth", temp_gw[2], "eth", temp_gw[3]); data += temp;
      sprintf_P(temp,htmL_NET_SN, "eth", temp_sn[0], "eth", temp_sn[1], "eth", temp_sn[2], "eth", temp_sn[3]); data += temp;

      data += FPSTR(htmL_NETWORK_ETH_END);
      sendHtml(data);
    #endif

    data += FPSTR(html_logout);
  }
  
  strcpy(temp,"</div></div></body></html>");
  data += temp;

  sendHtml(data);
  sendHtmlDone(data);
}

void processNetworkGet() {
  String v, v1;
  
  // Login --------------------------------------------------------------------
  v = server.arg("login");
  if (!v.equals(EmptyStr)) {
    if (!strcmp(masterPassword, (char*)v.c_str())) loginRequired = false;
  }

  v = server.arg("logout");
  if (!v.equals(EmptyStr)) loginRequired = true;
  if (loginRequired) return;

  v = server.arg("webpwd");
  if (!v.equals(EmptyStr)) {
    strcpy(masterPassword, (char*)v.c_str());
    nv.write(EE_PASSWORD, masterPassword);
  }

  // Timeouts -----------------------------------------------------------------
  // Cmd channel timeout
  v = server.arg("ccto");
  if (!v.equals(EmptyStr)) {
    cmdTimeout = v.toInt();
    nv.update(EE_TIMEOUT_CMD, (int16_t)cmdTimeout);
  }

  // Web channel timeout
  v = server.arg("wcto");
  if (!v.equals(EmptyStr)) {
    webTimeout = v.toInt();
    nv.update(EE_TIMEOUT_WEB, (int16_t)webTimeout);
  }

  #if OPERATIONAL_MODE == WIFI
    // --------------------------------------------------------------------------
    // Station MAC
    v=server.arg("stmac");
    if (!v.equals(EmptyStr)) {
      // 5c:cf:7f:0f:ad:85
      // first the length should be 17
      if (v.length()==17) {
        // seperators all in place
        if ((v.charAt(2)==':') && (v.charAt(5)==':') && (v.charAt(8)==':') && (v.charAt(11)==':') && (v.charAt(14)==':')) {
          // digits all in 0..9,A..F and validate
          v.toUpperCase();
          uint8_t mac[6];
          int imac[6];
          imac[0]=hexToInt(v.substring(0,2)); imac[1]=hexToInt(v.substring(3,2)); imac[2]=hexToInt(v.substring(6,2));
          imac[3]=hexToInt(v.substring(9,2)); imac[4]=hexToInt(v.substring(12,2)); imac[5]=hexToInt(v.substring(15,2));
          if ((imac[0]>=0) && (imac[1]>=0) && (imac[2]>=0) && (imac[3]>=0) && (imac[4]>=0) && (imac[5]>=0)) {
            mac[0]=imac[0]; mac[1]=imac[1]; mac[2]=imac[2]; mac[3]=imac[3]; mac[4]=imac[4]; mac[5]=imac[5]; 
            WiFi.macAddress(mac); restartRequired=true; 
          }
        }
      }
    }

    // Station SSID
    v = server.arg("stssid"); v1=v;
    if (!v.equals(EmptyStr)) {
      if (!strcmp(wifi_sta_ssid,(char*)v.c_str())) restartRequired=true;
      strcpy(wifi_sta_ssid,(char*)v.c_str());

      // if this section was submitted set the stationEnabled default to false
      stationDhcpEnabled=false;
      stationEnabled=false;
    }

    // Station password
    v=server.arg("stpwd");
    if (!v.equals(EmptyStr)) {
      if (!strcmp(wifi_sta_pwd,(char*)v.c_str())) restartRequired=true;
      strcpy(wifi_sta_pwd,(char*)v.c_str());
    }

    // Station dhcp enabled
    v=server.arg("stadhcp");
    if (!v.equals(EmptyStr)) {
      stationDhcpEnabled=v.toInt();
    }

    // Station enabled
    v=server.arg("staen");
    if (!v.equals(EmptyStr)) {
      stationEnabled=v.toInt();
    }

    // Access-Point ip
    IPAddress old_ip=wifi_sta_ip;
    v=server.arg("staip1"); if (!v.equals(EmptyStr)) wifi_sta_ip[0]=v.toInt();
    v=server.arg("staip2"); if (!v.equals(EmptyStr)) wifi_sta_ip[1]=v.toInt();
    v=server.arg("staip3"); if (!v.equals(EmptyStr)) wifi_sta_ip[2]=v.toInt();
    v=server.arg("staip4"); if (!v.equals(EmptyStr)) wifi_sta_ip[3]=v.toInt();

    // Access-Point SubNet
    old_ip=wifi_sta_sn;
    v=server.arg("stasn1"); if (!v.equals(EmptyStr)) wifi_sta_sn[0]=v.toInt();
    v=server.arg("stasn2"); if (!v.equals(EmptyStr)) wifi_sta_sn[1]=v.toInt();
    v=server.arg("stasn3"); if (!v.equals(EmptyStr)) wifi_sta_sn[2]=v.toInt();
    v=server.arg("stasn4"); if (!v.equals(EmptyStr)) wifi_sta_sn[3]=v.toInt();

    // Access-Point Gateway
    old_ip=wifi_sta_gw;
    v=server.arg("stagw1"); if (!v.equals(EmptyStr)) wifi_sta_gw[0]=v.toInt();
    v=server.arg("stagw2"); if (!v.equals(EmptyStr)) wifi_sta_gw[1]=v.toInt();
    v=server.arg("stagw3"); if (!v.equals(EmptyStr)) wifi_sta_gw[2]=v.toInt();
    v=server.arg("stagw4"); if (!v.equals(EmptyStr)) wifi_sta_gw[3]=v.toInt();
      
    if (!v1.equals(EmptyStr)) {
      nv.writeBytes(EE_STA_SSID, wifi_sta_ssid, -40);
      nv.writeBytes(EE_STA_PWD, wifi_sta_pwd, -40);
      nv.write(EE_DHCP_EN, (int)stationDhcpEnabled);
      nv.write(EE_STA_EN, (int)stationEnabled);
      nv.writeBytes(EE_STA_IP, wifi_sta_ip, 4);
      nv.writeBytes(EE_STA_SN, wifi_sta_sn, 4);
      nv.writeBytes(EE_STA_GW, wifi_sta_gw, 4);
      restartRequired=true;
    }

    // --------------------------------------------------------------------------
    // Access-Point MAC
    v=server.arg("apmac");
    if (!v.equals(EmptyStr)) {
      // 5c:cf:7f:0f:ad:85
      // first the length should be 17
      if (v.length()==17) {
        // seperators all in place
        if ((v.charAt(2)==':') && (v.charAt(5)==':') && (v.charAt(8)==':') && (v.charAt(11)==':') && (v.charAt(14)==':')) {
          // digits all in 0..9,A..F and validate
          v.toUpperCase();
          uint8_t mac[6];
          int imac[6];
          imac[0]=hexToInt(v.substring(0,2)); imac[1]=hexToInt(v.substring(3,2)); imac[2]=hexToInt(v.substring(6,2));
          imac[3]=hexToInt(v.substring(9,2)); imac[4]=hexToInt(v.substring(12,2)); imac[5]=hexToInt(v.substring(15,2));
          if ((imac[0]>=0) && (imac[1]>=0) && (imac[2]>=0) && (imac[3]>=0) && (imac[4]>=0) && (imac[5]>=0)) {
            mac[0]=imac[0]; mac[1]=imac[1]; mac[2]=imac[2]; mac[3]=imac[3]; mac[4]=imac[4]; mac[5]=imac[5];
            WiFi.softAPmacAddress(mac); restartRequired=true; 
          }
        }
      }
    }

    // Access-Point SSID
    v=server.arg("apssid");
    if (!v.equals(EmptyStr)) {
      if (!strcmp(wifi_ap_ssid,(char*)v.c_str())) restartRequired=true;
      strcpy(wifi_ap_ssid,(char*)v.c_str());

      // if this section was submitted set the accessPointEnabled default to false
      accessPointEnabled=false;
    }

    // Access-Point password
    v=server.arg("appwd");
    if (!v.equals(EmptyStr)) {
      if (!strcmp(wifi_ap_pwd,(char*)v.c_str())) restartRequired=true;
      strcpy(wifi_ap_pwd,(char*)v.c_str());
    }

    // Access-Point channel
    v=server.arg("apch");
    if (!v.equals(EmptyStr)) {
      if (wifi_ap_ch!=v.toInt()) restartRequired=true;
      wifi_ap_ch=v.toInt();
    }

    // Access-Point enabled
    v=server.arg("apen");
    if (!v.equals(EmptyStr)) {
      accessPointEnabled=v.toInt();
    }

    // Access-Point ip
    old_ip=wifi_ap_ip;
    v=server.arg("apip1"); if (!v.equals(EmptyStr)) wifi_ap_ip[0]=v.toInt();
    v=server.arg("apip2"); if (!v.equals(EmptyStr)) wifi_ap_ip[1]=v.toInt();
    v=server.arg("apip3"); if (!v.equals(EmptyStr)) wifi_ap_ip[2]=v.toInt();
    v=server.arg("apip4"); if (!v.equals(EmptyStr)) wifi_ap_ip[3]=v.toInt();

    // Access-Point SubNet
    old_ip=wifi_ap_sn;
    v=server.arg("apsn1"); if (!v.equals(EmptyStr)) wifi_ap_sn[0]=v.toInt();
    v=server.arg("apsn2"); if (!v.equals(EmptyStr)) wifi_ap_sn[1]=v.toInt();
    v=server.arg("apsn3"); if (!v.equals(EmptyStr)) wifi_ap_sn[2]=v.toInt();
    v=server.arg("apsn4"); if (!v.equals(EmptyStr)) wifi_ap_sn[3]=v.toInt();

    // Access-Point Gateway
    old_ip=wifi_ap_gw;
    v=server.arg("apgw1"); if (!v.equals(EmptyStr)) wifi_ap_gw[0]=v.toInt();
    v=server.arg("apgw2"); if (!v.equals(EmptyStr)) wifi_ap_gw[1]=v.toInt();
    v=server.arg("apgw3"); if (!v.equals(EmptyStr)) wifi_ap_gw[2]=v.toInt();
    v=server.arg("apgw4"); if (!v.equals(EmptyStr)) wifi_ap_gw[3]=v.toInt();

    if (!v.equals(EmptyStr)) {
      nv.writeBytes(EE_AP_SSID, wifi_ap_ssid, -40);
      nv.writeBytes(EE_AP_PWD, wifi_ap_pwd, -40);
      nv.write(EE_AP_CH, (int16_t)wifi_ap_ch);
      nv.write(EE_AP_EN, (int16_t)accessPointEnabled);
      nv.writeBytes(EE_AP_IP, wifi_ap_ip, 4);
      nv.writeBytes(EE_AP_SN, wifi_ap_sn, 4);
      nv.writeBytes(EE_AP_GW, wifi_ap_gw, 4);
      restartRequired=true;
    }
  #else
    // Ethernet ip
    v = server.arg("ethip1"); if (!v.equals(EmptyStr)) temp_ip[0] = v.toInt();
    v = server.arg("ethip2"); if (!v.equals(EmptyStr)) temp_ip[1] = v.toInt();
    v = server.arg("ethip3"); if (!v.equals(EmptyStr)) temp_ip[2] = v.toInt();
    v = server.arg("ethip4"); if (!v.equals(EmptyStr)) temp_ip[3] = v.toInt();

    // Ethernet SubNet
    v = server.arg("ethsn1"); if (!v.equals(EmptyStr)) temp_sn[0] = v.toInt();
    v = server.arg("ethsn2"); if (!v.equals(EmptyStr)) temp_sn[1] = v.toInt();
    v = server.arg("ethsn3"); if (!v.equals(EmptyStr)) temp_sn[2] = v.toInt();
    v = server.arg("ethsn4"); if (!v.equals(EmptyStr)) temp_sn[3] = v.toInt();

    // Ethernet Gateway
    v = server.arg("ethgw1"); if (!v.equals(EmptyStr)) temp_gw[0] = v.toInt();
    v = server.arg("ethgw2"); if (!v.equals(EmptyStr)) temp_gw[1] = v.toInt();
    v = server.arg("ethgw3"); if (!v.equals(EmptyStr)) temp_gw[2] = v.toInt();
    v = server.arg("ethgw4"); if (!v.equals(EmptyStr)) temp_gw[3] = v.toInt();

    if (!v.equals(EmptyStr)) {
      nv.writeBytes(EE_ETH_IP, temp_ip, 4);
      nv.writeBytes(EE_ETH_SN, temp_sn, 4);
      nv.writeBytes(EE_ETH_GW, temp_gw, 4);
      restartRequired = true;
    }
  #endif
}
