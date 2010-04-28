#include "derivative.h"
#include "lcd.h"  

#include "functions.h"
#include "pins.h"
#include "i2cDevices.h"

const unsigned char block[] = {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f};
const unsigned char degree[] = {0x08, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char largeDegree[] = {0x0c, 0x12, 0x21, 0x21, 0x12, 0x0c, 0x00, 0x00};

const unsigned char upperLeftCorner[] = {0x00, 0x00, 0x03, 0x0c, 0x10, 0x10, 0x20, 0x20};
const unsigned char upperRightCorner[] = {0x00, 0x00, 0x30, 0x0c, 0x02, 0x02, 0x01, 0x01};
const unsigned char lowerLeftCorner[] = {0x20, 0x20, 0x10, 0x10, 0x0c, 0x03, 0x00, 0x00};
const unsigned char lowerRightCorner[] = {0x01, 0x01, 0x02, 0x02, 0x0c, 0x30, 0x00, 0x00};

const unsigned char topBorder[] = {0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char bottomBorder[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00};
const unsigned char leftBorder[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
const unsigned char rightBorder[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};

const unsigned char solidUpperLeftCorner[] = {0x00, 0x00, 0x03, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f};
const unsigned char solidUpperRightCorner[] = {0x00, 0x00, 0x30, 0x3c, 0x3e, 0x3e, 0x3f, 0x3f};
const unsigned char solidLowerLeftCorner[] = {0x3f, 0x3f, 0x1f, 0x1f, 0x0f, 0x03, 0x00, 0x00};
const unsigned char solidLowerRightCorner[] = {0x3f, 0x3f, 0x3e, 0x3e, 0x3c, 0x30, 0x00, 0x00};

const unsigned char solidTopBorder[] = {0x00, 0x00, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f};
const unsigned char solidBottomBorder[] = {0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x00, 0x00};

const unsigned char hardLowerLeftCorner[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3f};
const unsigned char hardLowerRightCorner[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x3f};
const unsigned char hardUpperLeftCorner[] = {0x3f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};
const unsigned char hardUpperRightCorner[] = {0x3f, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
const unsigned char topMenuBorder[] = {0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char bottomMenuBorder[] =  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f};

const unsigned char hardLowerLeftButtonCorner[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x3f, 0x00, 0x00};
const unsigned char hardLowerRightButtonCorner[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x3f, 0x00, 0x00};
const unsigned char leftTabIntersection[] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x3f, 0x20, 0x20};
const unsigned char rightTabIntersection[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x3f, 0x01, 0x01};

unsigned char charAbs(int num) {  
  if (num < 0) {
    num = -num;
  }
  return num & 0xff;
}

void wait(unsigned int cycles){
  for (; cycles != 0; cycles--);
}

unsigned char readStatus() {
  unsigned char status;
  LCD_DB_DIR = 0x00;
  LCD_CD = 1;
  LCD_RD = 0;
  LCD_WR = 1;
  
  do {
    LCD_CE = 0;
    wait(1);
    status = LCD_DB;
    LCD_CE = 1;    
    status &= 0x03;     
  } while(status != 0x03);
           
  return 1;
}

void writeData(unsigned char data) {
  readStatus();
  LCD_DB_DIR = 0xff;
  LCD_DB = data; 
  LCD_CD = 0;
  LCD_RD = 1;
  LCD_WR = 0;
  LCD_CE = 0;
  LCD_CE = 1;
}

unsigned char readData() {
  unsigned char data;
  readStatus();
  LCD_DB_DIR = 0x00;
  LCD_CD = 0;
  LCD_RD = 0;
  LCD_WR = 1; 
  LCD_CE = 0; 
  wait(1);
  data = LCD_DB;
  LCD_CE = 1;
  
  return data; 
}

void writeCommand(unsigned char command) {
  readStatus();
  LCD_DB_DIR = 0xff;
  LCD_DB = command;
  LCD_CD = 1;
  LCD_RD = 1;
  LCD_WR = 0; 
  LCD_CE = 0;
  LCD_CE = 1; 
}

void setADP(unsigned short int ADP) {
  writeData(ADP & 0xff);
  writeData(ADP >> 8);
  writeCommand(SET_ADDRESS_POINTER);
}

void goToText(unsigned char x, unsigned char y) {
  unsigned short int address = TEXT_HOME + TEXT_AREA*y + x;
  setADP(address);
}

void goToGraphic(unsigned char x, unsigned char y) {
  unsigned short int address = GRAPHIC_HOME + GRAPHIC_AREA*y + x/FONT_WIDTH;
  setADP(address);
}

void display(unsigned char data) {
  writeData(data);
  writeCommand(DATA_WRITE_AND_INCREMENT);
}

void printChar(unsigned char character) {
  display(character - 32);
}

void printLargeChar(unsigned char x, unsigned char y, char character) {
  goToText(x, y);
  display(character);
  display(character + 1);
  goToText(x, y+1);
  display(character + 2);
  display(character + 3);
}

void printCG(char character) {
  display(character); 
}

void printNum(int num) {
  unsigned char number[6];
  unsigned char dividend, remainder, neg, i;
  
  if (num < 0) {
    dividend = -num;
    neg = 1;
  }
  else {
    dividend = num;
    neg = 0;
  }
  /*
  for (i = 5; i >= 0; i--) {
    remainder = dividend % 10;
    dividend /= 10;
    number[i] = remainder; 
    if (dividend == 0)
      break; 
  }
  
  if (neg)
    printChar('-');
  
  for ( ; i < 6; i++) {
    writeData(number[i] + 16);
    writeCommand(DATA_WRITE_AND_INCREMENT);  
  }
  */
  do {
    remainder = dividend % 10;
    dividend /= 10;
    writeData(remainder + 16);
    writeCommand(DATA_WRITE_AND_DECREMENT);
  } while(dividend != 0);
  
  if (neg)
    printChar('-');
}

void printDigit(unsigned char digit) {
  display(digit + 16);
}

void printBCD(unsigned char bcd) {
  display(((bcd & 0xf0) >> 4) + 16);
  display((bcd & 0x0f) + 16);
}

void printStr(char* string) {
  while (*string) {
    display(*string++ - 32);
  }
}

void clearText() {
  short unsigned int i;
  setADP(TEXT_HOME);  
  for (i = 0; i < TEXT_SIZE; i++)
    display(0x00);
}

void clearGraphic() {
  short unsigned int i;
  setADP(GRAPHIC_HOME);
  for (i = 0; i < GRAPHIC_SIZE; i++)
    display(0x00); 
}

unsigned char clearArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
  unsigned char i, j;
  
  for (i = 0; i < height; i++) {
    goToText(x, y + i);
    for (j = 0; j < width; j++) {
      display(0x00);
    }
  } 
  
  for (i = 0; i < (height << 3); i++) {
    goToGraphic(x*FONT_WIDTH, (y << 3) + i);
    for (j = 0; j < width; j++) {
      display(0x00);
    }
  }
}

void clearPartialScreen() {
  unsigned char i, j;
  for (i = 1; i < 16; i++) {
    goToText(6, i);
    for (j = 6; j < 40; j++) {
      display(0x00);
    }
  }
  
  for (i = 8; i < 128; i++) {
    goToGraphic(36, i);
    for (j = 6; j < 40; j++) {
      display(0x00);
    }
  }
}

void clearScreen() {
  clearText();
  clearGraphic();
}

void setPixel(unsigned char x, unsigned char y, unsigned char color) {
  unsigned char data;
  unsigned int address = GRAPHIC_HOME + GRAPHIC_AREA*y + x/FONT_WIDTH;

  setADP(address);

  writeCommand(DATA_READ_AND_NONVARIABLE);
  data = readData();

  if(color)
    data |= (1 << (FONT_WIDTH - (x % FONT_WIDTH) - 1));
  else
    data &= ~(1 << (FONT_WIDTH - (x % FONT_WIDTH) - 1));

  display(data);
}

void line(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
  unsigned char lessThan1, x, y;
  int dx, dy, d, delE, delNE;
  
  dx = x1 - x0;
  dy = y1 - y0;
  x = x0;
  y = y0;
  
  setPixel(x, y, 1);
  if (charAbs(dy) <= charAbs(dx)) {
    lessThan1 = 1; 
  } 
  else {
    lessThan1 = 0;  
  }
  // m = [0:1]
  if ( dy >= -dx && 0 >= dy ) {
    d = (dy << 1) + dx;
    delE = dy << 1;
    delNE = (dy + dx) << 1;

    if (lessThan1) {
      while(x < x1) {
        if(d >= 0) {
          d += delE; 
          x++;
        }
        else{
          d += delNE; 
          x++; 
          y--;
        }
        setPixel(x, y, 1);
      }
    } 
    else {
      while(x < x1) {
        if(d >= 0) {
          d += delE; 
          x++;
        }
        else{
          d += delNE; 
          x++; 
          y--;
        }  
        setPixel(y, x, 1);
      }
    }
  }
  // m = [-1:0)
  else if (dy <= dx && 0 < dy) {
    d = (dy << 1) - dx;
    delE = dy << 1;
    delNE = (dy - dx) << 1;

    if (lessThan1) {
      while(x < x1) {
        if(d <= 0) {
          d += delE; 
          x++;
        }
        else{
          d += delNE; 
          x++; 
          y++;
        }  
        setPixel(x, y, 1);
      }
    }
    else {
      while(x < x1) {
        if(d <= 0) {
          d += delE; 
          x++;
        }
        else{
          d += delNE; 
          x++; 
          y++;
        }  
        setPixel(y, x, 1);
      }  
    }
  }
}

void drawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
  int dx, dy;
  unsigned char lessThan1, y, i;

  if (y0 <= y1) {
    dx = x1 - x0;
    dy = y1 - y0;
    y = y0;
  }
  else {
    dx = x0 - x1;
    dy = y0 - y1;
    y = y1;
  }
  
  if (dy <= charAbs(dx))
    lessThan1 = 1;
  else {
    // Draw vertical line
    if (x0 == x1) {
      for (i = 0; i <= dy;) {
        setPixel(x0, y, 1);
        i++;
        y++;
      } 
      return;
    }
    lessThan1 = 0;
  }
    
  if (lessThan1) {
    if (x0 < x1) {
      line(x0, y0, x1, y1);
    }
    else {
      line(x1, y1, x0, y0);
    }
  }
  else {
    if (y0 < y1) {
      line(y0, x0, y1, x1);
    }
    else {
      line(y1, x1, y0, x0);
    }
  }
}

void drawBox(unsigned char x0, unsigned char y0, unsigned char width, unsigned char height) {
  drawLine(x0, y0, x0+width-1, y0);
  drawLine(x0+width-1, y0, x0+width-1, y0+height-1);
  drawLine(x0, y0, x0, y0+height-1);
  drawLine(x0, y0+height-1, x0+width-1, y0+height-1);
}

void drawGraphic(unsigned char startX, unsigned char startY, unsigned char width, unsigned char height, unsigned char* graphic) {
  unsigned char i, j;
  
  for (i = 0; i < height; i++) { 
    goToGraphic(startX, startY + i);
    for (j = 0; j < width; j++) {
      display(graphic[i*width + j]);  
    }  
  }
}

unsigned char getX(){
  unsigned char i;
  
  TS_LEFT_DIR = 1;
  TS_RIGHT_DIR = 1;
  TS_TOP_DIR = 0;
  TS_BOTTOM_DIR = 0;
  
  TS_LEFT = 0;
  TS_RIGHT = 1;
   
  wait(100);
  
  return (convertAD(TS_X_INPUT) >> 1); 
}

unsigned char getY(){
  unsigned char i;
  
  TS_TOP_DIR = 1;
  TS_BOTTOM_DIR = 1;
  TS_LEFT_DIR = 0;
  TS_RIGHT_DIR = 0;
  
  TS_TOP = 0;
  TS_BOTTOM = 1;
  
  wait(100);

  return (convertAD(TS_Y_INPUT) >> 2);
}

unsigned char isScreenTouched() {
  unsigned char i, current, previous;
  
  TS_LEFT_DIR = 1;
  TS_RIGHT_DIR = 1;
  TS_TOP_DIR = 0;
  TS_BOTTOM_DIR = 0;
  
  TS_LEFT = 0;
  TS_RIGHT = 1;
   
  wait(100);
  
  /* if touched 50 times in a row, return true
     else return false
  */
  previous = convertAD(TS_X_INPUT) >> 1;
  
  for (i = 0; i < 50; i++) {
    current = convertAD(TS_X_INPUT) >> 1;
    if (charAbs(current - previous) > 2) {
      return 0; 
    }   
    previous = current;
    wait(50);
  }
  return current;
}

unsigned char isScreenReleased() {
  unsigned char i, current, previous;
  
  TS_LEFT_DIR = 1;
  TS_RIGHT_DIR = 1;
  TS_TOP_DIR = 0;
  TS_BOTTOM_DIR = 0;
  
  TS_LEFT = 0;
  TS_RIGHT = 1;
   
  wait(100);
  
  previous = convertAD(TS_X_INPUT) >> 1;
  
  for (i = 0; i < 50; i++) {
    current = convertAD(TS_X_INPUT) >> 1;
    // if change is too small, break and return 0;
    if (charAbs(current - previous) < 2) {
      return 0;
    }   
    previous = current;
    wait(50);
  }
  return current;
}

void addCG(unsigned char* character, unsigned char address) {
  unsigned char i;
  setADP(CG_RAM_HOME | (address << CHAR_SHIFT));
  for (i = 0; i < 8; i++) {
    writeData(character[i]);
    writeCommand(DATA_WRITE_AND_INCREMENT); 
  }
}
void initializeCG() {
  unsigned char i;
  
  addCG(block, BLOCK);
  addCG(degree, DEGREE);
  addCG(largeDegree, LARGE_DEGREE);
  
  addCG(upperLeftCorner, UPPER_LEFT_CORNER);
  addCG(upperRightCorner, UPPER_RIGHT_CORNER);
  addCG(lowerLeftCorner, LOWER_LEFT_CORNER);
  addCG(lowerRightCorner, LOWER_RIGHT_CORNER);
  
  addCG(topBorder, TOP_BORDER);
  addCG(bottomBorder, BOTTOM_BORDER);
  addCG(leftBorder, LEFT_BORDER);
  addCG(rightBorder, RIGHT_BORDER);
  
  addCG(solidUpperLeftCorner, SOLID_UPPER_LEFT_CORNER);
  addCG(solidUpperRightCorner, SOLID_UPPER_RIGHT_CORNER);
  addCG(solidLowerLeftCorner, SOLID_LOWER_LEFT_CORNER);
  addCG(solidLowerRightCorner, SOLID_LOWER_RIGHT_CORNER);

  addCG(solidTopBorder, SOLID_TOP_BORDER);
  addCG(solidBottomBorder, SOLID_BOTTOM_BORDER);
  
  addCG(hardUpperLeftCorner, HARD_UPPER_LEFT_CORNER);
  addCG(hardUpperRightCorner, HARD_UPPER_RIGHT_CORNER);
  addCG(hardLowerLeftCorner, HARD_LOWER_LEFT_CORNER);
  addCG(hardLowerRightCorner, HARD_LOWER_RIGHT_CORNER);
  
  addCG(topMenuBorder, TOP_MENU_BORDER);
  addCG(bottomMenuBorder, BOTTOM_MENU_BORDER);
  
  addCG(hardLowerLeftButtonCorner, HARD_LOWER_LEFT_BUTTON_CORNER);
  addCG(hardLowerRightButtonCorner, HARD_LOWER_RIGHT_BUTTON_CORNER);
  addCG(leftTabIntersection, LEFT_TAB_INTERSECTION);
  addCG(rightTabIntersection, RIGHT_TAB_INTERSECTION);
}

void initializeTS() {
  // Put ADC high speed mode, input clock / 1, internal clock, 8-bit mode,
  // short sample time, synchronous clock
  ADCLK = 0x10;
}

void initializeDisplay() {

  LCD_CD_DIR = 1;
  LCD_RD_DIR = 1;
  LCD_WR_DIR = 1;
  LCD_CE_DIR = 1; 

  setBitsPortX(P_FONTSEL);
  
  // Reset
  clearBitsPortX(P_RESET);
  wait(10);
  setBitsPortX(P_RESET);
  wait(10);

  writeData(GRAPHIC_HOME & 0xff);
  writeData(GRAPHIC_HOME >> 8); 
  writeCommand(SET_GRAPHIC_HOME_ADDRESS); 

  writeData(GRAPHIC_AREA);
  writeData(0x00);
  writeCommand(SET_GRAPHIC_AREA); 
  
  writeData(TEXT_HOME & 0xff);
  writeData(TEXT_HOME >> 8);
  writeCommand(SET_TEXT_HOME_ADDRESS);

  writeData(TEXT_AREA);
  writeData(0x00);
  writeCommand(SET_TEXT_AREA);  
  
  writeData(OFFSET_REGISTER);
  writeData(0x00);
  writeCommand(SET_OFFSET_REGISTER);

  writeCommand(XOR_MODE);

  writeCommand(TEXT_ON_GRAPHIC_ON);
  
  initializeCG();
  clearText();
  clearGraphic(); 
}
