#ifndef INT_H
#define INT_H

#define I_OFF       0x00 
#define I_HEAT      0x01
#define I_COOL      0x02
#define I_FAN       0x04

extern unsigned char mode;
extern unsigned char status;

void tempISRAux(void);
void tempISRMain(void);
void setModeAux(unsigned char *);
void setModeMain(unsigned char tempF, unsigned char mode1);

#endif