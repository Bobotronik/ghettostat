#include "flash.h"
#include "derivative.h"

/* RAM Data Block */
unsigned char BUS_SPD  @0x0060; /* Value should equal 4x Bus speed */
unsigned char DATASIZE @0x0061; /* Number of bytes in data array (max 255) */
unsigned int  ADDR     @0x0062; /* Start address of FLASH to manipulate */
unsigned char DATA[64] @0x0064; /* Data to program into FLASH */

void loadFlash(void) {
  asm(LDHX FILE_PTR);    /* Load address of RAM Data Block to H:X */
  asm(JSR LDRNGE);       /* Call F_LOAD ROM Subroutine */
}

void eraseFlash(void) {
  asm(LDHX FILE_PTR);    /* Load address of RAM Data Block to H:X */
  asm(JSR ERARNGE);      /* Call F_ERASE ROM Subroutine */
}

void writeFlash(void) {
  asm(LDHX FILE_PTR);    /* Load address of RAM Data Block to H:X */
  asm(JSR PRGRNGE);      /* Call F_PROGRAM ROM Subroutine */        
}