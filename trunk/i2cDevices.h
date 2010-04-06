#ifndef I2CDEVICES_H
#define I2CDEVICES_H

// Temperature Sensor -------------------------------

// I2C Address
#define TEMPSENSE_ADDR  0x9E

// Registers
#define T_TEMP          0xAA
#define T_COUNTER       0xA8   
#define T_SLOPE         0xA9
#define T_TH            0xA1
#define T_TL            0xA2
#define T_CONFIG        0xAC

// Commands
#define T_START         0xEE
#define T_STOP          0x22

// Settings
#define T_SETTING       0x02
#define T_PLUSMINUS     1  

// Real Time Clock ----------------------------------

// I2C Address
#define RTC_ADDR        0xD0

// Registers
#define C_SECONDS       0x00
#define C_MINUTES       0x01
#define C_HOURS         0x02
#define C_DAY           0x03
#define C_DATE          0x04
#define C_MONTHS        0x05
#define C_YEARS         0x06
#define C_CONTROL       0x07
#define C_RAMSTART      0x08
#define C_RAMEND        0x3F

// Settings
#define C_SETTING       0x00 //not really important

// Port Expander ------------------------------------

// I2C Address
#define PORTX_ADDR      0x7E

// Bit Masks
#define P_COOL          0x01
#define P_FAN           0x02
#define P_HEAT          0x04
#define P_RESET         0x08
#define P_FONTSEL       0x10
#define P_6             0x20
#define P_7             0x40
#define P_8             0x80

// Initial Settings
#define P_INIT          0x18  // LCD reset off, font select to 6*8

// Global Variables ---------------------------------

// General I2C Data
extern unsigned char DEVICE_DATA[2];
// Real Time Clock
extern unsigned char RTC_TIME[7];
// Port Expander
extern unsigned char PORTX_DATA;

// Function Prototypes ------------------------------

// Temperature Sensor
void configureTemp(void);
void startTemp(void);
void stopTemp(void);
void getTempC(unsigned char *);
unsigned char getTempF(void);
void setTempC(unsigned char *);
void setTempF(unsigned char);
void setTempPolarity(void);
void clearTempPolarity(void);

// Real Time Clock
void configureClock(void);
void setTime(void);
void setTimeMinimal(void);
void getTime(void);
void getTimeMinimal(void);
void clearChBit(void);

// Port Expander
void initPortX(void);
void setPortX(void);
void setBitsPortX(unsigned char);
void clearBitsPortX(unsigned char);


#endif
