#include "humsense.h"

unsigned char humidityGetDec(void) {
 int tempHumidity = ADCconvert(H_CH);
 // Convert ADC reading to Relative Humidity
 tempHumidity = (tempHumidity*253)/400 - 26;
 if(tempHumidity < 0)
    tempHumidity = 0;
 return (unsigned char) tempHumidity; 
}

unsigned char humidityGetBCD(void) {
  unsigned char tempHumidity = humidityGetDec();            
  // Convert the RH reading to BCD format
  if(tempHumidity > 99)
    tempHumidity = 99;  
  return Dec2BCD(tempHumidity);
}