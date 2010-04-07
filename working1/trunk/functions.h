#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Analog to Digital Conversions
void initADC(void);
unsigned char convertAD(unsigned char);

// Input must be from 0 to 99
unsigned char convertDecToBCD(unsigned char);

// Temperature Conversions
unsigned char convertCtoF(unsigned char *);
void convertFtoC(unsigned char, unsigned char *);

#endif
