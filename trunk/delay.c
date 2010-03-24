#include "delay.h"

#pragma DATA_SEG __SHORT_SEG MY_ZEROPAGE
unsigned char delay0;
unsigned char delay1;
#pragma DATA_SEG DEFAULT

// del1m - Delay about 1msec times the "d" argument.
void del1m(int d)
{
    while (d--) {
        asm {
            ; The following code delays 1 millisecond by looping.
            ; Total time is 4 + 5 * (4 + 68 * (4 + 3) + 4 + 3) = 2439 cycles
            ; A 9.8304MHz external clock gives an internal CPU clock of
            ; 2.4576MHz (407ns/cycle).  Delay is then .993 milliseconds.
                    mov     #$5,delay1  ; 4 cycles
            m1:     mov     #68,delay0  ; 4 cycles
            m0:     dec     delay0      ; 4 cycles
                    bne     m0          ; 3 cycles
                    dec     delay1      ; 4 cycles
                    bne     m1          ; 3 cycles
        }
    }
}