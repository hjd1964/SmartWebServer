// update focuser state
#include "State.h"

#include "Status.h"
#include "../../lib/tasks/OnTask.h"
#include "../../libApp/cmd/Cmd.h"
#include "../../libApp/misc/Misc.h"
#include "../../locales/Locale.h"
#include "../../lib/convert/Convert.h"

void State::updateFocuser(bool now)
{
  if (!now && millis() - lastFocuserPageLoadTime > 2000) return;

  char temp[80];

  // identify active focuser
  if (status.getVersionMajor() >= 10) {
    if (onStep.command(":FA#", temp)) focuserSelected = atoi(temp); else focuserSelected = 0;
  } else {
    if (onStep.command(":Fa#", temp)) {
      focuserSelected = atoi(temp);
      switch (focuserSelected) {
        case 0: focuserSelected = 2; break;
        case 1: focuserSelected = 1; break;
        default: focuserSelected = 0; break;
      }
    } else focuserSelected = 0;
  }
  Y;

  if (focuserSelected >= 1 && focuserSelected <= 6) {

    // focuser/telescope temperature
    if (!onStep.command(":Ft#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpyex(focuserTemperatureStr, temp, 16); Y;

    // focuser backlash
    if (!onStep.command(":Fb#", temp)) strcpy(temp, "?");
    strncpyex(focuserBacklashStr, temp, 16); Y;

    // focuser deadband
    if (!onStep.command(":Fd#", temp)) strcpy(temp, "?");
    strncpyex(focuserDeadbandStr, temp, 16); Y;

    // focuser TCF enable
    focuserTcfEnable = onStep.commandBool(":Fc#"); Y;

    // focuser TCF
    if (onStep.command(":FC#", temp))
    {
      char *conv_end;
      double tcfCoef = strtod(temp, &conv_end);
      if (&temp[0] != conv_end) {
        dtostrf(tcfCoef, 1, 4, temp);
      } else strcpy(temp, "?");
    } else strcpy(temp, "?");
    strncpyex(focuserTcfCoefStr, temp, 16); Y;

    // focuser slew rate
    if (status.getVersionMajor() >= 10)
    {
      if (onStep.command(":FW#", temp))
      {
        int s = atoi(temp);
        if (s != 0) {
          sprintF(focuserSlewSpeedStr, "%0.2fmm/s", s/1000.0F);
        } else strcpy(focuserSlewSpeedStr, "?");
      } else strcpy(focuserSlewSpeedStr, "?");
    } else strcpy(focuserSlewSpeedStr, "?");

    // focuser status
    if (onStep.command(":FT#", temp))
    {
      focuserSlewing = (bool)strchr(temp, 'M');
      switch (temp[strlen(temp) - 1] - '0') {
        case 1: focuserGotoRate = 1; break;
        case 2: focuserGotoRate = 2; break;
        case 3: focuserGotoRate = 3; break;
        case 4: focuserGotoRate = 4; break;
        case 5: focuserGotoRate = 5; break;
        default: focuserGotoRate = 0; break;
      }
    } else {
      focuserSlewing = false;
      focuserGotoRate = 3;
    }
    Y;
  }
}