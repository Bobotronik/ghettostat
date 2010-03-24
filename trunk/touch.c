#include "derivative.h"
#include "pins.h"
#include "touch.h"

unsigned char isTouched(){
  if (getX() < MIN_TOUCH && getY() < MIN_TOUCH)
    return 0;
  return 1;
}

unsigned char getX(){
  unsigned char command;
  
  TS_PIN2_DIR = 1;
  TS_PIN3_DIR = 0;
  TS_PIN4_DIR = 1;
  TS_PIN2 = 1;
  TS_PIN4 = 0;
   
  ADCSC = 0x08;
  while(ADCSC_COCO == 0);
  
  return ADRL;
}

unsigned char getY(){
  TS_PIN1_DIR = 1;
  TS_PIN2_DIR = 0;
  TS_PIN3_DIR = 1;
  TS_PIN1 = 1;
  TS_PIN3 = 0;
  
  ADCSC = 0x09; 
  while(ADCSC_COCO == 0);
  
  return ADRL;
}

void initializeTS(){
  // Put ADC high speed mode, input clock / 1, internal clock, 8-bit mode,
  // short sample time, synchronous clock
  ADCLK = 0x10;
}
