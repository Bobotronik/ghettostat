#include "derivative.h"
#include "lcd.h"

void initializeCharMap(){
  charMap['0'] = 0x;
  charMap['1'] = 0x;
  charMap['2'] = 0x;
  charMap['3'] = 0x;
  charMap['4'] = 0x;
  charMap['5'] = 0x;
  charMap['6'] = 0x;
  charMap['7'] = 0x;
  charMap['8'] = 0x;
  charMap['9'] = 0x;
  charMap['a'] = 0x;
  charMap['b'] = 0x;
  charMap['c'] = 0x;
  charMap['d'] = 0x;
}

void wait(unsigned char cycles){
  for (;cycles == 0; cycles--){
  
  }
}

unsigned char readStatus(){    
  DDRB = 0x00;
  
  /*CD = 1;
  RD = 0;
  WR = 1;
  CE = 0;
  wait(1);
  while (PTB_PTB0 != 1 || PTB_PTB1 != 1){
    //return true; 
  }
  CE = 1;  */
  CD = 1;
  RD = 0;
  WR = 1;
  unsigned char status;
  
  do{
    CE = 0;
    wait(1);
    status = PTB;
    CE = 1;    
    status &= 0x03;     
  } while(status != 0x03);
  
  return 1;
}

void writeData(unsigned char data){
  readStatus();
  DDRB = 0xff;
  PTB = data; 
  CD = 0;
  WR = 0;
  RD = 1;
  CE = 0;
  CE = 1;
}

unsigned char readData(){
  readStatus();
  DDRB = 0x00;
  CD = 0;
  RD = 0;
  WR = 1; 
  CE = 0; 
  wait(1);
  unsigned char data = PTB;
  CE = 1;
  
  return data; 
}

void writeCommand(unsigned char command){
  readStatus();
  DDRB = 0xff;
  PTB = command;
  CD = 1;
  WR = 0;
  RD = 1; 
  CE = 0;
  CE = 1; 
}

void setADP(unsigned short int ADP){
  writeData(ADP & 0xff);
  writeData(ADP >> 8);
  writeCommand(SET_ADDRESS_POINTER);
}

void display(unsigned char data){
  writeData(data);
  writeCommand(WRITE_DATA_AND_INCREMENT);
}

void printChar(char character)[
  display(char - 32);
}

void printStr(char* string){
  while(*string){
    display(*string-32);
    string++;
  }
}

void printBox(unsigned char x, unsigned char y, unsigned char width, unsigned char height){

}

void goToText(unsigned char x, unsigned char y){
  unsigned short int address = TEXT_HOME + TEXT_AREA*y + x;
  setADP(address);
}

void goToGraphics(unsigned char x, unsigned char y){
  unsigned short int address = GRAPHIC_HOME + GRAPHIC_AREA*y + x/FONT_WIDTH;
  setADP(address);
}

void initializeDisplay(){

  // Set control lines as output
  DDRA_DDRA0 = 1;
  DDRA_DDRA1 = 1;
  DDRA_DDRA4 = 1;
  DDRA_DDRA5 = 1;  
  
  // Reset
  PTA_PTA5 = 0;
  wait(10);
  PTA_PTA5 = 1;
  wait(10);

  writeData(TEXT_HOME_ADDRESS & 0xff);
  writeData(TEXT_HOME_ADDRESS >> 8);
  writeCommand(SET_TEXT_HOME_ADDRESS);

  writeData(TEXT_AREA);
  writeData(0x00);
  writeCommand(SET_TEXT_AREA);
  
  writeData(GRAPHICS_HOME_ADDRESS & 0xff);
  writeData(GRAPHICS_HOME_ADDRESS >> 8);  

  writeData(GRAPHICS_AREA);
  writeData(0x00);
  writeCommand(SET_GRAPHICS_AREA);   
  
  // Mode Set
  writeCommand(OR_MODE);
  
  // Display Mode
  writeCommand(TEXT_ON_GRAPHIC_ON);
  
  // Test Program
  goToText(1, 1);
  display(0x21);
  
  /* Address Pointer
  writeData(0x23);
  writeData(0x01);
  writeCommand(0x24);
  
  writeData(0x21);
  writeCommand(0xC0);*/
}
