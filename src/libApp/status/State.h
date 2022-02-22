#pragma once

#include "../../Common.h"

// state polling rates in milli-seconds
#define STATE_POLLING_RATE_MS 2000
#define STATE_SLOW_POLLING_RATE_MS 4000
#define STATE_FAST_POLLING_RATE_MS 1000

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

    char dateStr[10] = "?";
    char timeStr[10] = "?";
    char lastStr[10] = "?";
    double latitude = NAN;
    char latitudeStr[10] = "?";
    char longitudeStr[11] = "?";
    char siteTemperatureStr[16] = "?";
    char sitePressureStr[16] = "?";
    char siteHumidityStr[16] = "?";
    char siteDewPointStr[16] = "?";

    char telescopeTemperatureStr[16] = "?";

    char indexRaStr[14] = "?";
    char indexDecStr[14] = "?";
    char targetRaStr[14] = "?";
    char targetDecStr[14] = "?";

    char angleAxis1Str[14] = "?";
    char angleAxis2Str[14] = "?";
    char encAngleAxis1Str[14] = "?";
    char encAngleAxis2Str[14] = "?";

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

    char slewSpeedStr[16] = "?";

    char driverStatusStr[9][40] = {"?","?","?","?","?","?","?","?","?"};

    char controllerTemperatureStr[16] = "?";;
    char lastErrorStr[40] = "?";;
    char workLoadStr[20] = "?";;
    char signalStrengthStr[20] = "?";

    char focuserPositionStr[20] = "?";
    int focuserActive = 0;

    char rotatorPositionStr[20] = "?";

   // DynamicJsonDocument *doc;

  private:
    bool rotatorChecked = false;

};

extern State state;
