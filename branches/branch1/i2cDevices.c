#include "derivative.h"
#include "i2cDevices.h"

#include "i2c.h"
#include "delay.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char DEVICE_DATA[2];
unsigned char RTC_TIME[7];
unsigned char PORTX_DATA;

// Temperature Sensor -------------------------------

// Write the configuration byte to the temperature sensor
void configureTemp(void) {
  DEVICE_DATA[0] = T_SETTING;  
  writeI2C(TEMPSENSE_ADDR, T_CONFIG, DEVICE_DATA,1);
  del1m(20);
}

// Start converting
void startTemp(void) {
 writeI2C(TEMPSENSE_ADDR, T_START, DEVICE_DATA, 0);
 del1m(1000); // wait 1000 ms for first temperature to be ready
}

// Stop converting
void stopTemp(void) {
  writeI2C(TEMPSENSE_ADDR, T_STOP, DEVICE_DATA, 0);
}

// Return the temperature
unsigned char getTemp(void) {
  readI2C(TEMPSENSE_ADDR, T_TEMP, DEVICE_DATA, 2);
  return DEVICE_DATA[0];
}

// Set the high and low registers based on some margin
void setTemp(unsigned char temp) {
  DEVICE_DATA[0] = temp -  T_PLUSMINUS;
  DEVICE_DATA[1] = 0x00;
  writeI2C(TEMPSENSE_ADDR, T_TL, DEVICE_DATA, 2);
  del1m(20);
  
  DEVICE_DATA[0] = temp + T_PLUSMINUS;
  writeI2C(TEMPSENSE_ADDR, T_TH, DEVICE_DATA, 2);
  del1m(20);
}

// Real Time Clock ----------------------------------

// Set the first two configuration bytes for the clock
void configureClock(void) {
  DEVICE_DATA[0] = C_SETTING;
  writeI2C(RTC_ADDR, C_CONTROL, DEVICE_DATA, 1);
}

// Set the time from the array to the clock
void setTime(void) {
  clearChBit();
  writeI2C(RTC_ADDR, C_SECONDS, RTC_TIME, 7);
}

// Set the time from the array to the clock
// Use only seconds, minutes, hours, and day of the week
void setTimeMinimal(void) {
  clearChBit();
  writeI2C(RTC_ADDR, C_SECONDS, RTC_TIME, 4);
}

// Read the time from the clock into the array
void getTime(void) {
  readI2C(RTC_ADDR, C_SECONDS, RTC_TIME, 7);
}

void getTimeMinimal(void) {
  readI2C(RTC_ADDR, C_SECONDS, RTC_TIME, 4);
}

// Clear the Clock Halt bit 
void clearChBit(void) {
  RTC_TIME[0] = RTC_TIME[0] & 0x7F;
}

// Port Expander ------------------------------------

// Setup the port expander initially
void initPortX(void) {
  PORTX_DATA = P_INIT;
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Write to the port expander
void setPortX(void) {
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Turn specific bits on
void setBitsPortX(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA | bitmask; 
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Turn specific bits off
void clearBitsPortX(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA & ~bitmask;
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}
