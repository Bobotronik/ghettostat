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
        
          // Left Bar
          if (x <= 42) { // 6*6 + 6
            if (y <= 10); // 2*8 - 6
            // Since already in Main, do nothing
            else if (y <= 48); // 7*8 - 6
            // Program
            else if (y <= 90) { // 12*8 - 6            
              clearScreen();
              drawProgramsScreen();
              state = PROGRAM_PROGRAMS;
            }
            // Settings
            else {
              clearScreen();
              drawSettingsScreen();
              state = SETTINGS;
            }
          } 
          else if (x <= 120); // 20*6 - 6
          
          // Set To
          else if (x <= 180) { // 29*6 + 6
            if (y <= 42); // 6*8 - 6
            else if (y <= 94) { // 11*8 + 6
              // Set To
            }
          }
          
          else if (x <= 198); // 34*6 - 6
          
          // Right Bar
          else {
            if (y <= 14); // 8 + 6
            else if (y <= 58) { // 8*8 - 6
              drawMainRoomMenu(); 
              state = MAIN_ROOM;
            } 
            else if (y <= 98) { // 13*8 - 6
              drawMainFanMenu();
              state = MAIN_FAN;
            } 
            else {
              drawMainModeMenu();
              state = MAIN_MODE;  
            }
          }
          break;
          
        /****END MAIN****/
          
        case PROGRAM_PROGRAMS:
        
          // Left Bar
          if (x <= 42) { // 6*6 + 6
            if (y <= 10); // 2*8 - 8
            // Main
            else if (y <= 48) { // 7*8 - 6
              clearScreen();
              drawMainScreen();
              state = MAIN;  
            }
            else if (y <= 90); // 12*8 - 6
            // Settings
            else {
              clearScreen();
              drawSettingsScreen();
              state = SETTINGS;
            }
          }
          
          // Right Side
          else {
            if (y < = 2); // 8-6
            else if (y <= 32) { // 4*8
              // check programs/days
              if (x <= 66); // 12*6 - 6
              else if (x <= 132) { // 21*6 + 6
                // programs tab
              } 
              else if (x <= 144); // 25*6 -6
              else if (x <= 210) { // 34*6 + 6
                // days tab
                drawDaysScreen();
                state = PROGRAM_DAYS;
              }
            }
            else if (y <= 56); // 7*8  
            else if (y <= 80); // 10*8 
            else if (y <= 104); // 13*8 
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

