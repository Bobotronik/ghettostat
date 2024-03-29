#include "derivative.h"
#include "humsense.h"

#include "pins.h"
#include "functions.h"

unsigned char getHumidityDec(void) {
 int tempHumidity = convertAD(H_CH);
 // Convert ADC reading to Relative Humidity
 //tempHumidity = (tempHumidity*253)/400 - 26;  //Seemed to overflow
 tempHumidity = (tempHumidity*5)/8 - 26;
 if(tempHumidity < 0)
    tempHumidity = 0;
 else if(tempHumidity > 99)
    tempHumidity = 99;
 return (unsigned char)tempHumidity; 
}

// Convert the RH reading to BCD format 
unsigned char getHumidityBCD(void) {            
  return convertDecToBCD(getHumidityDec());
}
