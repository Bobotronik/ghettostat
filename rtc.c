#include "rtc.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char DEVICE_TIME[7];
unsigned char DEVICE_ALRM[4];

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

// Set the time from the array to the clock
void clockSetTime(void) {
  i2c_write(RTC_ADDR, C_SECONDS, DEVICE_TIME, 7);
}

// Read the time from the clock into the array
void clockGetTime(void) {
  i2c_read(RTC_ADDR, C_SECONDS, DEVICE_TIME, 7);
}

// Set the alarms from the array to the clock
void clockSetAlrm(void) {
  i2c_write(RTC_ADDR, C_ALARM_MINUTE, DEVICE_ALRM, 4);
}