// -----------------------------------------------------------------------------------
// Locale German  ISO ISO639-1 language code "de"
#pragma once

// special German characters 
// Ã¤ --> &auml;
// Ã¶ --> &ouml;
// Ã¼ --> &uuml;
// Ã„ --> &Auml;
// Ã– --> &Ouml;
// Ãœ --> &Uuml;
// ÃŸ --> &szlig; 

// General
#define L_UPLOAD "Upload"
#define L_DOWNLOAD "Download"
#define L_ACCEPT "Akzeptieren"
#define L_CONTINUE "Fortsetzen"
#define L_ARE_YOU_SURE "Bist du sicher"
#define L_LOGOUT "Abmelden"

#define L_UT "UT"
#define L_LST "Sternzeit"
#define L_LONG "L&auml;nge"
#define L_LAT "Breite"
#define L_EAST "Ost"
#define L_WEST "West"
#define L_ZENITH "Zenit"
#define L_NCP "Nordpol"
#define L_SCP "S&uuml;dpol"

#define L_NONE "Kein"
#define L_AUTO "Auto"
#define L_SET_LABEL "Set"
#define L_OK "Tun"
#define L_ON "An"
#define L_OFF "Aus"
#define L_START "Anfang"
#define L_DELTA "Delta"
#define L_ZERO "Null"
#define L_SPAN "Spanne"
#define L_POWER "Macht"
#define L_GUIDE "Nachf&uuml;hrung"
#define L_CENTER "Zentrieren"
#define L_BLUE "Blau"
#define L_GREEN "Gr&uuml;"
#define L_RATE "Rate"
#define L_RANGE "Bereich"
#define L_UNKNOWN "Unbekannt"
#define L_FAULT "Fehler"
#define L_POINT "Punkt"
#define L_COMPLETE "Vollständig"
#define L_ACTIVE "Aktiv"
#define L_INACTIVE "Inaktiv"
#define L_BEST "Am besten"
#define L_ALTERNATE "Wechseln"

// ---------------------------- Index ----------------------------
// date/time
#define L_TIME_BROWSER_UT "UT (Webbrowser)"

// weather
#define L_TEMPERATURE "Temperatur"
#define L_AMBIENT_TEMPERATURE "Umgebungstemperatur"
#define L_PRESSURE "Luftdruck"
#define L_HUMIDITY "Relative Luftfeuchte"
#define L_DEW_POINT "Taupunkt-Temperatur"

// operation
#define L_TRACKING "die Verfolgung"
#define L_SITE "Ort"
#define L_COORDINATES "Koordinaten"
#define L_CURRENT "Aktuell"
#define L_TARGET "Ziel"
#define L_ENCODER "Encoder"
#define L_POLAR_ALIGN "Polausrichtung"

// pier side
#define L_PIER_SIDE "Teleskopseite"
#define L_MERIDIAN_FLIPS "Meridianflips"
#define L_ALIGN_MESSAGE "Montierung relativ zu"
#define L_MERIDIAN_FLIP_W_TO_E "Meridian Flip, West zu Ost"
#define L_MERIDIAN_FLIP_E_TO_W "Meridian Flip, Ost zu West"

#define L_OPERATIONS "Betrieb"

// parking
#define L_PARKING "Parken"
#define L_PARKED "Geparkt"
#define L_NOT_PARKED "Nicht Geparkt"
#define L_PARKING "Parken"
#define L_PARK_FAILED "Parken fehlgeschlagen"

// home
#define L_HOMING "Referenzfahrt"
#define L_AT_HOME "Bei Homeposition"
#define L_SLEWING "Schwenke"
#define L_PPS_SYNC "PPS Sync"
#define L_HOME_REV "Umkehren"
#define L_HOME_OFFSET "Versatz zur Home-Sense-Position:"
#define L_HOME_OFFSET_RANGE_AXIS1 "Axis1, +/-" STR(HOME_OFFSET_RANGE_AXIS1) " arc-sec"
#define L_HOME_OFFSET_RANGE_AXIS2 "Axis2, +/-" STR(HOME_OFFSET_RANGE_AXIS2) " arc-sec"
#define L_HOME_AUTO "Automatisch nach Hause beim Start:"

// tracking
#define L_REFR_COMP_RA "RC"
#define L_REFR_COMP_BOTH "RCD"
#define L_FULL_COMP_RA "FC"
#define L_FULL_COMP_BOTH "FCD"
#define L_TRACKING "Verfolgung"
#define L_CURRENT_MAXRATE "Aktuelle maximale Rate"
#define L_DEFAULT_MAXRATE "Standard maximale Rate"
#define L_CURRENT_MAXSLEW "Maximale Schwenkgeschwindigkeit"

#define L_FREQUENCY_ADJUST "Frequenzanpassung"

// driver status
#define L_DRIVER "Teriber"
#define L_STATUS "Status"
#define L_STATE "Zustand"
#define L_COMMS_FAILURE "Befehlsfehler"
#define L_STANDSTILL "Dtillstand"
#define L_OPEN_LOAD "Offene last"
#define L_SHORT_GND "Masseschluss"
#define L_SHUTDOWN_OVER "Abschaltung &uuml;ber"
#define L_PRE_WARNING "Vorwarnung"
#define L_DRIVER_FAULT "Fehler"

// misc. errors/workload
#define L_HARDWARE "Hardware"
#define L_INTERNAL_TEMP "Interne Controller Temperatur"
#define L_LAST_GENERAL_ERROR "Letzter allgemeiner (Hintergrund-) Fehler"
#define L_WORKLOAD "Auslastung"
#define L_WIRELESS_SIGNAL_STRENGTH "WiFi-Signalst&auml;rke"

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
#define BUTTON_E "O"
#define BUTTON_W "W"
#define BUTTON_SYNC "@"

#define L_HINT_CLOCK "Setze Datum/Zeit"
#define L_HINT_CONTINUE_GOTO "Setze Goto fort (wenn pausiert bei Homeposition)"
#define L_HOME_PARK_TITLE "Referenzieren und Parken"
#define L_FIND_HOME "Suche Homeposition"
#define L_RESET_HOME "Homeposition (Reset)"
#define L_UNPARK "Entparken"
#define L_PARK "Parken"
#define L_HINT_EMERGENCY_STOP "Notfall Stop!"
#define L_HINT_RATE_SIDEREAL "Sterngeschwindigkeit"
#define L_HINT_RATE_LUNAR "Mondgeschwindigkeit"
#define L_HINT_RATE_SOLAR "Sonnengeschwindigkeit"
#define L_HINT_RATE_KING "Königstarif"
#define L_HINT_FOC_SEL "W&auml;hle Fokussierer"
#define L_HINT_DEROTATE_ON "Derotation An"
#define L_HINT_ROTATOR_REV "umgekehrte Derotator-Richtung"
#define L_HINT_ROTATOR_PAR "Bewege Rotator zum parallaktischen Winkel"
#define L_HINT_DEROTATE_OFF "Derotation Aus"

#define L_ALIGN "Align"
#define L_AUX_FEATURES "Zusatzfunktionen"

#define L_FOCUSER "Fokussierer"
#define L_ROTATOR "Rotator"
#define L_DEROTATOR "Derotator"
#define L_CAMERA_EXPOSURE "Exposition"
#define L_CAMERA_DELAY "Verzögern"
#define L_CAMERA_COUNT "Anzahl"

// ---------------------------- Library --------------------------

#define L_CATALOG "Katalog"
#define L_CATALOGS "Kataloge"
#define L_RECS_AVAIL "Datens&auml;tze verf&uuml;gbar"
#define L_NO_CATALOG "Kein Katalog gew&auml;hlt"
#define L_CAT_DATA "Daten (heruntergeladen ODER zum Hochladen.)"

#define L_CAT_EXAMPLE1 "Felder:<br /><pre>"
#define L_CAT_EXAMPLE2 "  F&uumlr Zeile 1 ist ein Katalognamens-Datensatz erforderlich, der<br />"
#define L_CAT_EXAMPLE3 "  aus einem '$' gefolgt von bis zu 10 Zeichen besteht.<br /><br />"
#define L_CAT_EXAMPLE4 "  Der Objektname ist eine Folge von bis zu 11 Zeichen.<br /><br />"
#define L_CAT_EXAMPLE5 "  Cat ist ein Kategoriecode wie folgt:<br />"
#define L_CAT_EXAMPLE6 "  RA (Jnow) ist in Stunden, Minuten  und Sekunden.<br />"
#define L_CAT_EXAMPLE7 "  Dec (Jnow) ist in +/- Grad, Minuten, Secunden.<br />"

#define L_CAT_NO_NAME "Gew&auml;hlter Katalog hat keinen Namen."
#define L_CAT_GET_NAME_FAIL "Namensselektion f&uuml;r gew&auml;hlten Katalog fehlgeschlagen."
#define L_CAT_NO_CAT "Kein Katalog gew&auml;hlt."
#define L_CAT_NO_OBJECT "Kein Objekt ausgewählt."
#define L_SELECTED "gew&auml;hlt"
#define L_CAT_DOWNLOAD_FAIL "Download fehlgeschlagen. Ung&uuml;ltige Daten."
#define L_CAT_DOWNLOAD_SUCCESS "Download erfolgreich."
#define L_CAT_DOWNLOAD_INDEX_FAIL "Download fehlgeschlagen, Konnte Katalog nicht indizieren."
#define L_CAT_DATA_REMOVED "Katalogdaten entfernt"
#define L_CAT_UPLOAD_FAIL "Upload fehlgeschlagen. Ung&uuml;ltiger Katalogname."
#define L_CAT_UPLOAD_INDEX_FAIL "Upload fehlgeschlagen beim Indizieren des Katalognamensdatensatzes"
#define L_CAT_DELETE_FAIL "Upload fehlgeschlagen beim L&ouml;schen des Katalognamensdatensatzes"
#define L_CAT_WRITE_NAME_FAIL "Upload fehlgeschlagen beim Schreiben des Katalognamensdatensatzes."
#define L_CAT_UPLOAD_NO_NAME_FAIL "Upload fehlgeschlagen, Zeile 1 muss den Katalognamen enthalten."
#define L_CAT_BAD_FORM "Upload fehlgeschlagen, ung&uuml;ltiges Format in Zeile# "
#define L_CAT_UPLOAD_BAD_OBJECT_NAME "Upload fehlgeschlagen, ung&uuml;ltiger Objektname in Zeile# "
#define L_CAT_BAD_CATEGORY "Upload fehlgeschlagen, ung&uuml;ltige Kategorie in Zeile# "
#define L_CAT_BAD_RA "Upload fehlgeschlagen, ung&uuml;ltiges RA-Format in Zeile# "
#define L_CAT_BAD_DEC "Upload fehlgeschlagen, ung&uuml;ltiges Dec-Format in Zeile# "
#define L_CAT_UPLOAD_RA_FAIL "Upload fehlgeschlagen beim Senden von RA in Zeile# "
#define L_CAT_UPLOAD_DEC_FAIL "Upload fehlgeschlagen beim Senden von Dec in Zeile# "
#define L_CAT_UPLOAD_LINE_FAIL "Upload fehlgeschlagen beim Senden Zeile# "
#define L_CAT_UPLOAD_SUCCESS "Upload erfolgreich"
#define L_CAT_UPLOAD_LINES_WRITTEN "Zeilen geschrieben"
#define L_CAT_UPLOAD_SELECT_FAIL "Der Upload ist fehlgeschlagen, der Katalog konnte nicht ausgew&auml;hlt werden."
#define L_CAT_UPLOAD_NO_CAT "Upload fehlgeschlagen, kein Katalog ausgewÃ¤hlt."
#define L_CAT_CLEAR "Katalog löschen"
#define L_CAT_CLEAR_LIB "Bibliothek löschen"

// --------------------------- Encoders --------------------------

// general settings
#define L_ENC_SYNC_TO_ENC "OnStep -> Encoder synchronisieren"
#define L_ENC_SYNC_TO_ONS "Encoder -> OnStep synchronisieren"
#define L_ENC_ORIGIN_TO_ONS "Stellen Sie OnStep -> Encoders Origin ein"
#define L_ENC_AUTO_SYNC "Encoder -> OnStep automatisch synchronisieren"
#define L_ENC_CONF "Encoder-Konfiguration"
#define L_ENC_SET_TPD "Zählt pro Grad"
#define L_ENC_SYNC_THLD "Sync-Schwelle"
#define L_ENC_SYNC_THLD_UNITS "0 bis 9999 Bogensekunden"
#define L_ENC_AUTO_RATE "OnStep Raten Steuerung"
#define L_NOW "Jetzt"

// ------------------------------ PEC ----------------------------

#define L_PEC_STATUS "Status"
#define L_PEC_CONTROL "PEC Steuerung"
#define L_PEC_PLAY "Abspielen"
#define L_STOP "Stop"
#define L_PEC_CLEAR "L&ouml;schen"
#define L_PEC_REC "Aufzeichnen"
#define L_PEC_CLEAR_MESSAGE "L&ouml;schen l&ouml;scht den Speicher aber nicht den EEPROM.  W&auml;hrend der Aufnahme werden die Korrekturen 3:1 gemittelt, wobei der Puffer bevorzugt wird, es sei denn, er wird gel&ouml;scht. In diesem Fall wird die volle Korrektur verwendet."
#define L_PEC_EEWRITE "Schreibe in EEPROM"
#define L_PEC_EEWRITE_MESSAGE "Schreibt PEC-Daten in das EEPROM, damit diese verf&uuml;gbar sind wenn die Steuerung neu startet.  Das Schreiben der Daten kann einige Sekunden dauern."
#define L_DISABLED_MESSAGE "DEAKTIVIERT"
#define L_PEC_IDLE "Leerlauf"
#define L_PEC_WAIT_PLAY "Abspielen wartet auf Start"
#define L_PEC_PLAYING "Abspielen"
#define L_PEC_WAIT_REC "Aufzeichnung wartet auf Indexsignal"
#define L_PEC_RECORDING "Zeichne auf"
#define L_PEC_UNK "Unbekannt"
#define L_PEC_EEWRITING "schreibe in EEPROM"
#define L_PEC_STEPS_PER_WORM_ROTATION "Schritte pro Umdrehung"

// --------------------------- Settings --------------------------

#define L_REFINE_POLAR_ALIGN "Verfeinere Polausrichtung"
#define L_REFINE_PA "Verfeinere PA"
#define L_REFINE_MESSAGE1 "Setup und 3+ Stern-Ausrichtung der Montierung. F&auml;hrt zu einem hellen Stern in der N&auml;he von Himmelsnord- oder -s&uuml;dpols mit einer Deklination von 50 bis 80 Grad (N oder S). "
#define L_REFINE_MESSAGE2 "Dr&uuml;cken Sie dann die [Verfeinere PA] Taste. Verwenden Sie die Richtungstasten, um den Stern im Okular wieder zu zentrieren. "
#define L_REFINE_MESSAGE3 "Optional richten Sie die  Montierung erneut aus."

#define L_PARK "Parken"
#define L_SET_PARK "Setze Parkposition"
#define L_SET_PARK_CURRENT_COORDS "Parkposition auf aktuelle Koordinaten setzen"

#define L_TRK_FASTER "schneller"
#define L_TRK_SLOWER "langsamer"
#define L_TRK_RESET "Reset (standard)"
#define L_TRK_COMP "Kompensation der Nachf&uuml;hrgeschwindigkeit (Pointing Modell/Refraktion)"
#define L_TRK_FULL "Voll"
#define L_TRK_REFR "Nur Refraktion"
#define L_TRK_DUAL "Beide Achsen"
#define L_TRK_SINGLE "Eine Achse"

#define L_BUZZER "Goto Alarm, Buzzer"

#define L_MERIDIAN_FLIP_AUTO "Automatischer Meridianflip bei Grenz&uuml;berschreitung"
#define L_MERIDIAN_FLIP_NOW "Jetzt"
#define L_MERIDIAN_FLIP_PAUSE "Pausiere an Homeposition w&auml;hrend Meridian Flip"
#define L_MERIDIAN_FLIP_PPS "Meridianflip, bevorzugte Pierseite"

#define L_ORIENTATION_AUTO "Orientierungsänderung"
#define L_ORIENTATION_CHANGE_NOW L_MERIDIAN_FLIP_NOW
#define L_ORIENTATION_CHANGE_PAUSE "Umdrehen, zu Hause innehalten"
#define L_ORIENTATION_CHANGE_PPS "Umdrehen, bevorzugte Ausrichtung"

// ------------------------ Configuration ------------------------

#define L_BASIC_SET_TITLE "Basic:"

#define L_MAX_SLEW_SPEED "Maximale Schwenkgeschwindigkeit"

// goto Rate
#define L_VSLOW "Sehr langsam"
#define L_SLOW "Langsam"
#define L_NORMAL "Normal"
#define L_FAST "Schnell"
#define L_VFAST "Sehr schnell"

// backlash
#define L_BACKLASH "Getriebespiel"
#define L_BACKLASH_RANGE_AXIS1 "Getriebespiel, in Bogensekunden 0 to 3600"
#define L_BACKLASH_RANGE_AXIS2 "Getriebespiel, in Bogensekunden 0 to 3600"
#define L_BACKLASH_RANGE_AXIS3UP "Spiel in den Schritten 0 bis 32767"

// deadband/TCF
#define L_DEADBAND "TCF Deadband"
#define L_TCF_COEF "TCF Coef"
#define L_TCF_COEF_EN "TCF aktivieren"
#define L_DEADBAND_RANGE_AXIS4UP "TCF Deadband, in den Schritten 1 bis 32767"
#define L_TCF_COEF_RANGE_AXIS4UP "TCF Coef, in Mikrometern/Grad C &pm;999,0"
#define L_TCF_COEF_EN_AXIS4UP "TCF aktivieren, 0 falsch oder 1 wahr"

// limits
#define L_LIMITS_TITLE "Horizont- und Overhead-Grenzen"
#define L_LIMITS_RANGE_HORIZON "Horizont, in Grad +/- 30"
#define L_LIMITS_RANGE_OVERHEAD "Obergrenze, in Grad 60 to 90"
#define L_LIMITS_RANGE_MERIDIAN_E "Nach Meridian wenn Teleskop auf Ostseite der Montierung, in Grad +/- 270"
#define L_LIMITS_RANGE_MERIDIAN_W "Nach Meridian wenn Teleskop auf Westseite der Montierung, in Grad +/- 270"

// location
#define L_LOCATION_TITLE "Zeit und Ort"
#define L_LOCATION_LONG "L&auml;ngengad, in Grad +/- 180, W is +"
#define L_LOCATION_LAT "Breitengrad, in Grad +/- 90, N is +"
#define L_LOCATION_RANGE_UTC_OFFSET "UTC Offset, in Stunden -14 to +12"
#define L_LOCATION_MESSAGE_UTC_OFFSET "Entgegengesetzter Zeitzonenwert f&uuml;r die Standardzeit (Keine Winterzeit!)"

// advanced configuration

#define L_AXPN_STEPS_PER_DEG "Schritte/Grad"
#define L_AXPN_LIMIT_DEGS_MIN "Min. Grenzwert Grad"
#define L_AXPN_LIMIT_DEGS_MAX "Max. Grenzwert Grad"
#define L_AXPN_STEPS_PER_UM "Schritte/µm"
#define L_AXPN_LIMIT_UM_MIN "Min. Grenzwert µm"
#define L_AXPN_LIMIT_UM_MAX "Max. Grenzwert µm"
#define L_AXPN_REVERSE "Umkehren"
#define L_AXPN_MICROSTEPS "Mikroschritte"
#define L_AXPN_MICROSTEPS_GOTO "Mikroschritte Goto"
#define L_AXPN_DECAY_MODE "Zerfallsmodus"
#define L_AXPN_DECAY_MODE_GOTO "Zerfallsmodus Goto"
#define L_AXPN_CURRENT_HOLD "mA Halten"
#define L_AXPN_CURRENT_RUN "mA Betrieb"
#define L_AXPN_CURRENT_GOTO "mA Goto"
#define L_AXPN_INTERPOLATE "256x Interpolieren"
#define L_AXPN_PID_P "P-Nachführung"
#define L_AXPN_PID_I "I-Nachführung"
#define L_AXPN_PID_D "D-Nachführung"
#define L_AXPN_PID_SLEWING_P "P-Schwenken"
#define L_AXPN_PID_SLEWING_I "I-Schwenken"
#define L_AXPN_PID_SLEWING_D "D-Schwenken"
#define L_AXPN_RADS_PER_COUNT "Rad/Zählung"

#define L_ADV_DECAY_SLOW "Langsam"
#define L_ADV_DECAY_FAST "Schnell"
#define L_ADV_DECAY_MIXED "Gemischt"
#define L_ADV_DECAY_SPREADCYCLE "SpreadCycle"
#define L_ADV_DECAY_STEALTHCHOP "StealthChop"

#define L_ADV_MOTOR "Motor"
#define L_REVERT "Auf Standard zurücksetzen"
#define L_ADV_SET_TITLE "Fortgeschrittene"
#define L_ADV_SET_HEADER_MSG "sofern nicht anders angegeben, werden die unten aufgeführten Änderungen nach dem Neustart von OnStep wirksam:"
#define L_ADV_MOUNT_TYPE " wählen 1 GEM, 2 EQ Fork, 3 Alt/Azm"
#define L_ADV_BOOL "0 falsch oder 1 wahr"

#define L_ADV_SET_REV "Umgekehrte Richtung"

#define L_ADV_ENABLE "Aktivieren Sie die erweiterte Konfiguration"
#define L_ADV_DISABLE "Deaktivieren Sie die erweiterte Konfiguration (alle Standardeinstellungen beim Neustart)"
#define L_ADV_SET_AXIS_NO_EDIT "Die Bearbeitung dieser Achse ist derzeit deaktiviert."
#define L_ADV_SET_NO_EDIT "Deaktivieren Sie die erweiterte Konfiguration (alle Standardeinstellungen beim Neustart)"
#define L_ADV_SET_FOOTER_MSG0 "Die meisten Änderungen an den Achseneinstellungen erfordern einen Neustart, damit sie wirksam werden."
#define L_ADV_SET_FOOTER_MSG1 "Beachten Sie nach dem Drücken von [Upload] die obigen Änderungen, da der Upload möglicherweise aufgrund eines ungültigen Parameters abgelehnt wurde."
#define L_ADV_SET_FOOTER_MSG2 "Wenn Sie [Zurücksetzen] wählen, kann diese Gruppe erst bearbeitet werden, wenn Sie OnStep neu starten."
#define L_ADV_SET_FOOTER_MSG3 "Wenn IRUN auf einen anderen Wert als den Standardwert von Config.h eingestellt ist, wird IHOLD auf 50% (HALF) eingestellt."
#define L_ADV_SET_FOOTER_MSG4 "Durch Ändern des Verhältnisses von Axis1-Schritten pro Wurmdrehung und Schritten pro Grad kann der Speicherbereich von Library NV beschädigt werden. Sichern Sie bei Bedarf zuerst alle Kataloge."

// reset control
#define L_RESET_TITLE "OnStep Steuerung zurücksetzen:"
#define L_WIPE_RESET "Einstellungen zurücksetzen und OnStep neu starten"
#define L_RESET "Zurücksetzen OnStep"
#define L_RESET_FWU "Zurücksetzen OnStep zum Hochladen der Firmware"
#define L_RESET_MSG1 "OnStep muss zu Hause oder geparkt sein, andernfalls wird die Rücksetzanforderung ignoriert."

// ----------------------------  WiFi ----------------------------

#define L_NETWORK_PERFORMANCE "Leistung und Kompatibilität"

#define L_NETWORK_CMD_TO "Timeout f&uuml;r die serielles Auslesung des Befehlskanals:"
#define L_NETWORK_WWW_TO "Timeout f&uuml;r das serielles Auslesen des Webkanals:"

#define L_NETWORK_STA_TITLE "Stationsmodus (Verbunden an einen Access-Point)"
#define L_NETWORK_PWD "Passwort"
#define L_NETWORK_PWD_MSG "mindestens 8 Zeichen"
#define L_NETWORK_EN_DHCP "DHCP aktivieren"
#define L_NETWORK_EN_DHCP_MSG "Hinweis: obige Adressen werden ignoriert wenn DHCP aktiviert ist!"
#define L_NETWORK_EN_STA "Aktiviere Stationsmodus"
#define L_IP_ADDRESS "IP Addresse"
#define L_GATEWAY "Gateway"
#define L_SUBNET "Subnetzmaske"
#define L_NETWORK_CHA "Kanal"
#define L_NETWORK_EN_AP_MODE "Aktiviere Access-Point Modus"

#define L_NETWORK_CONFIG_SECURITY "Netzwerk Konfiguration Sicherheit"

#define L_NETWORK_AP "Access Point Modus"

#define L_NETWORK_RESTART_MSG "Sie m&uuml;ssen <u>manuell</u> neu starten, damit die &Auml;nderungen wirksam werden."

#define L_NETWORK_TITLE "Geben Sie das Passwort ein um die Konfiguration zu &auml;ndern:"

#define L_NETWORK_ADVICE1 "Setup:<br/><br/>"
#define L_NETWORK_ADVICE2 "Aktivieren Sie nur einen der beiden Modi <i>Stationsmodus</i> <b>ODER</b> <i>Access Point Modus</i>. Das gleichzeitige Aktivieren beider Optionen kann zu Performanceproblemen f&uuml;hren!<br/><br/>"
#define L_NETWORK_ADVICE3 "Wenn die Steuerung eine Verbindung zu Ihrem Heim-WLAN herstellen soll, aktivieren Sie nur den Stationsmodus und "
#define L_NETWORK_ADVICE4 "deaktivieren Sie den Access Point Modus. Wenn die Steuerung keine Verbindung zu Ihrem Heim-WLAN herstellen kann, wird "
#define L_NETWORK_ADVICE5 "der Access Point Modus automatisch wieder aktiviert. Dies ist n&uuml;tzlich, wenn Sie sich an einem dunklen Ort befinden und/oder "
#define L_NETWORK_ADVICE6 "um sich vor einer Aussperrung zu sch&uuml;tzen.<br/><br/>"
#define L_NETWORK_ADVICE7 "Wenn Sie vom WLAN ausgeschlossen sind, m&uuml;ssen Sie die Arduino IDE verwenden, um den ESP Flash-Speicher zur&uuml;ckzusetzen (und somit zu den Standardeinstellungen zur&uuml;ckzukehren)."
#define L_NETWORK_ADVICE8 "&Auml;ndern Sie im Men&uuml; Arduino Tools die Option Erase Flash 'Only Sketch' auf Erase Flash 'All Flash Contents' "
#define L_NETWORK_ADVICE9 "und laden Sie die WiFi-Bluetooth Firmware erneut hoch."

// --------------------------- Constants -------------------------

#define L_DOWN_TITLE "Serielles Interface zur OnStep ist nicht verbunden!"
#define L_DOWN_MESSAGE1 "Aus- und Wiedereinschalten kann helfen, wenn es eine Unstimmigkeit mit der Baudrate gibt."
#define L_DOWN_MESSAGE2 "Andere m&ouml;gliche Gr&uuml;nde:<br /><ul>"
#define L_DOWN_MESSAGE3 "Inkorrekt konfigurierte OnStep Baudrate."
#define L_DOWN_MESSAGE4 "Inkorrekt konfigurierte ESP Baudrate."
#define L_DOWN_MESSAGE5 "Inkorrekt konfigurierte MCU Baudrate."
#define L_DOWN_MESSAGE6 "Inkorrekte Verkabelung der  COM-Ports (oder Hardwareschaden wenn RX/TX vertauscht wurden.)"
#define L_DOWN_MESSAGE7 "Incorrekte Verkabelung der Stromversorgungs-Pins. Masse (GND) muss zwischen OnStep's MCU und der MCU des Addon's verbunden sein."

#define L_CONTROLLER "Status"
#define L_PAGE_MOUNT "Steuerung"
#define L_PAGE_AUXILIARY "Hilfs"
#define L_PAGE_LIBRARY "Bibliothek"
#define L_PAGE_ENCODERS "Encoder"
#define L_PAGE_PEC "PEC"
#define L_SETTINGS "Einstellungen"
#define L_CONTROLS "Kontrollen"
#define L_PAGE_CONFIG "Konfig."
#define L_PAGE_NETWORK "Netzwerk"

// ------------------------- MountStatus -------------------------

// general (background) errors
#define L_GE_NONE "keiner"
#define L_GE_MOTOR_FAULT "Motor/Treiber Fehler"
#define L_GE_ALT_MIN "Unter der Horizontgrenze" 
#define L_GE_LIMIT_SENSE "Grenzschalter ausgel&ouml;st"
#define L_GE_DEC "Deklinationsgrenze &uuml;berschritten"
#define L_GE_AZM "Azimutgrenze &uuml;berschritten"
#define L_GE_UNDER_POLE "Unterpolgrenze &uuml;berschritten"
#define L_GE_MERIDIAN "Meridiangrenze &uuml;berschritten"
#define L_GE_SYNC "Sync-Sicherheitsgrenze &uuml;berschritten"
#define L_GE_PARK "Parken fehlgeschlagen"
#define L_GE_GOTO_SYNC "Goto/Sync fehlgeschlagen"
#define L_GE_UNSPECIFIED "Unbekannter Fehler"
#define L_GE_ALT_MAX "&uuml;ber Obergrenze"
#define L_GE_WEATHER_INIT "Wettersensor Initialisierung fehlgeschlagen"
#define L_GE_SITE_INIT "Zeit oder Ort nicht aktualisiert"
#define L_GE_NV_INIT "Init NV/EEPROM schlecht"
#define L_GE_OTHER "Unbekannter Fehler, Code"

// command errors
#define L_CE_NONE "Keine Fehler"
#define L_CE_0 "Antwort 0"
#define L_CE_CMD_UNKNOWN "unbekanntes Kommando"
#define L_CE_REPLY_UNKNOWN "ung&uuml;ltige Antwort"
#define L_CE_PARAM_RANGE "Parameter ausserhalb des Bereiches"
#define L_CE_PARAM_FORM "ung&uuml;ltiges Parameterformat"
#define L_CE_ALIGN_FAIL "Ausrichtung fehlgeschlagen"
#define L_CE_ALIGN_NOT_ACTIVE "Ausrichtung nicht ativ"
#define L_CE_NOT_PARKED_OR_AT_HOME "nicht geparkt oder in Homeposition"
#define L_CE_PARKED "schon geparkt"
#define L_CE_PARK_FAILED "Parken fehlgeschlagen"
#define L_CE_NOT_PARKED "nicht geparkt"
#define L_CE_NO_PARK_POSITION_SET "keine Parkposition gesetzt"
#define L_CE_GOTO_FAIL "Goto fehlgeschlagen"
#define L_CE_LIBRARY_FULL "Bibliothek voll"
#define L_CE_GOTO_ERR_BELOW_HORIZON "Goto unter Horizont"
#define L_CE_GOTO_ERR_ABOVE_OVERHEAD "Goto &uuml;ber Obergrenze"
#define L_CE_SLEW_ERR_IN_STANDBY "Schwenken in Standby"
#define L_CE_SLEW_ERR_IN_PARK "Schwenken in Parkposition"
#define L_CE_GOTO_ERR_GOTO "bereits im Goto"
#define L_CE_GOTO_ERR_OUTSIDE_LIMITS "Goto ausserhalb der Grenzen"
#define L_CE_SLEW_ERR_HARDWARE_FAULT "Hardwarefehler"
#define L_CE_MOUNT_IN_MOTION "Montierung in Bewegung"
#define L_CE_GOTO_ERR_UNSPECIFIED "andere"
#define L_CE_UNK "unbekannt"
