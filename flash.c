#include "flash.h"
#include "derivative.h"
#include "thermostat.h"

// RAM Data Block
unsigned char F_BUS_SPD;
unsigned char F_DATASIZE;
unsigned int  F_ADDR;
unsigned char F_DATA[12];
unsigned int* F_FILE_PTR = (unsigned int*) &F_BUS_SPD;

void initFlash(void) {
  F_BUS_SPD = 10;
  F_DATASIZE = 12;
}

// Low level functions ------------------------------------  
void loadFlash(void) {
  asm(LDHX F_FILE_PTR);    /* Load address of RAM Data Block to H:X */
  asm(JSR F_LOAD);       /* Call F_LOAD ROM Subroutine */
}

void eraseFlash(void) {
  asm(LDHX F_FILE_PTR);    /* Load address of RAM Data Block to H:X */
  asm(JSR F_ERASE);      /* Call F_ERASE ROM Subroutine */
}

void writeFlash(void) {
  asm(LDHX F_FILE_PTR);    /* Load address of RAM Data Block to H:X */
  asm(JSR F_WRITE);      /* Call F_PROGRAM ROM Subroutine */        
}

// Mid level functions ------------------------------------
void loadProgramData(void) {
  unsigned char periodCount, programCount, roomCount;
  
  F_ADDR = F_START;
  for(roomCount = 0; roomCount < 2; roomCount++) { // 2 rooms
    for(programCount = 0; programCount < 4; programCount++) { // 4 programs
      // Load the current program
      loadFlash();
      for(periodCount = 0; periodCount < 4; periodCount++) {  // 4 periods
        rooms[roomCount].programs[programCount].periods[periodCount].startTime    = F_DATA[3*periodCount];
        rooms[roomCount].programs[programCount].periods[periodCount].temperature  = F_DATA[3*periodCount+1];
        rooms[roomCount].programs[programCount].periods[periodCount].mode         = F_DATA[3*periodCount+2];
      }
      // Increment the FLASH address
      F_ADDR += 12;
    }
  }
    
}

void saveProgramData(void) {
  unsigned char periodCount, programCount, roomCount;
  
  F_ADDR = F_START;
  // Erase the 64-byte blocks
  eraseFlash();
  F_ADDR += 64;
  eraseFlash();
  F_ADDR = F_START;
  
  // Save the current program to FLASH
  for(roomCount = 0; roomCount < 2; roomCount++) {
    for(programCount = 0; programCount < 4; programCount++) {
      for(periodCount = 0; periodCount < 4; periodCount++) {
        F_DATA[3*periodCount]   = rooms[roomCount].programs[programCount].periods[periodCount].startTime;
        F_DATA[3*periodCount+1] = rooms[roomCount].programs[programCount].periods[periodCount].temperature;
        F_DATA[3*periodCount+2] = rooms[roomCount].programs[programCount].periods[periodCount].mode;
      }
      
      // Write to the flash and increment address
      writeFlash();
      F_ADDR += 12;
    }
  } 
}