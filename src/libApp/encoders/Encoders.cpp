// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported

#include "Encoders.h"
extern NVS nv;

#include "../status/MountStatus.h"
#include "../cmd/Cmd.h"
#include "../../lib/convert/Convert.h"
#include "../misc/Misc.h"

#if defined(ESP8266) || defined(ESP32)
  #include <Esp.h>
#endif

#if AXIS1_ENC_RATE_CONTROL == ON
  #define MIN_ENC_PERIOD 0.2
  #define MAX_ENC_PERIOD 5.0

  volatile int32_t Tsta;
  volatile int32_t Tlta;

  unsigned long msPerTickMax = 100;
  #if AXIS1_ENC_BIN_AVG > 0
    volatile uint32_t StaBins[AXIS1_ENC_BIN_AVG];
    volatile uint32_t LtaBins[AXIS1_ENC_BIN_AVG];
    volatile uint32_t T1Bins[AXIS1_ENC_BIN_AVG];
    volatile uint32_t usPerBinTickMin = 100;
    volatile uint32_t usPerBinTickMax = 100;
  #endif

  volatile uint32_t clocksPerTickMin = 100;
  volatile uint32_t clocksPerTickMax = 100;
#endif

// ----------------------------------------------------------------------------------------------------------------
// background process position/rate control for encoders 

void Encoders::init() { 
  // confirm the data structure size
  if (EncoderSettingsSize < sizeof(EncoderSettings)) { nv.readOnly(true); DL("ERR: Encoders::init(); EncoderSettingsSize error NV subsystem writes disabled"); }

  // write the default settings to NV
  if (!nv.isKeyValid()) {
    VLF("MSG: Encoders, writing defaults to NV");
    nv.writeBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));
  }

  // read the settings
  nv.readBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));

  #if AXIS1_ENC_RATE_CONTROL == ON
    arcSecondsPerTick = (1.0/settings.axis1.ticksPerDeg)*3600.0; // (0.0018)*3600 = 6.48
    usPerTick = (arcSecondsPerTick/15.041)*1000000.0;            // 6.48/15.041 = 0.4308 seconds per tick

    msPerTickMax = (arcSecondsPerTick/15.041)*1000.0*MAX_ENC_PERIOD;
    #if AXIS1_ENC_BIN_AVG > 0
      usPerBinTickMin = (double)usPerTick*(double)AXIS1_ENC_BIN_AVG*MIN_ENC_PERIOD;
      usPerBinTickMax = (double)usPerTick*(double)AXIS1_ENC_BIN_AVG*MAX_ENC_PERIOD;
    #endif

    #if defined(ESP8266) || defined(ESP32)
      clocksPerTickMin = (double)usPerTick*(double)ESP.getCpuFreqMHz()*MIN_ENC_PERIOD;
      clocksPerTickMax = (double)usPerTick*(double)ESP.getCpuFreqMHz()*MAX_ENC_PERIOD;
    #elif defined(__MK20DX256__)
      clocksPerTickMin = (double)usPerTick*(double)(F_CPU/1000000L)*MIN_ENC_PERIOD;
      clocksPerTickMax = (double)usPerTick*(double)(F_CPU/1000000L)*MAX_ENC_PERIOD;
    #else
      clocksPerTickMin = (double)usPerTick*MIN_ENC_PERIOD;
      clocksPerTickMax = (double)usPerTick*MAX_ENC_PERIOD;
    #endif
  #endif
}

#if ENCODERS == ON
  void Encoders::syncFromOnStep() {
    if (Axis1EncDiffFrom == OFF || fabs(_osAxis1 - _enAxis1) <= (double)(Axis1EncDiffFrom/3600.0)) {
      if (settings.axis1.reverse == ON)
        axis1Pos.write(-_osAxis1*settings.axis1.ticksPerDeg);
      else
        axis1Pos.write(_osAxis1*settings.axis1.ticksPerDeg);
    }
    if (Axis2EncDiffFrom == OFF || fabs(_osAxis2 - _enAxis2) <= (double)(Axis2EncDiffFrom/3600.0)) {
      if (settings.axis2.reverse == ON)
        axis2Pos.write(-_osAxis2*settings.axis2.ticksPerDeg);
      else
        axis2Pos.write(_osAxis2*settings.axis2.ticksPerDeg);
    }
  }

  #ifdef ENC_HAS_ABSOLUTE
    void Encoders::zeroFromOnStep() {
      #ifdef ENC_HAS_ABSOLUTE_AXIS1
        if (settings.axis1.reverse == ON)
          axis1Pos.write(-_osAxis1*settings.axis1.ticksPerDeg);
        else
          axis1Pos.write(_osAxis1*settings.axis1.ticksPerDeg);
        axis1Pos.saveZero();
      #endif
      #ifdef ENC_HAS_ABSOLUTE_AXIS2
        if (settings.axis2.reverse == ON)
          axis2Pos.write(-_osAxis2*settings.axis2.ticksPerDeg);
        else
          axis2Pos.write(_osAxis2*settings.axis2.ticksPerDeg);
        axis2Pos.saveZero();
      #endif
    }
  #endif

  void Encoders::syncToOnStep() {
    char cmd[40];
    sprintF(cmd, ":SX40,%0.6f#", _enAxis1);
    onStep.commandBool(cmd);
    sprintF(cmd, ":SX41,%0.6f#", _enAxis2);
    onStep.commandBool(cmd);
    onStep.commandBool(":SX42,1#");
  }

  // check encoders and auto sync OnStep if diff is too great, checks every 1.5 seconds
  void Encoders::poll() {
    static unsigned long nextEncCheckMs = millis() + (unsigned long)(POLLING_RATE*1000.0);
    unsigned long temp = millis();
    char *conv_end;
    if ((long)(temp - nextEncCheckMs) > 0) {
      nextEncCheckMs = temp + (unsigned long)(POLLING_RATE*1000.0);

      char result[40];
      if (onStep.command(":GX42#", result) && strlen(result) > 1) {
        double f = strtod(result, &conv_end);
        if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) _osAxis1 = f;
      }
      if (onStep.command(":GX43#", result) && strlen(result) > 1) {
        double f = strtod(result, &conv_end);
        if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) _osAxis2 = f;
      }
      
      long pos = axis1Pos.read();
      if (pos == INT32_MAX) _enAxis1Fault = true; else _enAxis1Fault = false;
      _enAxis1 = (double)pos/settings.axis1.ticksPerDeg;
      if (settings.axis1.reverse == ON) _enAxis1 = -_enAxis1;

      pos = axis2Pos.read();
      if (pos == INT32_MAX) _enAxis2Fault = true; else _enAxis2Fault = false;
      _enAxis2 = (double)pos/settings.axis2.ticksPerDeg;
      if (settings.axis2.reverse == ON) _enAxis2 = -_enAxis2;

      mountStatus.update();
      if (autoSync && mountStatus.valid() && !_enAxis1Fault && !_enAxis2Fault) {
        if (mountStatus.atHome() || mountStatus.parked() || mountStatus.aligning() || mountStatus.syncToEncodersOnly()) {
          syncFromOnStep();
          // re-enable normal operation once we're updated here
          if (mountStatus.syncToEncodersOnly()) onStep.commandBool(":SX43,1#");
        } else
          if (!mountStatus.inGoto() && !mountStatus.guiding()) {
            if ((fabs(_osAxis1 - _enAxis1) > (double)(settings.axis1.diffTo/3600.0)) ||
                (fabs(_osAxis2 - _enAxis2) > (double)(settings.axis2.diffTo/3600.0))) syncToOnStep();
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
        rateSta = usPerTick/Tsta + settings.rateCtrl.rateComp;
        rateLta = usPerTick/Tlta + settings.rateCtrl.rateComp;

        // get the tracking rate OnStep thinks it has once every ten seconds
        static int pass = -1;
        pass++;
        if (pass%5 == 0) {
          char* result = onStep.commandString(":GX49#");
          if (strlen(result) > 1) axis1Rate = atof(result); else axis1Rate = 0;
        }

        // reset averages if rate is too fast or too slow
        static unsigned long resetTimeout = 0;
        if (fastMotion() || slowMotion()) resetTimeout = millis();

        // keep things reset for 15 seconds if just starting up again
        if ((long)(millis() - resetTimeout) < 15000L) { clearAverages(); return; }

        // encoder rate control disabled
        if (!rateControl) return;
        
        #if AXIS1_ENC_INTPOL_COS == ON
          long a1 = axis1Pos.read();
          cosIntpolPhaseF = (a1 + cosIntPolPhase) % cosIntPolPeriod;
          float a3 = (cosIntpolPhaseF/(float)cosIntPolPeriod)*3.1415*2.0;
          cosIntpolComp = cos(a3)*(cosIntPolMag/1000000.0);
          rateSta = rateSta + cosIntpolComp;
        #endif

        #if AXIS1_ENC_RATE_AUTO > 0
          if ((long)(millis() - nextWormPeriod) >= 0) {
            nextWormPeriod = millis()+(unsigned long)(AXIS1_ENC_RATE_AUTO)*997UL;
            settings.rateCtrl.rateComp += rateDelta/(double)(AXIS1_ENC_RATE_AUTO);
            if (settings.rateCtrl.rateComp > +0.01) settings.rateCtrl.rateComp = +0.01;
            if (settings.rateCtrl.rateComp < -0.01) settings.rateCtrl.rateComp = -0.01;
            rateDelta = 0;
          }
          rateDelta += (axis1Rate - rateSta)*POLLING_RATE;
        #endif

        // accumulate tracking rate departures for pulse-guide, rate delta * 2 seconds
        guideCorrection += (axis1Rate - rateSta)*((float)settings.rateCtrl.propResponse/100.0)*POLLING_RATE;

        char cmd[40];
        if (guideCorrection > POLLING_RATE) clearAverages(); else
        if (guideCorrection < -POLLING_RATE) clearAverages(); else
        if (guideCorrection > settings.rateCtrl.minGuide/1000.0) {
          guideCorrectionMillis = round(guideCorrection*1000.0);
          sprintf(cmd, ":Mgw%ld#", guideCorrectionMillis);
          onStep.commandBlind(cmd);
          guideCorrection = 0;
        } else
        if (guideCorrection < -settings.rateCtrl.minGuide/1000.0) {
          guideCorrectionMillis = round(guideCorrection*1000.0);
          sprintf(cmd, ":Mge%ld#", -guideCorrectionMillis);
          onStep.commandBlind(cmd);
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
      rateSta = usPerTick/d;
      rateLta = usPerTick/d;
      guideCorrection = 0.0;
      guideCorrectionMillis = 0;
      #if AXIS1_ENC_RATE_AUTO > 0
        settings.rateCtrl.rateComp = 0.0;
        rateDelta = 0;
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

