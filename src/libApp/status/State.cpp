// scan OnStep state in the background
#include "State.h"

#include "Status.h"
#include "../../lib/tasks/OnTask.h"
#include "../../libApp/cmd/Cmd.h"

void pollState() { state.poll(); }
void pollStateSlow() { state.pollSlow(); }
void pollStateFast() { state.pollFast(); }
void pollStateGpio() { state.pollGpio(); }

void State::init()
{
  VF("MSG: State, start polling task (rate "); V(STATE_POLLING_RATE_MS); VF("ms, priority 6)... ");
  if (tasks.add(STATE_POLLING_RATE_MS, 0, true, 6, pollState, "StaPoll")) { VLF("success"); } else { VLF("FAILED!"); }

  VF("MSG: State, start slow polling task (rate "); V(STATE_SLOW_POLLING_RATE_MS); VF("ms, priority 7)... ");
  if (tasks.add(STATE_SLOW_POLLING_RATE_MS, 0, true, 7, pollStateSlow, "StSPoll")) { VLF("success"); } else { VLF("FAILED!"); }

  VF("MSG: State, start fast polling task (rate "); V(STATE_FAST_POLLING_RATE_MS); VF("ms, priority 5)... ");
  if (tasks.add(STATE_FAST_POLLING_RATE_MS, 0, true, 5, pollStateFast, "StFPoll")) { VLF("success"); } else { VLF("FAILED!"); }

  status.update();
  if (status.getVersionMajor() >= 10) {
    char result[40];
    if (onStep.command(":GXGA#", result) && strlen(result) == 1 && result[0] == '8') {
      VF("MSG: State, start gpio polling task (rate "); V(STATE_GPIO_POLLING_RATE_MS); VF("ms, priority 1)... ");
      if (tasks.add(STATE_GPIO_POLLING_RATE_MS, 0, true, 1, pollStateGpio, "GioPoll")) { VLF("success"); } else { VLF("FAILED!"); }
    }
  }
}

void State::poll()
{
  unsigned long t0,t1,t2,t3,t = millis();

  if (status.mountFound)
  {
    updateMount();
 t0 = millis();

    updateEncoders();
 t1 = millis();
  }

  if (status.focuserFound) updateFocuser();
 t2 = millis();

  if (status.auxiliaryFound) updateAuxiliary();
 t3 = millis();

//  V("t0="); VL(t0 - t);
//  V("t1="); VL(t1 - t);
//  V("t2="); VL(t2 - t);
//  V("t3="); VL(t3 - t);
//  VL("");
}

void State::pollSlow()
{
  unsigned long t0,t = millis();

  updateController();
  status.update(true);

//  V("t0="); VL(t0 - t);
//  VL("");
}

void State::pollFast()
{
  unsigned long t0,t1,t = millis();
  char temp[80];

  if (status.focuserFound) {
    if (!onStep.command(":FG#", temp)) strcpy(temp, "?"); else strcat(temp, " microns"); Y;
    strncpy(focuserPositionStr, temp, 20); focuserPositionStr[19] = 0; Y;
  }
  t0=millis();

  if (status.rotatorFound) updateRotator();
  t1=millis();

//  V("t0="); VL(t0 - t);
//  V("t1="); VL(t1 - t);
//  VL("");
}

void State::pollGpio()
{
  const int vGpioPin[8] = {VGPIO_PIN_0, VGPIO_PIN_1, VGPIO_PIN_2, VGPIO_PIN_3, VGPIO_PIN_4, VGPIO_PIN_5, VGPIO_PIN_6, VGPIO_PIN_7};
  char cmd[40], result[40];

  if (!onStep.command(":GXGO#", result) || strlen(result) != 8) return;
  for (int i = 0; i < 8; i++) {
    if (result[i] == '1') {
      if (vGpioPin[i] != OFF) digitalWrite(vGpioPin[i], HIGH);
    } else
    if (result[i] == '0') {
      if (vGpioPin[i] != OFF) digitalWrite(vGpioPin[i], LOW);
    } else
    if (result[i] == 'I') {
      if (vGpioPin[i] != OFF) pinMode(vGpioPin[i], INPUT);
      vGpioMode[i] = 'I';
    } else
    if (result[i] == 'U') {
      if (vGpioPin[i] != OFF) pinMode(vGpioPin[i], INPUT_PULLUP);
      vGpioMode[i] = 'I';
    } else
    if (result[i] == 'O') {
      if (vGpioPin[i] != OFF) pinMode(vGpioPin[i], OUTPUT);
      vGpioMode[i] = 'O';
    }
  }

  for (int i = 0; i < 8; i++) {
    if (vGpioMode[i] == 'I' && vGpioPin[i] != OFF) {
      int vGpioState = digitalRead(vGpioPin[i]);
      if (vGpioState != vGpioLastState[i]) {
        sprintf(cmd, ":SXG%d,%d#", i, vGpioState);
        if (onStep.commandBool(cmd)) vGpioLastState[0] = vGpioState;
      }
    }
  }
}

State state;
