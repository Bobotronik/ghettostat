#include "derivative.h"
#include "thermostat.h"

#include "lcd.h"
#include "i2cDevices.h"
#include "humSense.h"
#include "functions.h"

/*
****Features****
manual override

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

const unsigned char upArrow[] = {
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x20, 0x00, 
0x00, 0x01, 0x20, 0x00, 
0x00, 0x03, 0x30, 0x00, 
0x00, 0x03, 0x30, 0x00, 
0x00, 0x07, 0x38, 0x00, 
0x00, 0x07, 0x38, 0x00, 
0x00, 0x0f, 0x3c, 0x00, 
0x00, 0x0f, 0x3c, 0x00, 
0x00, 0x1f, 0x3e, 0x00, 
0x00, 0x1f, 0x3e, 0x00, 
0x00, 0x3f, 0x3f, 0x00, 
0x00, 0x3f, 0x3f, 0x00, 
0x01, 0x3f, 0x3f, 0x20, 
0x01, 0x3f, 0x3f, 0x20};

const unsigned char downArrow[] = {
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 
0x01, 0x3f, 0x3f, 0x20, 
0x01, 0x3f, 0x3f, 0x20, 
0x00, 0x3f, 0x3f, 0x00, 
0x00, 0x3f, 0x3f, 0x00, 
0x00, 0x1f, 0x3e, 0x00, 
0x00, 0x1f, 0x3e, 0x00, 
0x00, 0x0f, 0x3c, 0x00, 
0x00, 0x0f, 0x3c, 0x00, 
0x00, 0x07, 0x38, 0x00, 
0x00, 0x07, 0x38, 0x00, 
0x00, 0x03, 0x30, 0x00, 
0x00, 0x03, 0x30, 0x00, 
0x00, 0x01, 0x20, 0x00, 
0x00, 0x01, 0x20, 0x00};

const unsigned char check[] = {
0x00, 0x02, 
0x00, 0x06, 
0x00, 0x0c, 
0x00, 0x0c, 
0x00, 0x18, 
0x00, 0x18, 
0x00, 0x18, 
0x00, 0x38, 
0x1c, 0x38, 
0x0f, 0x30, 
0x07, 0x30, 
0x01, 0x30};

// Buttons
// Format: {X, Y, width, height}

// Left Bar
const unsigned char leftBarX = 0;
const unsigned char leftBarY = 2;
const unsigned char leftBarOffset = 5;
const unsigned char leftBarWidth = 6;
const unsigned char leftBarHeight = 4;
const unsigned char homeButton[] = {leftBarX, leftBarY, leftBarWidth, leftBarHeight};
const unsigned char programButton[] = {leftBarX, leftBarY + leftBarOffset, leftBarWidth, leftBarHeight};
const unsigned char settingsButton[] = {leftBarX, leftBarY + 2*leftBarOffset, leftBarWidth, leftBarHeight};

// Main Screen
const unsigned char setToButton[] = {20, 6, 10, 6};
const unsigned char rightBarX = 34;
const unsigned char rightBarY = 3;
const unsigned char rightBarOffset = 5;
const unsigned char rightBarWidth = 6;
const unsigned char rightBarHeight = 3;
const unsigned char roomButton[] = {rightBarX, rightBarY, rightBarWidth, rightBarHeight};
const unsigned char fanButton[] = {rightBarX, rightBarY + rightBarOffset, rightBarWidth, rightBarHeight};
const unsigned char modeButton[] = {rightBarX, rightBarY + 2*rightBarOffset, rightBarWidth, rightBarHeight};
const unsigned char roomMenu[] = {roomButton[0] - 12, roomButton[1], 6, 3, 2};
const unsigned char fanMenu[] = {fanButton[0] - 18, fanButton[1], 6, 3, 3};
const unsigned char modeMenu[] = {modeButton[0] - 18, modeButton[1], 6, 3, 3};
                                         
// Program Screen
const unsigned char tabY = 1;
const unsigned char tabWidth = 16;
const unsigned char tabHeight = 3;
const unsigned char programsTab[] = {7, tabY, tabWidth, tabHeight};
const unsigned char daysTab[] = {24, tabY, tabWidth, tabHeight};

const unsigned char programsButtonX = 18;
const unsigned char programsButtonY = 4;
const unsigned char programsButtonOffset = 3;
const unsigned char programsButtonWidth = 11;
const unsigned char programsButtonHeight = 3;

const unsigned char program1Button[] = {programsButtonX, programsButtonY, programsButtonWidth, programsButtonHeight};
const unsigned char program2Button[] = {programsButtonX, programsButtonY + programsButtonOffset, programsButtonWidth, programsButtonHeight};
const unsigned char program3Button[] = {programsButtonX, programsButtonY + 2*programsButtonOffset, programsButtonWidth, programsButtonHeight};
const unsigned char program4Button[] = {programsButtonX, programsButtonY + 3*programsButtonOffset, programsButtonWidth, programsButtonHeight};

const unsigned char daysButtonY = 6;
const unsigned char daysButtonWidth = 5;
const unsigned char daysButtonHeight = 3;
const unsigned char monButton[] = {8, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char wedButton[] = {14, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char friButton[] = {20, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char tueButton[] = {11, daysButtonY + 4, daysButtonWidth, daysButtonHeight};
const unsigned char thuButton[] = {17, daysButtonY + 4, daysButtonWidth, daysButtonHeight};
const unsigned char satButton[] = {28, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char sunButton[] = {34, daysButtonY, daysButtonWidth, daysButtonHeight};
const unsigned char monMenu[] = {monButton[0], monButton[1] - 3, 5, 3, 4};
const unsigned char tueMenu[] = {tueButton[0], tueButton[1] - 6, 5, 3, 4};
const unsigned char wedMenu[] = {wedButton[0], wedButton[1] - 3, 5, 3, 4};
const unsigned char thuMenu[] = {thuButton[0], thuButton[1] - 6, 5, 3, 4};
const unsigned char friMenu[] = {friButton[0], friButton[1] - 3, 5, 3, 4};
const unsigned char satMenu[] = {satButton[0], satButton[1] - 3, 5, 3, 4};
const unsigned char sunMenu[] = {sunButton[0], sunButton[1] - 3, 5, 3, 4};                                         

const unsigned char okButton[] = {7, 2, 4, 3};

const unsigned char programsButtonsY = 4;
const unsigned char programsButtonsOffset = 3;
const unsigned char programsButtonsWidth = 4;
const unsigned char programsButtonsHeight = 3;

const unsigned char hour1Button[] = {12, programsButtonsY, programsButtonsWidth, programsButtonsHeight};
const unsigned char hour2Button[] = {12, programsButtonsY + programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char hour3Button[] = {12, programsButtonsY + 2*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char hour4Button[] = {12, programsButtonsY + 3*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};  
const unsigned char minutes1Button[] = {17, programsButtonsY, programsButtonsWidth, programsButtonsHeight};
const unsigned char minutes2Button[] = {17, programsButtonsY + programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char minutes3Button[] = {17, programsButtonsY + 2*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char minutes4Button[] = {17, programsButtonsY + 3*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight}; 
const unsigned char amPm1Button[] = {22, programsButtonsY, programsButtonsWidth, programsButtonsHeight};
const unsigned char amPm2Button[] = {22, programsButtonsY + programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char amPm3Button[] = {22, programsButtonsY + 2*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char amPm4Button[] = {22, programsButtonsY + 3*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight}; 
const unsigned char temp1Button[] = {28, programsButtonsY, programsButtonsWidth, programsButtonsHeight};
const unsigned char temp2Button[] = {28, programsButtonsY + programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char temp3Button[] = {28, programsButtonsY + 2*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight};
const unsigned char temp4Button[] = {28, programsButtonsY + 3*programsButtonsOffset, programsButtonsWidth, programsButtonsHeight}; 
const unsigned char mode1Button[] = {34, programsButtonsY, 6, programsButtonsHeight};
const unsigned char mode2Button[] = {34, programsButtonsY + programsButtonsOffset, 6, programsButtonsHeight};
const unsigned char mode3Button[] = {34, programsButtonsY + 2*programsButtonsOffset, 6, programsButtonsHeight};
const unsigned char mode4Button[] = {34, programsButtonsY + 3*programsButtonsOffset, 6, programsButtonsHeight};
const unsigned char hoursMenu[] = {12, 1, 4, 3, 5};
const unsigned char minutesMenu[] = {17, 4, 4, 3, 4};
const unsigned char programmingTempMenu[] = {28, 1, 4, 3, 5};
unsigned char programmingModeMenu[] = {16, 1, 6, 3, 3};

  
extern unsigned char DEVICE_DATA[2];

struct room rooms[2];

unsigned char weeklySchedule[] = {0, 0, 0, 0, 0, 0, 0};

// Format of *Period
// | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
//  PRO3 PRO2 PRO1 PRO0 PER3 PER2 PER1 PER0
unsigned char currentDay;
unsigned char currentTime;

unsigned char currentRoom; // Manually controlled
unsigned char fanSetting; // Manually controlled

unsigned char dayOfCurrentPeriod;
unsigned char currentPeriodIndex;
unsigned char dayOfNextPeriod;
unsigned char nextPeriodIndex;

unsigned char currentTemperature;

struct period currentPeriod;
struct period overridePeriod;

unsigned char isOverride;

unsigned char mainState, menuState;

void initializeThermostat() {
  unsigned char i, j;
  
  displayTemps(); // must be called first to initialize time variables
  
  currentRoom = ROOM_MAIN;
  fanMode = FAN_OFF;
  
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 4; j++) {
      rooms[i].programs[j].periods[MORNING].startTime = 24; //6*4
      rooms[i].programs[j].periods[MORNING].temperature = 72;
      rooms[i].programs[j].periods[MORNING].mode = HEAT;
      // At work
      rooms[i].programs[j].periods[WORK].startTime = 32; //8*4
      rooms[i].programs[j].periods[WORK].temperature = 72;
      rooms[i].programs[j].periods[WORK].mode = OFF;
      // At home
      rooms[i].programs[j].periods[EVENING].startTime = 68; //6*4 + 12*4
      rooms[i].programs[j].periods[EVENING].temperature = 72;
      rooms[i].programs[j].periods[EVENING].mode = HEAT;
      // Asleep
      rooms[i].programs[j].periods[NIGHT].startTime = 84; //10*4 + 12*4
      rooms[i].programs[j].periods[NIGHT].temperature = 62;
      rooms[i].programs[j].periods[NIGHT].mode = HEAT;  
    }
  }
  
  refreshThermostat();
}

void printHours(unsigned char temp) {
  if (temp <= 51) {
    if (temp/4)
      printNum(temp/4);
    else {
      printNum(12); 
    }
  }
  else
    printNum((temp - 48) / 4);  
}

void printMinutes(unsigned char temp) {
  if ((temp % 4) * 15)
    printNum((temp % 4) * 15);  
  else {
    printNum(0);
    printNum(0);
  }
}

void printAmPm(unsigned char temp) {
  if (temp < 48)
    printStr("AM");
  else 
    printStr("PM");
}

void printMode(unsigned char temp) {
  switch (temp) {
    case HEAT:
      printStr("HEAT");
      break;
    case COOL:
      printStr("COOL");
      break;
    case OFF:
      printStr("OFF");
      break;   
  }
}

void displayTime() {
  unsigned char temp;
  unsigned char tempTime;
  unsigned char tempHours;
  unsigned char tempMinutes;
  unsigned char pm;
  
  getTime();
  
  // Day of week
  temp = RTC_TIME[3];
  switch (temp) {
    case 1:
      printStr("Sun ");
      currentDay = 6;
      break;
    case 2:
      printStr("Mon ");
      currentDay = 0;
      break;
    case 3:
      printStr("Tue ");
      currentDay = 1;
      break;
    case 4:
      printStr("Wed ");
      currentDay = 2;
      break;
    case 5:
      printStr("Thu ");
      currentDay = 3;
      break;
    case 6:
      printStr("Fri ");
      currentDay = 4;
      break;
    case 7:
      printStr("Sat ");
      currentDay = 5;
      break;
  }
  
  // Month
  temp = RTC_TIME[5];
  tempTime = (temp >> 4)*10 + (temp & 0x0f);

  switch (tempTime) {
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
    //tempTime += 40;
  }
  else {
    printChar(' ');
  }
  
  // Display hour (1's digit)
  printDigit(temp & 0x0f);
  //tempTime += (temp & 0x0f);
  tempTime += (((temp & 0x10) >> 4)*10 + (temp & 0x0f)) << 2;
  printChar(':');
 
  // Display minutes
  temp = RTC_TIME[1];
  printDigit(temp >> 4);
  printDigit(temp & 0x0f);
  tempMinutes = ((temp & 0x70) >> 4)*10 + (temp & 0x0f);
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
  goToText(10, 6);
  printNum(getTempF());
  //goToText();
}

void displayTemps() {
  unsigned char quotient;
  unsigned char remainder;
  
  getTempF();
  
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

void updateCurrentPeriod() {

  // Update only applicable if next period is on current day
  if (dayOfNextPeriod == currentDay) {
  
    if(currentTime >= rooms[currentRoom].programs[weeklySchedule[dayOfNextPeriod]].periods[nextPeriodIndex].startTime) {
      currentPeriodIndex = nextPeriodIndex;
      
      // Updating period
      // Same day   
      if (nextPeriodIndex < NUM_PERIODS-1)
        nextPeriodIndex++;
      // New day
      else {
        nextPeriodIndex = 0;
        
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
      
      currentPeriod = rooms[currentRoom].programs[weeklySchedule[dayOfCurrentPeriod]].periods[currentPeriodIndex];
      // Update thermometer
      //setTemp(programs[weeklySchedule[currentDay]].periods[currentPeriod].temperature);
    }
  }
}

/*
"Refreshes the thermostat by finding the current period with only the current day and time
*/
void refreshThermostat() {
  unsigned char i, day;
    
  for (i = 0; i < 4; i++) {
    if (rooms[currentRoom].programs[weeklySchedule[currentDay]].periods[i].startTime > currentTime);
      break;
  }
  // Need to set dayOfCurrentPeriod, dayOfNextPeriod, currentPeriodIndex, nextPeriodIndex
  // Current day
  if (i != 0) {
    currentPeriodIndex = i - 1;
    dayOfCurrentPeriod = currentDay;
    if (i != 4) {
      nextPeriodIndex = i;
      dayOfNextPeriod = currentDay;
    }
    // Next day
    else {
      nextPeriodIndex = 0;
      if (currentDay != 6) {
        dayOfNextPeriod = currentDay + 1;
      }
      else {
        dayOfNextPeriod = 0;
      }
    }
  }
  
  // Previous day
  else {
    currentPeriodIndex = 3;
    nextPeriodIndex = 0;
    if (currentDay != 0)
      dayOfCurrentPeriod = currentDay - 1;
    else
      dayOfCurrentPeriod = 6;
    dayOfNextPeriod = currentDay;
  }
  
  // update sensors
  currentPeriod = rooms[currentRoom].programs[weeklySchedule[dayOfCurrentPeriod]].periods[currentPeriodIndex];
  
  goToText(5, 1);
  printNum(dayOfCurrentPeriod);
  goToText(10, 1);
  printNum(currentPeriodIndex);
  goToText(15, 1);
  printNum(dayOfNextPeriod);
  goToText(20, 1);
  printNum(nextPeriodIndex);
  goToText(25, 1);
  printNum(currentRoom);
  
  goToText(30, 1);
  printNum(currentTime);
}

void updateThermometer() {
  
}

void drawButton(unsigned char* button) {
  unsigned char i, j;
  
  goToText(button[0], button[1]);
  printCG(UPPER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++){
    printCG(TOP_BORDER);
  }
  printCG(UPPER_RIGHT_CORNER);
  
  for (i = 0; i < button[3] - 2; i++) {
    goToText(button[0], button[1] + 1 + i);
    printCG(LEFT_BORDER);
    for (j = 0; j < button[2] - 2; j++) {
      display(0x00); 
    }
    goToText(button[0] + button[2] - 1, button[1] + 1 + i);
    printCG(RIGHT_BORDER); 
  }
  
  goToText(button[0], button[1] + button[3] - 1);
  printCG(LOWER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++) {
    printCG(BOTTOM_BORDER);
  }
  printCG(LOWER_RIGHT_CORNER);
}

void drawButtonPrintLabel(unsigned char* button, unsigned char* label) {
  drawButton(button);
  goToText(button[0] + 1, button[1] + 1);
  printStr(label);
}

void drawButtonPrintNumber(unsigned char* button, unsigned char number) {
  drawButton(button);
  goToText(button[0] + button[2] - 2, button[1] + 1);
  printNum(number);
}

void drawButtonPrintHours(unsigned char* button, unsigned char number) {
  drawButton(button);
  goToText(button[0] + button[2] - 2, button[1] + 1);
  printHours(number);
}

void drawButtonPrintMinutes(unsigned char* button, unsigned char number) {
  drawButton(button);
  goToText(button[0] + button[2] - 2, button[1] + 1);
  printMinutes(number);
}

void drawSolidButton(unsigned char* button) {
  unsigned char i, j;
  
  goToText(button[0], button[1]);
  
  printCG(SOLID_UPPER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++) {
    printCG(SOLID_TOP_BORDER);
  }
  printCG(SOLID_UPPER_RIGHT_CORNER);
  
  for (i = 0; i < button[3] - 2; i++) {
    goToText(button[0], button[1] + 1 + i);
    for (j = 0; j < button[2]; j++) {
      printCG(BLOCK); 
    }
  }
  
  goToText(button[0], button[1] + button[3] - 1);
  printCG(SOLID_LOWER_LEFT_CORNER);
  for (i = 0; i < button[2] - 2; i++) {
    printCG(SOLID_BOTTOM_BORDER);
  }
  printCG(SOLID_LOWER_RIGHT_CORNER);
  
}

void drawTopBorder(unsigned char i) {
  for (; i > 0; i--)
    printCG(TOP_MENU_BORDER);  
}

void drawNothing(unsigned char i) {
  for (; i > 0; i--)
    display(0x00);  
}

void drawBottomBorder(unsigned char i) {
  for (; i > 0; i--)
    printCG(BOTTOM_MENU_BORDER);  
}

void drawVerticalMenu(unsigned char* menu) {
  unsigned char y, i, j;
  y = menu[1];
  
  goToText(menu[0], y++);
  printCG(HARD_UPPER_LEFT_CORNER);
  drawTopBorder(menu[2] - 2);
  printCG(HARD_UPPER_RIGHT_CORNER);
  
  for (i = 0; i < menu[3] - 2; i++) {
    goToText(menu[0], y++);
    printCG(LEFT_BORDER);
    drawNothing(menu[2] - 2);
    printCG(RIGHT_BORDER);
  }
  goToText(menu[0], y++);
  printCG(HARD_LOWER_LEFT_CORNER);
  drawBottomBorder(menu[2] - 2);
  printCG(HARD_LOWER_RIGHT_CORNER);
  
  for (i = 0; i < menu[4] - 1; i++) {
    for (j = 0; j < menu[3] - 1; j++) {
      goToText(menu[0], y++);
      printCG(LEFT_BORDER);
      drawNothing(menu[2] - 2);
      printCG(RIGHT_BORDER);
    }
    goToText(menu[0], y++);
    printCG(HARD_LOWER_LEFT_CORNER);
    drawBottomBorder(menu[2] - 2);
    printCG(HARD_LOWER_RIGHT_CORNER);
  }
}

void drawHorizontalMenu(unsigned char* menu) {
  unsigned char y, i, j;
  y = menu[1];
  
  goToText(menu[0], y++);
  for (i = 0; i < menu[4] - 1; i++) {
    printCG(HARD_UPPER_LEFT_CORNER);
    drawTopBorder(menu[2] - 1);
  }
  printCG(HARD_UPPER_LEFT_CORNER);
  drawTopBorder(menu[2] - 2);
  printCG(HARD_UPPER_RIGHT_CORNER);
  
  for (i = 0; i < menu[3] - 2; i++) {
    goToText(menu[0], y++);
    for (j = 0; j < menu[4] - 1; j++) {
      printCG(LEFT_BORDER);
      drawNothing(menu[2] - 1);
    }
    printCG(LEFT_BORDER);
    drawNothing(menu[2] - 2);
    printCG(RIGHT_BORDER);
  }
  
  goToText(menu[0], y++);
  for (i = 0; i < menu[4] - 1; i++) {
    printCG(HARD_LOWER_LEFT_CORNER);
    drawBottomBorder(menu[2] - 1);
  }
  printCG(HARD_LOWER_LEFT_CORNER);
  drawBottomBorder(menu[2] - 2);
  printCG(HARD_LOWER_RIGHT_CORNER);
}

unsigned char isButtonTouched(unsigned char x, unsigned char y, unsigned char* button) {
  if ( (x < button[0]*6) || (x > button[0]*6 + button[2]*6) )
    return 0;
  if ( (y < button[1]*8) || (y > button[1]*8 + button[3]*8) )
    return 0;
  return 1;  
}

unsigned char isVerticalMenuButtonTouched(unsigned char x, unsigned char y, unsigned char* menu) {
  if ( (x < menu[0]*FONT_WIDTH) || (x > menu[0]*FONT_WIDTH + menu[2]*FONT_WIDTH) )
    return 0;
  if ( (y < menu[1]*8) || (y > menu[1]*8 + menu[3]*menu[4]*8) )
    return 0;
  //   To Text Matrix    To Button Matrix Don't return 0 
  return ((y >> 3) - menu[1]) / menu[3] + 1;
}

unsigned char isHorizontalMenuButtonTouched(unsigned char x, unsigned char y, unsigned char* menu) {
  if ( (x < menu[0]*6) || (x > menu[0]*6 + menu[2]*menu[4]*6) )
    return 0;
  if ( (y < menu[1]*8) || (y > menu[1]*8 + menu[3]*8) )
    return 0;
  return ((x/FONT_WIDTH) - menu[0]) / menu[2] + 1;
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
  printStr("        Humidity   %");
  goToText(38, 0);
  printNum(getHumidityDec());
}

void drawLeftBar() {
  drawButton(homeButton);
  drawGraphic(0, 22, 6, 20, house);
  drawButton(programButton);
  drawGraphic(0, 62, 6, 20, calendar);
  drawButton(settingsButton); 
  drawGraphic(0, 102, 6, 20, wrench);
}

void drawMainScreen() { 

  clearPartialScreen();
  drawLeftBar();
  drawSolidButton(homeButton);
  
  // Right Side
  drawButton(roomButton);
  goToText(35, 2);
  printStr("Room");
  goToText(35, 4);
  switch (currentRoom) {
    case 0:
      printStr("MAIN");
      break;
    case 1:
      printStr("AUX");
      break;
  }
  
  drawButton(fanButton);
  goToText(35, 7);
  printStr("Fan");
  goToText(35, 9);
  
  drawButton(modeButton);
  goToText(35, 12);
  printStr("Mode");
  goToText(35, 14);
  switch (currentPeriod.mode) {
    case HEAT:
      printStr("HEAT");
      break;
    case COOL:
      printStr("COOL");
      break;
    case OFF:
      printStr("OFF");
      break; 
  }
  
  goToText(9, 5);
  printStr("Currently");

  drawButton(setToButton);
  goToText(21, 5);
  printStr("Set To");
  goToText(24, 8);
  printNum(currentPeriod.temperature);
}

void drawMainSetToMenu() {
  
}

void drawMainRoomMenu() {
  unsigned char i, j;
  
  drawHorizontalMenu(roomMenu);

  goToText(roomButton[0] - 11, roomButton[1] + 1);
  printStr("MAIN");
  goToText(roomButton[0] - 5, roomButton[1] + 1);
  printStr("AUX ");
}

void drawMainFanMenu() {
  unsigned char i;
  
  drawHorizontalMenu(fanMenu);

  goToText(fanButton[0] - 17, fanButton[1] + 1);
  printStr("ON");
  goToText(fanButton[0] - 11, fanButton[1] + 1);
  printStr("OFF");
  goToText(fanButton[0] - 5, fanButton[1] + 1);
  printStr("AUTO");
}

void drawMainModeMenu() {
  unsigned char i, j;
  
  drawHorizontalMenu(modeMenu);
  
  goToText(modeButton[0] - 17, modeButton[1] + 1);
  printStr("HEAT");
  goToText(modeButton[0] - 11, modeButton[1] + 1);
  printStr("COOL");
  goToText(modeButton[0] - 5, modeButton[1] + 1);
  printStr("OFF");
}

void drawProgramsTab() {
  unsigned char i, j;
  
  clearPartialScreen();
  drawLeftBar();
  drawSolidButton(programButton);
  
  // Draw top stuff
  drawSolidButton(programsTab);
  goToText(11, 2);
  printStr("Programs");
  
  drawButton(daysTab);
  goToText(28, 2);
  printStr("  Days  ");
  
  // Draw border
  for (i = 4; i < 16; i++) {
    goToText(7, i);
    printCG(LEFT_BORDER);
    goToText(39, i);
    printCG(RIGHT_BORDER);
  }
  
  // Make top pretty
  goToText(programsTab[0], programsTab[1] + programsTab[3] - 1);
  printCG(SOLID_BOTTOM_BORDER);    
  goToText(programsTab[0] + programsTab[2] - 1, programsTab[1] + programsTab[3] - 1);
  printCG(SOLID_BOTTOM_BORDER);  
  
  for (i = 0; i < 8;i++) {
    goToGraphic(11*6, 2*8 + i);
    for (j = 0; j < 8;j++) {
      display(0x3f); 
    }
  }
  
  goToText(programsTab[0] + programsTab[2], programsTab[1] + programsTab[3] - 1);
  printCG(BOTTOM_BORDER);
  printCG(HARD_LOWER_LEFT_BUTTON_CORNER);
  goToText(daysTab[0] + daysTab[2] - 1, daysTab[1] + daysTab[3] - 1);
  printCG(RIGHT_TAB_INTERSECTION);         
           
  drawButtonPrintLabel(program1Button, "Program 1");
  drawButtonPrintLabel(program2Button, "Program 2");
  drawButtonPrintLabel(program3Button, "Program 3");
  drawButtonPrintLabel(program4Button, "Program 4");
}

void drawDaysTab() {
  unsigned char i, j;
  
  clearPartialScreen();
  drawLeftBar();
  drawSolidButton(programButton);
  
  // Draw top stuff
  drawButton(programsTab);
  goToText(11, 2);
  printStr("Programs");
  
  drawSolidButton(daysTab);
  goToText(28, 2);
  printStr("  Days  ");
  
  // Draw border
  for (i = 3; i < 16; i++) {
    goToText(7, i);
    printCG(LEFT_BORDER);
    goToText(39, i);
    printCG(RIGHT_BORDER);
  }
  
  // Make top pretty
  goToText(daysTab[0], daysTab[1] + daysTab[3] - 1);
  printCG(SOLID_BOTTOM_BORDER);    
  goToText(daysTab[0] + daysTab[2] - 1, daysTab[1] + daysTab[3] - 1);
  printCG(SOLID_BOTTOM_BORDER);  
  
  for (i = 0; i < 8;i++) {
    goToGraphic(28*6, 2*8 + i);
    for (j = 0; j < 8;j++) {
      display(0x3f); 
    }
  }
  
  goToText(programsTab[0], programsTab[1] + programsTab[3] - 1);
  printCG(LEFT_TAB_INTERSECTION);
  goToText(programsTab[0] + programsTab[2] - 1, programsTab[1] + programsTab[3] - 1);
  printCG(HARD_LOWER_RIGHT_BUTTON_CORNER);
  printCG(BOTTOM_BORDER);
         
  drawButton(monButton);
  goToText(monButton[0] + 2, monButton[1] + 1);
  printNum(weeklySchedule[0] + 1);
  goToText(monButton[0] + 1, monButton[1] - 1);
  printStr("Mon");
  
  drawButton(wedButton);
  goToText(wedButton[0] + 2, wedButton[1] + 1);
  printNum(weeklySchedule[2] + 1);
  goToText(wedButton[0] + 1, wedButton[1] - 1);
  printStr("Wed");
  
  drawButton(friButton);
  goToText(friButton[0] + 2, friButton[1] + 1);
  printNum(weeklySchedule[4] + 1);
  goToText(friButton[0] + 1, friButton[1] - 1);
  printStr("Fri");
  
  drawButton(satButton);
  goToText(satButton[0] + 2, satButton[1] + 1);
  printNum(weeklySchedule[5] + 1);
  goToText(satButton[0] + 1, satButton[1] - 1);
  printStr("Sat");
  
  drawButton(sunButton);
  goToText(sunButton[0] + 2, sunButton[1] + 1);
  printNum(weeklySchedule[6] + 1);
  goToText(sunButton[0] + 1, sunButton[1] - 1);
  printStr("Sun");
  
  drawButton(tueButton);
  goToText(tueButton[0] + 2, tueButton[1] + 1);
  printNum(weeklySchedule[1] + 1);
  goToText(tueButton[0] + 1, tueButton[1] + tueButton[3]);
  printStr("Tue");
  
  drawButton(thuButton);
  goToText(thuButton[0] + 2, thuButton[1] + 1);
  printNum(weeklySchedule[3] + 1);
  goToText(thuButton[0] + 1, thuButton[1] + thuButton[3]);
  printStr("Thu");
}

void drawDaysMenu(unsigned char* menu) {
  char i;
  
  drawVerticalMenu(menu);
  
  for (i = 0; i < menu[4]; i++) { 
    goToText(menu[0] + 2, menu[1] + 1 + menu[3]*i);
    printNum(i + 1);
  }
}

void drawProgrammingScreen(unsigned char programIndex) {
  unsigned char time, i, j;
  
  clearPartialScreen();
  
  drawButton(okButton);
  drawGraphic((okButton[0]+1)*6, (okButton[1])*8 + 6, 2, 12, check);
  
  goToText(17, 1);
  printStr("Program ");
  
  drawButtonPrintHours(hour1Button, rooms[currentRoom].programs[programIndex].periods[0].startTime);
  drawButtonPrintHours(hour2Button, rooms[currentRoom].programs[programIndex].periods[1].startTime);
  drawButtonPrintHours(hour3Button, rooms[currentRoom].programs[programIndex].periods[2].startTime);
  drawButtonPrintHours(hour4Button, rooms[currentRoom].programs[programIndex].periods[3].startTime);
  
  goToText(hour1Button[0] + hour1Button[2], hour1Button[1] + 1);
  printChar(':');
  goToText(hour2Button[0] + hour2Button[2], hour2Button[1] + 1);
  printChar(':');
  goToText(hour3Button[0] + hour3Button[2], hour3Button[1] + 1);
  printChar(':');
  goToText(hour4Button[0] + hour4Button[2], hour4Button[1] + 1);
  printChar(':');
  
  drawButtonPrintMinutes(minutes1Button, rooms[currentRoom].programs[programIndex].periods[0].startTime);
  drawButtonPrintMinutes(minutes2Button, rooms[currentRoom].programs[programIndex].periods[1].startTime);
  drawButtonPrintMinutes(minutes3Button, rooms[currentRoom].programs[programIndex].periods[2].startTime);
  drawButtonPrintMinutes(minutes4Button, rooms[currentRoom].programs[programIndex].periods[3].startTime);

  drawButton(amPm1Button);
  goToText(amPm1Button[0] + 1, amPm1Button[1] + 1);
  printAmPm(rooms[currentRoom].programs[programIndex].periods[0].startTime);
  drawButton(amPm2Button);
  goToText(amPm2Button[0] + 1, amPm2Button[1] + 1);
  printAmPm(rooms[currentRoom].programs[programIndex].periods[1].startTime);
  drawButton(amPm3Button);
  goToText(amPm3Button[0] + 1, amPm3Button[1] + 1);
  printAmPm(rooms[currentRoom].programs[programIndex].periods[2].startTime);
  drawButton(amPm4Button);
  goToText(amPm4Button[0] + 1, amPm4Button[1] + 1);
  printAmPm(rooms[currentRoom].programs[programIndex].periods[3].startTime);
  
  drawButtonPrintNumber(temp1Button, rooms[currentRoom].programs[programIndex].periods[0].temperature);
  drawButtonPrintNumber(temp2Button, rooms[currentRoom].programs[programIndex].periods[1].temperature);
  drawButtonPrintNumber(temp3Button, rooms[currentRoom].programs[programIndex].periods[2].temperature);
  drawButtonPrintNumber(temp4Button, rooms[currentRoom].programs[programIndex].periods[3].temperature);
  
  drawButton(mode1Button);
  goToText(mode1Button[0] + 1, mode1Button[1] + 1);
  printMode(rooms[currentRoom].programs[programIndex].periods[0].mode);
  drawButton(mode2Button);
  goToText(mode2Button[0] + 1, mode2Button[1] + 1);
  printMode(rooms[currentRoom].programs[programIndex].periods[1].mode);
  drawButton(mode3Button);
  goToText(mode3Button[0] + 1, mode3Button[1] + 1);
  printMode(rooms[currentRoom].programs[programIndex].periods[2].mode);
  drawButton(mode4Button);
  goToText(mode4Button[0] + 1, mode4Button[1] + 1);
  printMode(rooms[currentRoom].programs[programIndex].periods[3].mode);
  
  goToText(hour1Button[0] + 2, hour1Button[1] - 1);
  printStr("Start Time");
  goToText(temp1Button[0], temp1Button[1] - 1);
  printStr("Temp");
  goToText(mode1Button[0] + 1, mode1Button[1] - 1);
  printStr("Mode");
}

void printMenuCells(unsigned char* menu, unsigned char menuMode, unsigned char num) {
  unsigned char i, j;
  
  switch (menuMode) {
    case 1:
      i = 0;
      j = 4;
      drawGraphic(menu[0]*FONT_WIDTH, menu[1]*8 + menu[3]*4*8, 4, 19, downArrow);
      break;
    case 2:
      i = 1;
      j = 4;
      drawGraphic(menu[0]*FONT_WIDTH, menu[1]*8, 4, 19, upArrow);
      drawGraphic(menu[0]*FONT_WIDTH, menu[1]*8 + menu[3]*4*8, 4, 19, downArrow);
      break;
    case 3:
      i = 1;
      j = 5;
      drawGraphic(menu[0]*FONT_WIDTH, menu[1]*8, 4, 19, upArrow);
      break;   
  }
  
  for (; i < j; i++) {
    goToText(menu[0] + 2, menu[1] + 1 + menu[3]*i);
    printNum(num++); 
  }
}

unsigned char determineHoursMenu(unsigned char time) {
  if (time > 48)
    time -= 48;
  if (time < 20)
    return 1;
  else if (time < 32)
    return 2;
  else if (time < 44)
    return 3;
  else
    return 4;
  return 0;
}

void drawHoursMenu(unsigned char menu) {

  clearArea(hoursMenu[0], hoursMenu[1], hoursMenu[2], hoursMenu[3]);
  clearArea(hoursMenu[0], hoursMenu[1] + hoursMenu[3]*(hoursMenu[4] - 1), hoursMenu[2], hoursMenu[3]);
  drawVerticalMenu(hoursMenu);
  
  switch (menu) {
    case 1:
      printMenuCells(hoursMenu, 1, 1);
      break;
    case 2:
      printMenuCells(hoursMenu, 2, 5);
      break;
    case 3:
      printMenuCells(hoursMenu, 2, 8);
      break;
    case 4:
      drawGraphic(hoursMenu[0]*FONT_WIDTH, hoursMenu[1]*8, 4, 19, upArrow);
      goToText(hoursMenu[0] + 2, hoursMenu[1] + 1 + hoursMenu[3]);
      printNum(11);
      goToText(hoursMenu[0] + 2, hoursMenu[1] + 1 + hoursMenu[3]*2);
      printNum(12);
      break;
  }

}

void updateHours(unsigned char* startTime, unsigned char newHour) {
  unsigned char tempTime = *startTime % 4;
  *startTime = newHour*4 + tempTime;
}

void drawMinutesMenu(unsigned char startTime) {
  drawVerticalMenu(minutesMenu);
  
  goToText(minutesMenu[0] + 1, minutesMenu[1] + 1);
  printStr("00");
  goToText(minutesMenu[0] + 1, minutesMenu[1] + 1 + minutesMenu[3]);
  printStr("15");
  goToText(minutesMenu[0] + 1, minutesMenu[1] + 1 + minutesMenu[3]*2);
  printStr("30");
  goToText(minutesMenu[0] + 1, minutesMenu[1] + 1 + minutesMenu[3]*3);
  printStr("45");
}

void updateMinutes(unsigned char* startTime, unsigned char newMinute) {
  *startTime -= (*startTime % 4);
  *startTime += newMinute;
}

void toggleAmPm(unsigned char program, unsigned char period) {
  if (rooms[currentRoom].programs[program].periods[period].startTime < 48)
    rooms[currentRoom].programs[program].periods[period].startTime += 48;
  else
    rooms[currentRoom].programs[program].periods[period].startTime -= 48;
}

unsigned char determineTempMenu(unsigned char temp) {
  if (temp < 64)
    return 1;
  else if (temp < 67)
    return 2;
  else if (temp < 70)
    return 3;
  else if (temp < 73)
    return 4;
  else if (temp < 76)
    return 5;
  else if (temp < 79)
    return 6;
  else if (temp < 82)
    return 7;
  else if (temp < 85)
    return 8;
  else if (temp >= 85)
    return 9;
  return 0;
}

void drawProgrammingTempMenu(unsigned char menu) {
  drawVerticalMenu(programmingTempMenu);
  
  switch (menu) {
    case 1:
      printMenuCells(programmingTempMenu, 1, 60);
      break;
    case 2:
      printMenuCells(programmingTempMenu, 2, 64);
      break;
    case 3:
      printMenuCells(programmingTempMenu, 2, 67);
      break;
    case 4:
      printMenuCells(programmingTempMenu, 2, 70);
      break;
    case 5:
      printMenuCells(programmingTempMenu, 2, 73);
      break;
    case 6:
      printMenuCells(programmingTempMenu, 2, 76);
      break;
    case 7:
      printMenuCells(programmingTempMenu, 2, 79);
      break;
    case 8:
      printMenuCells(programmingTempMenu, 2, 82);
      break;
    case 9:
      printMenuCells(programmingTempMenu, 3, 85);
      break;
  }
}

void updateProgrammingTemp(unsigned char* temperature, unsigned char newTemperature) {
  *temperature = newTemperature;
}

void drawProgrammingModeMenu(unsigned char currentPeriodEditing) {
  unsigned char menu[5];
  menu[0] = programmingModeMenu[0];
  menu[1] = programmingModeMenu[1] + 3*currentPeriodEditing;
  menu[2] = programmingModeMenu[2];
  menu[3] = programmingModeMenu[3];
  menu[4] = programmingModeMenu[4];
  drawHorizontalMenu(menu);
  
  goToText(menu[0] + 1, menu[1] + 1);
  printStr("HEAT");
  goToText(menu[0] + 7, menu[1] + 1);
  printStr("COOL");
  goToText(menu[0] + 13, menu[1] + 1);
  printStr("OFF");
}

void drawSettingsScreen() {
  clearPartialScreen();
  drawLeftBar();
  drawSolidButton(settingsButton);
}
