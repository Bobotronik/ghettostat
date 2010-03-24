#include "derivative.h"
#include "pins.h"

unsigned char isTouched(){
  unsigned char
}

unsigned char getX(){
  TS_PIN1_DIR = 1;
  TS_PIN2_DIR = 0;
  TS_PIN4_DIR = 1;
  TS_PIN1 = 1;
  TS_PIN4 = 0;
  return TS_PIN2;
}

unsigned char getY(){
  TS_PIN2_DIR = 1;
  TS_PIN3_DIR = 1;
  TS_PIN4_DIR = 0;
  TS_PIN2 = 1;
  TS_PIN3 = 0;
  return TS_PIN4;
}
