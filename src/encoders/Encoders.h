// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported
#pragma once

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"

#if AXIS1_ENC > 0 && AXIS2_ENC > 0
  #define ENCODERS ON
#endif

#if ENCODERS == ON

  // bring in support for the various encoder types
  #include "Enc_AB.h"
  #include "Enc_CwCcw.h"
  #include "Enc_BiSS_C_BC.h"

  // ----------------------------------------------------------------------------------------------------------------
  // background process position/rate control for encoders 

  extern double  Axis1EncTicksPerDeg;
  extern int16_t Axis1EncRev;
  extern int32_t Axis1EncDiffTo;
  extern int32_t Axis1EncDiffFrom;
  extern int32_t Axis1EncDiffAbs;

  extern double  Axis2EncTicksPerDeg;
  extern int16_t Axis2EncRev;
  extern int32_t Axis2EncDiffTo;
  extern int32_t Axis2EncDiffFrom;
  extern int32_t Axis2EncDiffAbs;

  extern char encAutoSync;

  class Encoders {
  public:
    inline void init() { }

    // automatically sync the encoders from OnStep's position when at home or parked
    void syncFromOnStep();

    // zero absolute encoders from OnStep's position
    #ifdef ENC_HAS_ABSOLUTE
      void zeroFromOnStep();
    #endif
        
    void syncToOnStep();
    void poll();

    #if AXIS1_ENC_RATE_CONTROL == ON
      void clearAverages();
    #endif
    
    double getAxis1();
    double getAxis2();
    bool   validAxis1();
    bool   validAxis2();
    double getOnStepAxis1();
    double getOnStepAxis2();

  private:
    double _osAxis1      = 0;
    double _osAxis2      = 0;
    double _enAxis1      = 0;
    double _enAxis2      = 0;
    bool   _enAxis1Fault = false;
    bool   _enAxis2Fault = false;

};

extern Encoders encoders;

#endif
