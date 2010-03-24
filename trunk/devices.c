#include "devices.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char DEVICE_DATA[2];
unsigned char DEVICE_TIME[7];
unsigned char DEVICE_ALRM[4];
unsigned char PORTX_DATA;

// Write the configuration byte to the temperature sensor
void tempConfig(void) {
  DEVICE_DATA[0] = T_SETTING;  
  i2c_write(TEMPSENSE_ADDR, T_CONFIG, DEVICE_DATA,1);
  del1m(20);
}

// Set the high and low registers based on some margin
void tempSet(unsigned char temp) {
  DEVICE_DATA[0] = temp -  T_PLUSMINUS;
  DEVICE_DATA[1] = 0x00;
  i2c_write(TEMPSENSE_ADDR, T_TL, DEVICE_DATA, 2);
  del1m(20);
  
  DEVICE_DATA[0] = temp + T_PLUSMINUS;
  i2c_write(TEMPSENSE_ADDR, T_TH, DEVICE_DATA, 2);
  del1m(20);
}

// Start converting
void tempStart(void) {
 i2c_write(TEMPSENSE_ADDR, T_START, DEVICE_DATA, 0);
 del1m(1000); // wait 1000 ms for first temperature to be ready
}

// Stop converting
void tempStop(void) {
  i2c_write(TEMPSENSE_ADDR, T_STOP, DEVICE_DATA, 0);
}

// Return the temperature
unsigned char tempGet(void) {
  i2c_read(TEMPSENSE_ADDR, T_TEMP, DEVICE_DATA, 2);
  return DEVICE_DATA[0];
}
// ------------------------------------------------------------------
// Setup the port expander initially
void portxInit(void) {
  PORTX_DATA = P_INIT;
  portxSet();
}

// Write to the port expander
void portxSet(void) {
  i2c_write(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Turn specific bits on
void portxOn(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA | bitmask; 
  portxSet();
}

// Turn specitif bits off
void portxOff(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA & ~bitmask;
  portxSet();
}

// ------------------------------------------------------------------
// Set the first two configuration bytes for the clock
void clockConfig(void) {
  DEVICE_DATA[0] = C_SETTING1;
  DEVICE_DATA[1] = C_SETTING2;
  i2c_write(RTC_ADDR, C_CONTROL1, DEVICE_DATA, 2);
}

// Clear the alarm interrupt
void clockClearAI(void) {
  DEVICE_DATA[0] = C_SETTING2 & 0xF7;
  i2c_write(RTC_ADDR, C_CONTROL2, DEVICE_DATA, 1);
}

// Clear the timer interrupt
void clockClearTI(void) {
  DEVICE_DATA[0] = C_SETTING2 & 0xFB;
  i2c_write(RTC_ADDR, C_CONTROL2, DEVICE_DATA, 1);
}

void clockSetTime(void) {
  i2c_write(RTC_ADDR, C_SECONDS, DEVICE_TIME, 7);
}

void clockGetTime(void) {
  i2c_read(RTC_ADDR, C_SECONDS, DEVICE_TIME, 7);
}

void clockSetAlrm(void) {
  i2c_write(RTC_ADDR, C_ALARM_MINUTE, DEVICE_ALRM, 4);
}


  