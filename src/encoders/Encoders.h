// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported
#pragma once

#include "../SmartWebServer.h"

#if AXIS1_ENC > 0 && AXIS2_ENC > 0
  #define ENCODERS ON
#endif

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
  // prepare encoders for operation, init NV if necessary
  void init();

#if ENCODERS == ON
  // automatically sync the encoders from OnStep's position when at home or parked
  void syncFromOnStep();

  // zero absolute encoders from OnStep's position
  #ifdef ENC_HAS_ABSOLUTE
    void zeroFromOnStep();
  #endif

  // automatically sync OnStep to the encoders' position
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
#endif
};

extern Encoders encoders;
extern bool encSweep;
extern bool encRateControl;
extern float axis1EncRateComp;
extern long Axis1EncProp;
extern long Axis1EncMinGuide;
extern volatile long Axis1EncStaSamples;
extern volatile long Axis1EncLtaSamples;
extern float axis1EncRateSta;
extern float axis1EncRateLta;
extern float axis1EncRateComp;
extern float axis1Rate;
extern long guideCorrectionMillis;
