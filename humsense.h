#ifndef HUMSENSE_H
#define HUMSENSE_H

#include "derivative.h"
#include "adc.h"
#include "bcd.h"

// ADC channel settings
#define H_CH            CH_11

// Humidity sensor functions
unsigned char humidityGetDec(void);
unsigned char humidityGetBCD(void);

#endif