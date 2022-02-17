// scan OnStep state in the background

#include "State.h"
#include "..\lib\tasks\OnTask.h"
#include "..\libApp\cmd\Cmd.h"

void pollState() { state.poll(); }

void State::init() {
  doc = new DynamicJsonDocument(2000);

  // start polling task
  VF("MSG: State, start polling task (priority 7)... ");
  if (tasks.add(STATE_POLLING_RATE_MS, 0, true, 7, pollState, "StaPoll")) { VLF("success"); } else { VLF("FAILED!"); }
}

void State::poll() {
  char temp[120];

  // UTC Date
  if (!onStep.command(":GX81#", temp)) strcpy(temp, "?");
  if (temp[0] == '0') strcpy(&temp[0], &temp[1]);
  (*doc)["date_ut"] = temp;
  Y;

  // UTC Time
  if (!onStep.command(":GX80#", temp)) strcpy(temp, "?");
  (*doc)["time_ut"] = temp;
  Y;

  // LST
  if (!onStep.command(":GS#", temp)) strcpy(temp, "?");
  (*doc)["time_lst"] = temp;
  Y;
}

State state;
