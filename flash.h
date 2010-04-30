#ifndef FLASH_H
#define FLASH_H

// Flash ROM Routines
#define F_LOAD    0xFF30          // Load FLASH data into RAM data array  (LDRNGE)
#define F_ERASE   0xFCBE          // Erase FLASH block                    (ERARNGE)
#define F_WRITE   0xFC06          // Write RAM data array to FLASH        (PRGRNGE)

#define F_START   0xFB80          // Starting address in FLASH to use

// RAM Data Block
extern unsigned char F_BUS_SPD;     // Value should equal 4x Bus speed
extern unsigned char F_DATASIZE;    // Number of bytes in data array (max 255)
extern unsigned int  F_ADDR;        // Start address of FLASH to manipulate
extern unsigned char F_DATA[];      // Data to program into FLASH
extern unsigned int* F_FILE_PTR;    // Points to the starting address of BUS_SPD

void initFlash(void);
void loadFlash(void);
void eraseFlash(void);
void writeFlash(void);

void loadProgramData(void);
void saveProgramData(void);

#endif
