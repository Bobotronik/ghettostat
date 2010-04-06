#include "derivative.h"
#include "sci.h"

//unsigned char SCI_DATA[4];

void initSCI(void) {
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
void sendByteSCI(unsigned char data_byte) {
    while (SCS1_SCTE == 0); // Wait until transmitter empty
    SCDR = data_byte;
}

// Input a single byte
unsigned char receiveByteSCI(void) {
  while (SCS1_SCRF == 0); // Wait until receiver full
  return SCDR;
}

// Output
void sendDataSCI(void) {
  unsigned char counter;
  // Output the dummy byte to ensure proper link
  for (counter=0;counter < NUM_DUMMY; counter++) {
    sendByteSCI(DUMMY_TX);
  }
  
  // Output the next-valid byte
  sendByteSCI(VALID_TX);
  // Output the type byte
  sendByteSCI(SCI_DATA[3]);
  // If setting the temperature, output the data bytes
  if( type_byte == TYPE_SETTEMP) {
    sendByteSCI(SCI_DATA[0]);
    sendByteSCI(SCI_DATA[1]);
    //Output the command byte
    sendByteSCI(SCI_DATA[2]);
  } else {
    sendByteSCI(DUMMY_TX);
    sendByteSCI(DUMMY_TX);
    sendByteSCI(DUMMY_TX);
  }
}

void receiveDataSCI(void) {
  unsigned char temp_byte;
  
  // Wait until reaching the next-valid byte
  temp_byte = receiveByteSCI();
  while (temp_byte != VALID_TX) {
    temp_byte = receiveByteSCI();
  }
 
  // Store the type byte
  SCI_DATA[3] = receiveByteSCI();
  // Store the temperature bytes
  SCI_DATA[0] = receiveByteSCI();
  SCI_DATA[1] = receiveByteSCI();
  // Store the command byte
  SCI_DATA[2] = receiveByteSCI();
}
  
