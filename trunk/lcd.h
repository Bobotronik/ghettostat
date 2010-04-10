// LCD Properties
#define LCD_WIDTH               240 
#define LCD_HEIGHT              128
#define FONT_WIDTH              6
#define TEXT_AREA			          (LCD_WIDTH/FONT_WIDTH)        
#define GRAPHIC_AREA            (LCD_WIDTH/FONT_WIDTH)     
#define TEXT_SIZE			          (TEXT_AREA*(LCD_HEIGHT/8))  
#define GRAPHIC_SIZE	          (GRAPHIC_AREA*LCD_HEIGHT)   
#define TEXT_HOME			          0
#define GRAPHIC_HOME					  (TEXT_HOME+TEXT_SIZE)
#define OFFSET_REGISTER	        (((TEXT_SIZE+GRAPHIC_SIZE) >> 11) + 1)                          
#define CG_RAM_HOME             (OFFSET_REGISTER << 11)

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

#define CHAR_SHIFT                3

// Touch stuff
#define MIN_X                     13
#define MAX_X                     116
#define MIN_Y                     14
#define MAX_Y                     51
#define X_DIFFERENCE              (MAX_X-MIN_X)
#define Y_DIFFERENCE              (MAX_Y-MIN_Y)

// Dot pitch = 0.45mm
// 1 6x8 pixel character = 2.7mm x 3.6mm

// Special characters
#define BLOCK                     0x80
#define DEGREE                    0x81
#define LARGE_DEGREE              0x82      
#define UPPER_LEFT_CORNER         0x83
#define UPPER_RIGHT_CORNER        0x84
#define LOWER_LEFT_CORNER         0x85
#define LOWER_RIGHT_CORNER        0x86
#define TOP_BORDER                0x87
#define BOTTOM_BORDER             0x88
#define LEFT_BORDER               0x89
#define RIGHT_BORDER              0x90

unsigned char charAbs(int);

void wait(unsigned int);
unsigned char readStatus();
void writeData(unsigned char);
unsigned char readData();
void writeCommand(unsigned char);

void setADP(short unsigned int);

// X: 0-39    Y: 0-15
void goToText(unsigned char, unsigned char);
// X: 0-239   Y: 0-127
// Note: this will only put you at the byte of the specified coordinates
void goToGraphic(unsigned char, unsigned char);

void display(unsigned char);
void printChar(unsigned char);
void printCG(unsigned char);
void printLargeChar(unsigned char);
void printNum(int);
void printBCD(unsigned char);
void printStr(char*);
void clearText();
void clearGraphic();
void clearScreen();
void clearCG();

void setPixel(unsigned char, unsigned char, unsigned char);
void line(unsigned char, unsigned char, unsigned char, unsigned char);
void drawLine(unsigned char, unsigned char, unsigned char, unsigned char);
void drawBox(unsigned char, unsigned char, unsigned char, unsigned char);

unsigned char isTouched();
unsigned char getX();
unsigned char getY();

void initializeCG();
void initializeTS();
void initializeDisplay();
