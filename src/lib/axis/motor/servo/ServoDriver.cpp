// -----------------------------------------------------------------------------------
// axis servo motor driver

#include "ServoDriver.h"

#ifdef SERVO_MOTOR_PRESENT

// the various servo driver models
#define SERVO_DRIVER_MODEL_COUNT (SERVO_DRIVER_LAST - SERVO_DRIVER_FIRST + 1)

#include "../../../gpioEx/GpioEx.h"

#include "../Motor.h"

#if DEBUG != OFF
  const char* SERVO_DRIVER_NAME[SERVO_DRIVER_MODEL_COUNT] =
  {
    "SERVO_PE",         // Phase (Direction)/Enable
    "SERVO_EE",         // Enable/Enable
    "SERVO_TMC2130_DC", // TMC2130 w/ DC motor
    "SERVO_TMC5160_DC", // TMC5160 w/ DC motor
    "SERVO_TMC2209",    // TMC2209 w/ stepper motor
    "SERVO_TMC5160",    // TMC5160 w/ stepper motor
    "SERVO_KTECH"       // KTech servo motor using velocity control
  };
#endif

ServoDriver::ServoDriver(uint8_t axisNumber, const ServoPins *Pins, const ServoSettings *Settings) {
  if (axisNumber < 1 || axisNumber > 9) return;

  this->axisNumber = axisNumber;

  this->Pins = Pins;
  if (Pins != NULL) {
    enablePin = Pins->enable;
    enabledState = Pins->enabledState;
    faultPin = Pins->fault;
  }

  this->Settings = Settings;
  driverModel = Settings->model;
  statusMode = Settings->status;

  velocityMax = Settings->velocityMax;
  acceleration = (Settings->acceleration/100.0F)*velocityMax;
  accelerationFs = acceleration/FRACTIONAL_SEC;
}

bool ServoDriver::init() {
  #if DEBUG == VERBOSE
    VF("MSG:"); V(axisPrefix); VF("init model "); VL(SERVO_DRIVER_NAME[driverModel - SERVO_DRIVER_FIRST]);
    VF("MSG:"); V(axisPrefix); VF("en=");
    if (enablePin == OFF) { VLF("OFF"); } else
    if (enablePin == SHARED) { VLF("SHARED"); } else { VL(enablePin); }
  #endif

  // show velocity control settings
  VF("MSG:"); V(axisPrefix); VF("Vmax="); V(Settings->velocityMax); VF(" steps/s, Acceleration="); V(Settings->acceleration); VLF(" %/s/s");
  VF("MSG:"); V(axisPrefix); VF("AccelerationFS="); V(accelerationFs); VLF(" steps/s/fs");

  // init default driver control pins
  if (enablePin != SHARED) {
    pinModeEx(enablePin, OUTPUT);
    digitalWriteEx(enablePin, !enabledState);
  }

  // automatically set fault status for known drivers
  status.active = statusMode != OFF;
  if (statusMode == ON) statusMode = LOW;
  if (statusMode == LOW) pinModeEx(faultPin, INPUT_PULLUP);
  #ifdef PULLDOWN
    if (statusMode == HIGH) pinModeEx(faultPin, INPUT_PULLDOWN);
  #else
    if (statusMode == HIGH) pinModeEx(faultPin, INPUT);
  #endif

  return true;
}

// update status info. for driver
void ServoDriver::updateStatus() {
  if (statusMode == ON) {
    if ((long)(millis() - timeLastStatusUpdate) > 200) {
      readStatus();

      // open load indication is not reliable in standstill
      if (status.outputA.shortToGround ||
          status.outputB.shortToGround ||
          status.overTemperatureWarning ||
          status.overTemperature) status.fault = true; else status.fault = false;

      timeLastStatusUpdate = millis();
    }
  }

  #if DEBUG == VERBOSE
    if (status.standstill) { status.outputA.openLoad = false; status.outputB.openLoad = false; status.standstill = false; }
    if ((status.outputA.shortToGround     != lastStatus.outputA.shortToGround) ||
        (status.outputA.openLoad          != lastStatus.outputA.openLoad) ||
        (status.outputB.shortToGround     != lastStatus.outputB.shortToGround) ||
        (status.outputB.openLoad          != lastStatus.outputB.openLoad) ||
        (status.overTemperatureWarning    != lastStatus.overTemperatureWarning) ||
        (status.overTemperature           != lastStatus.overTemperature) ||
        (status.standstill                != lastStatus.standstill) ||
        (status.fault                     != lastStatus.fault)) {
      VF("MSG:"); V(axisPrefix); VF("status change ");
      VF("SGA"); if (status.outputA.shortToGround) VF("< "); else VF(". "); 
      VF("OLA"); if (status.outputA.openLoad) VF("< "); else VF(". "); 
      VF("SGB"); if (status.outputB.shortToGround) VF("< "); else VF(". "); 
      VF("OLB"); if (status.outputB.openLoad) VF("< "); else VF(". "); 
      VF("OTP"); if (status.overTemperatureWarning) VF("< "); else VF(". "); 
      VF("OTE"); if (status.overTemperature) VF("< "); else VF(". "); 
      VF("SST"); if (status.standstill) VF("< "); else VF(". "); 
      VF("FLT"); if (status.fault) VLF("<"); else VLF("."); 
    }
    lastStatus = status;
  #endif
};

#endif
