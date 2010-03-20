/*

*/
#define WR   PTA_PTA0
#define RD   PTA_PTA1
#define CD   PTA_PTA4
#define CE   PTD_PTD4

void wait(unsigned char);

void writeData(unsigned char);

unsigned char readData();

void writeCommand(unsigned char);

unsigned char readStatus();

void initializeDisplay();



