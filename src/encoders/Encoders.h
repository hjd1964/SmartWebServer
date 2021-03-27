// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported
#pragma once

#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"

#if AXIS1_ENC > 0 && AXIS2_ENC > 0
  #define ENCODERS ON
#endif

#if ENCODERS == ON
  // ----------------------------------------------------------------------------------------------------------------
  // background process position/rate control for encoders 
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
