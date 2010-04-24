#ifndef FLASH_H
#define FLASH_H


#define F_LOAD    0xFF30      // Load FLASH data into RAM data array
#define F_ERASE   0xFCBE      // FLASH erase routine in internal ROM
#define F_WRITE   0xFC06      // FLASH program routine in internal ROM 

#define F_DATA_SIZE     64

// RAM Data Block
extern unsigned char BUS_SPD;           // Value should equal 4x Bus speed
extern unsigned char DATASIZE;          // Number of bytes in data array (max 255)
extern unsigned int  ADDR;              // Start address of FLASH to manipulate
extern unsigned char DATA[F_DATA_SIZE]; // Data to program into FLASH

void loadFlash(void);
void eraseFlash(void);
void writeFlash(void);

#endif
