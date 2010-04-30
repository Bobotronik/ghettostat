#include "flash.h"
#include "derivative.h"
#include "thermostat.h"

// RAM Data Block
unsigned char F_BUS_SPD     @0x0064;
unsigned char F_DATASIZE    @0x0065;
unsigned int  F_ADDR        @0x0066;
unsigned char F_DATA[64]    @0x0068;
unsigned int* F_FILE_PTR    @0x0062 = (unsigned int*)(&F_BUS_SPD);

void initFlash(void) {
  F_BUS_SPD = 10;
  F_DATASIZE = 64;
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
  unsigned char periodCount, programCount, roomCount, lol;
  
  F_ADDR = F_START;
  for(roomCount = 0; roomCount < 2; roomCount++) { // 2 rooms
    // Load the current program
    loadFlash();
    for(programCount = 0; programCount < 4; programCount++) { // 4 programs
      for(periodCount = 0; periodCount < 4; periodCount++) {  // 4 periods
        rooms[roomCount].programs[programCount].periods[periodCount].startTime    = F_DATA[12*programCount+3*periodCount];
        rooms[roomCount].programs[programCount].periods[periodCount].temperature  = F_DATA[12*programCount+3*periodCount+1];
        rooms[roomCount].programs[programCount].periods[periodCount].mode         = F_DATA[12*programCount+3*periodCount+2];
      }
    }
    // Load the weekly schedule
    for(periodCount = 0; periodCount < 7; periodCount++) {
      //weeklySchedule[roomCount][periodCount] = F_DATA[48+periodCount];
    }
    // Increment the FLASH address 
    F_ADDR += 64;
  }
    
}

void saveProgramData(void) {
  unsigned char periodCount, programCount, roomCount;
  
  F_ADDR = F_START;
  
  // Save the current program to FLASH
  for(roomCount = 0; roomCount < 2; roomCount++) {
    for(programCount = 0; programCount < 4; programCount++) {
      for(periodCount = 0; periodCount < 4; periodCount++) {
        F_DATA[12*programCount+3*periodCount]   = rooms[roomCount].programs[programCount].periods[periodCount].startTime;
        F_DATA[12*programCount+3*periodCount+1] = rooms[roomCount].programs[programCount].periods[periodCount].temperature;
        F_DATA[12*programCount+3*periodCount+2] = rooms[roomCount].programs[programCount].periods[periodCount].mode;
      }
    }
    // Weekly schedule
    for(periodCount = 0; periodCount < 7; periodCount++) {
      //F_DATA[48+periodCount] = weeklySchedule[roomCount][periodCount];
    }
    // Erase, Write to the flash, and increment address
    eraseFlash();
    writeFlash();
    F_ADDR += 64;
  } 
}