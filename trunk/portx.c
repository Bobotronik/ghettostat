#include "portx.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char PORTX_DATA;

// Setup the port expander initially
void portxInit(void) {
  PORTX_DATA = P_INIT;
  portxSet();
}

// Write to the port expander
void portxSet(void) {
  i2c_write(PORTX_ADDR, PORTX_DATA, DEVICE_DATA, 0);
}

// Turn specific bits on
void portxOn(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA | bitmask; 
  portxSet();
}

// Turn specitif bits off
void portxOff(unsigned char bitmask) {
  PORTX_DATA = PORTX_DATA & ~bitmask;
  portxSet();
}