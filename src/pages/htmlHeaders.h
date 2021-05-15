#pragma once

#include "../SmartWebServer.h"
#include "../locales/Locale.h"
#include "../misc/Misc.h"
#include "../commands/Commands.h"
#include "../status/MountStatus.h"
#include "../wifiServers/WifiServers.h"
#include "../ethernetServers/EthernetServers.h"
#include "../encoders/Encoders.h"

const char html_headB[] PROGMEM = "<!DOCTYPE HTML>\r\n"
                                  "<html>\r\n"
                                  "<head>\r\n"
                                  "<meta charset='utf-8'/>\r\n"
                                  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\r\n";
const char html_headerPec[] PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; URL=/pec.htm\">\r\n";
const char html_headerIdx[] PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; URL=/index.htm\">\r\n";
const char html_headE[] PROGMEM = "</head>\r\n";
const char html_bodyB[] PROGMEM = "<body bgcolor='#26262A'>\r\n";

const char html_main_cssB[] PROGMEM = 
"<STYLE>";
const char html_main_css1[] PROGMEM = 
".clear { clear: both; } .a { background-color: #111111; } "
".t { padding: 10px 10px 20px 10px; width: 640px; border: 5px solid #551111; margin: 25px 25px 0px 25px;";
const char html_main_css2[] PROGMEM = 
"color: #999999; background-color: #111111; min-width: 30em; } "
"input { text-align:center; padding: 2px; margin: 3px; font-weight: bold; width:5em; background-color: #B02020}";
const char html_main_css3[] PROGMEM = 
".b { padding: 10px; border-left: 5px solid #551111; border-right: 5px solid #551111; border-bottom: 5px solid #551111; margin: 0px 25px 25px 25px; width: 640px; color: #999999;";
const char html_main_css4[] PROGMEM = 
"background-color: #111111; min-width: 30em; } select { width:4em; font-weight: bold; background-color: #B02020; padding: 2px 2px; } "
".c { color: #B02020; font-weight: bold; }";
const char html_main_css5[] PROGMEM = 
"h1 { text-align: right; } a:hover, a:active { background-color: red; } .y { color: #FFFF00; font-weight: bold; }";
const char html_main_css6[] PROGMEM = 
"a:link, a:visited { background-color: #332222; color: #A07070; border:1px solid red; padding: 5px 10px;";
const char html_main_css7[] PROGMEM = 
" margin: none; text-align: center; text-decoration: none; display: inline-block; }";
const char html_main_css8[] PROGMEM = 
"button:disabled { background-color: #733; color: #000; } button { background-color: #B02020; font-weight: bold; border-radius: 5px; margin: 2px; padding: 4px 8px; }";
const char html_main_css_btns1[] PROGMEM = 
".btns_left { margin-left: -1px; border-top-left-radius: 0px; border-bottom-left-radius: 0px; }";
const char html_main_css_btns2[] PROGMEM = 
".btns_mid { margin-left: -1px; margin-right: -1px; border-top-left-radius: 0px; border-bottom-left-radius: 0px; border-top-right-radius: 0px; border-bottom-right-radius: 0px; }";
const char html_main_css_btns3[] PROGMEM = 
".btns_right { margin-right: -1px; border-top-right-radius: 0px; border-bottom-right-radius: 0px; }";
const char html_main_css_control1[] PROGMEM = 
".b1 { float: left; border: 2px solid #551111; background-color: #181818; text-align: center; margin: 5px; padding: 15px; padding-top: 3px; }";
const char html_main_css_control2[] PROGMEM = 
".gb {  font-weight: bold; font-size: 150%; font-family: 'Times New Roman', Times, serif; width: 60px; height: 50px; padding: 0px; }";
const char html_main_css_control3[] PROGMEM = 
".bb {  font-weight: bold; font-size: 105%; } .bbh {  font-weight: bold; font-size: 100%; height: 2.1em; }";
const char html_main_css_collapse1[] PROGMEM = 
".collapsible { background-color: #500808; color: #999; cursor: pointer; padding: 7px; width: 80%; border: none; text-align: left; outline: none; font-size: 14px; }";
const char html_main_css_collapse2[] PROGMEM = 
".active, .collapsible:hover { background-color: #661111; } .content { padding: 0px 18px; display: none; overflow: hidden; background-color: #301212; }";
const char html_main_cssE[] PROGMEM = 
"</STYLE>";
