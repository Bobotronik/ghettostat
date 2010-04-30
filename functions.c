#include "derivative.h"
#include "functions.h"

#include "pins.h"
#include "lcd.h"

#pragma DATA_SEG __SHORT_SEG MY_ZEROPAGE
unsigned char delay0;
unsigned char delay1;
#pragma DATA_SEG DEFAULT

// del1m - Delay about 1msec times the "d" argument.
void del1m(int d) {
  while (d--) {
    asm {
      ; The following code delays 1 millisecond by looping.
      ; Total time is 4 + 5 * (4 + 68 * (4 + 3) + 4 + 3) = 2439 cycles
      ; A 9.8304MHz external clock gives an internal CPU clock of
      ; 2.4576MHz (407ns/cycle).  Delay is then .993 milliseconds.
              mov     #$5,delay1  ; 4 cycles
      m1:     mov     #68,delay0  ; 4 cycles
      m0:     dec     delay0      ; 4 cycles
              bne     m0          ; 3 cycles
              dec     delay1      ; 4 cycles
              bne     m1          ; 3 cycles
    }
  }
}
         
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

// Convert Celsius to Fahrenheit
unsigned char convertCtoF(unsigned char * C) {
  unsigned int F;
  unsigned char temp;
  
  C[1] = C[1] >> 7;
  F = (unsigned int) C[0]*90/5 + 9*C[1] + 320;
  temp = F % 10;
  if( temp < 5 )
    temp = 0;
  else
    temp = 1;
  F = F/10 + temp;
  return (unsigned char) F;
}

// Convert Fahrenheit to Celsius
void convertFtoC(unsigned char F, unsigned char * C) {
  unsigned int Ctemp;
  unsigned char temp;
  
  Ctemp = ((unsigned int) F - 32)*50/9;  
  temp = Ctemp % 10;
  C[0] = Ctemp / 10;
  if( temp < 3 ) {
    C[1] = 0;
  } else if( temp > 6 ) {
    C[1] = 0;
    C[0]++;
  } else {
    C[1] = 0x80;
  }
}

unsigned char mapToXPixel(unsigned char data) {
  unsigned char pixel;
  pixel = (data - MIN_X)*LCD_WIDTH/X_DIFFERENCE;
  return pixel;
}

unsigned char mapToYPixel(unsigned char data) {
  unsigned char pixel;
  pixel = (data - MIN_Y)*LCD_HEIGHT/Y_DIFFERENCE; 
  return pixel; 
}

char compare(unsigned char* a, unsigned char* b) {
  return *a - *b;
}