#pragma once

const char html_libScript2[] PROGMEM =
"<script>\n"
"function t(key,v) { if (v.length==0) v='DELETE'; s(key,v.replace(/ /g, '_')); }\n"
"function busy() {\n"
  "document.getElementById('message').innerHTML='Working...';"
  "document.getElementById('up').disabled=true;"
  "document.getElementById('down').disabled=true;"
"}\n"
"</script>\n";

const char html_libScript1[] PROGMEM =
"<script>\n"
"function s(key,v) {\n"
  "var xhttp = new XMLHttpRequest();\n"
  "var res = 'libraryA.txt?'+key+'='+encodeURIComponent(v)+'&x='+new Date().getTime();\n"
  "xhttp.open('GET', res, true);\n"
  "xhttp.send();\n"
"}\n"
"</script>\n";

const char html_settingsScript[] PROGMEM =
"<script>\n"
"function s(key,v1) {\n"
  "var xhttp = new XMLHttpRequest();\n"
  "xhttp.open('GET', 'settingsA.txt?'+key+'='+v1+'&x='+new Date().getTime(), true);\n"
  "xhttp.send();\n"
"}\n"
"</script>\n";

// Javascript for Ajax return
const char html_encScript1[] PROGMEM =
"<script>"
"function s(key,v1) {"
  "var xhttp = new XMLHttpRequest();"
  "xhttp.open('GET', 'encA.txt?'+key+'='+v1+'&x='+new Date().getTime(), true);"
  "xhttp.send();"
"}"
"</script>\n";

// Javascript for Ajax return
const char html_configScript1[] PROGMEM =
"<script>\n"
"function s(key,v1) {"
  "var xhttp = new XMLHttpRequest();"
  "xhttp.open('GET', 'configurationA.txt?'+key+'='+v1+'&x='+new Date().getTime(), true);"
  "xhttp.send();"
"}</script>\n";

// Javascript for Ajax return
const char html_controlScript1[] PROGMEM =
"<script>\n"
"function s(key,v1) {"
  "var xhttp = new XMLHttpRequest();"
  "xhttp.open('GET', 'controlA.txt?'+key+'='+v1+'&x='+new Date().getTime(), true);"
  "xhttp.send();"
"}</script>\n";

const char html_controlScript2[] PROGMEM =
"<script>\n"
"function g(v1){s('dr',v1);}"
"function gf(v1){s('dr',v1);autoFastRun();}"
"function sf(key,v1){s(key,v1);autoFastRun();}\n"
"</script>\n";

// Javascript for Date/Time return
const char html_controlScript3A[] PROGMEM =
"<script>\r\n"
"function SetDateTime() {"
"var d1 = new Date();"
"var jan = new Date(d1.getFullYear(), 0, 1);"
"var d = new Date(d1.getTime()-(jan.getTimezoneOffset()-d1.getTimezoneOffset())*60*1000);";
const char html_controlScript3B[] PROGMEM =
"document.getElementById('dd').value = d.getDate();"
"document.getElementById('dm').value = d.getMonth();"
"document.getElementById('dy').value = d.getFullYear();";
const char html_controlScript3C[] PROGMEM =
"document.getElementById('th').value = d.getHours();"
"document.getElementById('tm').value = d.getMinutes();"
"document.getElementById('ts').value = d.getSeconds();"
"}\r\n"
"</script>\r\n";

// Javascript for Collapsibles
const char html_collapseScript[] PROGMEM =
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

// Javascript for Ajax
// be sure to define "var ajaxPage='control.txt';" etc.
const char html_ajax_active[] PROGMEM =
"<script>\n"
"var auto1Tick=-1;\n"
"var auto2Tick=0;\n"
"var auto2Rate=" DEFAULT_AJAX_RATE ";\n"
"var auto1=setInterval(autoRun1s,1000);\n"
// update once a second for 15 seconds then drop back to once every 5 seconds
"function autoFastRun() {\n"
  "auto2Rate=" DEFAULT_FAST_AJAX_RATE "\n"
  "auto2Tick=" DEFAULT_AJAX_SHED_TIME ";\n"
"}\n"
"function autoRun1s() {\n"
  "auto1Tick++;\n"
  "var i;\n"
  "if (auto2Tick>=0) auto2Tick--;\n"
  "if (auto2Tick==0) auto2Rate=" DEFAULT_AJAX_RATE ";\n"
  "if (auto1Tick%auto2Rate==0) {\n"
    "nocache='?nocache='+Math.random()*1000000;\n"
    "var request = new XMLHttpRequest();\n"
    "request.onreadystatechange = pageReady(ajaxPage);\n"
    "request.open('GET',ajaxPage.toLowerCase()+nocache,true); request.send(null);\n"
  "}"
"}\n"
"function pageReady(aPage) {\n"
  "return function() {\n"
    "if ((this.readyState==4)&&(this.status==200)) {\n"
      "lines=this.responseText.split('\\n');\n"
      "for (var i=0; i<lines.length; i++) {\n"
        "j=lines[i].indexOf('|');m=0;\n"
        "if (j==-1) {j=lines[i].indexOf('&');m=1;}\n"
        "v=lines[i].slice(j+1);\n"
        "k=lines[i].slice(0,j);\n"
        "if (k!=''&&document.getElementById(k)!=null) {"
        " if (m==1) document.getElementById(k).value=v; else "
        " if (v=='disabled') document.getElementById(k).disabled=true; else"
        " if (v=='enabled') document.getElementById(k).disabled=false; else"
        " document.getElementById(k).innerHTML=v;"
        "}\n"
      "}\n"
    "}"
  "}"
"}\n"
"</script>\n";
