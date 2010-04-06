#include "relayControl.h"
#include "i2cDevices.h"

#pragma CODE_SEG DEFAULT
#pragma DATA_SEG DEFAULT

// Aux Board Functions ------------------------------
// Disable the pullups and turn off the relays
void initAuxRelays(void) {
  AUX_COOL_PULLUP = 1;
  AUX_HEAT_PULLUP = 1;
  AUX_FAN_PULLUP  = 1;
  
  disableAuxCool();
  disableAuxFan();
  disableAuxHeat();
}

// Enable the cooling relay
void enableAuxCool(void) {
  disableAuxHeat();
  disableAuxFan();
  AUX_COOL_DIR = 0;
}

// Enable the fan relay
void enableAuxFan(void) {
  disableAuxCool();
  disableAuxHeat();
  AUX_FAN_DIR = 0;
}

// Enable the heating relay
void enableAuxHeat(void) {
  disableAuxCool();
  disableAuxFan();
  AUX_HEAT_DIR = 0;
}

// Disable the cooling relay
void disableAuxCool(void) {
  AUX_COOL_PIN = 0;
  AUX_COOL_DIR = 1;
}

// Disable the fan relay
void disableAuxFan(void) {
  AUX_FAN_PIN = 0;
  AUX_FAN_DIR = 1;
}

// Disable the heating relay
void disableAuxHeat(void) {
  AUX_HEAT_PIN = 0;
  AUX_HEAT_DIR = 1;
}

// Main Board Functions -----------------------------

// Enable the cooling relay
void enableMainCool(void) {
  clearBitsPortX(P_FAN | P_HEAT);
  setBitsPortX(P_COOL); 
}
// Enable the fan relay
void enableMainFan(void) {
  clearBitsPortX(P_COOL | P_HEAT);
  setBitsPortX(P_FAN);
}
// Enable the heating relay
void enableMainHeat(void) {
  clearBitsPortX(P_COOL | P_FAN);
  setBitsPortX(P_HEAT);
}

// Disable the cooling relay
void disableMainCool(void) {
  clearBitsPortX(P_COOL);
}

// Disable the fan relay
void disableMainFan(void) {
  clearBitsPortX(P_FAN);
}

// Disable the heating relay
void disableMainHeat(void) {
  clearBitsPortX(P_HEAT);
}