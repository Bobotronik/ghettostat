#ifndef I2C_H
#define I2C_H

#include "derivative.h"

#define DUMMY_BYTE 0xff

void i2c_init(unsigned char bdiv);
void i2c_write(unsigned char, unsigned char, unsigned char *, int);
void i2c_read(unsigned char, unsigned char, unsigned char *, int);

#endif