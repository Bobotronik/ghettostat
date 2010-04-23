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
  unsigned char state, menuState, x, y, 
                currentProgram, currentHour, currentMinute, currentTemp;
 
  EnableInterrupts; /* enable interrupts */
  /* include your code here */
 
  CONFIG1_COPD = 1;
 
  initI2C();
  initPortX();
  initializeDisplay();
  initializeThermostat();
  configureTemp();
  startTemp();
  
  drawMainScreen();
  drawSolidButton(homeButton);
  state = MAIN;
  menuState = NONE;
  
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
            menuState = NONE;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            drawSettingsScreen();
            state = SETTINGS;
            menuState = NONE;          
          } 
          else {  
            switch (menuState) {
              case NONE:
                if (isButtonTouched(x, y, setToButton)) {
                  //drawSettingsMenu();
                }
                else if (isButtonTouched(x, y, roomButton)) {
                  drawMainRoomMenu(); 
                  menuState = ROOM_MENU;
                }
                else if (isButtonTouched(x, y, fanButton)) {
                  drawMainFanMenu();
                  menuState = FAN_MENU;
                }
                else if (isButtonTouched(x, y, modeButton)) {
                  drawMainModeMenu();
                  menuState = MAIN_MODE_MENU;
                }
                break;
              case ROOM_MENU:
                if (isHorizontalMenuButtonTouched(x, y, roomMenu)) {
                  drawMainScreen();
                  menuState = NONE;  
                }
                break;
              case FAN_MENU:
                if (isHorizontalMenuButtonTouched(x, y, fanMenu)) {
                  drawMainScreen();
                  menuState = NONE;  
                }
                break;
              case MAIN_MODE_MENU:
                if (isHorizontalMenuButtonTouched(x, y, modeMenu)) {
                  drawMainScreen();
                  menuState = NONE;  
                }
                break;
              }
          }
          /****END MENUSTATE****/
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
            drawProgrammingScreen(0);
            state = PROGRAMMING_SCREEN;
          }
          else if (isButtonTouched(x, y, program2Button)) {
            drawProgrammingScreen(1);
            state = PROGRAMMING_SCREEN;
          }
          else if (isButtonTouched(x, y, program3Button)) {
            drawProgrammingScreen(2);
            state = PROGRAMMING_SCREEN;
          }
          else if (isButtonTouched(x, y, program4Button)) {
            drawProgrammingScreen(3);
            state = PROGRAMMING_SCREEN;
          }
          break;
        
        case PROGRAMMING_SCREEN:
          if (isButtonTouched(x, y, homeButton)) {
            drawMainScreen();
            state = MAIN;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            drawSettingsScreen();
            state = SETTINGS;          
          } 
          else if (isButtonTouched(x, y, check)) {
            drawProgramsTab();
            state = PROGRAMS_TAB;
          }
          switch (menuState) {
            case NONE:
              if (isButtonTouched(x, y, hour1Button)) {
                drawVerticalMenu(hoursMenu);
                menuState = HOURS_MENU;
              }                     
              else if (isButtonTouched(x, y, hour2Button)) {
                drawVerticalMenu(hoursMenu);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour3Button)) {
                drawVerticalMenu(hoursMenu);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour4Button)) {
                drawVerticalMenu(hoursMenu);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, minutes1Button)) {
                drawVerticalMenu(minutesMenu);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes2Button)) {
                drawVerticalMenu(minutesMenu);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes3Button)) {
                drawVerticalMenu(minutesMenu);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes4Button)) {
                drawVerticalMenu(minutesMenu);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, amPm1Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm2Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm3Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm4Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, temp1Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, temp2Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, temp3Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, temp1Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, mode1Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, mode2Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, mode3Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, mode4Button)) {
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              break;
            case HOURS_MENU:
              break;
            case MINUTES_MENU:
              break;
            case TEMP_MENU:
              break;
            case PROGRAM_MODE_MENU;
              break;
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
                drawDaysMenu(monMenu);
                menuState = MONDAY;
              }
              else if (isButtonTouched(x, y, tueButton)) {
                drawDaysMenu(tueMenu);
                menuState = TUESDAY;
              }
              else if (isButtonTouched(x, y, wedButton)) {
                drawDaysMenu(wedMenu);
                menuState = WEDNESDAY;
              }
              else if (isButtonTouched(x, y, thuButton)) {
                drawDaysMenu(thuMenu);
                menuState = THURSDAY;
              }
              else if (isButtonTouched(x, y, friButton)) {
                drawDaysMenu(friMenu);
                menuState = FRIDAY;
              }
              else if (isButtonTouched(x, y, satButton)) {
                drawDaysMenu(satMenu);
                menuState = SATURDAY;
              }
              else if (isButtonTouched(x, y, sunButton)) {
                drawDaysMenu(sunMenu);
                menuState = SUNDAY;
              }
              /*
              break;
            case MONDAY:
              break;
            case TUESDAY:
              break;
            case WEDNESDAY:
              break;
            case THURSDAY:
              break;
            case FRIDAY:
              break;
            case SATURDAY:
              break;
            case SUNDAY:
              break; 
          } */
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

