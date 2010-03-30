#include "derivative.h"
#include "tempsense.h"

#include "i2c.h"
#include "delay.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

// Write the configuration byte to the temperature sensor
void configureTemp(void) {
  DEVICE_DATA[0] = T_SETTING;  
  writeI2C(TEMPSENSE_ADDR, T_CONFIG, DEVICE_DATA,1);
  del1m(20);
}

// Start converting
void startTemp(void) {
 writeI2C(TEMPSENSE_ADDR, T_START, DEVICE_DATA, 0);
 del1m(1000); // wait 1000 ms for first temperature to be ready
}

// Stop converting
void stopTemp(void) {
  writeI2C(TEMPSENSE_ADDR, T_STOP, DEVICE_DATA, 0);
}

// Return the temperature
unsigned char getTemp(void) {
  readI2C(TEMPSENSE_ADDR, T_TEMP, DEVICE_DATA, 2);
  return DEVICE_DATA[0];
}

// Set the high and low registers based on some margin
void setTemp(unsigned char temp) {
  DEVICE_DATA[0] = temp -  T_PLUSMINUS;
  DEVICE_DATA[1] = 0x00;
  writeI2C(TEMPSENSE_ADDR, T_TL, DEVICE_DATA, 2);
  del1m(20);
  
  DEVICE_DATA[0] = temp + T_PLUSMINUS;
  writeI2C(TEMPSENSE_ADDR, T_TH, DEVICE_DATA, 2);
  del1m(20);
}
