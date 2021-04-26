// -----------------------------------------------------------------------------------
// Setup Network

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
#include "../ethernetServers/EthernetServers.h"
#include "../encoders/Encoders.h"

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#include "Network.h"

extern int webTimeout;
extern int cmdTimeout;

extern char masterPassword[40];

void processNetworkGet();

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

  SERIAL_ONSTEP.setTimeout(webTimeout);
  serialRecvFlush();
  
  mountStatus.update(true);
  
  processNetworkGet();

  sendHtmlStart();

  // send a standard http response header
  String data=FPSTR(html_headB);
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
  data.concat(FPSTR(html_main_cssE));
  data.concat(FPSTR(html_headE));
  data.concat(FPSTR(html_bodyB));
  sendHtml(data);

  // finish the standard http response header
  data.concat(FPSTR(html_onstep_header1)); data.concat("OnStep");
  data.concat(FPSTR(html_onstep_header2));
  if (mountStatus.getVersionStr(temp1)) data.concat(temp1); else data.concat("?");
  data.concat(FPSTR(html_onstep_header3));
  data.concat(FPSTR(html_linksStatN));
  data.concat(FPSTR(html_linksCtrlN));
  if (mountStatus.featureFound()) data.concat(FPSTR(html_linksAuxN));
  data.concat(FPSTR(html_linksLibN));
  #if ENCODERS == ON
    data.concat(FPSTR(html_linksEncN));
  #endif
  sendHtml(data);
  if (mountStatus.pecEnabled()) data.concat(FPSTR(html_linksPecN));
  data.concat(FPSTR(html_linksSetN));
  data.concat(FPSTR(html_linksCfgN));
  data.concat(FPSTR(html_linksSetupS));
  data.concat(FPSTR(html_onstep_header4));
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(data); return; }

  data.concat("<div>");

  if (restartRequired) {
    restartRequired = false;
    data.concat(FPSTR(html_reboot));
  } else
  if (loginRequired) {
    restartRequired = false;
    data.concat(FPSTR(html_login));
  } else {
    sprintf_P(temp, htmL_NETWORKSerial, cmdTimeout, webTimeout); data.concat(temp);
    
    #if OPERATIONAL_MODE == WIFI
      sprintf_P(temp, htmL_NETWORKSSID1, wifi_sta_ssid, ""); data.concat(temp);
      nv.readBytes(EE_STA_SSID, wifi_sta_ssid, -40);
      nv.readBytes(EE_STA_PWD, wifi_sta_pwd, -40);

      uint8_t macsta[6] = {0,0,0,0,0,0}; WiFi.macAddress(macsta); temp1[0] = 0;
      for (int i = 0; i < 6; i++) { sprintf(temp1, "%s%02x:", temp1, macsta[i]); } temp1[strlen(temp1) - 1] = 0;

      sprintf_P(temp,htmL_NET_MAC,"sta", temp1); data.concat(temp);
      sendHtml(data);
      sprintf_P(temp,htmL_NET_IP, "sta", wifi_sta_ip[0], "sta", wifi_sta_ip[1], "sta", wifi_sta_ip[2], "sta", wifi_sta_ip[3]); data.concat(temp);
      sprintf_P(temp,htmL_NET_GW, "sta", wifi_sta_gw[0], "sta", wifi_sta_gw[1], "sta", wifi_sta_gw[2], "sta", wifi_sta_gw[3]); data.concat(temp);
      sprintf_P(temp,htmL_NET_SN, "sta", wifi_sta_sn[0], "sta", wifi_sta_sn[1], "sta", wifi_sta_sn[2], "sta", wifi_sta_sn[3]); data.concat(temp);

      sprintf_P(temp,htmL_NETWORKSSID2, stationDhcpEnabled?"checked":"",stationEnabled?"checked":""); data.concat(temp);
      data.concat(FPSTR(htmL_NETWORKSSID3A));
      sprintf_P(temp,htmL_NETWORKSSID3B, wifi_ap_ssid, "", wifi_ap_ch); data.concat(temp);
      sendHtml(data);
    
      uint8_t macap[6] = {0,0,0,0,0,0}; WiFi.softAPmacAddress(macap); temp1[0] = 0;
      for (int i = 0; i < 6; i++) { sprintf(temp1, "%s%02x:", temp1, macap[i]); } temp1[strlen(temp1) - 1] = 0;
      
      sprintf_P(temp,htmL_NET_MAC,"ap", temp1); data.concat(temp);
      sendHtml(data);
      sprintf_P(temp,htmL_NET_IP, "ap", wifi_ap_ip[0], "ap", wifi_ap_ip[1], "ap", wifi_ap_ip[2], "ap", wifi_ap_ip[3]); data.concat(temp);
      sprintf_P(temp,htmL_NET_GW, "ap", wifi_ap_gw[0], "ap", wifi_ap_gw[1], "ap", wifi_ap_gw[2], "ap", wifi_ap_gw[3]); data.concat(temp);
      sprintf_P(temp,htmL_NET_SN, "ap", wifi_ap_sn[0], "ap", wifi_ap_sn[1], "ap", wifi_ap_sn[2], "ap", wifi_ap_sn[3]); data.concat(temp);

      sprintf_P(temp,htmL_NETWORKSSID7, accessPointEnabled?"checked":""); data.concat(temp);
    #else
      nv.readBytes(EE_ETH_IP, temp_ip, 4);
      nv.readBytes(EE_ETH_SN, temp_sn, 4);
      nv.readBytes(EE_ETH_GW, temp_gw, 4);

      data.concat(FPSTR(htmL_NETWORK_ETH_BEG)); temp1[0] = 0;
      for (int i = 0; i < 6; i++) { sprintf(temp1, "%s%02x:", temp1, eth_mac[i]); } temp1[strlen(temp1) - 1] = 0;

      sprintf_P(temp,htmL_NET_MAC,"eth", temp1); data.concat(temp);
      sendHtml(data);
      sprintf_P(temp,htmL_NET_IP, "eth", temp_ip[0], "eth", temp_ip[1], "eth", temp_ip[2], "eth", temp_ip[3]); data.concat(temp);
      sprintf_P(temp,htmL_NET_GW, "eth", temp_gw[0], "eth", temp_gw[1], "eth", temp_gw[2], "eth", temp_gw[3]); data.concat(temp);
      sprintf_P(temp,htmL_NET_SN, "eth", temp_sn[0], "eth", temp_sn[1], "eth", temp_sn[2], "eth", temp_sn[3]); data.concat(temp);

      data.concat(FPSTR(htmL_NETWORK_ETH_END));
      sendHtml(data);
    #endif

    data.concat(FPSTR(html_logout));
  }
  
  strcpy(temp,"</div></div></body></html>");
  data.concat(temp);

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
    nv.updateBytes(EE_PASSWORD, masterPassword, -40);
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
    v = server.arg("stmac");
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
      if (!strcmp(wifi_sta_ssid, (char*)v.c_str())) restartRequired = true;
      strcpy(wifi_sta_ssid, (char*)v.c_str());

      // if this section was submitted set the stationEnabled default to false
      stationDhcpEnabled = false;
      stationEnabled = false;
    }

    // Station password
    v = server.arg("stpwd");
    if (!v.equals(EmptyStr)) {
      if (!strcmp(wifi_sta_pwd, (char*)v.c_str())) restartRequired = true;
      strcpy(wifi_sta_pwd, (char*)v.c_str());
    }

    // Station dhcp enabled
    v = server.arg("stadhcp");
    if (!v.equals(EmptyStr)) {
      stationDhcpEnabled = v.toInt();
    }

    // Station enabled
    v = server.arg("staen");
    if (!v.equals(EmptyStr)) {
      stationEnabled = v.toInt();
    }

    // Station ip
    IPAddress old_ip = wifi_sta_ip;
    v = server.arg("staip1"); if (!v.equals(EmptyStr)) wifi_sta_ip[0] = v.toInt();
    v = server.arg("staip2"); if (!v.equals(EmptyStr)) wifi_sta_ip[1] = v.toInt();
    v = server.arg("staip3"); if (!v.equals(EmptyStr)) wifi_sta_ip[2] = v.toInt();
    v = server.arg("staip4"); if (!v.equals(EmptyStr)) wifi_sta_ip[3] = v.toInt();

    // Station SubNet
    old_ip = wifi_sta_sn;
    v = server.arg("stasn1"); if (!v.equals(EmptyStr)) wifi_sta_sn[0] = v.toInt();
    v = server.arg("stasn2"); if (!v.equals(EmptyStr)) wifi_sta_sn[1] = v.toInt();
    v = server.arg("stasn3"); if (!v.equals(EmptyStr)) wifi_sta_sn[2] = v.toInt();
    v = server.arg("stasn4"); if (!v.equals(EmptyStr)) wifi_sta_sn[3] = v.toInt();

    // Station Gateway
    old_ip = wifi_sta_gw;
    v = server.arg("stagw1"); if (!v.equals(EmptyStr)) wifi_sta_gw[0] = v.toInt();
    v = server.arg("stagw2"); if (!v.equals(EmptyStr)) wifi_sta_gw[1] = v.toInt();
    v = server.arg("stagw3"); if (!v.equals(EmptyStr)) wifi_sta_gw[2] = v.toInt();
    v = server.arg("stagw4"); if (!v.equals(EmptyStr)) wifi_sta_gw[3] = v.toInt();
      
    if (!v1.equals(EmptyStr)) {
      VLF(wifi_sta_ssid);
      nv.updateBytes(EE_STA_SSID, wifi_sta_ssid, -40);
      nv.updateBytes(EE_STA_PWD, wifi_sta_pwd, -40);
      nv.update(EE_DHCP_EN, (int16_t)stationDhcpEnabled);
      nv.update(EE_STA_EN, (int16_t)stationEnabled);
      for (int i = 0; i < 4; i++) { nv.update(EE_STA_IP + i, wifi_sta_ip[i]); }
      for (int i = 0; i < 4; i++) { nv.update(EE_STA_SN + i, wifi_sta_sn[i]); }
      for (int i = 0; i < 4; i++) { nv.update(EE_STA_GW + i, wifi_sta_gw[i]); }
      restartRequired=true;
    }

    // --------------------------------------------------------------------------
    // Access-Point MAC
    v = server.arg("apmac");
    if (!v.equals(EmptyStr)) {
      // 5c:cf:7f:0f:ad:85
      // first the length should be 17
      if (v.length() == 17) {
        // seperators all in place
        if (v.charAt(2) == ':' && v.charAt(5) == ':' && v.charAt(8) == ':' && v.charAt(11) == ':' && v.charAt(14) == ':') {
          // digits all in 0..9,A..F and validate
          v.toUpperCase();
          uint8_t mac[6];
          int imac[6];
          imac[0] = hexToInt(v.substring(0, 2)); imac[1] = hexToInt(v.substring(3, 2)); imac[2] = hexToInt(v.substring(6, 2));
          imac[3] = hexToInt(v.substring(9, 2)); imac[4] = hexToInt(v.substring(12, 2)); imac[5] = hexToInt(v.substring(15, 2));
          if (imac[0] >= 0 && imac[1] >= 0 && imac[2] >= 0 && imac[3] >= 0 && imac[4] >= 0 && imac[5] >= 0) {
            mac[0] = imac[0]; mac[1] = imac[1]; mac[2] = imac[2]; mac[3] = imac[3]; mac[4] = imac[4]; mac[5] = imac[5];
            WiFi.softAPmacAddress(mac); restartRequired = true; 
          }
        }
      }
    }

    // Access-Point SSID
    v = server.arg("apssid");
    if (!v.equals(EmptyStr)) {
      if (!strcmp(wifi_ap_ssid,(char*)v.c_str())) restartRequired = true;
      strcpy(wifi_ap_ssid, (char*)v.c_str());

      // if this section was submitted set the accessPointEnabled default to false
      accessPointEnabled = false;
    }

    // Access-Point password
    v = server.arg("appwd");
    if (!v.equals(EmptyStr)) {
      if (!strcmp(wifi_ap_pwd, (char*)v.c_str())) restartRequired = true;
      strcpy(wifi_ap_pwd, (char*)v.c_str());
    }

    // Access-Point channel
    v = server.arg("apch");
    if (!v.equals(EmptyStr)) {
      if (wifi_ap_ch != v.toInt()) restartRequired = true;
      wifi_ap_ch = v.toInt();
    }

    // Access-Point enabled
    v = server.arg("apen");
    if (!v.equals(EmptyStr)) {
      accessPointEnabled = v.toInt();
    }

    // Access-Point ip
    old_ip = wifi_ap_ip;
    v = server.arg("apip1"); if (!v.equals(EmptyStr)) wifi_ap_ip[0] = v.toInt();
    v = server.arg("apip2"); if (!v.equals(EmptyStr)) wifi_ap_ip[1] = v.toInt();
    v = server.arg("apip3"); if (!v.equals(EmptyStr)) wifi_ap_ip[2] = v.toInt();
    v = server.arg("apip4"); if (!v.equals(EmptyStr)) wifi_ap_ip[3] = v.toInt();

    // Access-Point SubNet
    old_ip = wifi_ap_sn;
    v = server.arg("apsn1"); if (!v.equals(EmptyStr)) wifi_ap_sn[0] = v.toInt();
    v = server.arg("apsn2"); if (!v.equals(EmptyStr)) wifi_ap_sn[1] = v.toInt();
    v = server.arg("apsn3"); if (!v.equals(EmptyStr)) wifi_ap_sn[2] = v.toInt();
    v = server.arg("apsn4"); if (!v.equals(EmptyStr)) wifi_ap_sn[3] = v.toInt();

    // Access-Point Gateway
    old_ip = wifi_ap_gw;
    v = server.arg("apgw1"); if (!v.equals(EmptyStr)) wifi_ap_gw[0] = v.toInt();
    v = server.arg("apgw2"); if (!v.equals(EmptyStr)) wifi_ap_gw[1] = v.toInt();
    v = server.arg("apgw3"); if (!v.equals(EmptyStr)) wifi_ap_gw[2] = v.toInt();
    v = server.arg("apgw4"); if (!v.equals(EmptyStr)) wifi_ap_gw[3] = v.toInt();

    if (!v.equals(EmptyStr)) {
      nv.writeBytes(EE_AP_SSID, wifi_ap_ssid, -40);
      nv.writeBytes(EE_AP_PWD, wifi_ap_pwd, -40);
      nv.write(EE_AP_CH, (int16_t)wifi_ap_ch);
      nv.write(EE_AP_EN, (int16_t)accessPointEnabled);
      for (int i = 0; i < 4; i++) { nv.update(EE_AP_IP + i, wifi_ap_ip[i]); }
      for (int i = 0; i < 4; i++) { nv.update(EE_AP_SN + i, wifi_ap_sn[i]); }
      for (int i = 0; i < 4; i++) { nv.update(EE_AP_GW + i, wifi_ap_gw[i]); }
      restartRequired = true;
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
      for (int i = 0; i < 4; i++) { nv.update(EE_ETH_IP + i, temp_ip[i]); }
      for (int i = 0; i < 4; i++) { nv.update(EE_ETH_SN + i, temp_sn[i]); }
      for (int i = 0; i < 4; i++) { nv.update(EE_ETH_GW + i, temp_gw[i]); }
      restartRequired = true;
    }
  #endif
}
