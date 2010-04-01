#include "derivative.h"
#include "humsense.h"

#include "pins.h"
#include "functions.h"

unsigned char getHumidityDec(void) {
 int tempHumidity = convertAD(H_CH);
 // Convert ADC reading to Relative Humidity
 tempHumidity = (tempHumidity*253)/400 - 26;
 if(tempHumidity < 0)
    tempHumidity = 0;
 return (unsigned char)tempHumidity; 
}

unsigned char getHumidityBCD(void) {
  unsigned char tempHumidity = getHumidityDec();            
  // Convert the RH reading to BCD format
  if(tempHumidity > 99)
    tempHumidity = 99;  
  return convertDecToBCD(tempHumidity);
}
