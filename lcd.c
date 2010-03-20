#include "derivative.h"
#include "lcd.h"

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
  
  // Text Home Address
  writeData(TEXT_HOME_ADDRESS);
  writeData(TEXT_HOME_ADDRESS >> 8);
  writeCommand(SET_TEXT_HOME_ADDRESS);
  
  // Text Area Set
  writeData(TEXT_AREA);
  writeData(0x00);
  writeCommand(SET_TEXT_AREA);
  
  // Graphics Home Address
  writeData(GRAPHICS_HOME_ADDRESS);
  writeData(GRAPHICS_HOME_ADDRESS >> 8);  
  writeCommand(SET_GRAPHICS_HOME_ADDRESS); 

  // Graphics Area Set
  writeData(GRAPHICS_AREA);
  writeData(0x00);
  writeCommand(SET_GRAPHICS_AREA);   
  
  // Mode Set
  writeCommand(OR_MODE);
  
  // Display Mode
  writeCommand(TEXT_ON_GRAPHIC_ON);
  
  /* Address Pointer
  writeData(0x23);
  writeData(0x01);
  writeCommand(0x24);
  
  writeData(0x21);
  writeCommand(0xC0);*/
}
