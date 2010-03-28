#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "devices.h"
#include "delay.h"
#include "i2c.h"
#include "lcd.h"
#include "pins.h"
#include "touch.h"

void main(void) {

  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  
  CONFIG1_COPD = 1;
  
  i2c_init((unsigned char)2);
  tempConfig();
  tempSet((unsigned char)23);
  tempStart();
  portxInit();
  initializeDisplay();
  
  for(;;) {
    if (isTouched()){
    
    }
    __RESET_WATCHDOG(); /* feeds the dog */    
  } /* loop forever */
  /* please make sure that you never leave main */
}
