#ifndef DEVICES_H
#define DEVICES_H

#include "derivative.h"
#include "i2c.h"
#include "delay.h"

// I2C Addresses
#define TEMPSENSE_ADDR  0x9E
#define RTC_ADDR        0xA2
#define PORTX_ADDR      0x7E

// Temperature Sensor Commands
#define T_TEMP          0xAA
#define T_COUNTER       0xA8   
#define T_SLOPE         0xA9
#define T_START         0xEE
#define T_STOP          0x22
#define T_TH            0xA1
#define T_TL            0xA2
#define T_CONFIG        0xAC

// Temperature Sensor Setting
#define T_SETTING       0x02
#define T_PLUSMINUS     1

// Port Expander Initial Setting
#define P_INIT          0x18  // LCD reset off, font select to 6*8          

// Port Expander Bit Masks
#define P_HEAT          0x01
#define P_FAN           0x02
#define P_COOL          0x04
#define P_RESET         0x08
#define P_FONTSEL       0x10
#define P_6             0x20
#define P_7             0x40
#define P_8             0x80

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

extern unsigned char DEVICE_DATA[2];
extern unsigned char DEVICE_TIME[7];
extern unsigned char DEVICE_ALRM[4];
extern unsigned char PORTX_DATA;

void tempConfig(void);
void tempSet(unsigned char temp);
void tempStart(void);
void tempStop(void);
unsigned char tempGet(void);

void portxInit(void);
void portxSet(void);
void portxOn(unsigned char);
void portxOff(unsigned char);

void clockConfig(void);
void clockClearAI(void);
void clockClearTI(void);
void clockSetTime(void);
void clockGetTime(void);
void clockSetAlrm(void);

#endif
