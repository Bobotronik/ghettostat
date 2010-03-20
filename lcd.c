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
  
  // Graphics Home Address
  writeData(0x00);
  writeData(0x00);  
  writeCommand(0x42); 

  // Graphics Area Set
  writeData(0x1e);
  writeData(0x00);
  writeCommand(0x43);   
  
  // Text Home Address
  writeData(0x00);
  writeData(0x00);
  writeCommand(0x40);
  
  // Text Area Set
  writeData(0x1e);
  writeData(0x00);
  writeCommand(0x41);
  
  // Mode Set
  writeCommand(0x80);
  
  // Display Mode
  writeCommand(0x9f);
  
  /* Address Pointer
  readStatus();
  writeData(0x23);
  readStatus();
  writeData(0x01);
  readStatus();
  writeCommand(0x24);
  
  readStatus();
  writeData(0x21);
  readStatus();
  writeCommand(0xC0);*/
}
