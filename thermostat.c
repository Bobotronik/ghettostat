#include "derivative.h"
#include "thermostat.h"

#include "lcd.h"
#include "i2cDevices.h"

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
      printStr("SUN ");
      break;
    case 2:
      printStr("MON ");
      break;
    case 3:
      printStr("TUE ");
      break;
    case 4:
      printStr("WED ");
      break;
    case 5:
      printStr("THU ");
      break;
    case 6:
      printStr("FRI ");
      break;
    case 7:
      printStr("SAT ");
      break;
  }
  
  // Month
  temp = RTC_TIME[5];
  //temp += (temp >> 4);
  
  printDigit(temp);

  switch (temp) {
    case 1:
      printStr("JAN ");
      break;
    case 2:
      printStr("FEB ");
      break;
    case 3:
      printStr("MAR ");
      break;
    case 4:
      printStr("APR ");
      break;
    case 5:
      printStr("MAY ");
      break;
    case 6:
      printStr("JUN ");
      break;
    case 7:
      printStr("JUL ");
      break;
    case 8:
      printStr("AUG ");
      break;
    case 9:
      printStr("SEP ");
      break;
    case 10:
      printStr("OCT ");
      break;
    case 11:
      printStr("NOV ");
      break;
    case 12:
      printStr("DEC ");
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
  printStr("  ");
  
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

void drawButton(unsigned char x, unsigned char y, unsigned char width, unsigned char* label) {
  unsigned char i;
  
  goToText(x, y);
  printCG(UPPER_LEFT_CORNER);
  for (i = 0; i < width; i++){
    printCG(TOP_BORDER);
  }
  printCG(UPPER_RIGHT_CORNER);
  
  goToText(x, y + 1);
  printCG(LEFT_BORDER);
  printStr(label);
  printCG(RIGHT_BORDER);
  
  goToText(x, y + 2);
  printCG(LOWER_LEFT_CORNER);
  for (i = 0; i < width; i++){
    printCG(BOTTOM_BORDER);
  }
  printCG(LOWER_RIGHT_CORNER);
}

void drawMainScreen() {
  // Drawing mode buttons on left side
  unsigned char i;
  
  drawButton(0, 3, 8, "  MODE  ");
  drawButton(0, 6, 8, "PROGRAM ");
  drawButton(0, 9, 8, "SETTINGS");
  drawButton(11, 0, 4, "MAIN");
  drawButton(24, 0, 4, "AUX ");
  
  goToText(16, 3);
  printStr("Currently");
  goToText(16, 9);
  printStr("Set To");
  goToText(16, 12);
  printStr("Humidity");
  goToText(10, 15);
  displayTime();
}

void drawProgramScreen() {
  unsigned char i, j;

  drawButton(0, 3, 8, "  MAIN  ");
  drawButton(0, 6, 8, "PROGRAMS");
  drawButton(0, 9, 8, "  DAYS  ");
  
  goToText(12, 0);
  for (i = 0; i < 4; i++) {
    printCG(UPPER_LEFT_CORNER);
    for (j = 0; j < 4; j++) {
      printCG(TOP_BORDER);  
    }
    printCG(UPPER_RIGHT_CORNER);
    printChar(' ');
  }
  
  goToText(12, 1);
  for (i = 0; i < 4; i++) {
    printCG(LEFT_BORDER);
    for (j = 0; j < 4; j++) {
      printChar(' ');  
    }
    printCG(RIGHT_BORDER);
    printChar(' ');
  }
  
  goToText(12, 2);
  for (i = 0; i < 4; i++) {
    printCG(LEFT_BORDER);
    for (j = 0; j < 4; j++) {
      printChar(' ');  
    }
    printCG(RIGHT_BORDER);
    printChar(' ');
  }
  
  for (i = 4; i < 15; i++) {
    goToText(11, i); 
    printCG(LEFT_BORDER);
    goToText(39, i);
    printCG(RIGHT_BORDER);
  }
  
  for (i = 12; i < 38; i++) {
    goToText(i, 15);
    printCG(BOTTOM_BORDER); 
  }
  
  goToText(11,3);
  printCG(UPPER_LEFT_CORNER);
  goToText(39,3);
  printCG(UPPER_RIGHT_CORNER);
  goToText(11,15);
  printCG(LOWER_LEFT_CORNER);
  goToText(39,15);
  printCG(LOWER_RIGHT_CORNER);
  
}

void drawSettingsScreen() {
  drawButton(0, 3, 8, "  MAIN  ");
}
