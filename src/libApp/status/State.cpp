// scan OnStep state in the background
#include "State.h"

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
  if (status.mountFound == SD_TRUE)
  {
    updateMount();
    updateEncoders();
  }
  if (status.focuserFound == SD_TRUE) updateFocuser();
  if (status.auxiliaryFound == SD_TRUE) updateAuxiliary();
}

void State::pollSlow()
{
  updateController();

  status.update();
}

void State::pollFast()
{
  if (status.focuserFound == SD_TRUE && millis() - lastFocuserPageLoadTime < 2000) {
    char temp[80];
    if (!onStep.command(":FG#", temp)) strcpy(temp, "?"); else strcat(temp, " microns"); Y;
    strncpy(focuserPositionStr, temp, 20); focuserPositionStr[19] = 0; Y;
  }

  if (status.rotatorFound == SD_TRUE) updateRotator();
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

void formatDegreesStr(char *s)
{
  char *tail;

  // degrees part
  tail = strchr(s, '*');
  if (tail)
  {
    char head[80];
    tail[0] = 0;
    strcpy(head, s);
    strcat(head, "&deg;");

    tail++;
    strcat(head, tail);
    strcpy(s, head);
  } else return;

  // minutes part
  tail = strchr(s, ':');
  if (tail) // indicates there is a seconds part
  {
    tail[0] = '\'';
    strcat(s, "\"");
  }
  else // there is no seconds part
  {
    strcat(s, "\'");
  }
}

void formatHoursStr(char *s)
{
  char *tail;

  // hours part
  tail = strchr(s, ':');
  if (tail)
  {
    tail[0] = 'h';
  } else return;

  // minutes part
  tail = strchr(s, ':');
  if (tail) // indicates there is a seconds part
  {
    tail[0] = 'm';
    strcat(s, "s");
  }
  else // there is no seconds part
  {
    strcat(s, "m");
  }
}

State state;
