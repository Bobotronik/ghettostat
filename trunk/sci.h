#ifndef SCI_H
#define SCI_H

// Settings for Control Register 1
#define S_LOOPS       0b0   // Disable loops mode
#define S_ENSCI       0b1   // Enable SCI
#define S_TXINV       0b0   // Don't invert transmission
#define S_M           0b1   // 9-bit characters
#define S_PEN         0b1   // Enable parity
#define S_PTY         0b1   // Odd parity

// Settings for BAUD rate
// 300bps w/ 9.83 MHz clock)
#define S_SCP         0b00  //Prescalar = 1
#define S_SCR         0b111 //Divisor = 128

// Settings for Control Register 2
#define S_TE          0b1   // Enable transmitter
#define S_RE          0b1   // Enable reciever

// Settings for Good Transmission
#define DUMMY_TX      0x00
#define VALID_TX      0xAA
#define NUM_DUMMY     20

// Settings for Message Types
#define TYPE_SETTEMP  0x5B
#define TYPE_GETTEMP  0x5C

// Data
extern unsigned char SCI_DATA[4];
 
// Functions
void initSCI(void);
void sendByteSCI(unsigned char);
unsigned char receiveByteSCI(void);
void sendDataSCI(void);
void receiveDataSCI(void);
void receiveTempC(unsigned char * C);
void sendModeAux(unsigned char tempF, unsigned char mode1);
void sendTempC(void);
void smallBoardReceive(void);

#endif
