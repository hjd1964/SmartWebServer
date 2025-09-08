// -----------------------------------------------------------------------------------
// axis servo DC motor driver
#pragma once

#include <Arduino.h>
#include "../../../../../../Common.h"

#ifdef SERVO_PE_PRESENT

#include "../DcServoDriver.h"

class ServoPE : public ServoDcDriver {
  public:
    // constructor
    ServoPE(uint8_t axisNumber, const ServoPins *Pins, const ServoSettings *Settings);

    // decodes driver model and sets up the pin modes
    bool init();

    // enable or disable the driver using the enable pin or other method
    void enable(bool state);

    // update status info. for driver
    void updateStatus();

    // get the driver name
    const char* name() {
      if (driverModel == SERVO_EE) return "DC Enable/Enable"; else
      if (driverModel == SERVO_PE) return "DC Phase/Enable" ; else
      return "?";
    }

  private:
    // motor control pwm update
    // \param power in SERVO_ANALOG_WRITE_RANGE units
    void pwmUpdate(long power);

    // regulate velocity changes
    float velocityRamp = 0;
};

#endif
