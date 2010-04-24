#include "int.h"
#include "derivative.h"
#include "i2cdevices.h"
#include "relayControl.h"
#include "delay.h"
#include "hidef.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char mode;
unsigned char status;

#pragma TRAP_PROC

void tempISRAux(void)  { 
    if (mode == HEAT) {      
      if(status != HEAT) {        //polarity is 1
        enableAuxHeat();          // turn on the heat
        status = HEAT;
        clearTempPolarity();      //switch the polarity to 0
      }
      else {
        disableAuxHeat();         // turn off the heat
        status = OFF;
        setTempPolarity();        //switch the polarity to  1
      }
    }
    else if (mode == COOL){         
      if(status != COOL) {        //polarity is 0
        enableAuxCool();          // turn on the AC
        status = COOL;
        setTempPolarity();        //switch the polarity to 1
      }
      else {
        disableAuxCool();         // turn off the AC
        status = OFF;
        clearTempPolarity();      //switch the polarity to 0
      }
    }
    del1m(1);
}

void tempISRMain(void) {
   if (mode == HEAT) {      
      if(status != HEAT) {        //polarity is 1
        enableMainHeat();          // turn on the heat
        status = HEAT;
        clearTempPolarity();      //switch the polarity to 0
      }
      else {
        disableMainHeat();         // turn off the heat
        status = OFF;
        setTempPolarity();        //switch the polarity to  1
      }
    }
    else if (mode == COOL){         
      if(status != COOL) {        //polarity is 0
        enableMainCool();          // turn on the AC
        status = COOL;
        setTempPolarity();        //switch the polarity to 1
      }
      else {
        disableMainCool();         // turn off the AC
        status = OFF;
        clearTempPolarity();      //switch the polarity to 0
      }
    }
    del1m(1);
}

void setModeAux(unsigned char * new_settings) {
  DisableInterrupts;
  // Turn everything off
  disableAuxFan();
  disableAuxCool();
  disableAuxHeat();
  setTempC(new_settings);       // Set the desired temp
  mode = new_settings[2];       // Set the mode
  status = mode;                // Set the status to the mode (so ISR call works)
  if( mode == FAN) {
    enableAuxFan();
  } else {
    tempISRAux();
  }
  EnableInterrupts;   
}

void setModeMain(unsigned char * new_settings) {
  DisableInterrupts;
  // Turn everything off
  disableMainFan();
  disableMainCool();
  disableMainHeat();
  setTempC(new_settings);       // Set the desired temp
  mode = new_settings[2];       // Set the mode
  status = mode;                // Set the status to the mode (so ISR call works)
  if( mode == FAN) {
    enableMainFan();
  } else {
    tempISRMain();
  }
  EnableInterrupts;   
}