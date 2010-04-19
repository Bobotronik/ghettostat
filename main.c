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
  drawSolidButton(homeButton);
  state = MAIN;
  
  for(;;) {
    // Poll clock
    drawTopBar();
    
    switch (state) {
      case MAIN:
        updateTemp();
        break;
      case PROGRAMS_TAB:
        break;
      case DAYS_TAB:
        break;
      case SETTINGS:
        break;
    }
    // Poll touchscreen
    if (isScreenTouched()) {
             
      x = getX();
      y = getY();
      x = mapToXPixel(x);
      y = mapToYPixel(y);
     
      setPixel(x, y, 1);
     
      switch (state) {
        case MAIN:
          if (isButtonTouched(x, y, programButton)) {
            clearScreen();
            drawProgramsTab();
            state = PROGRAMS_TAB;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            clearScreen();
            drawSettingsScreen();
            state = SETTINGS;          
          }
          else if (isButtonTouched(x, y, setToButton)) {
            //drawSettingsMenu();
          }
          else if (isButtonTouched(x, y, roomButton)) {
            drawMainRoomMenu(); 
            //state = MAIN_ROOM;
          }
          else if (isButtonTouched(x, y, fanButton)) {
            drawMainFanMenu();
            //state = MAIN_FAN;
          }
          else if (isButtonTouched(x, y, modeButton)) {
            drawMainModeMenu();
            //state = MAIN_MODE;
          }
          break;
          
        /****END MAIN****/
          
        case PROGRAMS_TAB:
        
          if (isButtonTouched(x, y, homeButton)) {
            clearScreen();
            drawMainScreen();
            state = MAIN;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            clearScreen();
            drawSettingsScreen();
            state = SETTINGS;          
          }
          else if (isButtonTouched(x, y, daysTab)) {
            clearScreen();
            drawDaysTab();
            state = DAYS_TAB;
          }
          else if (isButtonTouched(x, y, program1Button)) {
            clearScreen();
            drawProgrammingScreen();
          }
          else if (isButtonTouched(x, y, program2Button)) {
            clearScreen();
            drawProgrammingScreen();
          }
          else if (isButtonTouched(x, y, program3Button)) {
            clearScreen();
            drawProgrammingScreen();
          }
          else if (isButtonTouched(x, y, program4Button)) {
            clearScreen();
            drawProgrammingScreen();
          }
          break;
          
        case DAYS_TAB:  
          if (isButtonTouched(x, y, homeButton)) {
            clearScreen();
            drawMainScreen();
            state = MAIN;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            clearScreen();
            drawSettingsScreen();
            state = SETTINGS;          
          }
          else if (isButtonTouched(x, y, programsTab)) {
            clearScreen();
            drawProgramsTab();
            state = PROGRAMS_TAB;
          }
          else if (isButtonTouched(x, y, monButton)) {
            
          }
          else if (isButtonTouched(x, y, tueButton)) {
            
          }
          else if (isButtonTouched(x, y, wedButton)) {
            
          }
          else if (isButtonTouched(x, y, thuButton)) {
            
          }
          else if (isButtonTouched(x, y, friButton)) {
            
          }
          else if (isButtonTouched(x, y, satButton)) {
            
          }
          else if (isButtonTouched(x, y, sunButton)) {
            
          }
          break;
          
        case SETTINGS:
          if (isButtonTouched(x, y, homeButton)) {
            clearScreen();
            drawMainScreen();
            state = MAIN;
          }
          else if (isButtonTouched(x, y, programButton)) {
            clearScreen();
            drawProgramsTab();
            state = PROGRAMS_TAB;          
          }
          break;
      }    
    }
    __RESET_WATCHDOG(); /* feeds the dog */  
  } /* loop forever */
  /* please make sure that you never leave main */
}

