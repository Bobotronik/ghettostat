#include "int.h"
#include "derivative.h"
#include "i2cDevices.h"
#include "relayControl.h"
#include "functions.h"
#include "hidef.h"

#pragma DATA_SEG DEFAULT
#pragma CODE_SEG DEFAULT

unsigned char mode;
unsigned char status;
unsigned char goalTemp[2] = 0;

#pragma TRAP_PROC

void tempISRAux(void)  { 
    if (mode == I_HEAT) {      
      if(status != I_HEAT) {        //polarity is 1
        enableAuxHeat();          // turn on the heat
        status = I_HEAT;
        clearTempPolarity();      //switch the polarity to 0
      }
      else {
        disableAuxHeat();         // turn off the heat
        status = I_OFF;
        setTempPolarity();        //switch the polarity to  1
      }
    }
    else if (mode == I_COOL){         
      if(status != I_COOL) {        //polarity is 0
        enableAuxCool();          // turn on the AC
        status = I_COOL;
        setTempPolarity();        //switch the polarity to 1
      }
      else {
        disableAuxCool();         // turn off the AC
        status = I_OFF;
        clearTempPolarity();      //switch the polarity to 0
      }
    }
    del1m(10);
}

#pragma TRAP_PROC
void tempISRMain(void) {
  if (mode == I_HEAT) {      
      if(status != I_HEAT) {        //polarity is 1
        enableMainHeat();          // turn on the heat
        status = I_HEAT;
        clearTempPolarity();      //switch the polarity to 0
      }
      else {
        disableMainHeat();         // turn off the heat
        status = I_OFF;
        setTempPolarity();        //switch the polarity to  1
      }
    }
    else if (mode == I_COOL){         
      if(status != I_COOL) {        //polarity is 0
        enableMainCool();          // turn on the AC
        status = I_COOL;
        setTempPolarity();        //switch the polarity to 1
      }
      else {
        disableMainCool();         // turn off the AC
        status = I_OFF;
        clearTempPolarity();      //switch the polarity to 0
      }
    }
    del1m(10);
}

#pragma CODE_SEG DEFAULT

void setModeAux(unsigned char * new_settings) {
  if(new_settings[0] != goalTemp[0] || new_settings[1] != goalTemp[1] 
      || new_settings[2] != mode) {
    DisableInterrupts;
    // Turn everything off
    disableAuxFan();
    disableAuxCool();
    disableAuxHeat();
    setTempC(new_settings);       // Set the desired temp
    mode = new_settings[2];       // Set the mode
    status = I_OFF;
    goalTemp[0] = new_settings[0];
    goalTemp[1] = new_settings[1];               
    if( mode == I_FAN) {
      enableAuxFan();
    } else if(mode == I_HEAT) {
      setTempPolarity();
    } else {
      clearTempPolarity();
    }
    EnableInterrupts;
  }   
}

void setModeMain(unsigned char tempF, unsigned char mode1) {
  if( tempF != goalTemp[0] || mode1 != mode) {
    DisableInterrupts;
    // Turn everything off
    disableMainFan();
    disableMainCool();
    disableMainHeat();
    setTempF(tempF);       // Set the desired temp
    mode = mode1;
    status = I_OFF;
    goalTemp[0] = tempF;             
    if(mode == I_FAN) {
      enableMainFan();
    } else if(mode == I_HEAT){
      setTempPolarity();
    } else {
      clearTempPolarity();
    }
    EnableInterrupts;
  }   
}