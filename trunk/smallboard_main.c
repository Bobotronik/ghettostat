#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "i2c.h"
#include "i2cDevices.h"
#include "functions.h"
#include "sci.h"
#include "int.h"
#include "relayControl.h"

#pragma TRAP_PROC
void dummyISR(void) {

}


void main(void) {
  EnableInterrupts; /* enable interrupts */
  
  CONFIG1_COPD = 1;
  INTSCR_MODE = 1;
  INTSCR_IMASK = 1;
  
  initI2C();
  initSCI();
  initAuxRelays();
  configureTemp();
  startTemp();
  
  // Set the mode to off and enable interrupts
  SCI_DATA[0] = 28;
  SCI_DATA[1] = 0;
  SCI_DATA[2] = I_HEAT;
  setModeAux(SCI_DATA);
  INTSCR_IMASK = 0;       // Clear the interrupt mask
  
  while(1) {
    smallBoardReceive();
  }
}
