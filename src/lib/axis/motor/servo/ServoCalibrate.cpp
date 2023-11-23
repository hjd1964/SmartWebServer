// -----------------------------------------------------------------------------------
// axis servo motor driver encoder calibration

#include "Servo.h"

#ifdef SERVO_MOTOR_PRESENT
#ifdef ABSOLUTE_ENCODER_CALIBRATION

#include "../../../tasks/OnTask.h"

#ifndef AXIS1_SERVO_TRACKING_VELOCITY
#define AXIS1_SERVO_TRACKING_VELOCITY 0
#endif

volatile long _calibrateStepPosition = 0;
int _calStep;
int _calibrateStepPin;

void _calibrateMoveAxis()
{
  static bool toggle = false;
  toggle = !toggle;
  if (toggle)
    digitalWrite(_calibrateStepPin, LOW);
  else
  {
    digitalWrite(_calibrateStepPin, HIGH);
    _calibrateStepPosition += _calStep;
  }
}

void ServoMotor::calibrate(float value)
{
  if (axisNumber != 1)
    return;

  switch ((int)round(value))
  {
  case 0: // stop and [W]rite data
    tasks.setPeriodSubMicros(handle, 0); // idle any timer
    driver->alternateMode(false); // switch back to velocity control
    calibrateMode = CM_NONE;
    velocityOverride = 0.0F;
    endCount = encoder->count;
    calibrationLinearRegression();
    calibrationAveragingWrite();
  break;

  case 1: // start [R]ecording
    calibrationClear();
    V(axisPrefix);
    VLF("started recording encoder delta");
    calibrateMode = CM_RECORDING;
    velocityOverride = -(AXIS1_SERVO_TRACKING_VELOCITY) * SIDEREAL_RATIO;
    startCount = encoder->count;

    // set stepper driver to step/dir mode
    driver->alternateMode(true);

    // mark the position and init the step/dir pins
    noInterrupts();
    _calibrateStepPosition = motorSteps;
    interrupts();
    // m == -0586990, c == -0644885, f == 1.098
     _calibrateStepPosition *= ((AXIS1_SERVO_TRACKING_VELOCITY)/(AXIS1_STEPS_PER_DEGREE/240.0));
    _calibrateStepPin = AXIS1_STEP_PIN;
    if (velocityOverride >= 0) {
      _calStep = -1;
      digitalWriteEx(AXIS1_DIR_PIN, LOW);
    } else {
      _calStep = 1;
      digitalWriteEx(AXIS1_DIR_PIN, HIGH);
    }

    // check to see if we need a timer to move the motor
    if (handle == 0) {
      // create a h/w timer to move the motor at tracking speed
      V(axisPrefix);
      VLF("starting fixed rate step/dir based tracking");
      handle = tasks.add(0, 0, true, 0, _calibrateMoveAxis, "eCal");
      if (!tasks.requestHardwareTimer(handle, 0)) {
        V(axisPrefix);
        VLF("failed to get a hardware timer");
      };
    }
    tasks.setPeriodSubMicros(handle, round(8000000.0F/abs(velocityOverride)));
  break;

  case 2: // [T]rack normally
    tasks.setPeriodSubMicros(handle, 0); // idle any timer
    driver->alternateMode(false); // switch back to velocity control
    calibrateMode = CM_NONE;
    velocityOverride = 0.0F;
  break;

  case 3: // [F]ixed rate tracking (with encoder correction)
    calibrateMode = CM_FIXED_RATE;
    velocityOverride = -(AXIS1_SERVO_TRACKING_VELOCITY) * SIDEREAL_RATIO;

    // set stepper driver to step/dir mode
    driver->alternateMode(true);

    // init the step/dir pins
    _calibrateStepPin = AXIS1_STEP_PIN;
    if (velocityOverride >= 0) {
      _calStep = -1;
      digitalWriteEx(AXIS1_DIR_PIN, LOW);
    } else {
      _calStep = 1;
      digitalWriteEx(AXIS1_DIR_PIN, HIGH);
    }

    // check to see if we need a timer to move the motor
    if (handle == 0) {
      // create a h/w timer to move the motor at tracking speed
      V(axisPrefix);
      VLF("starting fixed rate step/dir based tracking");
      handle = tasks.add(0, 0, true, 0, _calibrateMoveAxis, "eCal");
      if (!tasks.requestHardwareTimer(handle, 0)) {
        V(axisPrefix);
        VLF("failed to get a hardware timer");
      };
    }
    tasks.setPeriodSubMicros(handle, round(8000000.0F/abs(velocityOverride)));
  break;

  case 4: // [!] erase buffer and any recorded data
    calibrationErase();
  break;

  case 5: // [A]pply lowpass filter to buffer
    calibrationLowPass();
  break;

  case 6: // [C]lear buffer (only)
    calibrationClear();
  break;

  case 7: // [L]oad
    calibrationRead("/encoder.dat");
  break;

  case 8: // [S]ave
    calibrationWrite("/encoder.dat");
  break;

  case 9: // debug [P]rint buffer
    calibrationPrint();
  break;

  case 10: // save buffer to [B]ackup file
    calibrationWrite("/encoder.bak");
  break;

  case 11: // reco[V]er buffer from backup
    calibrationRead("/encoder.bak");
  break;

  }
}

void ServoMotor::calibrateRecord(float &velocity, long &motorCounts, long &encoderCounts)
{
  if (axisNumber != 1)
    return;

  static int correctionCount = 0;
  static int32_t correctionSum = 0;

  correctionSum += motorCounts - encoderCounts;
  correctionCount++;

  static uint32_t lastIndex = 0;
  if (encoderIndex() != lastIndex)
  {
    encoderCorrection = correctionSum / correctionCount;
    if (encoderCorrection < -32767)
      encoderCorrection = -32767;
    if (encoderCorrection > 32767)
      encoderCorrection = 32767;
    if (encoderCorrectionBuffer != NULL && lastIndex != 0)
      encoderCorrectionBuffer[lastIndex] = encoderCorrection;
    correctionSum = 0;
    correctionCount = 0;
    lastIndex = encoderIndex();
  }
}

#include "FS.h"
#include "LittleFS.h"

bool ServoMotor::calibrationRead(const char *fileName)
{
  if (axisNumber != 1)
    return false;

  if (!LittleFS.begin(true))
  {
    DL("WRN: ServoMotor::calibrationRead(), filesystem creation failed!");
    return false;
  }

  if (encoderCorrectionBuffer != NULL)
    free(encoderCorrectionBuffer);
  encoderCorrectionBuffer = (int16_t *)malloc(ENCODER_ECM_BUFFER_SIZE * sizeof(encoderCorrectionBuffer[0]));

  if (encoderCorrectionBuffer != NULL)
  {
    calibrationClear();

    File file = LittleFS.open(fileName, FILE_READ);
    if (!file)
    {
      DL("WRN: ServoMotor::calibrationRead(), filesystem open for read failed!");
      return false;
    }

    V(axisPrefix);
    VF("buffer read ");
    V(fileName);
    VLF(" file");
    if (!file.read((uint8_t *)encoderCorrectionBuffer, ENCODER_ECM_BUFFER_SIZE * sizeof(encoderCorrectionBuffer[0])))
    {
      DL("WRN: ServoMotor::calibrationRead(), read buffer failed!");
    };

    file.close();
  }
  else
  {
    DL("WRN: ServoMotor::calibrationRead(), buffer malloc failed!");
  }

  LittleFS.end();

  return true;
}

bool ServoMotor::calibrationWrite(const char *fileName)
{
  if (axisNumber != 1)
    return false;
  if (encoderCorrectionBuffer == NULL)
  {
    DL("WRN: ServoMotor::calibrationWrite(), buffer not allocated!");
    return false;
  }

  if (!LittleFS.begin(true))
  {
    DL("WRN: ServoMotor::calibrationWrite(), filesystem creation failed!");
    return false;
  }

  File file = LittleFS.open(fileName, FILE_WRITE);
  if (!file)
  {
    DL("WRN: ServoMotor::calibrationWrite(), filesystem open for write failed!");
    return false;
  }

  if (!file.write((uint8_t *)encoderCorrectionBuffer, ENCODER_ECM_BUFFER_SIZE * sizeof(encoderCorrectionBuffer[0])))
  {
    DL("WRN: ServoMotor::calibrationWrite(), write buffer failed!");
  }
  else
  {
    V(axisPrefix);
    VF("buffer wrote ");
    V(fileName);
    VLF(" file");
  }

  file.close();

  LittleFS.end();

  return true;
}

bool ServoMotor::calibrationAveragingWrite()
{
  if (axisNumber != 1)
    return false;
  if (encoderCorrectionBuffer == NULL)
  {
    DL("WRN: ServoMotor::calibrationAveragingWrite(), buffer not allocated!");
    return false;
  }

  if (!LittleFS.begin(true))
  {
    DL("WRN: ServoMotor::calibrationAveragingWrite(), filesystem creation failed!");
    return false;
  }

  File file = LittleFS.open("/encoder.dat", FILE_READ);
  if (!file)
  {
    V(axisPrefix);
    VLF("buffer creating encoder data file");

    file = LittleFS.open("/encoder.dat", FILE_WRITE);
    if (!file)
    {
      DL("WRN: ServoMotor::calibrationAveragingWrite(), filesystem open for write failed!");
      return false;
    }

    for (int i = 0; i < ENCODER_ECM_BUFFER_SIZE; i++)
    {
      int16_t ec = ECB_NO_DATA;
      file.write((uint8_t *)&ec, 2);
    }

    file.close();

    file = LittleFS.open("/encoder.dat", FILE_READ);
    if (!file)
    {
      DL("WRN: ServoMotor::calibrationAveragingWrite(), filesystem open for read failed!");
      return false;
    }
  }

  for (int i = 0; i < ENCODER_ECM_BUFFER_SIZE; i++)
  {
    int16_t ec = 0;
    file.read((uint8_t *)&ec, 2);
    float now = encoderCorrectionBuffer[i];
    if (ec != ECB_NO_DATA)
    {
      if (encoderCorrectionBuffer[i] != ECB_NO_DATA)
      {
        encoderCorrectionBuffer[i] = round((ec * 2.0F + encoderCorrectionBuffer[i]) / 3.0F);
      }
      else
      {
        encoderCorrectionBuffer[i] = ec;
      }
    }
    if (encoderCorrectionBuffer[i] != ECB_NO_DATA)
    {
      D(i);
      D(" (");
      D(ec);
      D("*2 + ");
      D(now);
      D(")/3 = ");
      DL(encoderCorrectionBuffer[i]);
    }
    UNUSED(now);
  }

  file.close();

  file = LittleFS.open("/encoder.dat", FILE_WRITE);
  if (!file)
  {
    DL("WRN: ServoMotor::calibrationAveragingWrite(), filesystem open for write failed!");
    return false;
  }

  if (!file.write((uint8_t *)encoderCorrectionBuffer, ENCODER_ECM_BUFFER_SIZE * sizeof(encoderCorrectionBuffer[0])))
  {
    DL("WRN: ServoMotor::calibrationAveragingWrite(), write buffer failed!");
  }
  else
  {
    V(axisPrefix);
    VLF("buffer wrote encoder.dat file");
  }

  file.close();

  LittleFS.end();

  return true;
}

void ServoMotor::calibrationClear()
{
  if (encoderCorrectionBuffer == NULL)
  {
    DL("WRN: ServoMotor::calibrationClear(), buffer not allocated!");
    return;
  }

  for (int i = 0; i < ENCODER_ECM_BUFFER_SIZE; i++)
    encoderCorrectionBuffer[i] = ECB_NO_DATA;
  V(axisPrefix);
  VLF("buffer cleared");
}

void ServoMotor::calibrationErase()
{
  if (encoderCorrectionBuffer == NULL)
  {
    DL("WRN: ServoMotor::calibrationErase(), buffer not allocated!");
    return;
  }

  calibrationClear();

  if (!LittleFS.begin(true))
  {
    DL("WRN: ServoMotor::calibrationErase(), filesystem creation failed!");
    return;
  }
  LittleFS.remove("/encoder.dat");
  LittleFS.end();
}

bool ServoMotor::calibrationLinearRegression()
{
  if (encoderCorrectionBuffer == NULL)
  {
    DL("WRN: ServoMotor::calibrationLinearRegression(), buffer not allocated!");
    return false;
  }

  int32_t startIndex = startCount / ENCODER_ECM_BUFFER_RESOLUTION + ENCODER_ECM_BUFFER_SIZE / 2;
  if (startIndex < 0)
    startIndex = 0;
  if (startIndex > ENCODER_ECM_BUFFER_SIZE - 1)
    startIndex = ENCODER_ECM_BUFFER_SIZE - 1;

  int32_t endIndex = (endCount / ENCODER_ECM_BUFFER_RESOLUTION + ENCODER_ECM_BUFFER_SIZE / 2) - 1;
  if (endIndex < 0)
    endIndex = 0;
  if (endIndex > ENCODER_ECM_BUFFER_SIZE - 1)
    endIndex = ENCODER_ECM_BUFFER_SIZE - 1;
  if (endIndex < startIndex)
    return false;

  V(axisPrefix);
  VLF("buffer applying linear regression:");
  int16_t startValue = ecbn(encoderCorrectionBuffer[startIndex]);
  D("Start Value =");
  D(startValue);
  D(", Index = ");
  DL(startIndex);
  int16_t endValue = ecbn(encoderCorrectionBuffer[endIndex]);
  D("End Value =");
  D(endValue);
  D(", Index = ");
  DL(endIndex);
  float valueRange = endValue - startValue;

  D("Range Value =");
  DL(valueRange);
  for (int i = startIndex; i <= endIndex; i++)
  {
    float f = (float)(i - startIndex) / (float)(endIndex - startIndex);
    V("index = ");
    V(i);
    V(", f = ");
    V(f);
    V(", correction = ");
    V(encoderCorrectionBuffer[i]);
    V(" -> ");
    if (encoderCorrectionBuffer[i] != ECB_NO_DATA)
    {
      encoderCorrectionBuffer[i] = encoderCorrectionBuffer[i] - round(startValue + valueRange * f);
    }
    VL(encoderCorrectionBuffer[i]);
  }

  return true;
}

bool ServoMotor::calibrationLowPass()
{
  if (encoderCorrectionBuffer == NULL)
  {
    DL("WRN: ServoMotor::calibrationLowPass(), buffer not allocated!");
    return false;
  }

  V(axisPrefix);
  VLF("buffer applying low pass filter:");
  int j, J1, J4, J9, J17;
  for (int i = 3; i < ENCODER_ECM_BUFFER_SIZE + 3; i++)
  {
    j = ecbn(encoderCorrectionBuffer[i % ENCODER_ECM_BUFFER_SIZE]);

    J1 = lroundf(j * 0.01F);
    J4 = lroundf(j * 0.04F);
    J9 = lroundf(j * 0.09F);
    J17 = lroundf(j * 0.17F);
    encoderCorrectionBuffer[(i - 4) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i - 4) % ENCODER_ECM_BUFFER_SIZE])) + J1;
    encoderCorrectionBuffer[(i - 3) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i - 3) % ENCODER_ECM_BUFFER_SIZE])) + J4;
    encoderCorrectionBuffer[(i - 2) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i - 2) % ENCODER_ECM_BUFFER_SIZE])) + J9;
    encoderCorrectionBuffer[(i - 1) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i - 1) % ENCODER_ECM_BUFFER_SIZE])) + J17;
    encoderCorrectionBuffer[(i) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i) % ENCODER_ECM_BUFFER_SIZE])) - (J17 + J17 + J9 + J9 + J4 + J4 + J1 + J1);
    encoderCorrectionBuffer[(i + 1) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i + 1) % ENCODER_ECM_BUFFER_SIZE])) + J17;
    encoderCorrectionBuffer[(i + 2) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i + 2) % ENCODER_ECM_BUFFER_SIZE])) + J9;
    encoderCorrectionBuffer[(i + 3) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i + 3) % ENCODER_ECM_BUFFER_SIZE])) + J4;
    encoderCorrectionBuffer[(i + 4) % ENCODER_ECM_BUFFER_SIZE] = (ecbn(encoderCorrectionBuffer[(i + 4) % ENCODER_ECM_BUFFER_SIZE])) + J1;
  }

  return true;
}

void ServoMotor::calibrationPrint()
{
  if (encoderCorrectionBuffer == NULL)
  {
    DL("WRN: ServoMotor::calibrationPrint(), buffer not allocated!");
    return;
  }

  V(axisPrefix);
  VLF("buffer contents (index, correction):");
  for (int i = 0; i < ENCODER_ECM_BUFFER_SIZE; i++)
  {
    V(i);
    V(", ");
    VL(ecbn(encoderCorrectionBuffer[i]));
  }
}

#endif
#endif
