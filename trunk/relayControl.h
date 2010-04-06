#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include "derivative.h"

// Pin Definitions for Aux Board --------------------
#define AUX_COOL_PIN      PTA_PTA0
#define AUX_FAN_PIN       PTA_PTA1
#define AUX_HEAT_PIN      PTA_PTA4
#define AUX_COOL_DIR      DDRA_DDRA0
#define AUX_FAN_DIR       DDRA_DDRA1
#define AUX_HEAT_DIR      DDRA_DDRA4
#define AUX_COOL_PULLUP   PTAPUE_PTAPUE0
#define AUX_FAN_PULLUP    PTAPUE_PTAPUE1
#define AUX_HEAT_PULLUP   PTAPUE_PTAPUE4

// Functions for Aux Board --------------------------
void initAuxRelays(void);
void enableAuxCool(void);
void enableAuxFan(void);
void enableAuxHeat(void);
void disableAuxCool(void);
void disableAuxFan(void);
void disableAuxHeat(void);

// Functions for Main Board -------------------------
void enableMainCool(void);
void enableMainFan(void);
void enableMainHeat(void);
void disableMainCool(void);
void disableMainFan(void);
void disableMainHeat(void);

#endif