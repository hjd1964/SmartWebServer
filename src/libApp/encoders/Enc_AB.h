// ----------------------------------------------------------------------------------------------------------------
// Read Quadrature A/B type encoders
#pragma once

#include "../../Common.h"

// this is for Quadrature A/B type encoders (library based)
#if AXIS1_ENC == AB || AXIS2_ENC == AB

  #include <Encoder.h> // from https://github.com/hjd1964/Encoder 

  #if AXIS1_ENC == AB
    extern Encoder axis1Pos;
  #endif

  #if AXIS2_ENC == AB
    extern Encoder axis2Pos;
  #endif
#endif
