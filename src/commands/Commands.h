// -----------------------------------------------------------------------------------
// Help with commands, etc.
#pragma once

#include <Arduino.h>

// start serial comms at the specified rate and with swapping enabled or disabled or automatic
void serialBegin(long baudRate, int swap);

// returns OnStep command to set a given baud rate
const char* highSpeedCommsStr(long baud);

// flushes both the send and recv. buffers
char serialRecvFlush();

// sends 3x empty frames to OnStep to purge noise that might intefere with commands on the interface 
void clearSerialChannel();

// low level smart LX200 aware command and response over serial (includes any '#' frame char)
bool processCommand(const char* cmd, char* response, long timeOutMs);

// send command to OnStep and get any response (no '#' frame char)
bool command(const char* command, char* response);

// send command to OnStep, expects no reply
bool commandBlind(const char* command);

// send command to OnStep for debugging, expects a boolean reply
bool commandEcho(const char* command);

// send command to OnStep, expects a boolean reply
bool commandBool(const char* command);

// send command to OnStep, expects a string reply (no '#' frame char)
char *commandString(const char* command);

// timeout period for the web
extern int webTimeout;

// timeout periiod for the command channel(s)
extern int cmdTimeout;
