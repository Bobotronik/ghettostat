#ifndef PORTX_H
#define PORTX_H

// I2C Address
#define PORTX_ADDR      0x7E

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

// Global data variables
extern unsigned char PORTX_DATA;

// Port expander functions
void initPortX(void);
void setPortX(void);
void setBitsPortX(unsigned char);
void clearBitsPortX(unsigned char);

#endif
