#include "derivative.h"
#include "portx.h"

#include "i2c.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char PORTX_DATA;

// Setup the port expander initially
void initPortX(void) {
  PORTX_DATA = P_INIT;
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Write to the port expander
void setPortX(void) {
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Turn specific bits on
void setBitsPortX(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA | bitmask; 
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Turn specitif bits off
void clearBitsPortX(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA & ~bitmask;
  writeI2C(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}
