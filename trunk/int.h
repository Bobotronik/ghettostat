#ifndef INT_H
#define INT_H

#define OFF       0x00 
#define HEAT      0x01
#define COOL      0x02
#define FAN       0x04

extern unsigned char mode;
extern unsigned char status;

void temp_ISR(void);
void setMode(unsigned char *);

#endif