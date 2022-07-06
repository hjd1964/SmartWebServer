#pragma once

#include "../../Common.h"

#define leftTri  "&#x25c4;"
#define rightTri "&#x25ba;"
#define upTri    "&#x25b2;"
#define downTri  "&#x25bc;"

class State {
  public:
    void init();
    void poll();
    void pollSlow();
    void pollFast();
    void pollGpio();

    void updateController();
    void updateMount();
    void updateRotator();
    void updateFocuser();
    void updateAuxiliary();
    void updateEncoders();

    char dateStr[10] = "?";
    char timeStr[10] = "?";
    char lastStr[10] = "?";
    double latitude = NAN;
    char latitudeStr[20] = "?";
    char longitudeStr[20] = "?";
    char siteTemperatureStr[16] = "?";
    char sitePressureStr[16] = "?";
    char siteHumidityStr[16] = "?";
    char siteDewPointStr[16] = "?";

    char indexAzmStr[20] = "?";
    char indexAltStr[20] = "?";
    char indexRaStr[20] = "?";
    char indexDecStr[20] = "?";
    char targetRaStr[20] = "?";
    char targetDecStr[20] = "?";

    char angleAxis1Str[20] = "?";
    char angleAxis2Str[20] = "?";
    char encAngleAxis1Str[20] = "?";
    char encAngleAxis2Str[20] = "?";

    char pierSideStr[10] = "?";
    char meridianFlipStr[10] = "?";

    char alignUdStr[16] = "?";
    char alignLrStr[16] = "?";
    char alignProgress[32] = "?";

    char parkStr[40] = "?";
    char trackStr[40] = "?";
    bool trackingSidereal = false;
    bool trackingLunar = false;
    bool trackingSolar = false;
    bool trackingKing = false;

    float slewSpeedNominal = NAN;
    float slewSpeedCurrent = NAN;
    char slewSpeedStr[16] = "?";

    char driverStatusStr[9][40] = {"?","?","?","?","?","?","?","?","?"};

    char controllerTemperatureStr[16] = "?";
    char lastErrorStr[80] = "?";
    char workLoadStr[20] = "?";
    char signalStrengthStr[20] = "?";

    int focuserActive = 0;
    int focuserSelected = 0;
    char focuserPositionStr[20] = "?";
    bool focuserSlewing = false;
    char focuserTemperatureStr[16] = "?";
    char focuserBacklashStr[16] = "?";
    char focuserDeadbandStr[16] = "?";
    bool focuserTcfEnable = false;
    char focuserTcfCoefStr[16] = "?";

    char rotatorPositionStr[20] = "?";
    bool rotatorSlewing = false;
    bool rotatorDerotate = false;
    bool rotatorDerotateReverse = false;

   // DynamicJsonDocument *doc;

  private:
    char vGpioMode[8] = {
      'X', 'X', 'X', 'X',
      'X', 'X', 'X', 'X'
    };

    int vGpioLastState[8] = {
      -1, -1, -1, -1,
      -1, -1, -1, -1
    };

    bool rotatorChecked = false;

};

void formatDegreesStr(char *s);
void formatHoursStr(char *s);

extern State state;
