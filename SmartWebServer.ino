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
 * 
 *
 * Revision History, see GitHub
 *
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

// Use Config.h to configure the SWS to your requirements

// See Constants.h for version information

#include "Constants.h"
#include "Config.h"
#include "ConfigX.h"
#include "src/HAL/HAL.h"
NVS nv;
#include "src/debug/Debug.h"
#include "src/tasks/OnTask.h"
Tasks tasks;

#include "src/pinmaps/Models.h"
#include "src/commands/Commands.h"
#include "src/ethernetServers/EthernetServers.h"
#include "src/wifiServers/WifiServers.h"
#include "src/encoders/Encoders.h"
#include "src/pages/Pages.h"

bool connected = false;

void systemServices() {
  nv.poll();
}

void setup(void) {
  // start debug serial port
  if (DEBUG == ON || DEBUG == VERBOSE) SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
  delay(2000);

  VF("WEM: SmartWebServer "); V(FirmwareVersionMajor); V("."); V(FirmwareVersionMinor); VL(FirmwareVersionPatch);
  VF("WEM: MCU =  "); VF(MCU_STR); V(", "); VF("Pinmap = "); VLF(PINMAP_STR);

  // call hardware specific initialization
  VLF("WEM: Init HAL");
  HAL_INIT();

  // System services
  // add task for system services, runs at 10ms intervals so commiting 1KB of NV takes about 10 seconds
  VF("WEM: Setup, starting system services task (rate 10ms priority 7)... ");
  if (tasks.add(10, 0, true, 7, systemServices, "SysSvcs")) { VL("success"); } else { VL("FAILED!"); }

  // if requested, cause defaults to be written back into NV
  if (NV_WIPE == ON) { nv.update(EE_KEY_HIGH, (int16_t)0); nv.update(EE_KEY_LOW, (int16_t)0); }

  // read settings from NV or init. as required
  VLF("WEM: Init Encoders");
  encoders.init();

  VLF("WEM: Init Webserver");
  #if OPERATIONAL_MODE == WIFI
    wifiInit();
  #else
    ethernetInit();
  #endif

  // init is done, write the NV key if necessary
  nv.update(EE_KEY_HIGH, (int16_t)NV_KEY_HIGH); nv.update(EE_KEY_LOW, (int16_t)NV_KEY_LOW);

  #if LED_STATUS != OFF
    pinMode(LED_STATUS_PIN, OUTPUT);
  #endif

  // attempt to connect to OnStep
  int serialSwap = OFF;
  if (OPERATIONAL_MODE == WIFI) serialSwap = SERIAL_SWAP;
  if (serialSwap == AUTO) serialSwap = AUTO_OFF;

  long serial_baud = SERIAL_BAUD;
  serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
  uint8_t tb = 1;

Again:
  VLF("WEM: Clearing serial channel");
  clearSerialChannel();

  // look for OnStep
  VLF("WEM: Attempting to contact OnStep");
  Ser.print(":GVP#"); delay(100);
  String s = Ser.readString();
  if (s == "On-Step#" || s == "OnStepX#") {
    // check for fastest baud rate
    Ser.print(":GB#"); delay(100);
    if (Ser.available() != 1) { serialRecvFlush(); goto Again; }
    // Mega2560 returns '4' for 19200 baud recommended
    if (Ser.read() == '4' && serial_baud > 19200) {
      serial_baud = 19200;
    }

    // set fastest baud rate
    Ser.print(highSpeedCommsStr(serial_baud)); delay(100);
    if (Ser.available() != 1) { serialRecvFlush(); goto Again; }
    if (Ser.read() != '1') goto Again;
    
    // we're all set, just change the baud rate to match OnStep
    serialBegin(serial_baud, serialSwap);

    connected = true;
    VLF("WEM: OnStep Connection established");
  } else {
    if (DEBUG == ON || DEBUG == VERBOSE) { VF("WEM: No valid reply found ("); V(s); VL(")"); }
    #if LED_STATUS == ON
      digitalWrite(LED_STATUS_PIN, LED_STATUS_OFF_STATE);
    #endif
    // got nothing back, toggle baud rate and/or swap ports
    serialRecvFlush();
    tb++;
    if (tb == 16) { tb = 1; if (serialSwap == AUTO_OFF) serialSwap = AUTO_ON; else if (serialSwap == AUTO_ON) serialSwap = AUTO_OFF; }
    if (tb == 1) serialBegin(SERIAL_BAUD_DEFAULT, serialSwap);
    if (tb == 6) serialBegin(serial_baud, serialSwap);
    if (tb == 11) { if (SERIAL_BAUD_DEFAULT == 9600) { serialBegin(19200, serialSwap); } else tb = 15; }
    goto Again;
  }

  // bring servers up
  clearSerialChannel();

  VLF("WEM: Starting port 80 web svr");
  #if OPERATIONAL_MODE == WIFI
    wifiStart();
  #else
    ethernetStart();
  #endif

  VLF("WEM: Set webpage handlers");
  server.on("/index.htm", handleRoot);
  server.on("/configuration.htm", handleConfiguration);
  server.on("/configurationA.txt", configurationAjaxGet);
  server.on("/settings.htm", handleSettings);
  server.on("/settingsA.txt", settingsAjaxGet);
  server.on("/settings.txt", settingsAjax);
  #if ENCODERS == ON
    server.on("/enc.htm", handleEncoders);
    server.on("/encA.txt", encAjaxGet);
    server.on("/enc.txt", encAjax);
  #endif
  server.on("/library.htm", handleLibrary);
  server.on("/libraryA.txt", libraryAjaxGet);
  server.on("/library.txt", libraryAjax);
  server.on("/control.htm", handleControl);
  server.on("/controlA.txt", controlAjaxGet);
  server.on("/control.txt", controlAjax);
  server.on("/auxiliary.htm", handleAux);
  server.on("/auxiliaryA.txt", auxAjaxGet);
  server.on("/auxiliary.txt", auxAjax);
  server.on("/pec.htm", handlePec);
  server.on("/pec.txt", pecAjax);
  server.on("/net.htm", handleNetwork);
  server.on("/", handleRoot);
  
  server.onNotFound(handleNotFound);

  #if STANDARD_COMMAND_CHANNEL == ON
    VLF("WEM: Starting port 9999 cmd svr");
    #if OPERATIONAL_MODE == WIFI
      cmdSvr.begin();
      cmdSvr.setNoDelay(true);
    #else
      cmdSvr.init(9999, 500);
    #endif
  #endif

  #if PERSISTENT_COMMAND_CHANNEL == ON && OPERATIONAL_MODE == WIFI
    VLF("WEM: Starting port 9998 persistant cmd svr");
    persistentCmdSvr.begin();
    persistentCmdSvr.setNoDelay(true);
  #endif

  #if OPERATIONAL_MODE == WIFI
    VLF("WEM: Starting port 80 web svr");
    server.begin();
  #endif

  // allow time for the background servers to come up
  delay(2000);

  // clear the serial channel one last time
  clearSerialChannel();

  #if ENCODERS == ON
    VLF("WEM: Starting encoders");
    encoders.init();
  #endif
    
  VLF("WEM: SmartWebServer ready");
}

void loop(void) {
  server.handleClient();

  #if ENCODERS == ON
    encoders.poll();
  #endif

  #if OPERATIONAL_MODE == WIFI
    wifiCommandChannel();
    wifiPersistantCommandChannel();
  #else
    ethernetCommandChannel();
  #endif

  tasks.yield();
}
