// -----------------------------------------------------------------------------------
// axis servo TMC5160 DC motor driver

#include "Tmc5160.h"

#ifdef SERVO_TMC5160_DC_PRESENT

#include "../../../../../tasks/OnTask.h"
#include "../../../../../gpioEx/GpioEx.h"

// help with pin names
#define mosi m0
#define sck  m1
#define cs   m2
#define miso m3

ServoTmc5160DC::ServoTmc5160DC(uint8_t axisNumber, const ServoPins *Pins, const ServoSettings *Settings)
                               :ServoDcDriver(axisNumber, Pins, Settings) {
  if (axisNumber < 1 || axisNumber > 9) return;

  strcpy(axisPrefix, " Axis_ServoTmc5160DC, ");
  axisPrefix[5] = '0' + axisNumber;

  analogWriteRange = 255;
  accelerationFs = acceleration/FRACTIONAL_SEC;
}

bool ServoTmc5160DC::init() {
  if (!ServoDcDriver::init()) return false;

  // get TMC SPI driver ready
  pinModeEx(Pins->ph1, OUTPUT); // step
  pinModeEx(Pins->ph2, OUTPUT); // dir

  // set S/D motion pins to known, fixed state
  digitalWriteEx(Pins->ph1, LOW); // step
  digitalWriteEx(Pins->ph2, LOW); // dir

  VF("MSG:"); V(axisPrefix); VLF("TMC current control at max (IHOLD, IRUN, and IGOTO ignored)");

  driver = new TMC5160Stepper(Pins->cs, Pins->mosi, Pins->miso, Pins->sck);
  driver->begin();
  driver->direct_mode(true);
  driver->en_pwm_mode(true);
  driver->pwm_autoscale(false);
  driver->pwm_ofs(255);
  driver->pwm_grad(4);
  driver->ihold(31);
  driver->irun(31);
  driver->toff(5);
  digitalWriteF(enablePin, enabledState);
  driver->XTARGET((uint32_t)(lround(0) & 0b111111111));

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
void ServoTmc5160DC::enable(bool state) {
  enabled = state;

  VF("MSG:"); V(axisPrefix); VF("powered "); if (state) { VF("up"); } else { VF("down"); } VLF(" using SPI");

  if (state) { driver->ihold(31); } else { driver->ihold(0); }

  velocityRamp = 0.0F;

  ServoDriver::updateStatus();
}

// motor control pwm update
// \param power in SERVO_ANALOG_WRITE_RANGE units
void ServoTmc5160DC::pwmUpdate(long power) {
  driver->XTARGET((uint32_t)(power & 0b111111111));
}

// read status info. from driver
void ServoTmc5160DC::readStatus() {
  TMC2130_n::DRV_STATUS_t status_result;
  status_result.sr = 0;

  status_result.sr = driver->DRV_STATUS();

  status.outputA.shortToGround = status_result.s2ga;
  status.outputA.openLoad      = status_result.ola;
  status.outputB.shortToGround = false;
  status.outputB.openLoad      = false;
  status.overTemperatureWarning= status_result.otpw;
  status.overTemperature       = status_result.ot;
  status.standstill            = status_result.stst;
}

#endif
