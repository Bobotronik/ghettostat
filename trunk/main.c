#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "lcd.h"


void main(void) {

  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  
  CONFIG1_COPD = 1;

  initializeDisplay();
  
  for(;;) {
    __RESET_WATCHDOG(); /* feeds the dog */    
  } /* loop forever */
  /* please make sure that you never leave main */
}
