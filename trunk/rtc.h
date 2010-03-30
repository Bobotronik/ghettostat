#ifndef RTC_H
#define RTC_H

// I2C Address
#define RTC_ADDR    0xA2

// Real Time Clock Registers
#define C_CONTROL1      0x00
#define C_CONTROL2      0x01
#define C_SECONDS       0x02
#define C_MINUTES       0x03
#define C_HOURS         0x04
#define C_DAYS          0x05
#define C_WEEKDAYS      0x06
#define C_MONTHS        0x07
#define C_YEARS         0x08
#define C_ALARM_MINUTE  0x09
#define C_ALARM_HOUR    0x0A
#define C_ALARM_DAY     0x0B
#define C_ALARM_WEEKDAY 0x0C
#define C_CTRL_CLKOUT   0x0D
#define C_CTRL_TIMER    0x0E
#define C_TIMER         0x0F

// Real Time Clock Settings
#define C_SETTING1      0x00
#define C_SETTING2      0x02 // Alarm interrupts enabled

// Global Data Variables
extern unsigned char DEVICE_TIME[7];
extern unsigned char DEVICE_ALRM[4];

// Real time clock functions
void configureClock(void);
void clearAI(void);
void clearTI(void);
void setAlrm(void);
void getTime(void);
void setTime(void);

#endif
