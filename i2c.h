#ifndef I2C_H
#define I2C_H

#define DUMMY_BYTE 0xff

void initI2C(unsigned char bdiv);
void writeI2C(unsigned char, unsigned char, unsigned char *, int);
void readI2C(unsigned char, unsigned char, unsigned char *, int);

#endif
