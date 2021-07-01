// ----------------------------------------------------------------------------------------------------------------
// Read CW/CCW type encoders
#pragma once

#include "../Common.h"

#if AXIS1_ENC == CWCCW || AXIS2_ENC == CWCCW
  class CwCcwEncoder {
    public:
      CwCcwEncoder(int16_t cwPin, int16_t ccwPin, int16_t axis);
      int32_t read();
      void write(int32_t v);
    private:
      int16_t _cwPin;
      int16_t _ccwPin;
      int16_t _axis;
  };

  #if AXIS1_ENC == CWCCW
    extern CwCcwEncoder axis1Pos;
    extern bool fastMotion();
    extern bool slowMotion();
  #endif
  
  #if AXIS2_ENC == CWCCW
    extern CwCcwEncoder axis2Pos;
  #endif

#endif
