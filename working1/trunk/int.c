#include "int.h"
#include "derivative.h"
#include "i2cdevices.h"
#include "relayControl.h"
#include "delay.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char mode;
unsigned char status;

#pragma TRAP_PROC

void temp_ISR(void)  { 
    if (mode == HEAT) {      
      if(status != HEAT) {        //polarity is 1
        enableAuxHeat();             // turn on the heat
        status = HEAT;
        clearTempPolarity();      //switch the polarity to 0
      }
      else {
        disableAuxHeat();             // turn off the heat
        status = OFF;
        setTempPolarity();        //switch the polarity to  1
      }
    }
    else if (mode == COOL){         
      if(status != COOL) {        //polarity is 0
        enableAuxCool();             // turn on the AC
        status = COOL;
        setTempPolarity();        //switch the polarity to 1
      }
      else {
        disableAuxCool();             // turn off the AC
        status = OFF;
        clearTempPolarity();      //switch the polarity to 0
      }
    }
    del1m(1);
}