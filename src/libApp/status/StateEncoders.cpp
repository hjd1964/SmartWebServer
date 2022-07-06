// update encoder state
#include "State.h"

#include "Status.h"
#include "../../lib/tasks/OnTask.h"
#include "../../libApp/cmd/Cmd.h"
#include "../../locales/Locale.h"
#include "../../lib/convert/Convert.h"
#include "../../libApp/encoders/Encoders.h"

void State::updateEncoders()
{
  char temp[80];

  #if ENCODERS == ON
    // RA,Dec OnStep and Encoder positions
    double f;
    f = encoders.getOnStepAxis1();
    convert.doubleToDms(temp, f, true, true, PM_HIGH);
    strncpyex(angleAxis1Str, temp, 14);
    formatDegreesStr(angleAxis1Str);
    if (encoders.validAxis1())
    {
      f = encoders.getAxis1();
      convert.doubleToDms(temp, f, true, true, PM_HIGH);
    }
    else
      strcpy(temp, " ** " L_FAULT " ** ");
    strncpyex(encAngleAxis1Str, temp, 14);
    formatDegreesStr(encAngleAxis1Str);
    Y;

    f = encoders.getOnStepAxis2();
    convert.doubleToDms(temp, f, true, true, PM_HIGH);
    strncpyex(angleAxis2Str, temp, 14);
    formatDegreesStr(angleAxis2Str);
    if (encoders.validAxis2())
    {
      f = encoders.getAxis2();
      convert.doubleToDms(temp, f, true, true, PM_HIGH);
    }
    else
      strcpy(temp, " ** " L_FAULT " ** ");
    strncpyex(encAngleAxis2Str, temp, 14);
    formatDegreesStr(encAngleAxis2Str);
    Y;
  #endif
}
