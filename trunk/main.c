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
  unsigned int justTouched = 50;
  
  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  
  CONFIG1_COPD = 1;
  
  initI2C();
  initPortX();
  initializeDisplay();
  
  drawMainScreen();
  /*
  General Algorightm
  -Check if touched
  -If touched, check state and decide which button was touched
  -Update state and LCD
  
  */
  del1m(2000);
  state = MAIN;
  for(;;) {
    // Poll clock
    
    // Poll touchscreen
    if (isTouched()){
      if (justTouched) {
           
      } 
      else { 
        justTouched = 15; 
             
        x = getX();
        y = getY();
        x = mapToXPixel(x);
        y = mapToYPixel(y);
        
        setPixel(x, y, 1);
        
        switch (state) {
          case MAIN:
            if (x <= 60) {
              if (y <= 32) {
                // Nothing
              }
              else if (y <= 56) {
                // Mode  
              }
              else if (y <= 80) {
                // Program
                clearScreen();
                drawProgramScreen();
                state = PROGRAM;
              }
              else if (y <= 104) {
                // Settings
                clearScreen();
                drawSettingsScreen();
                state = SETTINGS;
              }
            }
            break;
          case PROGRAM:
            if (x <= 60) {
              if (y <= 32) {
                break; 
              }
              else if (y <= 56) {
                // Main  
                clearScreen();
                drawMainScreen();
                state = MAIN;
              }
              else if (y <= 80) {
                // Programs
              }
              else if (y <= 104) {
                // Days
              }
            }
            break;
          case SETTINGS:
            break;
        }
      }
    }
    if (justTouched) {
      justTouched--;
    }
    __RESET_WATCHDOG(); /* feeds the dog */   
  } /* loop forever */
  /* please make sure that you never leave main */
}
