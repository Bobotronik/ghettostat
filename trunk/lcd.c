#include "derivative.h"
#include "lcd.h"  

#include "pins.h"

unsigned char charAbs(short int num) {
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
  for (; cycles != 0; cycles--) {
  
  }
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

void printNum(unsigned char num) {
  display(num + 16);
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
    data |= (1 << (FONT_WIDTH - (x%FONT_WIDTH) - 1));
  else
    data &= ~(1 << (FONT_WIDTH - (x%FONT_WIDTH) - 1));

  display(data);
}

void drawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
  short int dx, dy, x, y;
  unsigned char lessThan1;
  short int d, delE, delNE;
  
  if (y0 <= y1) {
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;
    y = y0;
  }
  else {
    dx = x0 - x1;
    dy = y0 - y1;
    x = x1;
    y = y1;
  }
    
  setPixel(x, y, 1);

  // Draw vertical line
  if (x0 == x1) {
    for (; y <= y1; y++) {
      setPixel(x0, y, 1);
    } 
    return;
  }
  
  if (charAbs(dy) <= charAbs(dx))
    lessThan1 == 0;
  else
    lessThan1 == 1;
    
  // m = [0:1]
  if ( dy >= -dx && 0 >= dy ) {
    d = (dy+dx) << 1;
    delE = dy << 1;
    delNE = (dy+dx) << 1;
    
    if (lessThan1) {
      for (; x <= x1; ) {
        if (d >= 0) {
          d += delE; 
        }
        else {
          d += delNE; 
          y--;
        }
        x++;
        setPixel(x, y, 1);
      }
    }
    else {
      for(; x <= x1; x++) {
        if(d >= 0) {
          d += delE; 
        }
        else {
          d += delNE; 
          y--;
        }
        x++;
        setPixel(y, x, 1);
      }
    }
  }
  // m = [-1:0)
  else if (dy <= dx && 0 < dy) {
    d = (dy-dx) << 1;
    delE = dy << 1;
    delNE = (dy-dx) << 1;
    
    if (lessThan1) {
      for (; x <= x1; x++) {
        if (d <= 0) {
          d += delE; 
        }
        else {
          d += delNE; 
          y++;
        }
        x++;
        setPixel(x, y, 1);
      }   
    }
    else {
      for (; x <= x1; x++) {
        if (d <= 0) {
          d += delE; 
        }
        else {
          d += delNE; 
          y++;
        }
        x++;
        setPixel(y, x, 1);
      }
    }
  }
}

void drawBox(unsigned char x0, unsigned char y0, unsigned char width, unsigned char height) {
  drawLine(x0, y0, x0+width, y0);
  drawLine(x0+width, y0, x0+width, y0+height);
  drawLine(x0, y0, x0, y0+height);
  drawLine(x0, y0+height, x0+width, y0+height);
}

unsigned char isTouched(){
  if (getX() < MIN_TOUCH && getY() < MIN_TOUCH)
    return 0;
  return 1;
}

unsigned char getX(){
  unsigned char command;
  
  TS_LEFT_DIR = 1;
  TS_RIGHT_DIR = 1;
  TS_TOP_DIR = 0;
  TS_BOTTOM_DIR = 0;
  
  TS_LEFT = 0;
  TS_RIGHT = 1;
   
  // delay
  
  return convertAD(TS_X_INPUT);
}

unsigned char getY(){
  TS_TOP_DIR = 1;
  TS_BOTTOM_DIR = 1;
  TS_LEFT_DIR = 0;
  TS_RIGHT_DIR = 0;
  
  TS_TOP = 0;
  TS_BOTTOM = 1;
  
  // delay
  
  return convertAD(TS_Y_INPUT);
}

void initializeTS(){
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
  clearBitsPortX(P_FONTSEL);
  
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
  
  // Mode Set
  writeCommand(OR_MODE);
  
  // Display Mode
  writeCommand(TEXT_ON_GRAPHIC_ON);
  
  // Test Program
  clearText();
  clearGraphic();
  goToText(2, 7);
  printStr("HELLO WORLD!");
  //setPixel(20, 20, 1);
  //drawLine(120, 0, 120, 127);
  //drawBox(9, 9, 219, 117);
  
  /* Address Pointer
  LCD_WRiteData(0x23);
  LCD_WRiteData(0x01);
  LCD_WRiteCommand(0x24);
  
  LCD_WRiteData(0x21);
  LCD_WRiteCommand(0xC0);*/
}
