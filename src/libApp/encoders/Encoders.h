// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported
#pragma once

#include "../../Common.h"

#if AXIS1_ENC > 0 && AXIS2_ENC > 0
  #define ENCODERS ON
#endif

// bring in support for the various encoder types
#include "Enc_AB.h"
#include "Enc_CwCcw.h"
#include "Enc_BiSS_C_BC.h"

// encoder polling rate in milli-seconds
#define ENCODER_POLLING_RATE_MS 1500

#if defined(ESP8266) || defined(ESP32)
  #define GetClockCount ESP.getCycleCount()
  #define ClockCountToMicros ((uint32_t)ESP.getCpuFreqMHz())
#elif defined(__MK20DX256__)
  #define GetClockCount ARM_DWT_CYCCNT
  #define ClockCountToMicros (F_CPU/1000000L)
#else
  #define GetClockCount micros()
  #define ClockCountToMicros (1L)
#endif

// ----------------------------------------------------------------------------------------------------------------
// background process position/rate control for encoders 

#define EncoderSettingsSize 88
typedef struct EncoderAxis {
  int32_t zero;
  int32_t diffTo;
  double ticksPerDeg;
  int16_t reverse;
} EncoderAxis;

typedef struct EncoderRateControl {
  volatile int32_t staSamples;
  volatile int32_t ltaSamples;
  float rateComp;
  int32_t intpolPhase;
  int32_t intpolMagnitude;
  int32_t propResponse;
  int32_t minGuide;
} EncoderRateControl;

typedef struct EncoderSettings {
    char autoSync;
    EncoderAxis axis1;
    EncoderAxis axis2;
    EncoderRateControl rateCtrl;
} EncoderSettings;

class Encoders {
  public:
    // prepare encoders for operation, init NV if necessary
    void init();

    EncoderSettings settings = {
      false,
      {0, AXIS1_ENC_DIFF_LIMIT_TO, AXIS1_ENC_TICKS_DEG, AXIS1_ENC_REVERSE},
      {0, AXIS2_ENC_DIFF_LIMIT_TO, AXIS2_ENC_TICKS_DEG, AXIS2_ENC_REVERSE},
      {20, 200, 0.0, 1, 0, 10, 100}
    };

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

      double getAxis1();
      double getAxis2();
      bool   validAxis1();
      bool   validAxis2();
      double getOnStepAxis1();
      double getOnStepAxis2();

      #if ENC_AUTO_SYNC_DEFAULT == ON
        char autoSync = true;
      #else
        char autoSync = false;
      #endif

      int32_t Axis1EncDiffFrom    = AXIS1_ENC_DIFF_LIMIT_FROM;
      int32_t Axis1EncDiffAbs     = 0;
      int32_t Axis2EncDiffFrom    = AXIS2_ENC_DIFF_LIMIT_FROM;
      int32_t Axis2EncDiffAbs     = 0;

    private:
      double osAxis1      = 0;
      double osAxis2      = 0;
      double enAxis1      = 0;
      double enAxis2      = 0;
      bool   enAxis1Fault = false;
      bool   enAxis2Fault = false;

    #endif
};

extern Encoders encoders;
