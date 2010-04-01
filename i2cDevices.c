#include "derivative.h"
#include "i2c.h"

unsigned char PORTX_DATA;
unsigned char DEVICE_TIME[7];
unsigned char DEVICE_ALARM[4];

/**** Port Expander ****/

void initPortX(void) {
  writeI2C(PORTX_ADDR, 0x00, DEVICE_DATA, 0);
}

// Turn specific bits on
void setBitsPortX(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA | bitmask; 
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Turn specitif bits off
void clearBitsPortX(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA & ~bitmask;
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

/**** Real Time Clock ****/

// Set the first two configuration bytes for the clock
void configureClock(void) {
  DEVICE_DATA[0] = C_SETTING1;
  DEVICE_DATA[1] = C_SETTING2;
  writeI2C(RTC_ADDR, C_CONTROL1, DEVICE_DATA, 2);
}

// Clear the alarm interrupt
void clearAI(void) {
  DEVICE_DATA[0] = C_SETTING2 & 0xF7;
  writeI2C(RTC_ADDR, C_CONTROL2, DEVICE_DATA, 1);
}

// Clear the timer interrupt
void clearTI(void) {
  DEVICE_DATA[0] = C_SETTING2 & 0xFB;
  writeI2C(RTC_ADDR, C_CONTROL2, DEVICE_DATA, 1);
}

// Set the time from the array to the clock
void setTime(void) {
  writeI2C(RTC_ADDR, C_SECONDS, DEVICE_TIME, 7);
}

// Read the time from the clock into the array
void getTime(void) {
  readI2C(RTC_ADDR, C_SECONDS, DEVICE_TIME, 7);
}

// Set the alarms from the array to the clock
void setAlarm(void) {
  writeI2C(RTC_ADDR, C_ALARM_MINUTE, DEVICE_ALARM, 4);
}

/**** Temperature Sensor ****/

// Write the configuration byte to the temperature sensor
void configureTemp(void) {
  DEVICE_DATA[0] = T_SETTING;  
  writeI2C(TEMPSENSE_ADDR, T_CONFIG, DEVICE_DATA,1);
  del1m(20);
}

// Start converting
void startTemp(void) {
 writeI2C(TEMPSENSE_ADDR, T_START, DEVICE_DATA, 0);
 del1m(1000);
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
  DEVICE_DATA[0] = temp - T_MARGIN;
  DEVICE_DATA[1] = 0x00;
  writeI2C(TEMPSENSE_ADDR, T_TL, DEVICE_DATA, 2);
  del1m(20);
  
  DEVICE_DATA[0] = temp + T_MARGIN;
  writeI2C(TEMPSENSE_ADDR, T_TH, DEVICE_DATA, 2);
  del1m(20);
}
