#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "pins.h"

#include "delay.h"
#include "i2c.h"
#include "lcd.h"
#include "functions.h"
#include "thermostat.h"

#pragma TRAP_PROC
void dummyISR(void) {

}


void main(void) {
  unsigned char state, x, y;
  
  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  
  CONFIG1_COPD = 1;
  
  initI2C();
  initPortX();
  initializeDisplay();
  
  //drawMainScreen();
  /*
  General Algorightm
  -Check if touched
  -If touched, check state and decide which button was touched
  -Update state and LCD
  
  */
  state = MAIN;
  for(;;) {
    // Poll clock
    //getTime();
    //clearText();
    //goToText(20, 6);
    //printStr("X:          ");
    //printNum(getX());
    //goToText(20, 7);
    //printNum(getY());
    //printStr("Y:          ");
    // Poll touchscreen
    if (isTouched()){
      x = getX();
      y = getY();
      x = mapToXPixel(x);
      y = mapToYPixel(y);
      
      setPixel(x, y, 1);
      switch (state) {
        case MAIN:
          if (x <= 42) {
            if (y <= 16) {
              break; 
            }
            else if (y <= 12) {
                
            }
          }
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
