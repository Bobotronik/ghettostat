#include "derivative.h"
#include "i2c.h"

//i2c_init - Initialize the MMIIC port
void initI2C(unsigned char bdiv) {
  MIMCR_MMBR = bdiv;                // Set baud rate divisor
  MMCR_MMEN = 1;                    // Enable MMIIC
}

//i2c_write - write bytes to an I2C device
void writeI2C(unsigned char device_addr, unsigned char command, unsigned char *data, int n) {
  while (MIMCR_MMBB);               // Wait for bus not busy
  while (!(MMSR_MMTXBE));           // Wait for TX buffer empty
  MMADR = device_addr & 0xFE;       // Device address -> address reg
  MMDTR = command;                  // Command byte
  MIMCR_MMRW = 0;                   // Set for transmit
  MIMCR_MMAST = 1;                  // Start transmission
  while (MMSR_MMRXAK);

  while (n >= 0) {
    while (!(MMSR_MMTXBE));       // Wait for TX buffer empty
    MMDTR = *data++;              // data -> DTR
    while (MMSR_MMRXAK);          // Wait for ACK from slave
    n--;
  }
  MIMCR_MMAST = 0;                  // Generate STOP bit
}

// i2c_read - read bytes from an I2C device
void readI2C(unsigned char device_addr, unsigned char command, unsigned char *data, int n) {
  unsigned char x;
  x = MMDRR;                        // This should clear the MMRXBF flag

  MMCR_MMTXAK = 0;                  // MCU will ACK received data
  MMADR = device_addr & 0xFE;       // Device address -> address reg.
  MMDTR = command;                  // Command byte
  MIMCR_MMRW = 0;                   // Set for transmit
  MIMCR_MMAST = 1;                  // Initiate transfer

  while (MMSR_MMRXAK);              // Wait for ACK from slave
  MMDTR = DUMMY_BYTE;
  while (MMSR_MMRXAK);

  MMADR = device_addr | 0x01;       // device address -> address reg.
  MIMCR_MMRW = 1;                   // Set for receive
  MMCR_REPSEN = 1;                  // Enable repeated start
  MIMCR_MMAST = 1;                  // Initiate transfer
  while (!(MMSR_MMTXBE));           // Wait for TX buffer empty
  MMDTR = DUMMY_BYTE;               // Dummy data -> data reg
  while (MMSR_MMRXAK);              // Wait for ACK from slave

  while (n > 0) {                   // Up to last byte
    if (n == 1) {
      MMCR_MMTXAK = 1;            // Switch from Ack to NAck
    }
    while (!(MMSR_MMRXBF));       // Wait for RX buffer full
    *data++ = MMDRR;              // Get data
    n--;                          // Decrement total count
  }
  MIMCR_MMAST = 0;                  // Generate STOP bit
}
