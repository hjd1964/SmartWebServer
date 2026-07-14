// -----------------------------------------------------------------------------------
// Locale Romanian, ISO ISO639-1 language code "ro"
#pragma once

// General
#define L_UPLOAD "Încarcă"
#define L_DOWNLOAD "Descarcă"
#define L_ACCEPT "Acceptă"
#define L_CONTINUE "Continuă"
#define L_ARE_YOU_SURE "Sigur?"
#define L_LOGOUT "Deconectare"

#define L_UT "UT"
#define L_LST "LST"
#define L_LONG "Longitudine"
#define L_LAT "Latitudine"
#define L_EAST "Est"
#define L_WEST "Vest"
#define L_ZENITH "Zenit"
#define L_NCP "NCP"
#define L_SCP "SCP"

#define L_NONE "Niciuna"
#define L_AUTO "Auto"
#define L_SET_LABEL "Goto"
#define L_OK "Ok"
#define L_ON "Pornit"
#define L_OFF "Oprit"
#define L_VISIT "Vizită"
#define L_PAUSE "Pauză"
#define L_START "Start"
#define L_DELTA "Delta"
#define L_DP_MSG "Relativ"
#define L_DP_ZERO "Putere maximă sub"
#define L_DP_SPAN "Încălzire oprită peste"
#define L_POWER "Put"
#define L_GUIDE "Ghidare"
#define L_CENTER "Centrare"
#define L_BLUE "Albastru"
#define L_GREEN "Verde"
#define L_RATE "rată"
#define L_RANGE "interval"
#define L_UNKNOWN "Necunoscut"
#define L_FAULT "Defect"
#define L_POINT "Punct"
#define L_COMPLETE "Complet"
#define L_ACTIVE "Activ"
#define L_INACTIVE "Inactiv"
#define L_BEST "Optim"
#define L_ALTERNATE "Alternativ"

// ---------------------------- Index ----------------------------
// date/time
#define L_TIME_BROWSER_UT "UT (browser web)"

// weather
#define L_TEMPERATURE "Temperatură"
#define L_AMBIENT_TEMPERATURE "Temperatură Ambient"
#define L_PRESSURE "Presiune Barometrică"
#define L_HUMIDITY "Umiditate Relativă"
#define L_DEW_POINT "Punct de Rouă"

// operation
#define L_TRACKING "Urmărire"
#define L_SITE "Locație"
#define L_COORDINATES "Coordonate"
#define L_CURRENT "Poziție"
#define L_TARGET "Țintă&nbsp;&nbsp;"
#define L_ENCODER "Encodr"
#define L_POLAR_ALIGN "Aliniere Polară"

// pier side
#define L_PIER_SIDE "Latură Pivot"
#define L_MERIDIAN_FLIPS "fliperi meridian"
#define L_ALIGN_MESSAGE "Montura relativ la"
#define L_MERIDIAN_FLIP_W_TO_E "Flip Meridian, Vest la Est"
#define L_MERIDIAN_FLIP_E_TO_W "Flip Meridian, Est la Vest"

#define L_OPERATIONS "Operații"

// parking
#define L_PARKING "Parcare"
#define L_PARKED "Parcat"
#define L_NOT_PARKED "Neparcat"
#define L_PARKING "Parcare"
#define L_PARK_FAILED "Parcare Eșuată"

// home
#define L_HOMING "Mergere Acasă"
#define L_AT_HOME "Acasă"
#define L_SLEWING "În mișcare"
#define L_PPS_SYNC "Sincronizare PPS"
#define L_HOME_REV "Inversare"
#define L_HOME_OFFSET "Decalaj față de poziția senzor home:"
#define L_HOME_OFFSET_RANGE_AXIS1 "Axa1, +/-" STR(HOME_OFFSET_RANGE_AXIS1) " arc-sec"
#define L_HOME_OFFSET_RANGE_AXIS2 "Axa2, +/-" STR(HOME_OFFSET_RANGE_AXIS2) " arc-sec"
#define L_HOME_AUTO "Mergere automată acasă la pornire:"

// tracking
#define L_REFR_COMP_RA "RC"
#define L_REFR_COMP_BOTH "RCD"
#define L_FULL_COMP_RA "FC"
#define L_FULL_COMP_BOTH "FCD"
#define L_TRACKING "Urmărire"
#define L_CURRENT_MAXRATE "Rată Max Curentă"
#define L_DEFAULT_MAXRATE "Rată Max Implicită"
#define L_CURRENT_MAXSLEW "Viteză maximă de mișcare"

#define L_FREQUENCY_ADJUST "Ajustare Frecvență"

// driver status
#define L_DRIVER "Driver"
#define L_STATUS "Stare"
#define L_STATE "Stare"
#define L_COMMS_FAILURE "Eroare comunicații"
#define L_STANDSTILL "Oprit"
#define L_OPEN_LOAD "Sarcină deschisă"
#define L_SHORT_GND "Scurt la masă"
#define L_SHUTDOWN_OVER "Oprire supraîncărcare"
#define L_PRE_WARNING "Pre-avertizare"
#define L_DRIVER_FAULT "Defect"

// misc. errors/workload
#define L_HARDWARE "Hardware"
#define L_INTERNAL_TEMP "Temperatură Controller"
#define L_LAST_GENERAL_ERROR "Ultima Eroare"
#define L_WORKLOAD "Încărcare"
#define L_WIRELESS_SIGNAL_STRENGTH "Putere semnal wireless"

// --------------------------- Control ---------------------------

#if DISPLAY_SPECIAL_CHARS == ON
  #define HOME_CH "&#x1F3E0;"
  #define ARROW_DR "&#x27A5;"
  #define ARROW_UR "&#x27A6;"
  #define ARROW_R2 "&#x27A4;"
  #define CAUTION_CH "&#9888;"
  #define CLOCK_CH "&#x1F565;"
  #define ARROW_LL "&lt;&lt;"
  #define ARROW_L "&lt;"
  #define ARROW_R "&gt;"
  #define ARROW_RR "&gt;&gt;"
  #define ARROW_DD "&lt;&lt;"
  #define ARROW_D "&lt;"
  #define ARROW_U "&gt;"
  #define ARROW_UU "&gt;&gt;"
  #define SIDEREAL_CH "&#9733;"
  #define LUNAR_CH "&#9790;"
  #define SOLAR_CH "&#9737;"
  #define KING_CH "&#9812;"
#else
  #define HOME_CH "H"
  #define ARROW_DR "-&gt;"
  #define ARROW_UR "-&gt;"
  #define ARROW_R2 "&gt;"
  #define CAUTION_CH "!"
  #define CLOCK_CH "T"
  #define ARROW_LL "&lt;&lt;"
  #define ARROW_L "&lt;"
  #define ARROW_R "&gt;"
  #define ARROW_RR "&gt;&gt;"
  #define ARROW_DD "&lt;&lt;"
  #define ARROW_D "&lt;"
  #define ARROW_U "&gt;"
  #define ARROW_UU "&gt;&gt;"
  #define SIDEREAL_CH "*"
  #define LUNAR_CH "("
  #define SOLAR_CH "O"
  #define KING_CH "K"
#endif

#define BUTTON_N "N"
#define BUTTON_S "S"
#define BUTTON_E "E"
#define BUTTON_W "V"
#define BUTTON_SYNC "@"

#define L_HINT_CLOCK "Setează Dată/Oră"
#define L_HINT_CONTINUE_GOTO "Continuă Goto (când e în pauză acasă)"
#define L_HOME_PARK_TITLE "Mergere Acasă și Parcare"
#define L_RETURN_HOME "Mergi Acasă"
#define L_RESET_HOME "Acasă (Reset)"
#define L_UNPARK "Deparcare"
#define L_PARK "Parcare"
#define L_HINT_EMERGENCY_STOP "Oprire de Urgență!"
#define L_HINT_RATE_SIDEREAL "Rată Siderală"
#define L_HINT_RATE_LUNAR "Rată Lunară"
#define L_HINT_RATE_SOLAR "Rată Solară"
#define L_HINT_RATE_KING "Rată King"
#define L_HINT_FOC_SEL "Selectează Focuser"
#define L_HINT_DEROTATE_ON "Derotire Pornită"
#define L_HINT_ROTATOR_REV "Inversare Mișcare Derotor"
#define L_HINT_ROTATOR_PAR "Mută Rotorul la unghi Paralactic"
#define L_HINT_DEROTATE_OFF "Derotire Oprită"

#define L_ALIGN "Aliniere"
#define L_AUX_FEATURES "Funcții Auxiliare"

#define L_FOCUSER "Focuser"
#define L_ROTATOR "Rotor"
#define L_DEROTATOR "Derotor"
#define L_CAMERA_EXPOSURE "Expunere"
#define L_CAMERA_DELAY "Pauză"
#define L_CAMERA_COUNT "Număr"

// ---------------------------- Library --------------------------

#define L_CATALOG "Catalog"
#define L_CATALOGS "Cataloage"
#define L_RECS_AVAIL "înregistrări disponibile"
#define L_CAT_DATA "Date (descărcate SAU de încărcat.)"

#define L_CAT_EXAMPLE1 "Câmpuri:<br /><pre>"
#define L_CAT_EXAMPLE2 "  Un Nume Catalog este necesar în rândul 1, acesta<br />"
#define L_CAT_EXAMPLE3 "  constă într-un '$' urmat de până la 10 caractere.<br /><br />"
#define L_CAT_EXAMPLE4 "  Numele Obiect este o secvență de până la 11 caractere.<br /><br />"
#define L_CAT_EXAMPLE5 "  Cat este un cod de categorie după cum urmează:<br />"
#define L_CAT_EXAMPLE6 "  RA (Jnow) este în Ore, Minute și Secunde.<br />"
#define L_CAT_EXAMPLE7 "  Dec (Jnow) este în +/- Grade, Minute, Secunde.<br />"

#define L_CAT_NO_NAME "Catalogul selectat nu are nume."
#define L_CAT_GET_NAME_FAIL "Catalog selectat, citirea numelui a eșuat."
#define L_CAT_NO_CAT "Niciun catalog selectat."
#define L_CAT_NO_OBJECT "Niciun obiect selectat."
#define L_SELECTED "selectat"
#define L_CAT_DOWNLOAD_FAIL "Descărcare eșuată, date eronate."
#define L_CAT_DOWNLOAD_SUCCESS "Descărcare reușită."
#define L_CAT_DOWNLOAD_INDEX_FAIL "Descărcare eșuată, indexare catalog eșuată."
#define L_CAT_DATA_REMOVED "Date catalog șterse"
#define L_CAT_UPLOAD_FAIL "Încărcare eșuată, nume catalog eronat."
#define L_CAT_UPLOAD_INDEX_FAIL "Încărcare eșuată, indexare nume catalog."
#define L_CAT_DELETE_FAIL "Încărcare eșuată, ștergere nume catalog."
#define L_CAT_WRITE_NAME_FAIL "Încărcare eșuată, scriere nume catalog."
#define L_CAT_UPLOAD_NO_NAME_FAIL "Încărcare eșuată, linia 1 trebuie să conțină numele catalogului."
#define L_CAT_BAD_FORM "Încărcare eșuată, format eronat la linia# "
#define L_CAT_UPLOAD_BAD_OBJECT_NAME "Încărcare eșuată, nume obiect eronat la linia# "
#define L_CAT_BAD_CATEGORY "Încărcare eșuată, categorie eronată la linia# "
#define L_CAT_BAD_RA "Încărcare eșuată, format RA eronat la linia# "
#define L_CAT_BAD_DEC "Încărcare eșuată, format Dec eronat la linia# "
#define L_CAT_UPLOAD_RA_FAIL "Încărcare eșuată, trimitere RA la linia# "
#define L_CAT_UPLOAD_DEC_FAIL "Încărcare eșuată, trimitere Dec la linia# "
#define L_CAT_UPLOAD_LINE_FAIL "Încărcare eșuată, trimitere linia# "
#define L_CAT_UPLOAD_SUCCESS "Încărcare reușită"
#define L_CAT_UPLOAD_LINES_WRITTEN "linii scrise"
#define L_CAT_UPLOAD_SELECT_FAIL "Încărcare eșuată, nu pot selecta catalogul."
#define L_CAT_UPLOAD_NO_CAT "Încărcare eșuată, niciun catalog selectat."
#define L_CAT_CLEAR "Șterge Catalog"
#define L_CAT_CLEAR_LIB "Șterge Bibliotecă"

// --------------------------- Encoders --------------------------

// general settings
#define L_ENC_SYNC_TO_ENC "Sincronizare OnStep -> Encodere"
#define L_ENC_SYNC_TO_ONS "Sincronizare Encodere -> OnStep"
#define L_ENC_ORIGIN_TO_ONS "Setează Origine OnStep -> Encodere"
#define L_ENC_AUTO_SYNC "Sincronizare automată Encodere -> OnStep"
#define L_ENC_CONF "Configurare Encodere"
#define L_ENC_SET_TPD "Pași per grad"
#define L_ENC_SYNC_THLD "Prag Auto Sync"
#define L_ENC_SYNC_THLD_UNITS "0 la 9999 arc-sec"
#define L_ENC_AUTO_RATE "Control rată OnStep"
#define L_NOW "Acum"

// ------------------------------ PEC ----------------------------

#define L_PEC_STATUS "Stare"
#define L_PEC_CONTROL "Control PEC"
#define L_PEC_PLAY "Redare"
#define L_STOP "Stop"
#define L_PEC_CLEAR "Șterge"
#define L_PEC_REC "Înregistrare"
#define L_PEC_CLEAR_MESSAGE "Ștergerea elimină bufferul de memorie, nu și EEPROM. În timpul înregistrării corecțiile sunt mediate 3:1 favorizând bufferul, decât dacă este șters, caz în care se folosește corecția completă."
#define L_PEC_EEWRITE "Scrie în EEPROM"
#define L_PEC_EEWRITE_MESSAGE "Scrie datele PEC în EEPROM ca să fie reținute la repornirea OnStep. Scrierea poate dura câteva secunde."
#define L_DISABLED_MESSAGE "DEZACTIVAT"
#define L_PEC_IDLE "Inactiv"
#define L_PEC_WAIT_PLAY "Redare în așteptare"
#define L_PEC_PLAYING "Redare"
#define L_PEC_WAIT_REC "Înregistrare în așteptarea indexului"
#define L_PEC_RECORDING "Înregistrare"
#define L_PEC_UNK "Necunoscut"
#define L_PEC_EEWRITING "scriere în EEPROM"
#define L_PEC_STEPS_PER_WORM_ROTATION "Pași per rotație melc"

// --------------------------- Settings --------------------------

#define L_REFINE_POLAR_ALIGN "Rafinare Aliniere Polară"
#define L_REFINE_PA "Rafinare PA"
#define L_REFINE_MESSAGE1 "Setup &amp; aliniere 3+ stele. Goto stea aprinsă lângă NCP sau SCP cu Dec între 50 și 80&deg; (N sau S). "
#define L_REFINE_MESSAGE2 "Apasă butonul [Rafinare PA]. Folosește controalele PA ale monturii pentru a centra steaua din nou. "
#define L_REFINE_MESSAGE3 "Opțional, re-aliniază montura."

#define L_PARK "Parcare"
#define L_SET_PARK "Setează Parc."
#define L_SET_PARK_CURRENT_COORDS "Setează poziția de parcare la coordonatele curente"
#define L_COORD_ORIGIN_TITLE "Origine Coordonate"
#define L_COORD_ORIGIN "Setează Origine Coordonate"
#define L_COORD_ORIGIN_MSG "Cu montura acasă și urmărirea oprită, stabilește originea fizică/coordonată folosită pentru limite de sincronizare."

#define L_TRK_FASTER "mai rapid"
#define L_TRK_SLOWER "mai încet"
#define L_TRK_RESET "Reset (implicit)"
#define L_TRK_COMP "Rată Urmărire Compensată (Model/Refracție)"
#define L_TRK_FULL "Total"
#define L_TRK_REFR "Doar Refracție"
#define L_TRK_DUAL "Două Axe"
#define L_TRK_SINGLE "O Axă"

#define L_BUZZER "Alertă Goto, Sonerie"

#define L_MERIDIAN_FLIP_AUTO "Flip Meridian Automat la Limită"
#define L_MERIDIAN_FLIP_NOW "Acum"
#define L_MERIDIAN_FLIP_PAUSE "Flip Meridian, Pauză Acasă"
#define L_MERIDIAN_FLIP_HOMING "Flip Meridian, Acasă"
#define L_MERIDIAN_FLIP_PPS "Flip Meridian, Latură Pivot Preferată"

#define L_ORIENTATION_AUTO "Schimbare Orientare"
#define L_ORIENTATION_CHANGE_NOW L_MERIDIAN_FLIP_NOW
#define L_ORIENTATION_CHANGE_PAUSE "Flip, Pauză Acasă"
#define L_ORIENTATION_CHANGE_PPS "Flip, Orientare Preferată"

// ------------------------ Configuration ------------------------

#define L_BASIC_SET_TITLE "De bază:"

#define L_MAX_SLEW_SPEED "Viteză Maximă de Mișcare"

// goto Rate
#define L_VSLOW "FoarteLent"
#define L_SLOW "Lent"
#define L_NORMAL "Normal"
#define L_FAST "Rapid"
#define L_VFAST "FoarteRapid"

// backlash
#define L_BACKLASH "Backlash"
#define L_BACKLASH_RANGE_AXIS1 "Axa1, 0 la 3600 arc-sec"
#define L_BACKLASH_RANGE_AXIS2 "Axa2, 0 la 3600 arc-sec"
#define L_BACKLASH_RANGE_AXIS3UP "Backlash, 0 la 32767 pași"

// deadband/TCF
#define L_DEADBAND "Deadband TCF"
#define L_TCF_COEF "Coef TCF"
#define L_TCF_COEF_EN "Activare TCF"
#define L_DEADBAND_RANGE_AXIS4UP "Deadband TCF, 1 la 32767 pași"
#define L_TCF_COEF_RANGE_AXIS4UP "Coef TCF, &pm;999.0 microni/grad C"
#define L_TCF_COEF_EN_AXIS4UP "Activare TCF, 0 fals sau 1 adevărat"

// limits
#define L_LIMITS_TITLE "Limite Orizont și Zenit"
#define L_LIMITS_RANGE_HORIZON "Orizont, altitudine min &pm;30&deg;"
#define L_LIMITS_RANGE_OVERHEAD "Zenit, altitudine max 60 la 90&deg;"
#define L_LIMITS_RANGE_MERIDIAN_E "Depășire Meridian, Est pivot &pm;270&deg;"
#define L_LIMITS_RANGE_MERIDIAN_W "Depășire Meridian, Vest pivot &pm;270&deg;"

// location
#define L_LOCATION_TITLE "Oră și Locație"
#define L_LOCATION_LONG "Longitudine grad. min. sec. &pm;180, V este +"
#define L_LOCATION_LAT  "Latitudine grad. min. sec. &pm;90, N este +"
#define L_LOCATION_RANGE_UTC_OFFSET "Decalaj UTC, în ore și min. -14 la +12"
#define L_LOCATION_MESSAGE_UTC_OFFSET "Opusul valorii fusului orar, este pentru Ora Standard (nu Ora de Vară)."

// advanced configuration

// AxisToTile.cpp has a count and array matching these values
#define L_AXPN_STEPS_PER_DEG "Pași/grad"           // "$1" (first axis parameter)
#define L_AXPN_LIMIT_DEGS_MIN "Limită min, grade"  // "$2"
#define L_AXPN_LIMIT_DEGS_MAX "Limită max, grade"  // "$3"
#define L_AXPN_STEPS_PER_UM "Pași/um"              // "$4"
#define L_AXPN_LIMIT_UM_MIN "Limită min, um"       // "$5"
#define L_AXPN_LIMIT_UM_MAX "Limită max, um"       // "$6"
#define L_AXPN_REVERSE "Inversare"                 // "$7" (first motor parameter)
#define L_AXPN_MICROSTEPS "Microstepi"             // "$8"
#define L_AXPN_MICROSTEPS_GOTO "Microstepi Goto"   // "$9"
#define L_AXPN_DECAY_MODE "Mod Decay"              // "$10"
#define L_AXPN_DECAY_MODE_GOTO "Mod Decay Goto"    // "$11"
#define L_AXPN_CURRENT_HOLD "mA Menținere"         // "$12"
#define L_AXPN_CURRENT_RUN "mA Mișcare"            // "$13"
#define L_AXPN_CURRENT_GOTO "mA Goto"              // "$14"
#define L_AXPN_INTERPOLATE "Interpolare 256x"      // "$15"
#define L_AXPN_PID_P "<big><code>P</code></big> Urmărire" // "$16"
#define L_AXPN_PID_I "<big><code>I</code></big> Urmărire" // "$17"
#define L_AXPN_PID_D "<big><code>D</code></big> Urmărire" // "$18"
#define L_AXPN_PID_SLEWING_P "<big><code>P</code></big> Mișcare" // "$19"
#define L_AXPN_PID_SLEWING_I "<big><code>I</code></big> Mișcare" // "$20"
#define L_AXPN_PID_SLEWING_D "<big><code>D</code></big> Mișcare" // "$21"
#define L_AXPN_RADS_PER_COUNT "Rad/impuls"         // "$22"
#define L_AXPN_STEPS_TO_COUNTS "Raport pași/impulsuri" // "$23"
#define L_AXPN_MAX_ACCEL "Accel max, %/s/s"        // "$24"
#define L_AXPN_MIN_PWR "Putere min, %"             // "$25"
#define L_AXPN_MAX_PWR "Putere max, %"             // "$26"

#define L_ADV_DECAY_SLOW "Slow"
#define L_ADV_DECAY_FAST "Fast"
#define L_ADV_DECAY_MIXED "Mixed"
#define L_ADV_DECAY_SPREADCYCLE "SpreadCycle"
#define L_ADV_DECAY_STEALTHCHOP "StealthChop"

#define L_ADV_MOTOR "Motor"
#define L_REVERT "Revino la Valori Implicite"
#define L_ADV_SET_TITLE "Avansat"
#define L_ADV_SET_HEADER_MSG "modificările de mai jos au efect după repornirea OnStep, decât dacă se specifică altfel:"
#define L_ADV_MOUNT_TYPE " selectează 1 GEM, 2 EQ Fork sau 3 Alt/Azm"
#define L_ADV_BOOL "0 fals sau 1 adevărat"

#define L_ADV_SET_REV "Inversare direcție"

#define L_ADV_ENABLE "Activează Configurare Avansată"
#define L_ADV_DISABLE "Dezactivează Configurarea Avansată (toate valorile implicite la repornire)"
#define L_ADV_SET_AXIS_NO_EDIT "Editarea acestei axe a fost dezactivată."
#define L_ADV_SET_NO_EDIT "Editarea tuturor axelor este dezactivată momentan. Poate că această funcție nu este activată (deci valorile implicite sunt active) sau trebuie să reporniți OnStep."
#define L_ADV_SET_FOOTER_MSG0 "Majoritatea modificărilor de axe necesită repornire pentru a avea efect."
#define L_ADV_SET_FOOTER_MSG1 "După ce apăsați [Încarcă] verificați modificările de mai sus, încărcarea poate fi respinsă din cauza unui parametru invalid."
#define L_ADV_SET_FOOTER_MSG2 "Când alegeți [Revino], acel grup devine indisponibil pentru editare până la repornirea OnStep."
#define L_ADV_SET_FOOTER_MSG4 "Modificarea raportului dintre Axa1 Pași per rotație melc și Pași per grad poate corupe zona NV Library, faceți backup la cataloage înainte dacă e necesar."

// reset control
#define L_RESET_TITLE "Control Repornire OnStep:"
#define L_WIPE_RESET "Șterge toate setările și Repornește OnStep"
#define L_RESET "Repornire OnStep"
#define L_RESET_FWU "Repornire OnStep pentru încărcare firmware"
#define L_RESET_MSG1 "OnStep trebuie să fie Acasă sau Parcat, altfel cererea de repornire este ignorată."

// ----------------------------  SETUP ----------------------------

#define L_NETWORK_PERFORMANCE "Performanță și compatibilitate"
#define L_NETWORK_CMD_TO "Timeout citire serial canal comandă"
#define L_NETWORK_WWW_TO "Timeout citire serial canal web"

#define L_NETWORK_STA_TITLE "Mod Stație (conectare la Access-Point)"
#define L_NETWORK_PWD "Parolă"
#define L_NETWORK_PWD_MSG "min. 8 caract."
#define L_NETWORK_EN_DHCP "Activare DHCP"
#define L_NETWORK_EN_DHCP_MSG "Notă: adresele de mai sus sunt ignorate dacă DHCP este activat"
#define L_NETWORK_EN_STA "Activare Mod Stație"
#define L_IP_ADDRESS "Adresă IP"
#define L_GATEWAY "Gateway"
#define L_SUBNET "Mască Subrețea"
#define L_NETWORK_CHA "Canal"
#define L_NETWORK_EN_AP_MODE "Activare Mod Access-Point"

#define L_NETWORK_OTA_TITLE "Actualizare firmware SWS prin OTA"
#define L_NETWORK_EN_OTA "Activare Mod OTA"
#define L_REBOOT_OTA "Repornire în mod OTA"

#define L_NETWORK_CONFIG_SECURITY "Securitate Configurare Rețea"

#define L_NETWORK_AP "Mod Access-Point (auto-activat dacă modul Stație eșuează la conectare)"

#define L_NETWORK_RESTART_MSG "Trebuie să reporniți <u>manual</u> pentru ca modificările să aibă efect."

#define L_NETWORK_TITLE "Introduceți parola pentru a modifica configurația:"

#define L_NETWORK_ADVICE1 "Setup:<br/><br/>"
#define L_NETWORK_ADVICE2 "Activați doar modul Stație <b>SAU</b> Access Point, activarea ambelor poate cauza probleme de performanță.<br/><br/>"
#define L_NETWORK_ADVICE3 "Dacă vreți ca OnStep să se conecteze la rețeaua WiFi de acasă, activați doar modul Stație și "
#define L_NETWORK_ADVICE4 "dezactivați modul Access Point. Dacă OnStep nu se poate conecta la rețeaua WiFi de acasă, va "
#define L_NETWORK_ADVICE5 "reactiva automat modul Stație; util când sunteți la un sit întunecat și/sau "
#define L_NETWORK_ADVICE6 "pentru a evita să fiți blocat în afara dispozitivului.<br/><br/>"
#define L_NETWORK_ADVICE7 "Dacă sunteți blocat de WiFi, puteți folosi Arduino IDE pentru a reseta memoria flash ESP (și "
#define L_NETWORK_ADVICE8 "astfel revine la valorile implicite). Din meniul Arduino Tools, schimbați Erase Flash 'Only Sketch' "
#define L_NETWORK_ADVICE9 "în Erase Flash 'All Flash Contents' și încărcați sketch-ul WiFi-Bluetooth din nou."

// --------------------------- Constants -------------------------

#define L_DOWN_TITLE "Interfața Serial către OnStep este Întreruptă!"
#define L_DOWN_MESSAGE1 "Dacă tocmai ați repornit OnStep, așteptați câteva secunde pentru ca totul să pornească din nou, apoi verificați dacă sunteți încă conectat la WiFi."
#define L_DOWN_MESSAGE2 "Dacă este o problemă persistentă iată alte cauze posibile:<br /><ul>"
#define L_DOWN_MESSAGE3 "Nepotrivire baud rate din cauza repornirii doar a OnStep, opriți/porniți totul din nou."
#define L_DOWN_MESSAGE4 "Baud rate SWS configurat greșit."
#define L_DOWN_MESSAGE5 "Baud rate OnStep configurat greșit."
#define L_DOWN_MESSAGE6 "Cablare incorectă a porturilor com."
#define L_DOWN_MESSAGE7 "Cablare incorectă a pinilor de alimentare. GND trebuie partajat între MCU OnStep și MCU Addon."

#define L_CONTROLLER "Controller" // was Status
#define L_PAGE_MOUNT "Montură" // was Control
#define L_PAGE_AUXILIARY "Auxiliar"
#define L_PAGE_LIBRARY "Bibliotecă"
#define L_PAGE_ENCODERS "Encodere"
#define L_PAGE_PEC "PEC"
#define L_SETTINGS "Setări"
#define L_CONTROLS "Controale"
#define L_PAGE_CONFIG "Configurare"
#define L_PAGE_NETWORK "Rețea"

// ------------------------- MountStatus -------------------------

// general (background) errors
#define L_GE_NONE "Niciuna"
#define L_GE_MOTOR_FAULT "Defect motor/driver"
#define L_GE_ALT_MIN "Sub limita de orizont"
#define L_GE_LIMIT_SENSE "Senzor de limită"
#define L_GE_DEC "Limită Dec depășită"
#define L_GE_AZM "Limită Azm depășită"
#define L_GE_UNDER_POLE "Limită sub pol depășită"
#define L_GE_MERIDIAN "Limită meridian depășită"
#define L_GE_SYNC "Limită siguranță sincronizare depășită"
#define L_GE_PARK "Parcare eșuată"
#define L_GE_GOTO_SYNC "Sincronizare goto eșuată"
#define L_GE_UNSPECIFIED "Eroare necunoscută"
#define L_GE_ALT_MAX "Peste limita de zenit"
#define L_GE_WEATHER_INIT "Init senzor meteo eșuat"
#define L_GE_SITE_INIT "Ora/loc. neactualizate"
#define L_GE_NV_INIT "Eroare init NV/EEPROM"
#define L_GE_OTHER "Eroare necunoscută, cod"

// command errors
#define L_CE_NONE "Fără Erori"
#define L_CE_0 "Răspuns 0"
#define L_CE_CMD_UNKNOWN "comandă necunoscută"
#define L_CE_REPLY_UNKNOWN "răspuns invalid"
#define L_CE_PARAM_RANGE "parametru în afara intervalului"
#define L_CE_PARAM_FORM "format parametru greșit"
#define L_CE_ALIGN_FAIL "aliniere eșuată"
#define L_CE_ALIGN_NOT_ACTIVE "aliniere inactivă"
#define L_CE_NOT_PARKED_OR_AT_HOME "neparcat sau nu acasă"
#define L_CE_PARKED "deja parcat"
#define L_CE_PARK_FAILED "parcare eșuată"
#define L_CE_NOT_PARKED "neparcat"
#define L_CE_NO_PARK_POSITION_SET "nicio poziție de parcare setată"
#define L_CE_GOTO_FAIL "goto eșuat"
#define L_CE_LIBRARY_FULL "bibliotecă plină"
#define L_CE_GOTO_ERR_BELOW_HORIZON "goto sub orizont"
#define L_CE_GOTO_ERR_ABOVE_OVERHEAD "goto peste zenit"
#define L_CE_SLEW_ERR_IN_STANDBY "mișcare în standby"
#define L_CE_SLEW_ERR_IN_PARK "mișcare în parcare"
#define L_CE_GOTO_ERR_GOTO "deja în goto"
#define L_CE_GOTO_ERR_OUTSIDE_LIMITS "goto în afara limitelor"
#define L_CE_SLEW_ERR_HARDWARE_FAULT "defect hardware"
#define L_CE_MOUNT_IN_MOTION "montură în mișcare"
#define L_CE_GOTO_ERR_UNSPECIFIED "altă"
#define L_CE_UNK "necunoscut"
