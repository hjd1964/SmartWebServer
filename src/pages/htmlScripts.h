#pragma once

#include "Pages.common.h"

// Javascript for Ajax return
const char html_script_ajax_get[] PROGMEM =
"<script>\n"
"function s(key,v1) {"
  "var xhttp = new XMLHttpRequest();"
  "xhttp.open('GET','%s?'+key+'='+encodeURIComponent(v1)+'&x='+new Date().getTime(), true);"
  "xhttp.send();"
"}</script>\n";

const char html_script_ajax_shortcuts[] PROGMEM =
"<script>\n"
"function g(v1){s('dr',v1);}"
"function gf(v1){s('dr',v1);autoFastRun();}"
"function sf(key,v1){s(key,v1);autoFastRun();}"
"let lzh; function sz(key,v1){clearTimeout(lzh);lzh=setTimeout(s," STR(AJAX_PAGE_LAZY_GET_MS) ",key,v1);}\n"
"</script>\n";

// Javascript for hold-to-run motion controls
const char html_script_motion_controls_1[] PROGMEM =
"<script>"
"function motionControls(o){"
"var m={},q=[],b=0,g=0;"
"function send(v){q.push(v);run();}"
"function run(){if(b||!q.length)return;b=1;var v=q.shift(),x=new XMLHttpRequest(),d=0;function f(){if(d)return;d=1;b=0;run();}x.onreadystatechange=function(){if(x.readyState==4)f();};x.onerror=x.ontimeout=f;x.timeout=1000;x.open('GET',o.p+'?'+o.k+'='+encodeURIComponent(v)+'&x='+new Date().getTime(),true);x.send();}"
"function active(){for(var k in m)if(m[k])return true;return false;}"
"function stopValue(a){return typeof o.s=='function'?o.s(a):o.s;}"
"function allStopValue(){return typeof o.a=='function'?o.a():o.a;}"
"function haltValue(){return typeof o.h=='function'?o.h():o.h||allStopValue()||stopValue('');}"
"function repeat(a,h){return o.g?h==g&&!active():!m[a];}"
"function stop(a){if(!m[a])return;m[a]=0;var h=++g,v=stopValue(a);send(v);setTimeout(function(){if(repeat(a,h))send(v)},150);setTimeout(function(){if(repeat(a,h))send(v)},500);}"
"function clearActive(){var r=0;for(var k in m){if(m[k])r=1;m[k]=0;}return r;}";

const char html_script_motion_controls_2[] PROGMEM =
"function repeatSend(v){send(v);setTimeout(function(){send(v)},150);setTimeout(function(){send(v)},500);}"
"function stopAll(){var v=allStopValue();if(v){if(clearActive()){g++;repeatSend(v);}return;}for(var k in m)stop(k);}"
"function halt(){clearActive();g++;repeatSend(haltValue());}"
"function down(e,v,a){if(e&&e.preventDefault)e.preventDefault();a=a||v;m[a]=1;g++;send(v);}"
"function up(e,a){if(e&&e.preventDefault)e.preventDefault();stop(a);}"
"function pointerDown(e,v,a){if(e.pointerType=='touch')return;down(e,v,a);}"
"function pointerUp(e,a){if(e.pointerType=='touch')return;up(e,a);}"
"function windowUp(e){if(e&&e.pointerType=='touch')return;stopAll();}"
"function cancel(e){if(e&&e.preventDefault)e.preventDefault();return false;}"
"window.addEventListener('pointerup',windowUp);"
"window.addEventListener('pointercancel',windowUp);"
"window.addEventListener('touchend',stopAll);"
"window.addEventListener('touchcancel',stopAll);"
"window.addEventListener('blur',stopAll);"
"document.addEventListener('visibilitychange',function(){if(document.hidden)stopAll();});"
"return{send:send,stop:stop,stopAll:stopAll,halt:halt,t:down,u:up,p:pointerDown,r:pointerUp,c:cancel};"
"}"
"</script>\n";

// Javascript for Date/Time return
const char html_script_ajax_date_time_return[] PROGMEM =
"<script>\n"
"function SetDateTime() {"
"var d1 = new Date();"
"var jan = new Date(d1.getFullYear(), 0, 1);"
"var d = new Date(d1.getTime()-(jan.getTimezoneOffset()-d1.getTimezoneOffset())*60*1000);"
"document.getElementById('dd').value = d.getDate();"
"document.getElementById('dm').value = d.getMonth();"
"document.getElementById('dy').value = d.getFullYear();"
"document.getElementById('th').value = d.getHours();"
"document.getElementById('tm').value = d.getMinutes();"
"document.getElementById('ts').value = d.getSeconds();"
"}\n"
"</script>\n";

// Javascript for Collapsibles
const char html_script_collapsible[] PROGMEM =
"<script>"
"var cc = document.getElementsByClassName('collapsible');"
"var i;"
"for (i = 0; i < cc.length; i++) {"
  "cc[i].addEventListener('click', function() {"
    "this.classList.toggle('active');"
    "var ct = this.nextElementSibling;"
    "if (ct.style.display === 'block') { ct.style.display = 'none'; } else { ct.style.display = 'block'; }"
  "});"
"}"
"</script>\n";

// Javascript for Ajax active refresh of controls
// be sure to define "var ajaxPage='control.txt';" etc.
const char html_script_ajax[] PROGMEM =
"<script>\n"
"function pad(num, size) { var s = '000000000' + num; return s.substr(s.length-size); }\n"
"function update_date_time() {\n"
  "var now = new Date();\n"
  "document.getElementById('datetime').innerHTML = \n"
  "(now.getUTCMonth()+1)+'/'+pad(now.getUTCDate().toString(),2)+'/'+pad(now.getUTCFullYear().toString().substr(-2),2)+' '+\n"
  "pad(now.getUTCHours().toString(),2)+':'+pad(now.getUTCMinutes().toString(),2)+':'+pad(now.getUTCSeconds().toString(),2);\n"
"}\n"
"var auto1Tick=-1;\n"
"var auto2Tick=0;\n"
"var auto2Rate=" STR(AJAX_PAGE_UPDATE_RATE_MS) "/10;\n"
"var auto1=setInterval(autoRun,10);\n"
"function autoFastRun() {\n"
  "auto2Rate=" STR(AJAX_PAGE_UPDATE_RATE_FAST_MS) "/10\n"
  "auto2Tick=" STR(AJAX_PAGE_UPDATE_FAST_SHED_MS) "/10;\n"
"}\n"
"function autoRun() {\n"
  "auto1Tick++;\n"
  "var i;\n"
  "if (auto2Tick>=0) auto2Tick--;\n"
  "if (auto2Tick==0) auto2Rate=" STR(AJAX_PAGE_UPDATE_RATE_MS) "/10;\n"
  "if (auto1Tick%auto2Rate==0) {\n"
    "nocache='?nocache='+Math.random()*1000000;\n"
    "var request = new XMLHttpRequest();\n"
    "request.onreadystatechange = pageReady(ajaxPage);\n"
    "request.open('GET',ajaxPage.toLowerCase()+nocache,true); request.send(null);\n"
  "}\n"
"}\n"
"function pageReady(aPage) {\n"
  "return function() {\n"
    "if ((this.readyState==4)&&(this.status==200)) {\n"
      "lines=this.responseText.split('\\n');\n"
      "for (var i=0; i<lines.length; i++) {\n"
        "var j=lines[i].indexOf('|');\n"
        "var m=0;\n"
        "if (j==-1) {j=lines[i].indexOf('&');m=1;}\n"
        "var v=lines[i].slice(j+1);\n"
        "var k=lines[i].slice(0,j);\n"
        "if (k=='call' && v=='update_date_time') {\n"
          "update_date_time();\n"
        "} else {\n"
          "if (k!='') {"
            "var e=document.getElementById(k);\n"
            "if (e!=null) {\n"
              "if (m==1) e.value=v; else\n"
              "if (v=='selected') e.style.background='#E02020'; else\n"
              "if (v=='unselected') e.style.background='#B02020'; else\n"
              "if (v=='disabled') e.disabled=true; else\n"
              "if (v=='enabled') e.disabled=false; else\n"
              "e.innerHTML=v;\n"
            "}\n"
          "}\n"
        "}\n"
      "}\n"
    "}\n"
  "}\n"
"}\n"
"</script>\n";
