#pragma once

enum RateCompensation {RC_NONE, RC_REFR_RA, RC_REFR_BOTH, RC_FULL_RA, RC_FULL_BOTH};

enum MountTypes {MT_UNKNOWN, MT_GEM, MT_FORK, MT_FORKALT, MT_ALTAZM};

enum Errors {
  ERR_NONE, ERR_MOTOR_FAULT, ERR_ALT_MIN, ERR_LIMIT_SENSE, ERR_DEC, ERR_AZM, 
  ERR_UNDER_POLE, ERR_MERIDIAN, ERR_SYNC, ERR_PARK, ERR_GOTO_SYNC, ERR_UNSPECIFIED,
  ERR_ALT_MAX, ERR_WEATHER_INIT, ERR_SITE_INIT, ERR_NV_INIT};

enum CommandErrors {
  CE_NONE, CE_0, CE_CMD_UNKNOWN, CE_REPLY_UNKNOWN, CE_PARAM_RANGE, CE_PARAM_FORM,
  CE_ALIGN_FAIL, CE_ALIGN_NOT_ACTIVE, CE_NOT_PARKED_OR_AT_HOME, CE_PARKED,
  CE_PARK_FAILED, CE_NOT_PARKED, CE_NO_PARK_POSITION_SET, CE_GOTO_FAIL, CE_LIBRARY_FULL,
  CE_GOTO_ERR_BELOW_HORIZON, CE_GOTO_ERR_ABOVE_OVERHEAD, CE_SLEW_ERR_IN_STANDBY, 
  CE_SLEW_ERR_IN_PARK, CE_GOTO_ERR_GOTO, CE_GOTO_ERR_OUTSIDE_LIMITS, CE_SLEW_ERR_HARDWARE_FAULT,
  CE_MOUNT_IN_MOTION, CE_GOTO_ERR_UNSPECIFIED, CE_NULL};

#define PierSideNone     0
#define PierSideEast     1
#define PierSideWest     2
#define PierSideBest     3
#define PierSideFlipWE1  10
#define PierSideFlipWE2  11
#define PierSideFlipWE3  12
#define PierSideFlipEW1  20
#define PierSideFlipEW2  21
#define PierSideFlipEW3  22

class MountStatus {
  public:
    bool update(bool all = false);
    bool getId(char id[]);
    bool getVer(char ver[]);
    bool valid();
    bool aligning();
    bool tracking();
    bool slewing();
    bool parked();
    bool parking();
    bool parkFail();
    bool pecEnabled();
    bool pecIgnore();
    bool pecReadyPlay();
    bool pecPlaying();
    bool pecReadyRec();
    bool pecRecorded();
    bool pecRecording();
    bool syncToEncodersOnly();
    bool atHome();
    bool ppsSync();
    bool guiding();

    bool focuserPresent();

    bool axisFault();
    bool axisStatusValid();
    bool axis1Comms();
    bool axis1StSt();
    bool axis1OLa();
    bool axis1OLb();
    bool axis1S2Ga();
    bool axis1S2Gb();
    bool axis1OT();
    bool axis1OTPW();
    bool axis2Comms();
    bool axis2StSt();
    bool axis2OLa();
    bool axis2OLb();
    bool axis2S2Ga();
    bool axis2S2Gb();
    bool axis2OT();
    bool axis2OTPW();

    bool waitingHome();
    bool pauseAtHome();
    bool buzzerEnabled();
    MountTypes mountType();
    RateCompensation rateCompensation();
    bool meridianFlips();
    bool autoMeridianFlips();
    byte pierSide();
    int alignMaxStars();

    bool featureFound();
    void selectFeature(int f);
    char* featureName();
    int featurePurpose();
    int featureValue1();
    float featureValue2();
    float featureValue3();
    float featureValue4();
    bool featureScan();
    bool featureUpdate(bool all = false);
    
    Errors lastError();
    
    bool getLastErrorMessage(char message[]);

  private:
    // hold state of aux features
    bool _featureFound = false;
    int _featureSelected=0;
    typedef struct Features {
       char name[11];
       int purpose;
       int value1;
       float value2;
       float value3;
       float value4;
    } features;
    features _feature[8];

    char _id[10]="";
    char _ver[10]="";
    bool _valid=false;
    bool _tracking=false;
    bool _slewing=false;
    bool _parked=false;
    bool _parking=false;
    bool _parkFail=false;
    bool _pecEnabled=false;
    bool _pecIgnore=false;
    bool _pecReadyPlay=false;
    bool _pecPlaying=false;
    bool _pecReadyRec=false;
    bool _pecRecorded=false;
    bool _pecRecording=false;
    bool _toEncOnly=false;
    bool _atHome=false;
    bool _ppsSync=false;
    bool _guiding=false;
    bool _axisFault=false;
    bool _waitingHome=false;
    bool _pauseAtHome=false;
    bool _buzzerEnabled=false;
    MountTypes _mountType=MT_UNKNOWN;
    RateCompensation _rateCompensation=RC_NONE;
    bool _meridianFlips=true;
    bool _autoMeridianFlips=false;
    byte _pierSide=PierSideNone;
    int _alignMaxStars = -1;

    Errors _lastError=ERR_NONE;
    bool _validStepperDriverStatus = false;
    bool _comms1 = false;
    bool _stst1 = false;
    bool _olb1 = false;
    bool _ola1 = false;
    bool _s2ga1 = false;
    bool _s2gb1 = false;
    bool _ot1 = false;
    bool _otpw1 = false;
    bool _valid2 = false;
    bool _comms2 = false;
    bool _stst2 = false;
    bool _olb2 = false;
    bool _ola2 = false;
    bool _s2ga2 = false;
    bool _s2gb2 = false;
    bool _ot2 = false;
    bool _otpw2 = false;

};

extern MountStatus mountStatus;

char* commandErrorToStr(int e);
