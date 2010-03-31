#include "derivative.h"
#include "rtc.h"

#include "i2c.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char DEVICE_TIME[7];
unsigned char DEVICE_ALARM[4];

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
