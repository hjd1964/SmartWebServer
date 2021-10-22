/*
 * Title       OnStep Smart Web Server
 * by          Howard Dutton
 *
 * Copyright (C) 2016 to 2021 Howard Dutton
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Revision History, see GitHub
 *
 * Author: Howard Dutton
 * http://www.stellarjourney.com
 * hjd1964@gmail.com
 *
 * Description
 *
 * Web and IP Servers for OnStep and OnStepX
 *
 */

#define Product "Smart Web Server"
#define FirmwareVersionMajor  "2"
#define FirmwareVersionMinor  "01"
#define FirmwareVersionPatch  "b"

// Use Config.h to configure the SWS to your requirements

#include "src/Common.h"
NVS nv;
#include "src/lib/tasks/OnTask.h"
#include "src/libApp/cmd/Cmd.h"
#include "src/libApp/bleGamepad/BleGamepad.h"
#include "src/libApp/encoders/Encoders.h"
#include "src/pages/Pages.h"
#include "src/libApp/status/MountStatus.h"

#include "src/lib/ethernet/cmdServer/CmdServer.h"
#include "src/lib/ethernet/webServer/WebServer.h"
#include "src/lib/wifi/cmdServer/CmdServer.h"
#include "src/lib/wifi/webServer/WebServer.h"

#if PERSISTENT_COMMAND_CHANNEL == ON
  CmdServer persistentCmdSvr1(9996, 120L*1000L, true);
  CmdServer persistentCmdSvr2(9997, 120L*1000L, true);
  CmdServer persistentCmdSvr3(9998, 120L*1000L, true);
#endif

#if STANDARD_COMMAND_CHANNEL == ON
  CmdServer cmdSvr(9999, 2L*1000L);
#endif

void systemServices() {
  nv.poll();
}

void setup(void) {
  #if OPERATIONAL_MODE == WIFI
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
  #endif

  strcpy(firmwareVersion.str, FirmwareVersionMajor "." FirmwareVersionMinor FirmwareVersionPatch);

  // start debug serial port
  if (DEBUG == ON || DEBUG == VERBOSE) SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
  delay(2000);

  VF("MSG: SmartWebServer "); VL(firmwareVersion.str);
  VF("MSG: MCU = "); VF(MCU_STR); V(", "); VF("Pinmap = "); VLF(PINMAP_STR);

  delay(2000);

  // call gamepad BLE initialization
  #if BLE_GAMEPAD == ON
    VLF("MSG: Init BLE");
    bleInit();
  #endif

  // call hardware specific initialization
  VLF("MSG: Init HAL");
  HAL_INIT();

  // if requested, cause defaults to be written back into NV
  if (NV_WIPE == ON) { nv.writeKey(0); }

  // get NV ready
  if (!nv.isKeyValid(INIT_NV_KEY)) {
    VF("MSG: Wipe NV "); V(nv.size); VLF(" Bytes");
    nv.wipe();
    VLF("MSG: Wipe NV waiting for commit");
    nv.wait();
    VLF("MSG: NV reset to defaults");
  } else { VLF("MSG: Correct NV key found"); }

  // get the command and web timeouts
  if (!nv.isKeyValid()) {
    nv.write(NV_TIMEOUT_CMD, (int16_t)cmdTimeout);
    nv.write(NV_TIMEOUT_WEB, (int16_t)webTimeout);
  }
  cmdTimeout = nv.readUI(NV_TIMEOUT_CMD);
  webTimeout = nv.readUI(NV_TIMEOUT_WEB);

  // System services
  // add task for system services, runs at 10ms intervals so commiting 1KB of NV takes about 10 seconds
  VF("MSG: Setup, starting system services task (rate 10ms priority 7)... ");
  if (tasks.add(10, 0, true, 7, systemServices, "SysSvcs")) { VL("success"); } else { VL("FAILED!"); }

  // read settings from NV or init. as required
  VLF("MSG: Init Encoders");
  encoders.init();

  // bring servers up
  #if OPERATIONAL_MODE == WIFI
    VLF("MSG: Init WiFi");
    wifiManager.init();
  #else
    VLF("MSG: Init Ethernet");
    ethernetManager.init();
  #endif
  
  // init is done, write the NV key if necessary
  if (!nv.isKeyValid()) {
    nv.writeKey((uint32_t)INIT_NV_KEY);
    nv.ignoreCache(true);
    if (nv.isKeyValid(INIT_NV_KEY)) { DLF("ERR: NV reset failed to read back key!"); } else { VLF("MSG: NV reset complete"); }
    nv.ignoreCache(false);
  }

  #if LED_STATUS != OFF
    pinMode(LED_STATUS_PIN, OUTPUT);
  #endif

  // attempt to connect to OnStep
  int serialSwap = OFF;
  if (OPERATIONAL_MODE == WIFI) serialSwap = SERIAL_SWAP;
  if (serialSwap == AUTO) serialSwap = AUTO_OFF;

  long serial_baud = SERIAL_BAUD;
  onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
  uint8_t tb = 1;

Again:
  VLF("MSG: Clearing serial channel");
  onStep.clearSerialChannel();

  // look for OnStep
  VLF("MSG: Attempting to contact OnStep");
  SERIAL_ONSTEP.print(":GVP#"); delay(100);
  String s = SERIAL_ONSTEP.readString();
  if (s == "On-Step#") {
    // if there is more than one baud rate specified
    if (SERIAL_BAUD != SERIAL_BAUD_DEFAULT) {
      // get fastest baud rate, Mega2560 returns '4' for 19200 baud recommended
      SERIAL_ONSTEP.print(":GB#"); delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again; }
      if (SERIAL_ONSTEP.read() == '4' && serial_baud > 19200) serial_baud = 19200;
      // set faster baud rate
      SERIAL_ONSTEP.print(onStep.highSpeedCommsStr(serial_baud)); delay(100);
      if (SERIAL_ONSTEP.available() != 1) { onStep.serialRecvFlush(); goto Again; }
      if (SERIAL_ONSTEP.read() != '1') goto Again;
    }

    // we're all set, just change the baud rate to match OnStep
    onStep.serialBegin(serial_baud, serialSwap);

    #if DEBUG == REMOTE
      debugRemoteConnected = true;
    #endif
    
    VLF("MSG: OnStep Connection established");
  } else {
    if (DEBUG == ON || DEBUG == VERBOSE) { VF("MSG: No valid reply found ("); V(s); VL(")"); }
    #if LED_STATUS == ON
      digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    #endif
    // got nothing back, toggle baud rate and/or swap ports
    onStep.serialRecvFlush();
    tb++;
    if (tb == 16) { tb = 1; if (serialSwap == AUTO_OFF) serialSwap = AUTO_ON; else if (serialSwap == AUTO_ON) serialSwap = AUTO_OFF; }
    if (tb == 1) onStep.serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
    if (tb == 6) onStep.serialBegin(serial_baud, serialSwap);
    if (tb == 11) { if (SERIAL_BAUD_DEFAULT == 9600) { onStep.serialBegin(19200, serialSwap); } else tb = 15; }
    goto Again;
  }
  onStep.clearSerialChannel();

  #if BLE_GAMEPAD == ON
    bleSetup();
  #endif

  VLF("MSG: Set webpage handlers");
  www.on("/index.htm", handleRoot);
  www.on("/configuration.htm", handleConfiguration);
  www.on("/configurationA.txt", configurationAjaxGet);
  www.on("/settings.htm", handleSettings);
  www.on("/settingsA.txt", settingsAjaxGet);
  www.on("/settings.txt", settingsAjax);
  #if ENCODERS == ON
    www.on("/enc.htm", handleEncoders);
    www.on("/encA.txt", encAjaxGet);
    www.on("/enc.txt", encAjax);
  #endif
  www.on("/library.htm", handleLibrary);
  www.on("/libraryA.txt", libraryAjaxGet);
  www.on("/library.txt", libraryAjax);
  www.on("/control.htm", handleControl);
  www.on("/controlA.txt", controlAjaxGet);
  www.on("/control.txt", controlAjax);
  www.on("/auxiliary.htm", handleAux);
  www.on("/auxiliaryA.txt", auxAjaxGet);
  www.on("/auxiliary.txt", auxAjax);
  www.on("/pec.htm", handlePec);
  www.on("/pec.txt", pecAjax);
  www.on("/net.htm", handleNetwork);
  www.on("/", handleRoot);
  
  www.onNotFound(handleNotFound);

  #if PERSISTENT_COMMAND_CHANNEL == ON
    VLF("MSG: Starting port 9996 cmd server");
    persistentCmdSvr3.begin();
    VLF("MSG: Starting port 9997 cmd server");
    persistentCmdSvr2.begin();
    VLF("MSG: Starting port 9998 cmd server");
    persistentCmdSvr1.begin();
  #endif

  #if STANDARD_COMMAND_CHANNEL == ON
    VLF("MSG: Starting port 9999 cmd server");
    cmdSvr.begin();
  #endif

  VLF("MSG: Starting port 80 web server");
  www.begin();

  // allow time for the background servers to come up
  delay(2000);

  // clear the serial channel one last time
  onStep.clearSerialChannel();

  #if ENCODERS == ON
    VLF("MSG: Starting encoders");
    encoders.init();
  #endif

  if (mountStatus.valid()) {
    mountStatus.update(false);
    delay(100);
  }
    
  VLF("MSG: SmartWebServer ready");
}

void loop(void) {
  #if ENCODERS == ON
    encoders.poll(); Y;
  #endif
  
  #if BLE_GAMEPAD == ON
    bleTimers(); Y;
    bleConnTest(); Y;
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON
    persistentCmdSvr1.handleClient(); Y;
    persistentCmdSvr2.handleClient(); Y;
    persistentCmdSvr3.handleClient(); Y;
  #endif

  #if STANDARD_COMMAND_CHANNEL == ON
    cmdSvr.handleClient(); Y;
  #endif

  www.handleClient();

  tasks.yield();
}
