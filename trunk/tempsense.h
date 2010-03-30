#ifndef TEMPSENSE_H
#define TEMPSENSE_H

// I2C Address
#define TEMPSENSE_ADDR  0x9E

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

// Temperature sensor functions
void configureTemp(void);
void startTemp(void);
void stopTemp(void);
unsigned char getTemp(void);
void setTemp(unsigned char);

#endif
