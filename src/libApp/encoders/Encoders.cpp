// -------------------------------------------------------------------------------------------------------
// Handle encoders, both CW/CCW and Quadrature A/B types are supported

#include "Encoders.h"
extern NVS nv;

#include "../status/Status.h"
#include "../cmd/Cmd.h"
#include "../../lib/tasks/OnTask.h"
#include "../../lib/convert/Convert.h"
#include "../misc/Misc.h"

#if defined(ESP8266) || defined(ESP32)
  #include <Esp.h>
#endif

#if ENCODERS == ON
  void pollEncoders() { encoders.poll(); }
#endif

// ----------------------------------------------------------------------------------------------------------------
// background process position/rate control for encoders 

void Encoders::init() { 
  // confirm the data structure size
  if (EncoderSettingsSize < sizeof(EncoderSettings)) { nv.initError = true; DL("ERR: Encoders::init(); EncoderSettingsSize error NV subsystem writes disabled"); }

  // write the default settings to NV
  if (!nv.hasValidKey()) {
    VLF("MSG: Encoders, writing defaults to NV");
    nv.writeBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));
  }

  // read the settings
  nv.readBytes(NV_ENCODER_SETTINGS_BASE, &settings, sizeof(EncoderSettings));

  #if ENCODERS == ON
    VF("MSG: Encoders, start polling task (priority 4)... ");
    if (tasks.add(ENCODER_POLLING_RATE_MS, 0, true, 4, pollEncoders, "EncPoll")) { VLF("success"); } else { VLF("FAILED!"); }
  #endif
}

#if ENCODERS == ON
  void Encoders::syncFromOnStep() {
    if (Axis1EncDiffFrom == OFF || fabs(osAxis1 - enAxis1) <= (double)(Axis1EncDiffFrom/3600.0)) {
      if (settings.axis1.reverse == ON)
        axis1Pos.write(-osAxis1*settings.axis1.ticksPerDeg);
      else
        axis1Pos.write(osAxis1*settings.axis1.ticksPerDeg);
    }
    if (Axis2EncDiffFrom == OFF || fabs(osAxis2 - enAxis2) <= (double)(Axis2EncDiffFrom/3600.0)) {
      if (settings.axis2.reverse == ON)
        axis2Pos.write(-osAxis2*settings.axis2.ticksPerDeg);
      else
        axis2Pos.write(osAxis2*settings.axis2.ticksPerDeg);
    }
  }

  #ifdef ENC_HAS_ABSOLUTE
    void Encoders::zeroFromOnStep() {
      #ifdef ENC_HAS_ABSOLUTE_AXIS1
        if (settings.axis1.reverse == ON)
          axis1Pos.write(-osAxis1*settings.axis1.ticksPerDeg);
        else
          axis1Pos.write(osAxis1*settings.axis1.ticksPerDeg);
        axis1Pos.saveZero();
      #endif
      #ifdef ENC_HAS_ABSOLUTE_AXIS2
        if (settings.axis2.reverse == ON)
          axis2Pos.write(-osAxis2*settings.axis2.ticksPerDeg);
        else
          axis2Pos.write(osAxis2*settings.axis2.ticksPerDeg);
        axis2Pos.saveZero();
      #endif
    }
  #endif

  void Encoders::syncToOnStep() {
    char cmd[40];
    sprintF(cmd, ":SX40,%0.6f#", enAxis1);
    onStep.commandBool(cmd);
    sprintF(cmd, ":SX41,%0.6f#", enAxis2);
    onStep.commandBool(cmd);
    onStep.commandBool(":SX42,1#");
  }

  // check encoders and auto sync OnStep if diff is too great
  void Encoders::poll() {
    char *conv_end;

    if (!status.mountFound) return;

    char result[80];
    if (onStep.command(":GX42#", result) && strlen(result) > 1) {
      double f = strtod(result, &conv_end);
      if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) osAxis1 = f;
    }
    if (onStep.command(":GX43#", result) && strlen(result) > 1) {
      double f = strtod(result, &conv_end);
      if (&result[0] != conv_end && f >= -999.9 && f <= 999.9) osAxis2 = f;
    }

    long pos = axis1Pos.read();
    if (pos == INT32_MAX) enAxis1Fault = true; else enAxis1Fault = false;
    enAxis1 = (double)pos/settings.axis1.ticksPerDeg;
    if (settings.axis1.reverse == ON) enAxis1 = -enAxis1;

    pos = axis2Pos.read();
    if (pos == INT32_MAX) enAxis2Fault = true; else enAxis2Fault = false;
    enAxis2 = (double)pos/settings.axis2.ticksPerDeg;
    if (settings.axis2.reverse == ON) enAxis2 = -enAxis2;

    if (settings.autoSync && status.valid && !enAxis1Fault && !enAxis2Fault) {
      if (status.atHome || status.parked || status.aligning || status.syncToEncodersOnly) {
        syncFromOnStep();
        // re-enable normal operation once we're updated here
        if (status.syncToEncodersOnly) onStep.commandBool(":SX43,1#");
      } else
        if (!status.inGoto && !status.guiding) {
          if ((fabs(osAxis1 - enAxis1) > (double)(settings.axis1.diffTo/3600.0)) ||
              (fabs(osAxis2 - enAxis2) > (double)(settings.axis2.diffTo/3600.0))) syncToOnStep();
      }
    }
  }

  double Encoders::getAxis1() { return enAxis1; }
  double Encoders::getAxis2() { return enAxis2; }
  bool Encoders::validAxis1() { return !enAxis1Fault; }
  bool Encoders::validAxis2() { return !enAxis2Fault; }
  double Encoders::getOnStepAxis1() { return osAxis1; }
  double Encoders::getOnStepAxis2() { return osAxis2; }
#endif

Encoders encoders;

