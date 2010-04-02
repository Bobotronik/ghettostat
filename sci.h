#ifndef SCI_H
#define SCI_H

// Settings for Control Register 1
#define S_LOOPS       0b0   // Disable loops mode
#define S_ENSCI       0b1   // Enable SCI
#define S_TXINV       0b0   // Don't invert transmission
#define S_M           0b1   // 9-bit characters
#define S_PEN         0b1   // Enable parity
#define S_PTY         0b1   // Odd parity

// Settings for BUAD rate
// 300bps w/ 9.83 MHz clock)
#define S_SCP         0b00  //Prescalar = 1
#define S_SCR         0b111 //Divisor = 128

// Settings for Control Register 2
#define S_TE          0b1   // Enable transmitter
#define S_RE          0b1   // Enable reciever

// Settings for Good Transmission
#define DUMMY_TX      0xAA
#define VALID_TX      0xAB
#define TYPE_CMD      0xAC
#define TYPE_TEMP     0xAD
#define NUM_DUMMY     20

// Data 

void initSCI(void);
void sendByteSCI(unsigned char);
unsigned char receiveByteSCI(void);
void sendDataSCI(unsigned char, unsigned char);
void receiveDataSCI(unsigned char* data_bytes);

#endif
