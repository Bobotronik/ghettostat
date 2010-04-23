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
  unsigned char state, menuState, x, y, menuButtonTouched,
                currentProgramEditing, currentPeriodEditing,
                currentMenuViewing;
  unsigned char tempTime;
  char currentTempMenuViewing;
 
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
            currentProgramEditing = 1;
          }
          else if (isButtonTouched(x, y, program2Button)) {
            drawProgrammingScreen(1);
            state = PROGRAMMING_SCREEN;
            currentProgramEditing = 2;
          }
          else if (isButtonTouched(x, y, program3Button)) {
            drawProgrammingScreen(2);
            state = PROGRAMMING_SCREEN;
            currentProgramEditing = 3;
          }
          else if (isButtonTouched(x, y, program4Button)) {
            drawProgrammingScreen(3);
            state = PROGRAMMING_SCREEN;
            currentProgramEditing = 4;
          }
          break;
        
        case PROGRAMMING_SCREEN:
          if (isButtonTouched(x, y, homeButton)) {
            drawMainScreen();
            state = MAIN;
            menuState = NONE;
            currentProgramEditing = 0;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            drawSettingsScreen();
            state = SETTINGS; 
            menuState = NONE; 
            currentProgramEditing = 0;        
          } 
          else if (isButtonTouched(x, y, check)) {
            drawProgramsTab();
            state = PROGRAMS_TAB;
            menuState = NONE;
            currentProgramEditing = 0;
          }
          switch (menuState) {
            case NONE:
              if (isButtonTouched(x, y, hour1Button)) {
                currentPeriodEditing = 1;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }                     
              else if (isButtonTouched(x, y, hour2Button)) {
                currentPeriodEditing = 2;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour3Button)) {
                currentPeriodEditing = 3;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour4Button)) {
                currentPeriodEditing = 4;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, minutes1Button)) {
                currentPeriodEditing = 1;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes2Button)) {
                currentPeriodEditing = 2;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes3Button)) {
                currentPeriodEditing = 3;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes4Button)) {
                currentPeriodEditing = 4;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, amPm1Button)) {
                toggleAmPm(currentProgram, 0);
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm2Button)) {
                toggleAmPm(currentProgram, 1);
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm3Button)) {
                toggleAmPm(currentProgram, 2);
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm4Button)) {
                toggleAmPm(currentProgram, 3);
                drawProgrammingScreen(currentProgram);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, temp1Button)) {
                currentPeriodEditing = 1;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp2Button)) {
                currentPeriodEditing = 2;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp3Button)) {
                currentPeriodEditing = 3;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp4Button)) {
                currentPeriodEditing = 4;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, mode1Button)) {
                currentPeriodEditing = 1;
                drawProgrammingModeMenu(currentProgram);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode2Button)) {
                currentPeriodEditing = 2;
                drawProgrammingModeMenu(currentProgram);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode3Button)) {
                currentPeriodEditing = 3;
                drawProgrammingModeMenu(currentProgram);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode4Button)) {
                currentPeriodEditing = 4;
                drawProgrammingModeMenu(currentProgram);
                menuState = PROGRAM_MODE_MENU;
              }
              break;
            case HOURS_MENU:
              menuButtonTouched = isMenuButtonTouched(x, y, hours);
              if (menuButtonTouched) {
                switch (currentMenuViewing) {
                  case 1:
                    if (menuButtonTouched == 5) {
                      drawHoursMenu(++currentMenuViewing);
                    }
                    else {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing].startTime), menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 2:
                    if (menuButtonTouched == 1)
                      drawHoursMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawHoursMenu(++currentMenuViewing);
                    else {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing].startTime), menuButtonTouched + 3);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 3:
                    if (menuButtonTouched == 1)
                      drawHoursMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawHoursMenu(++currentMenuViewing);
                    else {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing].startTime), menuButtonTouched + 6);
                      menuState = NONE;
                    }
                    break;
                  case 4:
                    if (menuButtonTouched == 1)
                      drawHoursMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 2) {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing].startTime), 11);
                    }
                    else if (menuButtonTouched == 3) {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing].startTime), 12);
                      menuState = NONE;
                    }
                  break;
                }
              }
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
            menuState = NONE;
          }
          else if (isButtonTouched(x, y, settingsButton)) {
            drawSettingsScreen();
            state = SETTINGS; 
            menuState = NONE;         
          }
          else if (isButtonTouched(x, y, programsTab)) {
            drawProgramsTab();
            state = PROGRAMS_TAB;
            menuState = NONE;
          }
          switch (menuState) {
            case NONE:
              if (isButtonTouched(x, y, monButton)) {
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
              break;
            case MONDAY:
              menuButtonTouched = isMenuButtonTouched(x, y, monMenu);
              if (menuButtonTouched) {
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case TUESDAY:
              menuButtonTouched = isMenuButtonTouched(x, y, tueMenu)
              if (menuButtonTouched) {
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case WEDNESDAY:
              menuButtonTouched = isMenuButtonTouched(x, y, wedMenu)
              if (menuButtonTouched) {
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case THURSDAY:
              menuButtonTouched = isMenuButtonTouched(x, y, thuMenu)
              if (menuButtonTouched) {
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case FRIDAY:
              menuButtonTouched = isMenuButtonTouched(x, y, friMenu)
              if (menuButtonTouched) {
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case SATURDAY:
              menuButtonTouched = isMenuButtonTouched(x, y, satMenu)
              if (menuButtonTouched) {
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case SUNDAY:
              menuButtonTouched = isMenuButtonTouched(x, y, sunMenu)
              if (menuButtonTouched) {
                drawDaysTab();
                menuState = NONE;
              }
              break; 
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

