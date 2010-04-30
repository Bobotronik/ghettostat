#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "pins.h"

#include "flash.h"
#include "functions.h"
#include "i2c.h"
#include "i2cDevices.h"
#include "lcd.h"
#include "functions.h"
#include "thermostat.h"
#include "int.h"

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
  
  extern struct room rooms[2]; // For programs Tab
  extern unsigned char weeklySchedule[]; // For days tab
  
  //EnableInterrupts; /* enable interrupts */
  /* include your code here */
 
  CONFIG1_COPD = 1;
  INTSCR_MODE = 1;
  INTSCR_IMASK = 1;
 
  initI2C();
  initPortX();
  configureTemp();
  startTemp();
  initFlash();
  initializeDisplay();
  
  initializeThermostat(); // Pass this function before enabling interrupts
  
  drawMainScreen();
  drawSolidButton(homeButton);
  state = MAIN;
  menuState = NONE;
  
  //setModeMain(82,I_HEAT);
  EnableInterrupts;
  INTSCR_IMASK = 0;
  
  for(;;) {
    saveProgramData();
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
          if (isOverride[currentRoom]) {
            if (isButtonTouched(x, y, overrideButton)) {
              isOverride[currentRoom] = 0;
              drawMainScreen();
            }
          }
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
                  switch (isOverride[currentRoom]) {
                    case 0:
                      currentMenuViewing = determineTempMenu(currentPeriod[currentRoom].temperature);
                      break;
                    case 1:
                      currentMenuViewing = determineTempMenu(overridePeriod[currentRoom].temperature);
                      break;
                  }
                  goToText(20, 1);
                  printNum(menuButtonTouched);
                  drawMainSetToMenu(currentMenuViewing);
                  menuState = SET_TO_MENU;
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
              case SET_TO_MENU:
                menuButtonTouched = isVerticalMenuButtonTouched(x, y, setToMenu);
                  if(menuButtonTouched) {
                    switch (currentMenuViewing) {
                      case 1:
                        if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 59 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                      case 2:
                        if (menuButtonTouched == 1)
                          drawMainSetToMenu(--currentMenuViewing);
                        else if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 62 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                      case 3:
                        if (menuButtonTouched == 1)
                          drawMainSetToMenu(--currentMenuViewing);
                        else if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 65 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);                                                
                          menuState = NONE;
                        }
                        break;
                      case 4:
                        if (menuButtonTouched == 1)
                          drawMainSetToMenu(--currentMenuViewing);
                        else if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 68 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                      case 5:
                        if (menuButtonTouched == 1)
                          drawMainSetToMenu(--currentMenuViewing);
                        else if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 71 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                      case 6:
                        if (menuButtonTouched == 1)
                          drawMainSetToMenu(--currentMenuViewing);
                        else if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 74 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                      case 7:
                        if (menuButtonTouched == 1)
                          drawMainSetToMenu(--currentMenuViewing);
                        else if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 77 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                      case 8:
                        if (menuButtonTouched == 1) {
                          drawMainSetToMenu(--currentMenuViewing);
                        }
                        else if (menuButtonTouched == 5)
                          drawMainSetToMenu(++currentMenuViewing);
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 80 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                      case 9:
                        if (menuButtonTouched == 1) {
                          drawMainSetToMenu(--currentMenuViewing);
                        }
                        else {
                          updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 83 + menuButtonTouched);
                          drawProgrammingScreen(currentProgramEditing);
                          menuState = NONE;
                        }
                        break;
                    }
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
                menuButtonTouched = isHorizontalMenuButtonTouched(x, y, fanMenu);
                switch (menuButtonTouched) {
                  case 1:
                    fanMode[currentRoom] = FAN_ON;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                  case 2:
                    fanMode[currentRoom] = FAN_OFF;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                  case 3:
                    fanMode[currentRoom] = FAN_AUTO;
                    drawMainScreen();
                    menuState = NONE; 
                    break;
                }
                break;
              // Will cause manual override
              case MAIN_MODE_MENU:
                menuButtonTouched = isHorizontalMenuButtonTouched(x, y, modeMenu);
                switch (menuButtonTouched) {
                  case 1:
                    overridePeriod[currentRoom].mode = HEAT;
                    menuState = NONE; 
                    break;
                  case 2:
                    overridePeriod[currentRoom].mode = COOL;
                    menuState = NONE; 
                    break;
                  case 3:
                    overridePeriod[currentRoom].mode = OFF;
                    menuState = NONE; 
                    break;
                }
                
                if (overridePeriod[currentRoom].mode != currentPeriod[currentRoom].mode) {
                  isOverride[currentRoom] = 1;  
                } 
                else {
                  isOverride[currentRoom] = 0;   
                }  
                
                if (menuButtonTouched) {
                  drawMainScreen(); 
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
                currentPeriodEditing = 0;
                currentMenuViewing = determineHoursMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }                     
              else if (isButtonTouched(x, y, hour2Button)) {
                currentPeriodEditing = 1;
                currentMenuViewing = determineHoursMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour3Button)) {
                currentPeriodEditing = 2;
                currentMenuViewing = determineHoursMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, hour4Button)) {
                currentPeriodEditing = 3;
                currentMenuViewing = determineHoursMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                drawHoursMenu(currentMenuViewing);
                menuState = HOURS_MENU;
              }
              else if (isButtonTouched(x, y, minutes1Button)) {
                currentPeriodEditing = 0;
                drawMinutesMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes2Button)) {
                currentPeriodEditing = 1;
                drawMinutesMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes3Button)) {
                currentPeriodEditing = 2;
                drawMinutesMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
                menuState = MINUTES_MENU;
              }
              else if (isButtonTouched(x, y, minutes4Button)) {
                currentPeriodEditing = 3;
                drawMinutesMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime);
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
                currentPeriodEditing = 0;
                currentMenuViewing = determineTempMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp2Button)) {
                currentPeriodEditing = 1;
                currentMenuViewing = determineTempMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp3Button)) {
                currentPeriodEditing = 2;
                currentMenuViewing = determineTempMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, temp4Button)) {
                currentPeriodEditing = 3;
                currentMenuViewing = determineTempMenu(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature);
                drawProgrammingTempMenu(currentMenuViewing);
                menuState = TEMP_MENU;
              }
              else if (isButtonTouched(x, y, mode1Button)) {
                currentPeriodEditing = 0;
                drawProgrammingModeMenu(currentPeriodEditing);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode2Button)) {
                currentPeriodEditing = 1;
                drawProgrammingModeMenu(currentPeriodEditing);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode3Button)) {
                currentPeriodEditing = 2;
                drawProgrammingModeMenu(currentPeriodEditing);
                menuState = PROGRAM_MODE_MENU;
              }
              else if (isButtonTouched(x, y, mode4Button)) {
                currentPeriodEditing = 3;
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
                      updateHours(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime), menuButtonTouched);
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
                      updateHours(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime), menuButtonTouched + 3);
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
                      updateHours(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime), menuButtonTouched + 6);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 4:
                    if (menuButtonTouched == 1)
                      drawHoursMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 2) {
                      updateHours(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime), 11);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    else if (menuButtonTouched == 3) {
                      updateHours(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime), 12);
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
                updateMinutes(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].startTime), menuButtonTouched - 1);
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
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 59 + menuButtonTouched);
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
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 62 + menuButtonTouched);
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
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 65 + menuButtonTouched);
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
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 68 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 5:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 71 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 6:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 74 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 7:
                    if (menuButtonTouched == 1)
                      drawProgrammingTempMenu(--currentMenuViewing);
                    else if (menuButtonTouched == 5)
                      drawProgrammingTempMenu(++currentMenuViewing);
                    else {
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 77 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
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
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 80 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
                      menuState = NONE;
                    }
                    break;
                  case 9:
                    if (menuButtonTouched == 1) {
                      drawProgrammingTempMenu(--currentMenuViewing);
                    }
                    else {
                      updateProgrammingTemp(&(rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].temperature), 83 + menuButtonTouched);
                      drawProgrammingScreen(currentProgramEditing);
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
                  rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].mode = HEAT;
                  drawProgrammingScreen(currentProgramEditing);
                  menuState = NONE;
                  break;
                case 2:
                  rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].mode = COOL;
                  drawProgrammingScreen(currentProgramEditing);
                  menuState = NONE;
                  break;
                case 3:
                  rooms[currentRoom].programs[currentProgramEditing].periods[currentPeriodEditing].mode = OFF;
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

