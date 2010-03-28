#include "tempsense.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

// Write the configuration byte to the temperature sensor
void tempConfig(void) {
  DEVICE_DATA[0] = T_SETTING;  
  i2c_write(TEMPSENSE_ADDR, T_CONFIG, DEVICE_DATA,1);
  del1m(20);
}

// Set the high and low registers based on some margin
void tempSet(unsigned char temp) {
  DEVICE_DATA[0] = temp -  T_PLUSMINUS;
  DEVICE_DATA[1] = 0x00;
  i2c_write(TEMPSENSE_ADDR, T_TL, DEVICE_DATA, 2);
  del1m(20);
  
  DEVICE_DATA[0] = temp + T_PLUSMINUS;
  i2c_write(TEMPSENSE_ADDR, T_TH, DEVICE_DATA, 2);
  del1m(20);
}

// Start converting
void tempStart(void) {
 i2c_write(TEMPSENSE_ADDR, T_START, DEVICE_DATA, 0);
 del1m(1000); // wait 1000 ms for first temperature to be ready
}

// Stop converting
void tempStop(void) {
  i2c_write(TEMPSENSE_ADDR, T_STOP, DEVICE_DATA, 0);
}

// Return the temperature
unsigned char tempGet(void) {
  i2c_read(TEMPSENSE_ADDR, T_TEMP, DEVICE_DATA, 2);
  return DEVICE_DATA[0];
}