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
     
      switch (state) {
        case MAIN:
          if (isButtonTouched(x, y, programButton)) {
            drawProgramsTab();
            state = PROGRAMS_TAB;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
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
            drawMainScreen();
            state = MAIN;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            drawSettingsScreen();
            state = SETTINGS;          
          }
          else if (isButtonTouched(x, y, daysTab)) {
            drawDaysTab();
            state = DAYS_TAB;
          }
          else if (isButtonTouched(x, y, program1Button)) {
            drawProgrammingScreen();
          }
          else if (isButtonTouched(x, y, program2Button)) {
            drawProgrammingScreen();
          }
          else if (isButtonTouched(x, y, program3Button)) {
            drawProgrammingScreen();
          }
          else if (isButtonTouched(x, y, program4Button)) {
            drawProgrammingScreen();
          }
          break;
          
        case DAYS_TAB:  
          if (isButtonTouched(x, y, homeButton)) {
            drawMainScreen();
            state = MAIN;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            drawSettingsScreen();
            state = SETTINGS;          
          }
          else if (isButtonTouched(x, y, programsTab)) {
            drawProgramsTab();
            state = PROGRAMS_TAB;
          }
          else if (isButtonTouched(x, y, monButton)) {
            drawDaysMenu(monButton);
            menuState = MONDAY;
          }
          else if (isButtonTouched(x, y, tueButton)) {
            drawDaysMenu(tueButton);
            menuState = TUESDAY;
          }
          else if (isButtonTouched(x, y, wedButton)) {
            drawDaysMenu(wedButton);
            menuState = WEDNESDAY;
          }
          else if (isButtonTouched(x, y, thuButton)) {
            drawDaysMenu(thuButton);
            menuState = THURSDAY;
          }
          else if (isButtonTouched(x, y, friButton)) {
            drawDaysMenu(friButton);
            menuState = FRIDAY;
          }
          else if (isButtonTouched(x, y, satButton)) {
            drawDaysMenu(satButton);
            menuState = SATURDAY;
          }
          else if (isButtonTouched(x, y, sunButton)) {
            drawDaysMenu(sunButton);
            menuState = SUNDAY;
          }
          break;
          
        case SETTINGS:
          if (isButtonTouched(x, y, homeButton)) {
            drawMainScreen();
            state = MAIN;
          }
          else if (isButtonTouched(x, y, programButton)) {
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

