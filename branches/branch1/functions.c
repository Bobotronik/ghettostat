#include "derivative.h"
#include "functions.h"

#include "pins.h"

void initADC(void) {
  ADCLK_ACLKEN =  0; // Internal bus clock
  ADCLK_ADLSMP =  1; // Long sample time
  ADCLK_MODE =    0; // 8 bit mode
  ADCLK_ADICLK =  1; // Internal bus clock
  ADCLK_ADIV =    1; // Divide clock by 2 
  ADCLK_ADLPC =   0; // High speed
}

unsigned char convertAD(unsigned char channel) {
  ADCSC = 0x00 | channel; // Interrupt disabled, one conversion
  while(ADCSC_COCO == 0); // Wait until conversion complete
  return ADRL;
}

// Input must be from 0 to 99
unsigned char convertDecToBCD(unsigned char value) {
  unsigned char result;
  result = value % 10;
  result += ((value - result)/10 << 4);
  return result;  // Result is BCD
}
