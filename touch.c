#include "derivative.h"
#include "pins.h"
#include "touch.h"
#include "adc.h"

unsigned char isTouched(){
  if (getX() < MIN_TOUCH && getY() < MIN_TOUCH)
    return 0;
  return 1;
}

unsigned char getX(){
  unsigned char command;
  
  TS_LEFT_DIR = 1;
  TS_RIGHT_DIR = 1;
  TS_TOP_DIR = 0;
  TS_BOTTOM_DIR = 0;
  
  TS_LEFT = 1;
  TS_RIGHT = 0;
   
  //ADCSC = 0x08;
  //while(ADCSC_COCO == 0);
  
  return convertADC(TS_X_INPUT);
}

unsigned char getY(){
  TS_TOP_DIR = 1;
  TS_BOTTOM_DIR = 1;
  TS_LEFT_DIR = 0;
  TS_RIGHT_DIR = 0;
  
  TS_BOTTOM = 1;
  TS_TOP = 0;
  
  //ADCSC = 0x09; 
  //while(ADCSC_COCO == 0);
  
  return convertADC(TS_Y_INPUT);
}

void initializeTS(){
  // Put ADC high speed mode, input clock / 1, internal clock, 8-bit mode,
  // short sample time, synchronous clock
  ADCLK = 0x10;
}
