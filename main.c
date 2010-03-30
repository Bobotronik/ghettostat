#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "pins.h"

#include "delay.h"
#include "i2c.h"
#include "lcd.h"
#include "rtc.h"
#include "touch.h"

#pragma TRAP_PROC
void thermometerISR(void) {

}


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
  
  /*
  General Algorightm
  -Check if touched
  -If touched, check state and decide which button was touched
  -Update state and LCD
  
  */
  for(;;) {
    // Poll clock
    getTime();
    
    // Poll touchscreen
    if (isTouched()){
      
    }
    __RESET_WATCHDOG(); /* feeds the dog */    
  } /* loop forever */
  /* please make sure that you never leave main */
}
