#include "sci.h"

void sci_init(void) {
    SCC1_LOOPS  = S_LOOPS;
    SCC1_ENSCI  = S_ENSCI;
    SCC1_TXINV  = S_TXINV;
    SCC1_M      = S_M;
    SCC1_PEN    = S_PEN;
    SCC1_PTY    = S_PTY;

    SCBR_SCP    = S_SCP;
    SCBR_SCR    = S_SCR;
    
    SCC2_TE     = S_TE;
    SCC2_RE     = S_RE;
}

// Output a single byte
void sci_out(unsigned char data_byte) {
    while (SCS1_SCTE == 0); // Wait until transmitter empty
    SCDR = data_byte;
}

// Input a single byte
unsigned char sci_in(void) {
  while (SCS1_SCRF == 0); // Wait until receiver full
  return SCDR;
}

// Output a combination TYPE byte and DATA byte 
void sci_send(unsigned char type_byte, unsigned char data_byte) {
  unsigned char counter;
  // Output the dummy byte to ensure proper link
  for(counter=0;counter < NUM_DUMMY; counter++){
    sci_out(DUMMY_TX);
  }
  
  // Output the next-valid byte
  sci_out(VALID_TX);
  //Output the type indicator
  sci_out(type_byte);
  //Output the data
  sci_out(data_byte);}

void sci_receive(unsigned char* data_bytes) {
  unsigned char temp_byte;
  
  // Wait until reaching the next-valid byte
  temp_byte = sci_in();
  while (temp_byte != VALID_TX) {
    temp_byte = sci_in();
  }
  
  // Store the type byte
  data_bytes[0] = sci_in();
  // Store the data byte
  data_bytes[1] = sci_in();
}
  