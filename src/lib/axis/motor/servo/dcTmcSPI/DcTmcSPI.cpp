// -----------------------------------------------------------------------------------
// axis servo TMC2130 and TMC5160 DC motor driver

#include "DcTmcSPI.h"

#ifdef SERVO_DC_TMC_SPI_PRESENT

#include "../../../../tasks/OnTask.h"
#include "../../../../gpioEx/GpioEx.h"

// help with pin names
#define mosi m0
#define sck  m1
#define cs   m2
#define miso m3

ServoDcTmcSPI::ServoDcTmcSPI(uint8_t axisNumber, const ServoDcTmcSpiPins *Pins, const ServoDcTmcSettings *TmcSettings) {
  this->axisNumber = axisNumber;

  strcpy(axisPrefix, " Axis_ServoTmcDC, ");
  axisPrefix[5] = '0' + axisNumber;

  this->Pins = Pins;
  enablePin = Pins->enable;
  enabledState = Pins->enabledState;
  faultPin = Pins->fault;

  this->Settings = TmcSettings;
  model = TmcSettings->model;
  statusMode = TmcSettings->status;
  velocityMax = (Settings->velocityMax/100.0F)*255*getVelocityScale();
  acceleration = (Settings->acceleration/100.0F)*velocityMax;
  accelerationFs = acceleration/FRACTIONAL_SEC;
}

bool ServoDcTmcSPI::init() {
  ServoDriver::init();

  // automatically set fault status for known drivers
  status.active = statusMode != OFF;

  // get TMC SPI driver ready
  pinModeEx(Pins->step, OUTPUT);
  pinModeEx(Pins->dir, OUTPUT);

  // set S/D motion pins to known, fixed state
  digitalWriteEx(Pins->step, LOW);
  digitalWriteEx(Pins->dir, LOW);

  // show velocity control settings
  VF("MSG:"); V(axisPrefix); VF("Vmax="); V(Settings->velocityMax); VF("% power, Acceleration="); V(Settings->acceleration); VLF("%/s");
  VF("MSG:"); V(axisPrefix); VF("AccelerationFS="); V(accelerationFs); VLF("%/s/fs");

  VF("MSG:"); V(axisPrefix); VLF("TMC current control at max (IHOLD, IRUN, and IGOTO ignored)");

  if (model == SERVO_TMC2130_DC) {
    driver = new TMC2130Stepper(Pins->cs, Pins->mosi, Pins->miso, Pins->sck);
    ((TMC2130Stepper*)driver)->begin();
    ((TMC2130Stepper*)driver)->direct_mode(true);
    ((TMC2130Stepper*)driver)->en_pwm_mode(true);
    ((TMC2130Stepper*)driver)->pwm_autoscale(false);
    ((TMC2130Stepper*)driver)->pwm_ampl(255);
    ((TMC2130Stepper*)driver)->pwm_grad(4);
    ((TMC2130Stepper*)driver)->ihold(31);
    ((TMC2130Stepper*)driver)->irun(31);
    ((TMC2130Stepper*)driver)->toff(5);
    digitalWriteF(enablePin, enabledState);
    ((TMC2130Stepper*)driver)->XDIRECT((uint32_t)(lround(0) & 0b111111111));
  } else
  if (model == SERVO_TMC5160_DC) {
    driver = new TMC5160Stepper(Pins->cs, Pins->mosi, Pins->miso, Pins->sck);
    ((TMC5160Stepper*)driver)->begin();
    ((TMC5160Stepper*)driver)->direct_mode(true);
    ((TMC5160Stepper*)driver)->en_pwm_mode(true);
    ((TMC5160Stepper*)driver)->pwm_autoscale(false);
    ((TMC5160Stepper*)driver)->pwm_ofs(255);
    ((TMC5160Stepper*)driver)->pwm_grad(4);
    ((TMC5160Stepper*)driver)->ihold(31);
    ((TMC5160Stepper*)driver)->irun(31);
    ((TMC5160Stepper*)driver)->toff(5);
    digitalWriteF(enablePin, enabledState);
    ((TMC5160Stepper*)driver)->XTARGET((uint32_t)(lround(0) & 0b111111111));
  }

  // automatically set fault status for known drivers
  status.active = statusMode == ON;

  // check to see if the driver is there and ok
  #ifdef MOTOR_DRIVER_DETECT
    #ifndef DRIVER_TMC_STEPPER_HW_SPI
      if (Pins->miso != OFF)
    #endif
    {
      readStatus();
      if (!status.standstill || status.overTemperature) { DF("ERR:"); D(axisPrefix); DLF("no driver detected!"); return false; }
    }
  #endif

  return true;
}

// enable or disable the driver using the enable pin or other method
void ServoDcTmcSPI::enable(bool state) {
  enabled = state;

  VF("MSG:"); V(axisPrefix); VF("powered "); if (state) { VF("up"); } else { VF("down"); } VLF(" using SPI");

  if (state) { driver->ihold(31); } else { driver->ihold(0); }

  currentVelocity = 0.0F;

  ServoDriver::updateStatus();
}

// set motor velocity by adjusting power (0 to 255 for 0 to 100% power)
float ServoDcTmcSPI::setMotorVelocity(float velocity) {
  if (!enabled) velocity = 0.0F;

  if (velocity > velocityMax) velocity = velocityMax; else
  if (velocity < -velocityMax) velocity = -velocityMax;

  if (velocity > currentVelocity) {
    currentVelocity += accelerationFs;
    if (currentVelocity > velocity) currentVelocity = velocity;
  } else
  if (velocity < currentVelocity) {
    currentVelocity -= accelerationFs;
    if (currentVelocity < velocity) currentVelocity = velocity;
  }
  if (currentVelocity >= 0) motorDirection = DIR_FORWARD; else motorDirection = DIR_REVERSE;

  if (model == SERVO_TMC2130_DC) { ((TMC2130Stepper*)driver)->XDIRECT((uint32_t)(lround(currentVelocity/getVelocityScale()) & 0b111111111)); } else
  if (model == SERVO_TMC5160_DC) { ((TMC5160Stepper*)driver)->XTARGET((uint32_t)(lround(currentVelocity/getVelocityScale()) & 0b111111111)); }

  return currentVelocity;
}

// read status info. from driver
void ServoDcTmcSPI::readStatus() {
  TMC2130_n::DRV_STATUS_t status_result;
  status_result.sr = 0;

  if (model == SERVO_TMC2130_DC) { status_result.sr = ((TMC2130Stepper*)driver)->DRV_STATUS(); } else
  if (model == SERVO_TMC5160_DC) { status_result.sr = ((TMC5160Stepper*)driver)->DRV_STATUS(); }

  status.outputA.shortToGround = status_result.s2ga;
  status.outputA.openLoad      = status_result.ola;
  status.outputB.shortToGround = false;
  status.outputB.openLoad      = false;
  status.overTemperatureWarning= status_result.otpw;
  status.overTemperature       = status_result.ot;
  status.standstill            = status_result.stst;
}

#endif
