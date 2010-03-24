
// ****LCD****

// Control Lines
#define LCD_WR_DIR      DDRA_DDRA0
#define LCD_RD_DIR      DDRA_DDRA1
#define LCD_CD_DIR      DDRA_DDRA4
#define LCD_CE_DIR      DDRD_DDRD4
#define LCD_WR          PTA_PTA0
#define LCD_RD          PTA_PTA1
#define LCD_CD          PTA_PTA4
#define LCD_CE          PTD_PTD4

// Data Lines
#define LCD_DB_DIR      DDRB
#define LCD_DB0_DIR     DDRB_DDRB0
#define LCD_DB1_DIR     DDRB_DDRB1
#define LCD_DB2_DIR     DDRB_DDRB2
#define LCD_DB3_DIR     DDRB_DDRB3
#define LCD_DB4_DIR     DDRB_DDRB4
#define LCD_DB5_DIR     DDRB_DDRB5
#define LCD_DB6_DIR     DDRB_DDRB6
#define LCD_DB7_DIR     DDRB_DDRB7
#define LCD_DB          PTB
#define LCD_DB0         PTB_PTB0
#define LCD_DB1         PTB_PTB1
#define LCD_DB2         PTB_PTB2
#define LCD_DB3         PTB_PTB3
#define LCD_DB4         PTB_PTB4
#define LCD_DB5         PTB_PTB5
#define LCD_DB6         PTB_PTB6
#define LCD_DB7         PTB_PTB7

// ****Touch Screen****
#define TS_PIN1_DIR     DDRD_DDRD1
#define TS_PIN2_DIR     DDRD_DDRD2
#define TS_PIN3_DIR     DDRD_DDRD3
#define TS_PIN4_DIR     DDRD_DDRD4
#define TS_PIN1         PTD_PTD1
#define TS_PIN2         PTD_PTD2
#define TS_PIN3         PTD_PTD3
#define TS_PIN4         PTD_PTD4
