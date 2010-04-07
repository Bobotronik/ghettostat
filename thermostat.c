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
  
  // Determine whether AM or PM;
  temp = RTC_TIME[2];
  if ((temp & 0x20) == 0x00) {
    pm = 0;
    tempTime = 0;
  }
  else {
    pm = 1;
    tempTime = 48;
  }
  
  // Determine whether to display nothing or 1
  if ((temp & 0x10) == 0x00) {
    display(NOTHING);
  }
  else {
    printNum(1);
    tempTime += 40;
  }
  
  // Display hour (1's digit)
  printNum(temp & 0x0f);
  tempTime += (temp & 0x0f);
  printChar(':');
 
  // Display minutes
  temp = RTC_TIME[1];
  printNum((temp & 0x70) >> 4);
  printNum(temp & 0x0f);
  tempMinutes = ((temp & 0x70) >> 4)*10;
  tempMinutes += temp & 0x0f;
  tempTime += tempMinutes/15;
  currentTime = tempTime;
  
  if (pm == 0) {
    printStr("AM");
  }
  else {
    printStr("PM");
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
    display(NOTHING);
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

void drawMainScreen() {
  // Drawing mode buttons on left side
  unsigned char i;
  unsigned char startX = 6;
  unsigned char startY = 18;
  unsigned char buttonWidth = 36;
  unsigned char buttonHeight = 20;
  
  for (i = 0; i < 4; i++) {
    drawBox(startX, startY, buttonWidth, buttonHeight);
    startY += 24;
  }
  
  goToText(2, 3);
  printStr("OFF");
  goToText(2, 6);
  printStr("HEAT");
  goToText(2, 9)  ;
  printStr("COOL");
  goToText(2, 12);
  printStr("CIRC");
   
}

void determineButtons() {
  unsigned char xTouch;
  unsigned char yTouch;
  
  // Determin state, then button
  /*if (state == MAIN) {
    xTouch = getX();
    if (xTouch < ) {
    
    }
    
    else if (xTouch > ) {
    
    }
    
    else
  }  */
}
