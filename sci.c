#include "derivative.h"
#include "sci.h"

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

// Output a combination TYPE byte and DATA byte 
void sendDataSCI(unsigned char type_byte, unsigned char data_byte) {
  unsigned char counter;
  // Output the dummy byte to ensure proper link
  for (counter=0;counter < NUM_DUMMY; counter++) {
    sendByteSerial(DUMMY_TX);
  }
  
  // Output the next-valid byte
  sendByteSerial(VALID_TX);
  //Output the type indicator
  sendByteSerial(type_byte);
  //Output the data
  sendByteSerial(data_byte);
}

void receiveDataSCI(unsigned char* data_bytes) {
  unsigned char temp_byte;
  
  // Wait until reaching the next-valid byte
  temp_byte = receiveByteSerial();
  while (temp_byte != VALID_TX) {
    temp_byte = receiveByteSerial();
  }
  
  // Store the type byte
  data_bytes[0] = receiveByteSerial();
  // Store the data byte
  data_bytes[1] = receiveByteSerial();
}
  
