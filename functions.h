#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Delay Function
void del1m(int);

// Analog to Digital Conversions
void initADC(void);
unsigned char convertAD(unsigned char);

// Input must be from 0 to 99
unsigned char convertDecToBCD(unsigned char);

// Temperature Conversions
unsigned char convertCtoF(unsigned char *);
void convertFtoC(unsigned char, unsigned char *);

unsigned char mapToXPixel(unsigned char);
unsigned char mapToYPixel(unsigned char);
char compare(unsigned char*, unsigned char*);

#endif
