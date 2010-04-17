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
    drawTopBar();
    
    switch (state) {
      case MAIN:
        updateTemp();
        break;
      case PROGRAM_PROGRAMS:
        break;
      case SETTINGS:
        break;
    }
    // Poll touchscreen
    if (isTouched()) {
             
      x = getX();
      y = getY();
      x = mapToXPixel(x);
      y = mapToYPixel(y);
     
      setPixel(x, y, 1);
     
      switch (state) {
        case MAIN:
          // 6*6 + 6
          if (x <= 42) {
            // 2*8 - 8
            if (y <= 10) {
              // Nothing
            }
            // 7*8 - 6
            else if (y <= 48) {
              // Main/Nothing  
            }
            // 12*8 - 6
            else if (y <= 90) {
              // Program
              clearScreen();
              drawProgramsScreen();
              state = PROGRAM_PROGRAMS;
            }
            else {
              // Settings
              clearScreen();
              drawSettingsScreen();
              state = SETTINGS;
            }
          } 
          // 120 - 6
          else if (x <= 114) {
            // Nothing                   
          }

          else {
            // 3*8 - 6
            if (y <= 18) {
              // Nothing  
            }
            // 8*8 - 6
            else if (y <= 58) {
              // Room  
            } 
            // 13*8 - 6
            else if (y <= 98) {
              // Fan  
            } 
            else {
              // Mode    
            }
          }
          
          break;
        case PROGRAM_PROGRAMS:
          // 6*6 + 6
          if (x <= 42) {
            // 2*8 - 8
            if (y <= 10) {
              // Nothing
            }
            // 7*8 - 6
            else if (y <= 48) {
              // Main
              clearScreen();
              drawMainScreen();
              state = MAIN;  
            }
            // 12*8 - 6
            else if (y <= 90) {
              // Program/Nothing
            }
            else {
              // Settings
              clearScreen();
              drawSettingsScreen();
              state = SETTINGS;
            }
          }
          break;
        case SETTINGS:
            // 6*6 + 6
            if (x <= 42) {
              // 2*8 - 8
              if (y <= 10) {
                // Nothing
              }
              // 7*8 - 6
              else if (y <= 48) {
                // Main
                clearScreen();
                drawMainScreen();
                state = MAIN;  
              }
              // 12*8 - 6
              else if (y <= 90) {
                // Program
                clearScreen();
                drawProgramsScreen();
                state = PROGRAM_PROGRAMS;
              }
              else {
                // Settings/Nothing
              }
            }
          break;
      }    
    }
    __RESET_WATCHDOG(); /* feeds the dog */  
  } /* loop forever */
  /* please make sure that you never leave main */
}

