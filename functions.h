#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void initADC(void);
unsigned char convertAD(unsigned char);

// Input must be from 0 to 99
unsigned char convertDecToBCD(unsigned char);

#endif
