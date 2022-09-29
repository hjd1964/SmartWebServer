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
  // bring in support for the various encoder types
  #include "../../lib/encoder/as37h39bb/As37h39bb.h"
  #include "../../lib/encoder/cwCcw/CwCcw.h"
  #include "../../lib/encoder/pulseDir/PulseDir.h"
  #include "../../lib/encoder/quadrature/Quadrature.h"

  void pollEncoders() { encoders.poll(); }

  #if AXIS1_ENCODER == AB
    Quadrature encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == CW_CCW
    CwCcw encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == PULSE_DIR
    PulseDir encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == PULSE_ONLY
    PulseOnly encAxis1(AXIS1_ENCODER_A_PIN, &servoControlAxis1.directionHint, 1);
  #elif AXIS1_ENCODER == AS37_H39B_B
    As37h39bb encAxis1(AXIS1_ENCODER_A_PIN, AXIS1_ENCODER_B_PIN, 1);
  #elif AXIS1_ENCODER == SERIAL_BRIDGE
    SerialBridge encAxis1(1);
  #endif

  #if AXIS2_ENCODER == AB
    Quadrature encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == CW_CCW
    CwCcw encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == PULSE_DIR
    PulseDir encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == PULSE_ONLY
    PulseOnly encAxis2(AXIS2_ENCODER_A_PIN, &servoControlAxis2.directionHint, 2);
  #elif AXIS2_ENCODER == AS37_H39B_B
    As37h39bb encAxis2(AXIS2_ENCODER_A_PIN, AXIS2_ENCODER_B_PIN, 2);
  #elif AXIS2_ENCODER == SERIAL_BRIDGE
    SerialBridge encAxis2(2);
  #endif
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
    encAxis1.init();
    encAxis2.init();

    #ifdef AXIS1_ENCODER_ABSOLUTE
      encAxis1.offset = nv.readL(NV_ENCODER_A1_ZERO);
    #endif
    #ifdef AXIS2_ENCODER_ABSOLUTE
      encAxis2.offset = nv.readL(NV_ENCODER_A2_ZERO);
    #endif

    VF("MSG: Encoders, start polling task (priority 4)... ");
    if (tasks.add(ENCODER_POLLING_RATE_MS, 0, true, 4, pollEncoders, "EncPoll")) { VLF("success"); } else { VLF("FAILED!"); }
  #endif
}

#if ENCODERS == ON
  void Encoders::syncFromOnStep(bool force) {
    if (Axis1EncDiffFrom == OFF || force || fabs(osAxis1 - enAxis1) <= (double)(Axis1EncDiffFrom/3600.0)) {
      if (settings.axis1.reverse == ON)
        encAxis1.write(-osAxis1*settings.axis1.ticksPerDeg);
      else
        encAxis1.write(osAxis1*settings.axis1.ticksPerDeg);
    }
    if (Axis2EncDiffFrom == OFF || force || fabs(osAxis2 - enAxis2) <= (double)(Axis2EncDiffFrom/3600.0)) {
      if (settings.axis2.reverse == ON)
        encAxis2.write(-osAxis2*settings.axis2.ticksPerDeg);
      else
        encAxis2.write(osAxis2*settings.axis2.ticksPerDeg);
    }
  }

  #ifdef ENC_ABSOLUTE
    void Encoders::zeroFromOnStep() {
      #ifdef AXIS1_ENCODER_ABSOLUTE
        if (settings.axis1.reverse == ON)
          encAxis1.write(-osAxis1*settings.axis1.ticksPerDeg);
        else
          encAxis1.write(osAxis1*settings.axis1.ticksPerDeg);
        nv.update(NV_ENCODER_A1_ZERO, encAxis1.offset);
      #endif
      #ifdef AXIS2_ENCODER_ABSOLUTE
        if (settings.axis2.reverse == ON)
          encAxis2.write(-osAxis2*settings.axis2.ticksPerDeg);
        else
          encAxis2.write(osAxis2*settings.axis2.ticksPerDeg);
        nv.update(NV_ENCODER_A2_ZERO, encAxis2.offset);
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

    long pos = encAxis1.read();
    if (pos == INT32_MAX) enAxis1Fault = true; else enAxis1Fault = false;
    enAxis1 = (double)pos/settings.axis1.ticksPerDeg;
    if (settings.axis1.reverse == ON) enAxis1 = -enAxis1;

    pos = encAxis2.read();
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
  bool   Encoders::validAxis1() { return !enAxis1Fault; }
  bool   Encoders::validAxis2() { return !enAxis2Fault; }
  double Encoders::getOnStepAxis1() { return osAxis1; }
  double Encoders::getOnStepAxis2() { return osAxis2; }
#endif

Encoders encoders;

