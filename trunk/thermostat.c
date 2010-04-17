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

unsigned char state;

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
  
  printDigit(temp);

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
  if (temp & 0xf0) { 
    printDigit(temp >> 4);
  }
  else {
    printChar(' ');  
  }
  printDigit(temp & 0x0f);
  printChar(' ');
  
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

void drawButton(unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
  unsigned char i;
  
  goToText(x, y);
  printCG(UPPER_LEFT_CORNER);
  for (i = 0; i < width - 2; i++){
    printCG(TOP_BORDER);
  }
  printCG(UPPER_RIGHT_CORNER);
  
  for (i = y + 1; i < y + height - 1; i++) {
    goToText(x, i);
    printCG(LEFT_BORDER);
    goToText(x + width - 1, i);
    printCG(RIGHT_BORDER); 
  }
  
  goToText(x, y + height - 1);
  printCG(LOWER_LEFT_CORNER);
  for (i = 0; i < width - 2; i++){
    printCG(BOTTOM_BORDER);
  }
  printCG(LOWER_RIGHT_CORNER);
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
  drawButton(34, 3, 6, 3);
  goToText(35, 2);
  printStr("ROOM");
  
  drawButton(34, 8, 6, 3);
  goToText(35, 7);
  printStr("FAN");
  
  drawButton(34, 13, 6, 3);
  goToText(35, 12);
  printStr("MODE");
  
  goToText(8, 4);
  printStr("Currently");

  goToText(20, 4);
  printStr("Set To");
  
  drawButton(20, 5, 10, 7);
}

void drawProgramsScreen() {
  unsigned char i, j;

  drawLeftBar();
  /*
  for (i = 1; i < 5; i++) {
    goToText(7, 1 + i*3);
    printCG(UPPER_LEFT_CORNER);
    printCG(TOP_BORDER);
    printCG(TOP_BORDER);
    
    goToText(7, 1 + i*3 + 1);
    printCG(LEFT_BORDER);
    printDigit(i);
    
    goToText(7, 1 + i*3 + 2);
    printCG(LOWER_LEFT_CORNER);
    printCG(BOTTOM_BORDER);
    printCG(BOTTOM_BORDER);
  }
  
  goToText(11, 5);
  printStr("Morning");
  goToText(11, 8);
  printStr("Afternoon");
  goToText(11, 11);
  printStr("Evening");
  goToText(11, 14);
  printStr("Night");
  */
  
  drawButton(12, 1, 10, 3);
  goToText(13, 2);
  printStr("Programs");
  drawButton(25, 1, 10, 3);
  goToText(26, 2);
  printStr("  Days  ");
  
  for (i = 0; i < 4; i++) {
    drawButton(18, 4 + i*3, 11, 3);
    goToText(19, 5 + i*3);
    printStr("Program ");
    printDigit(i); 
  }
  
}

void drawDaysScreen() {
  unsigned char i, j;
  
  drawLeftBar();
  
  drawButton(12, 1, 10, 3);
  goToText(13, 2);
  printStr("Programs");
  drawButton(25, 1, 10, 3);
  goToText(26, 2);
  printStr("  Days  ");
  
  for (i = 0; i < 9; i++) {
    drawButton(9 + i*4, 9, 3, 3); 
  }
  
}

void drawProgrammingScreen() {
  
}

void drawProgramDaysScreen() {
  
}
void drawSettingsScreen() {
  drawLeftBar();
}
