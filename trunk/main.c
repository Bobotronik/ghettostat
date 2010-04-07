#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "pins.h"

#include "delay.h"
#include "i2c.h"
#include "lcd.h"
#include "functions.h"

#pragma TRAP_PROC
void thermometerISR(void) {

}


void main(void) {
  unsigned char x, y;
  
  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  
  CONFIG1_COPD = 1;
  
  initI2C();
  initPortX();
  initializeDisplay();
  
  /*
  General Algorightm
  -Check if touched
  -If touched, check state and decide which button was touched
  -Update state and LCD
  
  */
  for(;;) {
    // Poll clock
    getTime();
    //goToText(2, 6);
    //printStr("X:          ");
    //printNum(getX());
    //goToText(2, 7);
    //printNum(getY());
    //printStr("Y:          ");
    // Poll touchscreen
    if (isTouched()){
      x = getX();
      y = getY();
      
      switch (state) {
        case MAIN:
          break;
        case PROGRAM:
          break;
        case SETTINGS:
          break;
      }
    }
    __RESET_WATCHDOG(); /* feeds the dog */    
  } /* loop forever */
  /* please make sure that you never leave main */
}
