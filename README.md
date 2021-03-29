# OnStep Smart Web Server

***
THIS IS AN PRELIMINARY DESIGN, IT COMPILES CORRECTLY FOR ETHERNET AND WIFI.  IT BASICALLY SEEMS TO WORK.
***

This is the WiFi and Ethernet webserver for OnStep.
It enables OnStep to be controlled over IP. This access can be over command channel(s) and/or website.
This server is known to work from personal computers (using a browser), phones and tablets,
(using the Android OnStep App), Sky Safari, and my ASCOM driver (PC.)

# Installing the board Platform for your hardware
Before you can flash the firmware on your ESP8266 or ESP32 device, you need to install the ESP8266 OR ESP32
platform for the Arduino IDE.  If using an Arduino M0 or Teensy you don't need to perform this step.  For the
Teensy's you instead need to install the Teensyduino.exe package to add support to the Arduino IDE.

Under the menu File->Preferences open the dialog and add this line in the additional "Boards Manager" area:
  http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://dl.espressif.com/dl/package_esp32_index.json


Then, from the "Tools, Boards, Boards Manager" menu select and install the ESP8266, ESP32, etc. as required.  

# Flashing The Smart Web Server
The exact flashing procedure depends on which device you will be using.

In the Arduino IDE, pick your device from "Tools, Boards" menu.

Most devices have a USB port you plug into then a virtual serial port appears, direct the Arduino IDE to use that serial port and upload.  Be sure the device is not electrically connect to anything but the USB cable when said USB cable is plugged it.

# Starting Up
The servers will only start if OnStep is detected on the designated device serial port.  Usually there is an device LED that will flash while the SWS attempts to connect then turn steady on once the connection is established.

