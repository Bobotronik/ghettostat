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
                currentMenuViewing = 0;
  unsigned char tempTime;
  char currentTempMenuViewing;
  
  unsigned char released = 1; // Semaphore
  unsigned char touched = 0;
  
  extern struct program programs[NUM_PROGRAMS]; // For programs Tab
  extern unsigned char weeklySchedule[]; // For days tab
  
  EnableInterrupts; /* enable interrupts */
  /* include your code here */
 
  CONFIG1_COPD = 1;
 
  initI2C();
  initPortX();
  configureTemp();
  startTemp();
  initializeDisplay();
  initializeThermostat();
  
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
    if (!isScreenTouched()) {
      released = 1;
    } 
    
    if (isScreenTouched() && released) {
      released = 0;    
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
                menuButtonTouched = isHorizontalMenuButtonTouched(x, y, roomMenu);
                switch (menuButtonTouched) {
                  case 1:
                    currentRoom = ROOM_MAIN;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                  case 2:
                    currentRoom = ROOM_AUX;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                }
                break;
              case FAN_MENU:
                menuButtonTouched = isHorizontalMenuButtonTouched(x, y, roomMenu);
                switch (menuButtonTouched) {
                  case 1:
                    fanMode = FAN_ON;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                  case 2:
                    fanMode = FAN_OFF;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                  case 3:
                    fanMode = FAN_AUTO;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                }
                break;
              // Will cause manual override
              case MAIN_MODE_MENU:
                menuButtonTouched = isHorizontalMenuButtonTouched(x, y, roomMenu);
                switch (menuButtonTouched) {
                  case 1:
                    isOverride = 1;
                    fanMode = FAN_ON;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                  case 2:
                    isOverride = 1;
                    fanMode = FAN_OFF;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                  case 3:
                    isOverride = 1;
                    fanMode = FAN_AUTO;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                }
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
            currentProgramEditing = 0;
          }
          else if (isButtonTouched(x, y, program2Button)) {
            drawProgrammingScreen(1);
            state = PROGRAMMING_SCREEN;
            currentProgramEditing = 1;
          }
          else if (isButtonTouched(x, y, program3Button)) {
            drawProgrammingScreen(2);
            state = PROGRAMMING_SCREEN;
            currentProgramEditing = 2;
          }
          else if (isButtonTouched(x, y, program4Button)) {
            drawProgrammingScreen(3);
            state = PROGRAMMING_SCREEN;
            currentProgramEditing = 3;
          }
          break;
        
        case PROGRAMMING_SCREEN:
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
          else if (isButtonTouched(x, y, okButton)) {
            drawProgramsTab();
            state = PROGRAMS_TAB;
            menuState = NONE;
          }
          switch (menuState) {
            case NONE:
              if (isButtonTouched(x, y, hour1Button)) {
                currentPeriodEditing = 1;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }                     
              else if (isButtonTouched(x, y, hour2Button)) {
                currentPeriodEditing = 2;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour3Button)) {
                currentPeriodEditing = 3;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour4Button)) {
                currentPeriodEditing = 4;
                currentMenuViewing = determineHoursMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, minutes1Button)) {
                currentPeriodEditing = 1;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes2Button)) {
                currentPeriodEditing = 2;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes3Button)) {
                currentPeriodEditing = 3;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes4Button)) {
                currentPeriodEditing = 4;
                drawMinutesMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, amPm1Button)) {
                toggleAmPm(currentProgramEditing, 0);
                drawProgrammingScreen(currentProgramEditing);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm2Button)) {
                toggleAmPm(currentProgramEditing, 1);
                drawProgrammingScreen(currentProgramEditing);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm3Button)) {
                toggleAmPm(currentProgramEditing, 2);
                drawProgrammingScreen(currentProgramEditing);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, amPm4Button)) {
                toggleAmPm(currentProgramEditing, 3);
                drawProgrammingScreen(currentProgramEditing);
                menuState = NONE;
              }
              else if (isButtonTouched(x, y, temp1Button)) {
                currentPeriodEditing = 1;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp2Button)) {
                currentPeriodEditing = 2;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp3Button)) {
                currentPeriodEditing = 3;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp4Button)) {
                currentPeriodEditing = 4;
                currentMenuViewing = determineTempMenu(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, mode1Button)) {
                currentPeriodEditing = 1;
                drawProgrammingModeMenu(currentPeriodEditing);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode2Button)) {
                currentPeriodEditing = 2;
                drawProgrammingModeMenu(currentPeriodEditing);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode3Button)) {
                currentPeriodEditing = 3;
                drawProgrammingModeMenu(currentPeriodEditing);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode4Button)) {
                currentPeriodEditing = 4;
                drawProgrammingModeMenu(currentPeriodEditing);
                menuState = PROGRAM_MODE_MENU;
              }
              break;
            case HOURS_MENU:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, hoursMenu);
              if (menuButtonTouched) {
                switch (currentMenuViewing) {
                  case 1:
                    if (menuButtonTouched == 5) {
                      drawHoursMenu(++currentMenuViewing);
                    }
                    else {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime), menuButtonTouched);
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
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime), menuButtonTouched + 3);
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
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime), menuButtonTouched + 6);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 4:
                    if (menuButtonTouched == 1)
                      drawHoursMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 2) {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime), 11);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    else if (menuButtonTouched == 3) {
                      updateHours(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime), 12);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                }
              }
              break;
            case MINUTES_MENU:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, minutesMenu);
              if(menuButtonTouched) {
                updateMinutes(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].startTime), menuButtonTouched - 1);
                drawProgrammingScreen(currentProgramEditing);
                menuState = NONE;
              }
              break;
            case TEMP_MENU:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, programmingTempMenu);
              if(menuButtonTouched) {
                switch (currentMenuViewing) {
                  case 1:
                    if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 59 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 2:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 62 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 3:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 65 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);                                                
                      menuState = NONE;
                    }
                    break;
                  case 4:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 68 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing - 1);
                      menuState = NONE;
                    }
                    break;
                  case 5:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 71 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing - 1);
                      menuState = NONE;
                    }
                    break;
                  case 6:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 74 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing - 1);
                      menuState = NONE;
                    }
                    break;
                  case 7:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 77 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing - 1);
                      menuState = NONE;
                    }
                    break;
                  case 8:
                    if (menuButtonTouched == 1) {
                      drawProgrammingTempMenu(--currentMenuViewing);
                    }
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 80 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing - 1);
                      menuState = NONE;
                    }
                    break;
                  case 9:
                    if (menuButtonTouched == 1) {
                      drawProgrammingTempMenu(--currentMenuViewing);
                    }
                    else {
                      updateProgrammingTemp(&(programs[currentProgramEditing].periods[currentPeriodEditing - 1].temperature), 83 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing - 1);
                      menuState = NONE;
                    }
                    break;
                }
              }
              break;
            case PROGRAM_MODE_MENU:
              programmingModeMenu[1] = (1 + 3*currentPeriodEditing);
              menuButtonTouched = isHorizontalMenuButtonTouched(x, y, programmingModeMenu);
              switch (menuButtonTouched) {
                case 1:
                  programs[currentProgramEditing].periods[currentPeriodEditing - 1].setting = HEAT;
                  drawProgrammingScreen(currentProgramEditing);
                  menuState = NONE;
                  break;
                case 2:
                  programs[currentProgramEditing].periods[currentPeriodEditing - 1].setting = COOL;
                  drawProgrammingScreen(currentProgramEditing);
                  menuState = NONE;
                  break;
                case 3:
                  programs[currentProgramEditing].periods[currentPeriodEditing - 1].setting = OFF;
                  drawProgrammingScreen(currentProgramEditing);
                  menuState = NONE;
                  break;
              }
              programmingModeMenu[1] = 1;
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
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, monMenu);
              if (menuButtonTouched) {
                weeklySchedule[0] = menuButtonTouched - 1;
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case TUESDAY:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, tueMenu);
              if (menuButtonTouched) {
                weeklySchedule[1] = menuButtonTouched - 1;
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case WEDNESDAY:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, wedMenu);
              if (menuButtonTouched) {
                weeklySchedule[2] = menuButtonTouched - 1;
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case THURSDAY:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, thuMenu);
              if (menuButtonTouched) {
                weeklySchedule[3] = menuButtonTouched - 1;
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case FRIDAY:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, friMenu);
              if (menuButtonTouched) {
                weeklySchedule[4] = menuButtonTouched - 1;
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case SATURDAY:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, satMenu);
              if (menuButtonTouched) {
                weeklySchedule[5] = menuButtonTouched - 1;
                drawDaysTab();
                menuState = NONE;
              }
              break;
            case SUNDAY:
              menuButtonTouched = isVerticalMenuButtonTouched(x, y, sunMenu);
              if (menuButtonTouched) {
                weeklySchedule[6] = menuButtonTouched - 1;
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
      refreshThermostat();
    }
    __RESET_WATCHDOG(); /* feeds the dog */  
  } /* loop forever */
  /* please make sure that you never leave main */
}

