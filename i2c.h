#ifndef I2C_H
#define I2C_H

#define DUMMY_BYTE    0xff
#define BAUD_DIVISOR  0x02

void initI2C(void);
void writeI2C(unsigned char, unsigned char, unsigned char *, unsigned char);
void readI2C(unsigned char, unsigned char, unsigned char *, unsigned char);

#endif
