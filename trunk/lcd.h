// Control Lines
#define WR          PTA_PTA0
#define RD          PTA_PTA1
#define CD          PTA_PTA4
#define CE          PTD_PTD4

// Data Lines
#define DB          PTB
#define DB0         PTB_PTB0
#define DB1         PTB_PTB1
#define DB2         PTB_PTB2
#define DB3         PTB_PTB3
#define DB4         PTB_PTB4
#define DB5         PTB_PTB5
#define DB6         PTB_PTB6
#define DB7         PTB_PTB7

// LCD Properties
#define LCD_WIDTH               240 
#define LCD_HEIGHT              128
#define FONT_WIDTH              8
#define TEXT_AREA			          (LCD_WIDTH/FONT_WIDTH)        // 240/6=40
#define GRAPHIC_AREA            (LCD_WIDTH/FONT_WIDTH)        // 240/6=40
#define TEXT_SIZE			          (TEXT_AREA*(LCD_HEIGHT/8))    // 40*16=640
#define GRAPHIC_SIZE	          (GRAPHIC_AREA*LCD_HEIGHT)     // 40*128=5120
#define TEXT_HOME			          0
#define GRAPHIC_HOME					  (TEXT_HOME+TEXT_SIZE)
#define OFFSET_REGISTER	        3                            
//#define EXTERNAL_CG_HOME   (GLCD_OFFSET_REGISTER << 11)

// Commands

// Registers Setting
#define SET_CURSOR_POINTER        0x21
#define SET_OFFSET_REGISTER       0x22
#define SET_ADDRESS_POINTER       0x24

// Set Control Word
#define SET_TEXT_HOME_ADDRESS     0x40
#define SET_TEXT_AREA             0x41
#define SET_GRAPHIC_HOME_ADDRESS  0x42
#define SET_GRAPHIC_AREA          0x43

// Mode Set
#define OR_MODE                   0x80
#define XOR_MODE                  0x81
#define AND_MODE                  0x83
#define TEXT_ATTRIBUTE_MODE       0x84
#define INTERNAL_CG_ROM_MODE      0x80
#define EXTERNAL_CG_RAM_MODE      0x88

// Display Mode
#define DISPLAY_OFF               0x90
#define CURSOR_ON_BLINK_OFF       0x92
#define CURSOR_ON_BLINK_ON        0x93
#define TEXT_ON_GRAPHIC_OFF       0x94
#define TEXT_OFF_GRAPHIC_ON       0x98
#define TEXT_ON_GRAPHIC_ON        0x9c

// Set Cursor Pattern
#define ONE_LINE_CURSOR           0xa0
#define TWO_LINE_CURSOR           0xa1
#define THREE_LINE_CURSOR         0xa2
#define FOUR_LINE_CURSOR          0xa3
#define FIVE_LINE_CURSOR          0xa4
#define SIX_LINE_CURSOR           0xa5
#define SEVEN_LINE_CURSOR         0xa6
#define EIGHT_LINE_CURSOR         0xa7

// Data Auto Read/Write
#define SET_DATA_AUTO_WRITE       0xb0
#define SET_DATA_AUTO_READ        0xb1
#define AUTO_RESET                0xb2

// Data Read/Write
#define DATA_WRITE_AND_INCREMENT  0xc0
#define DATA_READ_AND_INCREMENT   0xc1
#define DATA_WRITE_AND_DECREMENT  0xc2
#define DATA_READ_AND_DECREMENT   0xc3
#define WRITE_AND_NONVARIABLE     0xc4
#define DATA_READ_AND_NONVARIABLE 0xc5

#define SCREEN_PEEK               0xe0
#define SCREEN_COPY               0xe8

void wait(unsigned char);
unsigned char readStatus();
void writeData(unsigned char);
unsigned char readData();
void writeCommand(unsigned char);

void setADP(short unsigned int);
void display(unsigned char);
void printChar(char);
void printStr(char*);
void printBox(unsigned char, unsigned char, unsigned char, unsigned char);

void clearText();
void clearGraphic();

// X: 0-39    Y: 0-15
void goToText(unsigned char, unsigned char);
// X: 0-239   Y: 0-127
// Note: this will only put you at the byte of the specified coordinates
void goToGraphic(unsigned char, unsigned char);

void initializeDisplay(void);
