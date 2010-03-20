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
#define FONT_WIDTH              6
#define TEXT_AREA			          (LCD_WIDTH/FONT_WIDTH)
#define TEXT_SIZE			          (TEXT_AREA*(LCD_HEIGHT/8))
#define GRAPHIC_AREA            (LCD_WIDTH/FONT_WIDTH)
#define GRAPHIC_SIZE	          (GRAPHIC_AREA*LCD_HEIGHT)
#define TEXT_HOME			          0
#define GRAPHIC_HOME					  (TEXT_HOME+TEXT_SIZE)
//#define GLCD_OFFSET_REGISTER	  2
//#define GLCD_EXTERNAL_CG_HOME   (GLCD_OFFSET_REGISTER << 11)

// Commands
#define SET_CURSOR_POINTER        0x21
#define SET_OFFSET_REGISTER       0x22
#define SET_ADDRESS_POINTER       0x24

#define SET_TEXT_HOME_ADDRESS     0x40
#define SET_TEXT_AREA             0x41
#define SET_GRAPHIC_HOME_ADDRESS  0x42
#define SET_GRAPHIC_AREA          0x43

#define OR_MODE                   0x80
#define XOR_MODE                  0x81
#define AND_MODE                  0x83
#define TEXT_ATTRIBUTE_MODE       0x84
#define INTERNAL_CG_ROM_MODE      0x80
#define EXTERNAL_CG_RAM_MODE      0x88

#define DISPLAY_OFF               0x90
#define CURSOR_ON_BLINK_OFF       0x92
#define CURSOR_ON_BLINK_OFF       0x93
#define TEXT_ON_GRAPHIC_OFF       0x94
#define TEXT_OFF_GRAPHIC_ON       0x98
#define TEXT_ON_GRAPHIC_OFF       0x9c

#define 1_LINE_CURSOR             0xa0
#define 2_LINE_CURSOR             0xa1
#define 3_LINE_CURSOR             0xa2
#define 4_LINE_CURSOR             0xa3
#define 5_LINE_CURSOR             0xa4
#define 6_LINE_CURSOR             0xa5
#define 7_LINE_CURSOR             0xa6
#define 8_LINE_CURSOR             0xa7

#define SET_DATA_AUTO_WRITE       0xb0
#define SET_DATA_AUTO_READ        0xb1
#define AUTO_RESET                0xb2

#define DATA_WRITE_AND_INCREMENT  0xc0
#define DATA_READ_AND_INCREMENT   0xc1
#define DATA_WRITE_AND_DECREMENT  0xc2
#define DATA_READ_AND_DECREMENT   0xc3
#define WRITE_AND_NONVARIABLE     0xc4
#define DATA_READ_AND_NONVARIABLE 0xc5

#define SCREEN_PEEK               0xe0
#define SCREEN_COPY               0xe8


void wait(unsigned char);

void writeData(unsigned char);

unsigned char readData();

void writeCommand(unsigned char);

unsigned char readStatus();

void initializeDisplay();
