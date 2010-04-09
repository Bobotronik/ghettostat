#include "derivative.h"
#include "lcd.h"  

#include "functions.h"
#include "pins.h"
#include "i2cDevices.h"
#include <stdlib.h>

const unsigned char degree[] = {0x08, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char largeDegree[] = {0x0c, 0x12, 0x21, 0x21, 0x12, 0x0c, 0x00, 0x00};
const unsigned char upperLeftCorner[] = {0x00, 0x00, 0x03, 0x0c, 0x10, 0x10, 0x20, 0x20};
const unsigned char upperRightCorner[] = {0x00, 0x00, 0x30, 0x0c, 0x02, 0x02, 0x01, 0x01};
const unsigned char lowerLeftCorner[] = {0x20, 0x20, 0x10, 0x10, 0x0c, 0x03, 0x00, 0x00};
const unsigned char lowerRightCorner[] = {0x01, 0x01, 0x02, 0x02, 0x0c, 0x30, 0x00, 0x00};
const unsigned char largeZero[] = {0x00};
const unsigned char largeOne[] = {0x00};
const unsigned char largeTwo[] = {0x00};
const unsigned char largeThree[] = {0x00};
const unsigned char largeFour[] = {0x00};
const unsigned char largefive[] = {0x00};
const unsigned char largeSix[] = {0x00};
const unsigned char largeSeven[] = {0x00};
const unsigned char largeEight[] = {0x00};
const unsigned char largeNine[] = {0x00};

unsigned char charAbs(int num) {
  unsigned char absNum;
  
  if (num < 0) {
    num = -num;
    absNum = num & 0xff;
  }
  else {
    absNum = num & 0xff;
  }
  
  return absNum;
}

void wait(unsigned char cycles){
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

void printChar(char character) {
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
  unsigned char dividend, remainder, neg;
  
  if (num < 0) {
    dividend = -num;
    neg = 1;
  }
  else {
    dividend = num;
    neg = 0;
  }
  
  do {
    remainder = dividend % 10;
    dividend /= 10;
    writeData(remainder + 16);
    writeCommand(DATA_WRITE_AND_DECREMENT);
  } while(dividend != 0);
  
  if (neg)
    printChar('-');
}

void printBCD(unsigned char bcd) {
  display(bcd + 16);
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
/*
void clearArea(unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
  unsigned char i, j;
  unsigned char textWidth = width/FONT_WIDTH;
  unsigned char textHeight = height/8;
  
  goToText(x/FONT_WIDTH,  y/8);
  for (i = 0; i < textHeight; i++) {
    for (j = 0; j < textWidth; j++) {
      display(0x00);
    }
  } 
  
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      display(0x00);
    }
  } 
  
  goToGraphic(x, y);
  
}
*/

void clearScreen() {
  clearText();
  clearGraphic();
}
    
void clearCG() {
  unsigned int i;
  setADP(CG_RAM_HOME);
  for(i = 0; i < 256; i++)
    display(0x00);
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

unsigned char getX(){
  unsigned char x[9];
  unsigned char i;
  
  TS_LEFT_DIR = 1;
  TS_RIGHT_DIR = 1;
  TS_TOP_DIR = 0;
  TS_BOTTOM_DIR = 0;
  
  TS_LEFT = 0;
  TS_RIGHT = 1;
   
  wait(100);
  
  for (i = 0; i < 9; i++) {
    x[i] = convertAD(TS_X_INPUT) >> 1;  
    wait(100);
  }
  qsort(x, 9, sizeof(unsigned char), compare);
  return x[4];
}

unsigned char getY(){
  unsigned char y;
  
  TS_TOP_DIR = 1;
  TS_BOTTOM_DIR = 1;
  TS_LEFT_DIR = 0;
  TS_RIGHT_DIR = 0;
  
  TS_TOP = 0;
  TS_BOTTOM = 1;
  
  wait(100);
  
  y = convertAD(TS_Y_INPUT) >> 2;
  return y;
}

unsigned char isTouched(){
  if (getX() == 127)
    return 0;
  return 1;
}

void initializeCG() {
  unsigned char i;
  
  setADP(CG_RAM_HOME | (UPPER_LEFT_CORNER << CHAR_SHIFT));
  for (i = 0; i < 8; i++) {
    writeData(upperLeftCorner[i]);
    writeCommand(DATA_WRITE_AND_INCREMENT); 
  }
  
  setADP(CG_RAM_HOME | (UPPER_RIGHT_CORNER << CHAR_SHIFT));
  for (i = 0; i < 8; i++) {
    writeData(upperRightCorner[i]);
    writeCommand(DATA_WRITE_AND_INCREMENT); 
  }
  
  setADP(CG_RAM_HOME | (LOWER_LEFT_CORNER << CHAR_SHIFT));
  for (i = 0; i < 8; i++) {
    writeData(lowerLeftCorner[i]);
    writeCommand(DATA_WRITE_AND_INCREMENT); 
  }
  
  setADP(CG_RAM_HOME | (LOWER_RIGHT_CORNER << CHAR_SHIFT));
  for (i = 0; i < 8; i++) {
    writeData(lowerRightCorner[i]);
    writeCommand(DATA_WRITE_AND_INCREMENT); 
  }
  
  setADP(CG_RAM_HOME | (DEGREE << CHAR_SHIFT));
  for (i = 0; i < 8; i++) {
    writeData(degree[i]);
    writeCommand(DATA_WRITE_AND_INCREMENT); 
  }
}

void initializeTS() {
  // Put ADC high speed mode, input clock / 1, internal clock, 8-bit mode,
  // short sample time, synchronous clock
  ADCLK = 0x10;
}

void initializeDisplay() {

  // Set control lines as output
  LCD_CD_DIR = 1;
  LCD_RD_DIR = 1;
  LCD_WR_DIR = 1;
  LCD_CE_DIR = 1; 
  
  // Set font select to 6
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
  
  // Mode Set
  writeCommand(OR_MODE);
  
  // Display Mode
  writeCommand(TEXT_ON_GRAPHIC_ON);
  
  initializeCG();
  
  // Test Program
  clearText();
  clearGraphic();
  goToText(1, 1);
  printCG(UPPER_LEFT_CORNER);
  goToText(2, 1);
  printCG(UPPER_RIGHT_CORNER);
  goToText(1, 2);
  printCG(LOWER_LEFT_CORNER);
  goToText(2, 2);
  printCG(LOWER_RIGHT_CORNER);
  //goToText(2, 5);
  //printNum(69);
  //goToText(2, 6);
  //printNum(-69);
  //goToText(2, 6);
  //printNum(getX());
  //setPixel(20, 20, 1);
  //drawLine(5, 5, 100, 100);
  //drawLine(10, 10, 230, 100);
  //drawLine(10, 118, 230, 10);
  //drawLine(120, 0, 120, 127);
  //drawBox(9, 9, 219, 117);
  
  /* Address Pointer
  LCD_WRiteData(0x23);
  LCD_WRiteData(0x01);
  LCD_WRiteCommand(0x24);
  
  LCD_WRiteData(0x21);
  LCD_WRiteCommand(0xC0);*/
}
