// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported

#include "Encoders.h"
extern NVS nv;

#include "../status/MountStatus.h"
#include "../commands/Commands.h"
#include "../misc/Misc.h"

#if defined(ESP8266) || defined(ESP32)
  #include <Esp.h>
#endif

#if ENC_AUTO_SYNC_DEFAULT == ON
  char encAutoSync = true;
#else
  char encAutoSync = false;
#endif

double  Axis1EncTicksPerDeg = AXIS1_ENC_TICKS_DEG;
int16_t Axis1EncRev         = AXIS1_ENC_REVERSE;
int32_t Axis1EncDiffTo      = AXIS1_ENC_DIFF_LIMIT_TO;
int32_t Axis1EncDiffFrom    = AXIS1_ENC_DIFF_LIMIT_FROM;
int32_t Axis1EncDiffAbs     = 0;
double  Axis2EncTicksPerDeg = AXIS2_ENC_TICKS_DEG;
int16_t Axis2EncRev         = AXIS2_ENC_REVERSE;
int32_t Axis2EncDiffTo      = AXIS2_ENC_DIFF_LIMIT_TO;
int32_t Axis2EncDiffFrom    = AXIS2_ENC_DIFF_LIMIT_FROM;
int32_t Axis2EncDiffAbs     = 0;

// encoder polling rate in seconds, default 1.5
#define POLLING_RATE 1.5

// encoder rate control
#if AXIS1_ENC_RATE_CONTROL == ON

  // user interface and settings
  bool encSweep = true;
  bool encRateControl = false;
  long Axis1EncProp = 10;
  long Axis1EncMinGuide = 100;

  float arcSecondsPerTick = (1.0/Axis1EncTicksPerDeg)*3600.0; // (0.0018)*3600 = 6.48
  float usPerTick = (arcSecondsPerTick/15.041)*1000000.0;     // 6.48/15.041 = 0.4308 seconds per tick
  
  // averages & rate calculation
  volatile long Axis1EncStaSamples = 20;
  volatile long Axis1EncLtaSamples = 200;
  volatile int32_t Tsta = 0;
  volatile int32_t Tlta = 0;
  #if AXIS1_ENC_BIN_AVG > 0
    volatile uint32_t StaBins[AXIS1_ENC_BIN_AVG];
    volatile uint32_t LtaBins[AXIS1_ENC_BIN_AVG];
    volatile uint32_t T1Bins[AXIS1_ENC_BIN_AVG];
  #endif
  float axis1EncRateSta = 1.0;
  float axis1EncRateLta = 1.0;
  float axis1EncRateComp = 0.0;
  float axis1Rate = 1.0;

  #if AXIS1_ENC_INTPOL_COS == ON
    long Axis1EncIntPolPeriod = AXIS1_ENC_BIN_AVG;
    long Axis1EncIntPolPhase = 1;
    long Axis1EncIntPolMag = 0;
    float intpolComp = 0;
    float intpolPhase = 0;
  #endif

  #if AXIS1_ENC_RATE_AUTO > 0
    static unsigned long nextWormPeriod = 0;
    static float axis1RateDelta = 0;
  #endif

  // guiding
  float guideCorrection = 0;
  long guideCorrectionMillis = 0;

#endif

// ----------------------------------------------------------------------------------------------------------------
// background process position/rate control for encoders 

void Encoders::init() { 
  if (nv.readI(EE_KEY_HIGH) != NV_KEY_HIGH || nv.readI(EE_KEY_LOW) != NV_KEY_LOW) {
    VLF("SWS: bad NV key, reset Encoder defaults");
    nv.write(EE_ENC_AUTO_SYNC, (int16_t)ENC_AUTO_SYNC_DEFAULT);

    nv.write(EE_ENC_A1_DIFF_TO,(int32_t)AXIS1_ENC_DIFF_LIMIT_TO);
    nv.write(EE_ENC_A1_TICKS,  (double)AXIS1_ENC_TICKS_DEG);
    nv.write(EE_ENC_A1_REV,    (int16_t)AXIS1_ENC_REVERSE);

    nv.write(EE_ENC_A2_DIFF_TO,(int32_t)AXIS2_ENC_DIFF_LIMIT_TO);
    nv.write(EE_ENC_A2_TICKS,  (double)AXIS2_ENC_TICKS_DEG);
    nv.write(EE_ENC_A2_REV,    (int16_t)AXIS2_ENC_REVERSE);

    nv.write(EE_ENC_RC_STA,    (int32_t)20);   // enc short term average samples
    nv.write(EE_ENC_RC_LTA,    (int32_t)200);  // enc long term average samples
    nv.write(EE_ENC_RC_RCOMP,  (int32_t)0);    // enc rate comp
    nv.write(EE_ENC_RC_INTP_P, (int32_t)1);    // intpol phase
    nv.write(EE_ENC_RC_INTP_M, (int32_t)0);    // intpol mag
    nv.write(EE_ENC_RC_PROP,   (int32_t)10);   // prop
    nv.write(EE_ENC_MIN_GUIDE, (int32_t)100);  // minimum guide duration
    nv.write(EE_ENC_A1_ZERO,   (int32_t)0);    // absolute Encoder Axis1 zero
    nv.write(EE_ENC_A2_ZERO,   (int32_t)0);    // absolute Encoder Axis2 zero
  }

  #if ENCODERS == ON
    VLF("SWS: NV reading Encoder settings");
    if (ENC_AUTO_SYNC_MEMORY == ON) encAutoSync = nv.readI(EE_ENC_AUTO_SYNC);
    Axis1EncDiffTo = nv.readL(EE_ENC_A1_DIFF_TO);
    Axis2EncDiffTo = nv.readL(EE_ENC_A2_DIFF_TO);

    Axis1EncTicksPerDeg = nv.readD(EE_ENC_A1_TICKS);
    Axis2EncTicksPerDeg = nv.readD(EE_ENC_A2_TICKS);
    Axis1EncRev = nv.readI(EE_ENC_A1_REV);
    Axis2EncRev = nv.readI(EE_ENC_A2_REV);

    #ifdef ENC_HAS_ABSOLUTE
      axis1Pos.restoreZero();
      axis2Pos.restoreZero();
    #endif

    #if AXIS1_ENC_RATE_CONTROL == ON
      Axis1EncStaSamples = nv.readL(EE_ENC_RC_STA);
      Axis1EncLtaSamples = nv.readL(EE_ENC_RC_LTA);
      long l = nv.readL(EE_ENC_RC_RCOMP);
      axis1EncRateComp = (float)l/1000000.0;
      #if AXIS1_ENC_INTPOL_COS == ON
        Axis1EncIntPolPhase = nv.readL(EE_ENC_RC_INTP_P);
        Axis1EncIntPolMag = nv.readL(EE_ENC_RC_INTP_M);
      #endif
      Axis1EncProp = nv.readL(EE_ENC_RC_PROP);
      Axis1EncMinGuide = nv.readL(EE_ENC_MIN_GUIDE);
    #endif
  #endif
}

#if ENCODERS == ON
  void Encoders::syncFromOnStep() {
    if (Axis1EncDiffFrom == OFF || fabs(_osAxis1 - _enAxis1) <= (double)(Axis1EncDiffFrom/3600.0)) {
      if (Axis1EncRev == ON)
        axis1Pos.write(-_osAxis1*Axis1EncTicksPerDeg);
      else
        axis1Pos.write(_osAxis1*Axis1EncTicksPerDeg);
    }
    if (Axis2EncDiffFrom == OFF || fabs(_osAxis2 - _enAxis2) <= (double)(Axis2EncDiffFrom/3600.0)) {
      if (Axis2EncRev == ON)
        axis2Pos.write(-_osAxis2*Axis2EncTicksPerDeg);
      else
        axis2Pos.write(_osAxis2*Axis2EncTicksPerDeg);
    }
  }

  #ifdef ENC_HAS_ABSOLUTE
    void Encoders::zeroFromOnStep() {
      #ifdef ENC_HAS_ABSOLUTE_AXIS1
        if (Axis1EncRev == ON)
          axis1Pos.write(-_osAxis1*Axis1EncTicksPerDeg);
        else
          axis1Pos.write(_osAxis1*Axis1EncTicksPerDeg);
        axis1Pos.saveZero();
      #endif
      #ifdef ENC_HAS_ABSOLUTE_AXIS2
        if (Axis2EncRev == ON)
          axis2Pos.write(-_osAxis2*Axis2EncTicksPerDeg);
        else
          axis2Pos.write(_osAxis2*Axis2EncTicksPerDeg);
        axis2Pos.saveZero();
      #endif
    }
  #endif

  void Encoders::syncToOnStep() {
    char cmd[40];
    sprintF(cmd, ":SX40,%0.6f#", _enAxis1); commandBool(cmd);
    sprintF(cmd, ":SX41,%0.6f#", _enAxis2); commandBool(cmd);
    commandBool(":SX42,1#");
  }

  // check encoders and auto sync OnStep if diff is too great, checks every 1.5 seconds
  void Encoders::poll() {
    static unsigned long nextEncCheckMs = millis() + (unsigned long)(POLLING_RATE*1000.0);
    unsigned long temp = millis();
    char *conv_end;
    if ((long)(temp - nextEncCheckMs) > 0) {
      nextEncCheckMs = temp + (unsigned long)(POLLING_RATE*1000.0);

      char result[40];
      if (command(":GX42#", result) && strlen(result) > 1) {
        double f = strtod(result, &conv_end);
        if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) _osAxis1 = f;
      }
      if (command(":GX43#", result) && strlen(result) > 1) {
        double f = strtod(result, &conv_end);
        if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) _osAxis2 = f;
      }
      
      long pos = axis1Pos.read();
      if (pos == INT32_MAX) _enAxis1Fault = true; else _enAxis1Fault = false;
      _enAxis1 = (double)pos/(double)Axis1EncTicksPerDeg;
      if (Axis1EncRev == ON) _enAxis1 = -_enAxis1;

      pos = axis2Pos.read();
      if (pos == INT32_MAX) _enAxis2Fault = true; else _enAxis2Fault = false;
      _enAxis2 = (double)pos/(double)Axis2EncTicksPerDeg;
      if (Axis2EncRev == ON) _enAxis2 = -_enAxis2;

      mountStatus.update();
      if (encAutoSync && mountStatus.valid() && !_enAxis1Fault && !_enAxis2Fault) {
        if (mountStatus.atHome() || mountStatus.parked() || mountStatus.aligning() || mountStatus.syncToEncodersOnly()) {
          syncFromOnStep();
          // re-enable normal operation once we're updated here
          if (mountStatus.syncToEncodersOnly()) commandBool(":SX43,1#");
        } else
          if (!mountStatus.inGoto() && !mountStatus.guiding()) {
            if ((fabs(_osAxis1 - _enAxis1) > (double)(Axis1EncDiffTo/3600.0)) ||
                (fabs(_osAxis2 - _enAxis2) > (double)(Axis2EncDiffTo/3600.0))) syncToOnStep();
          }
      }

      // automatic rate compensation
      #if AXIS1_ENC_RATE_CONTROL == ON

        // get the averages
        #if AXIS1_ENC_BIN_AVG > 0
          Tsta = 0; Tlta = 0;
          for (int i = 0; i < AXIS1_ENC_BIN_AVG; i++) { Tsta += StaBins[i]; Tlta += LtaBins[i]; }
          Tsta /= AXIS1_ENC_BIN_AVG; // average
          Tlta /= AXIS1_ENC_BIN_AVG;
          Tsta /= AXIS1_ENC_BIN_AVG; // each period is AXIS1_ENC_BIN_AVG X longer than the step to step frequency
          Tlta /= AXIS1_ENC_BIN_AVG;
        #endif
        axis1EncRateSta = usPerTick/Tsta + axis1EncRateComp;
        axis1EncRateLta = usPerTick/Tlta + axis1EncRateComp;

        // get the tracking rate OnStep thinks it has once every ten seconds
        static int pass = -1;
        pass++;
        if (pass%5 == 0) {
          char* result = commandString(":GX49#");
          if (strlen(result) > 1) axis1Rate = atof(result); else axis1Rate = 0;
        }

        // reset averages if rate is too fast or too slow
        static unsigned long resetTimeout = 0;
        if (fastMotion() || slowMotion()) resetTimeout = millis();

        // keep things reset for 15 seconds if just starting up again
        if ((long)(millis() - resetTimeout) < 15000L) { clearAverages(); return; }

        // encoder rate control disabled
        if (!encRateControl) return;
        
        #if AXIS1_ENC_INTPOL_COS == ON
          long a1 = axis1Pos.read();
          intpolPhase=(a1 + Axis1EncIntPolPhase)%Axis1EncIntPolPeriod;
          float a3 = (intpolPhase/(float)Axis1EncIntPolPeriod)*3.1415*2.0;
          intpolComp = cos(a3)*(Axis1EncIntPolMag/1000000.0);
          axis1EncRateSta = axis1EncRateSta+intpolComp;
        #endif

        #if AXIS1_ENC_RATE_AUTO > 0
          if ((long)(millis() - nextWormPeriod) >= 0) {
            nextWormPeriod = millis()+(unsigned long)(AXIS1_ENC_RATE_AUTO)*997UL;
            axis1EncRateComp += axis1RateDelta/(double)(AXIS1_ENC_RATE_AUTO);
            if (axis1EncRateComp > +0.01) axis1EncRateComp = +0.01;
            if (axis1EncRateComp < -0.01) axis1EncRateComp = -0.01;
            axis1RateDelta = 0;
          }
          axis1RateDelta += (axis1Rate - axis1EncRateSta)*POLLING_RATE;
        #endif

        // accumulate tracking rate departures for pulse-guide, rate delta * 2 seconds
        guideCorrection += (axis1Rate - axis1EncRateSta)*((float)Axis1EncProp/100.0)*POLLING_RATE;

        char cmd[40];
        if (guideCorrection > POLLING_RATE) clearAverages(); else
        if (guideCorrection < -POLLING_RATE) clearAverages(); else
        if (guideCorrection > Axis1EncMinGuide/1000.0) {
          guideCorrectionMillis = round(guideCorrection*1000.0);
          sprintf(cmd, ":Mgw%ld#", guideCorrectionMillis); commandBlind(cmd);
          guideCorrection = 0;
        } else
        if (guideCorrection < -Axis1EncMinGuide/1000.0) {
          guideCorrectionMillis = round(guideCorrection*1000.0);
          sprintf(cmd, ":Mge%ld#", -guideCorrectionMillis); commandBlind(cmd);
          guideCorrection = 0;
        } else 
          guideCorrectionMillis = 0;
      #endif
    }
  }

  #if AXIS1_ENC_RATE_CONTROL == ON
    void Encoders::clearAverages() {
      double d = usPerTick*axis1Rate;
      #if AXIS1_ENC_BIN_AVG > 0
        for (int i = 0; i < AXIS1_ENC_BIN_AVG; i++) { StaBins[i] = d*AXIS1_ENC_BIN_AVG; LtaBins[i] = d*AXIS1_ENC_BIN_AVG; }
      #endif
      Tsta = d;
      Tlta = d;
      axis1EncRateSta = usPerTick/d;
      axis1EncRateLta = usPerTick/d;
      guideCorrection = 0.0;
      guideCorrectionMillis = 0;
      #if AXIS1_ENC_RATE_AUTO > 0
        axis1EncRateComp = 0.0;
        axis1RateDelta = 0;
        nextWormPeriod = millis() + (unsigned long)(AXIS1_ENC_RATE_AUTO)*997UL;;
      #endif
    }
  #endif

  double Encoders::getAxis1() { return _enAxis1; }
  double Encoders::getAxis2() { return _enAxis2; }
  bool Encoders::validAxis1() { return !_enAxis1Fault; }
  bool Encoders::validAxis2() { return !_enAxis2Fault; }
  double Encoders::getOnStepAxis1() { return _osAxis1; }
  double Encoders::getOnStepAxis2() { return _osAxis2; }
#endif

Encoders encoders;

