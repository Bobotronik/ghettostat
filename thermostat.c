#include "derivative.h"
#include "thermostat.h"

#include "lcd.h"
#include "i2cDevices.h"
#include "humSense.h"
#include "functions.h"

/*
****Features****
4 time periods per day (fixed number of periods)
7 day programmable
manual override
copy settings from one day to another

general algorithm

query rtc
update program setting
check program setting

*/

// Icons

const unsigned char house[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 
0x00, 0x00, 0x06, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 
0x00, 0x01, 0x20, 0x01, 0x20, 0x00, 
0x00, 0x06, 0x00, 0x00, 0x18, 0x00, 
0x00, 0x18, 0x00, 0x00, 0x06, 0x00, 
0x01, 0x20, 0x00, 0x00, 0x01, 0x20, 
0x06, 0x00, 0x00, 0x00, 0x00, 0x18, 
0x00, 0x10, 0x00, 0x00, 0x02, 0x00, 
0x00, 0x10, 0x00, 0x00, 0x02, 0x00, 
0x00, 0x10, 0x00, 0x00, 0x02, 0x00, 
0x00, 0x10, 0x07, 0x38, 0x02, 0x00, 
0x00, 0x10, 0x04, 0x08, 0x02, 0x00, 
0x00, 0x10, 0x04, 0x08, 0x02, 0x00, 
0x00, 0x10, 0x04, 0x08, 0x02, 0x00, 
0x00, 0x10, 0x04, 0x08, 0x02, 0x00, 
0x00, 0x10, 0x04, 0x08, 0x02, 0x00, 
0x00, 0x1f, 0x3c, 0x0f, 0x3e, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char calendar[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x30, 0x3f, 0x3f, 0x3f, 0x30, 
0x01, 0x08, 0x20, 0x00, 0x00, 0x10, 
0x01, 0x28, 0x20, 0x00, 0x00, 0x10, 
0x01, 0x08, 0x20, 0x00, 0x00, 0x10, 
0x01, 0x28, 0x3f, 0x3f, 0x3f, 0x30, 
0x01, 0x08, 0x20, 0x00, 0x00, 0x10, 
0x01, 0x28, 0x2f, 0x3f, 0x3f, 0x10, 
0x01, 0x08, 0x29, 0x09, 0x09, 0x10, 
0x01, 0x38, 0x29, 0x09, 0x09, 0x10, 
0x01, 0x08, 0x2f, 0x3f, 0x3f, 0x10, 
0x01, 0x08, 0x29, 0x09, 0x09, 0x10, 
0x01, 0x08, 0x29, 0x09, 0x09, 0x10, 
0x01, 0x08, 0x2f, 0x3f, 0x3f, 0x10, 
0x01, 0x08, 0x29, 0x09, 0x09, 0x10, 
0x02, 0x04, 0x29, 0x09, 0x09, 0x10, 
0x02, 0x04, 0x2f, 0x3f, 0x3f, 0x10, 
0x01, 0x08, 0x20, 0x00, 0x00, 0x10, 
0x00, 0x30, 0x3f, 0x3f, 0x3f, 0x30, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char wrench[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x08, 0x20, 0x00, 0x00, 0x00, 
0x00, 0x04, 0x20, 0x00, 0x00, 0x00, 
0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 
0x01, 0x08, 0x0c, 0x00, 0x00, 0x00, 
0x02, 0x30, 0x03, 0x20, 0x00, 0x00, 
0x02, 0x00, 0x00, 0x18, 0x00, 0x00, 
0x01, 0x07, 0x20, 0x06, 0x07, 0x00, 
0x00, 0x38, 0x18, 0x01, 0x38, 0x20, 
0x00, 0x00, 0x07, 0x00, 0x00, 0x10, 
0x00, 0x00, 0x00, 0x30, 0x03, 0x10, 
0x00, 0x00, 0x00, 0x0c, 0x04, 0x20, 
0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 

// Buttons

// {X, Y, width, height}

// Left Bar
const unsigned char leftBarX = 0;
const unsigned char leftBarWidth = 6;
const unsigned char leftBarHeight = 4;
const unsigned char homeButton[] = {leftBarX, 2, leftBarWidth, leftBarHeight};
const unsigned char programButton[] = {leftBarX, 7, leftBarWidth, leftBarHeight};
const unsigned char settingsButton[] = [leftBarX, 12, leftBarWidth, leftBarHeight};

// Main Screen
const unsigned char setToButton[] = {20, 6, 10, 6};
const unsigned char rightBarX = 34;
const unsigned char rightBarWidth = 6;
const unsigned char rightBarHeight = 3;
const unsigned char roomButton[] = {rightBarX, 3, rightBarWidth, rightBarHeight};
const unsigned char fanButton[] = {rightBarX, 8, rightBarWidth, rightBarHeight};
const unsigned char modeButton[] = {rightBarX, 13, rightBarWidth, rightBarHeight};

// Program Screen
const unsigned char tabY = 1;
const unsigned char tabWidth = 10;
const unsigned char tabHeight = 3;
const unsigned char programTab[] = {12, tabY, tabWidth, tabHeight};
const unsigned char daysTab[] = {25, tabY, tabWidth, tabHeight};

const unsigned char programsButtonX = 18;
const unsigned char programsButtonWidth = 11;
const unsigned char programsButtonHeight = 3;

const unsigned char program1Button[] = {programsButtonX, 4, programsButtonWidth, programsButtonHeight};
const unsigned char program2Button[] = {programsButtonX, 7, programsButtonWidth, programsButtonHeight};
const unsigned char program3Button[] = {programsButtonX, 10, programsButtonWidth, programsButtonHeight};
const unsigned char program4Button[] = {programsButtonX, 13, programsButtonWidth, programsButtonHeight};

const unsigned char daysButtonY = 6;
const unsigned char daysButtonWidth = 5;
const unsigned char daysButtonHeight = 3;
const unsigned char monButton[] = {7, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char wedButton[] = {13, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char friButton[] = {19, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char tueButton[] = {9, daysButtonY + 4, daysButtonWidth, daysButtonHeight};
const unsigned char thuButton[] = {15, daysButtonY + 4, daysButtonWidth, daysButtonHeight};
const unsigned char satButton[] = {27, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char sunButton[] = {33, daysButtonY, daysButtonWidth, daysButtonHeight};

extern unsigned char DEVICE_DATA[2];

struct period {
  unsigned char startTime;
  unsigned char temperature;
  unsigned char setting;
};

// 4 periods per program
struct program {
  struct period periods[NUM_PERIODS];
};

// 5 program slots available
struct program programs[NUM_PROGRAMS];

// Which day has which program (0 - numPrograms)
unsigned char day[7];

// Format of *Period
// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
//  PRO3 PRO2 PRO1 PRO0 PER3 PER2 PER1 PER0

unsigned char currentTemperature; 
unsigned char currentTime;
unsigned char currentPeriod;
unsigned char currentDay;

unsigned char nextPeriod;
unsigned char dayOfNextPeriod;

struct period overridePeriod;
unsigned char isOverride;

unsigned char mainState, menuState;

void initializeThermostat() {
  unsigned char i;
  
  // Wakeup
  programs[0].periods[MORNING].startTime = 24; //6*4
  programs[0].periods[MORNING].temperature = 72;
  programs[0].periods[MORNING].setting = HEAT;
  // At work
  programs[0].periods[WORK].startTime = 32; //8*4
  programs[0].periods[WORK].setting = OFF;
  // At home
  programs[0].periods[EVENING].startTime = 68; //6*4 + 12*4
  programs[0].periods[EVENING].temperature = 72;
  programs[0].periods[EVENING].setting = HEAT;
  // Asleep
  programs[0].periods[NIGHT].startTime = 84; //10*4 + 12*4
  programs[0].periods[NIGHT].temperature = 62;
  programs[0].periods[NIGHT].setting = HEAT;
  
  for (i = 0; i < 7; i++) {
    day[i] = 0;
  }
}

void updateTime() {

}

void displayTime() {
  unsigned char temp;
  unsigned char tempTime;
  unsigned char tempMinutes;
  unsigned char pm;
  
  getTime();
  
  // Day of week
  temp = RTC_TIME[3];
  switch (temp) {
    case 1:
      printStr("Sun ");
      break;
    case 2:
      printStr("Mon ");
      break;
    case 3:
      printStr("Tue ");
      break;
    case 4:
      printStr("Wed ");
      break;
    case 5:
      printStr("Thu ");
      break;
    case 6:
      printStr("Fri ");
      break;
    case 7:
      printStr("Sat ");
      break;
  }
  
  // Month
  temp = RTC_TIME[5];
  temp += (temp >> 4);

  switch (temp) {
    case 1:
      printStr("Jan ");
      break;
    case 2:
      printStr("Feb ");
      break;
    case 3:
      printStr("Mar ");
      break;
    case 4:
      printStr("Apr ");
      break;
    case 5:
      printStr("May ");
      break;
    case 6:
      printStr("Jun ");
      break;
    case 7:
      printStr("Jul ");
      break;
    case 8:
      printStr("Aug ");
      break;
    case 9:
      printStr("Sep ");
      break;
    case 10:
      printStr("Oct ");
      break;
    case 11:
      printStr("Nov ");
      break;
    case 12:
      printStr("Dec ");
      break;
  }
  
  // Date
  temp = RTC_TIME[4];
  
  if (temp & 0xf0)
    printDigit(temp >> 4);
  else
    printChar(' ');  
 
  printDigit(temp & 0x0f);
  printStr(", ");
  
  // Determine whether AM or PM;
  temp = RTC_TIME[2];
  if (temp & 0x20) {
    pm = 1;
    tempTime = 48;
  }
  else {
    pm = 0;
    tempTime = 0;
  }
  
  // Determine whether to display nothing or 1
  if (temp & 0x10) {
    printDigit(1);
    tempTime += 40;
  }
  else {
    printChar(' ');
  }
  
  // Display hour (1's digit)
  printDigit(temp & 0x0f);
  tempTime += (temp & 0x0f);
  printChar(':');
 
  // Display minutes
  temp = RTC_TIME[1];
  printDigit(temp >> 4);
  printDigit(temp & 0x0f);
  tempMinutes = ((temp & 0x70) >> 4)*10;
  tempMinutes += temp & 0x0f;
  tempTime += tempMinutes/15;
  currentTime = tempTime;
  
  if (pm == 0) {
    printStr(" AM");
  }
  else {
    printStr(" PM");
  }
  
  /* Check if new day
  if (DEVICE_TIME[2]-1 > currentDay)
    isLastPeriodOfDay = 0;
  currentDay = DEVICE_TIME[2] - 1;*/
}

void updateTemp() {
  goToText(10, 5);
  printNum(getTempF());
  //goToText();
}

void displayTemps() {
  unsigned char quotient;
  unsigned char remainder;
  
  getTemp();
  
  quotient = currentTemperature/100;
  remainder = currentTemperature%100;
  
  if (quotient == 0) {
    display(0x00);
  }
  else {
    printNum(1);
  }
  
  quotient = remainder/10;
  remainder %= 10;
  printNum(quotient);
  printNum(remainder);
}

void updatePeriods() {
  // Update only applicable if next period is on current day
  if (currentDay == dayOfNextPeriod) {
  
    if(currentTime >= programs[day[dayOfNextPeriod]].periods[nextPeriod].startTime) {
      currentPeriod = nextPeriod;
      
      // Updating period
      // Same day   
      if (nextPeriod < NUM_PERIODS-1) {
        nextPeriod++;
      }
      // New day
      else {
        nextPeriod = 0;
        
        // Updating day
        // Same week
        if (dayOfNextPeriod < 6){
          dayOfNextPeriod++;
        }
        // New week
        else {
          dayOfNextPeriod = 0;
        }
      }
      
      // Update thermometer
      setTemp(programs[day[currentDay]].periods[currentPeriod].temperature);
    }
  }
}

void updateThermometer() {
  setTemp(programs[day[currentDay]].periods[currentPeriod].temperature);
}

void drawButton(unsigned char* button[]) {
  unsigned char i;
  
  goToText(button[0], button[1]);
  printCG(UPPER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++){
    printCG(TOP_BORDER);
  }
  printCG(UPPER_RIGHT_CORNER);
  
  for (i = button[1] + 1; i < button[1] + button[3] - 1; i++) {
    goToText(button[0], i);
    printCG(LEFT_BORDER);
    goToText(button[0] + button[2] - 1, i);
    printCG(RIGHT_BORDER); 
  }
  
  goToText(button[0], button[1] + button[3] - 1);
  printCG(LOWER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++) {
    printCG(BOTTOM_BORDER);
  }
  printCG(LOWER_RIGHT_CORNER);
}

void drawSolidButton(unsigned char* button[]) {
  unsigned char i, j;
  
  goToText(button[0], button[1]);
  printCG(SOLID_UPPER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++) {
    printCG(SOLID_TOP_BORDER);
  }
  printCG(SOLID_UPPER_RIGHT_CORNER);
  
  for (i = button[1] + 1; i < button[1] + button[3] - 1; i++) {
    goToText(button[0], i);
    printCG(LEFT_BORDER);
    goToText(button[0] + button[2] - 1, i);
    printCG(RIGHT_BORDER); 
  }
  
  goToText(button[0], button[1] + button[3] - 1);
  printCG(SOLID_LOWER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++) {
    printCG(SOLID_BOTTOM_BORDER);
  }
  printCG(SOLID_LOWER_RIGHT_CORNER);
  
  for (i = 0; i < button[3]*8 - 16; i++) {
    goToGraphic((button[0]+1)*FONT_WIDTH, (button[1]+1)*8 + i);
    for (j = 0; j < button[2] - 2; j++) {
      display(0xcf);
    }
  }
}

unsigned char isButtonTouched(unsigned char x, unsigned char y, unsigned char* button) {
  if ( (x < button[0]*6) || (x > button[0]*6 + button[2]*6) )
    return 0;
  if ( (y < button[1]*8) || (y > button[1]*8 + button[3]*8) )
    return 0;
  return 1;  
}

void drawTopBar() {
  unsigned char i, j;
  
  // Top Bar
  for (i = 0; i < 8; i++) {
    setADP(GRAPHIC_HOME + GRAPHIC_AREA*i);
    for (j = 0; j < 40; j++) {
      display(0x3f); 
    }
  }
  
  goToText(0,0);
  displayTime();
  goToText(28, 0);
  printStr("Humidity   %");
  goToText(38, 0);
  printNum(getHumidityDec());
}

void drawLeftBar() {
  drawButton(0, 2, 6, 4);
  drawGraphic(0, 22, 6, 20, house);
  drawButton(0, 7, 6, 4);
  drawGraphic(0, 62, 6, 20, calendar);
  drawButton(0, 12, 6, 4); 
  drawGraphic(0, 102, 6, 20, wrench);
}

void drawMainScreen() {
  drawLeftBar();
  
  // Right Side
  drawButton(roomButton);
  goToText(35, 2);
  printStr("Room");
  
  drawButton(fanButton);
  goToText(35, 7);
  printStr("Fan");
  
  drawButton(modeButton);
  goToText(35, 12);
  printStr("Mode");
  
  goToText(9, 5);
  printStr("Currently");

  drawButton(setToButton);
  goToText(21, 5);
  printStr("Set To");
}

void drawMainSetToMenu() {
  
}

void drawMainRoomMenu() {
  drawButton(19, 2, 15, 5);
  
  drawButton(20, 3, 6, 3); 
  goToText(21, 9);
  printStr("MAIN");
  
  drawButton(27, 3, 6, 3); 
  goToText(28, 9);
  printStr("AUX");
}

void drawMainFanMenu() {
  drawButton(12, 7, 22, 5);
  
  drawButton(13, 8, 6, 3); 
  goToText(14, 9);
  printStr(" ON");
  
  drawButton(20, 8, 6, 3); 
  goToText(21, 9);
  printStr("OFF");
  
  drawButton(27, 8, 6, 3); 
  goToText(28, 9);
  printStr("AUTO");
}

// Heat, Cool, Off
void drawMainModeMenu() {
  drawButton(12, 12, 22, 5);
  
  drawButton(13, 13, 6, 3); 
  goToText(14, 14);
  printStr("HEAT");
  
  drawButton(20, 13, 6, 3); 
  goToText(21, 14);
  printStr("COOL");
  
  drawButton(27, 13, 6, 3); 
  goToText(28, 14);
  printStr("OFF");
}

void drawProgramsTab) {
  unsigned char i, j;

  drawLeftBar();
  
  drawButton(programsTab);
  goToText(13, 2);
  printStr("Programs");
  drawButton(daysTab);
  goToText(26, 2);
  printStr("  Days  ");
  
  drawButton(program1Button);
  goToText(19, 5);
  printStr("Program 1");
  drawButton(program2Button);
  goToText(19, 8);
  printStr("Program 2");
  drawButton(program3Button);
  goToText(19, 11);
  printStr("Program 3");
  drawButton(program4Button);
  goToText(19, 14);
  printStr("Program 4");
  
  goToText(8, 3);
  for (i = 0; i < 31 ; i++) {
    printCG(BOTTOM_BORDER);
  }
  for (i = 4; i < 16; i++) {
    goToText(7, i);
    printCG(LEFT_BORDER);
    goToText(39, i);
    printCG(RIGHT_BORDER);
  }
  
  goToText(programsTab[0] - 1, programsTab[1] + programsTab[3] - 1);
  printCG(LOWER_RIGHT_CORNER);
  goToText(programsTab[0] + programsTab[2], programsTab[1] + programsTab[3] - 1);
  printCG(LOWER_LEFT_CORNER);
  goToText(daysTab[0] - 1, daysTab[1] + daysTab[3] - 1);
  printCG(LOWER_RIGHT_CORNER);
  goToText(daysTab[0] + daysTab[2], daysTab[1] + daysTab[3] - 1);
  printCG(LOWER_LEFT_CORNER);
}

void drawDaysTab() {
  unsigned char i, j;
  
  drawLeftBar();
  
  drawButton(programsTab);
  goToText(13, 2);
  printStr("Programs");
  drawButton(daysTab);
  goToText(26, 2);
  printStr("  Days  ");
  
  drawButton(monButton);
  goToText(monButton[0] + 1, monButton[1] - 1);
  printStr("Mon");
  drawButton(wedButton);
  goToText(wedButton[0] + 1, wedButton[1] - 1);
  printStr("Wed");
  drawButton(friButton);
  goToText(friButton[0] + 1, friButton[1] - 1);
  printStr("Fri");
  drawButton(satButton);
  goToText(satButton[0] + 1, satButton[1] - 1);
  printStr("Sat");
  drawButton(sunButton);
  goToText(sunButton[0] + 1, sunButton[1] - 1);
  printStr("Sun");
  
  drawButton(tueButton);
  goToText(tueButton[0] + tueButton[2], tueButton[1] - 1);
  printStr("Tue");
  drawButton(thuButton);
  goToText(thuButton[0] + thuButton[2], thuButton[1] - 1);
  printStr("Thu");
  
  goToText(programsTab[0] - 1, programsTab[1] + programsTab[3] - 1);
  printCG(LOWER_RIGHT_CORNER);
  goToText(programsTab[0] + programsTab[2], programsTab[1] + programsTab[3] - 1);
  printCG(LOWER_LEFT_CORNER);
  goToText(daysTab[0] - 1, daysTab[1] + daysTab[3] - 1);
  printCG(LOWER_RIGHT_CORNER);
  goToText(daysTab[0] + daysTab[2], daysTab[1] + daysTab[3] - 1);
  printCG(LOWER_LEFT_CORNER);
}

void drawProgrammingScreen() {
  
}

void drawProgramDaysScreen() {
  
}
void drawSettingsScreen() {
  drawLeftBar();
}
