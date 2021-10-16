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

// encoder polling rate in seconds, default 1.5
#define POLLING_RATE 1.5

#if AXIS1_ENC_RATE_CONTROL == ON
  #if AXIS1_ENC_BIN_AVG > 0
    extern volatile uint32_t StaBins[AXIS1_ENC_BIN_AVG];
    extern volatile uint32_t LtaBins[AXIS1_ENC_BIN_AVG];
    extern volatile uint32_t T1Bins[AXIS1_ENC_BIN_AVG];
  #endif
  extern volatile int32_t Tsta;
  extern volatile int32_t Tlta;
  extern unsigned long msPerTickMax;
  extern volatile uint32_t usPerBinTickMin;
  extern volatile uint32_t usPerBinTickMax;
  extern volatile uint32_t clocksPerTickMin;
#endif

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

      #if AXIS1_ENC_RATE_CONTROL == ON
        void clearAverages();
      #endif
      
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

      // encoder rate control
      #if AXIS1_ENC_RATE_CONTROL == ON
        // user interface and settings
        bool sweep = true;

        float arcSecondsPerTick;
        float usPerTick;
        
        // averages & rate calculation
        float rateSta = 1.0;
        float rateLta = 1.0;
        float rateControl = 0.0;
        float axis1Rate = 1.0;

        #if AXIS1_ENC_INTPOL_COS == ON
          long cosIntPolPeriod = AXIS1_ENC_BIN_AVG;
          long cosIntPolPhase = 1;
          long cosIntPolMag = 0;
          float cosIntpolComp = 0.0F;
          float cosIntpolPhaseF = 0.0F;
        #endif

        #if AXIS1_ENC_RATE_AUTO > 0
          static unsigned long nextWormPeriod = 0;
          static float rateDelta = 0.0F;
        #endif

        // guiding
        float guideCorrection = 0;
        long guideCorrectionMillis = 0;
      #endif

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
