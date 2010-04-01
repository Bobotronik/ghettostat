// I2C Address
#define PORTX_ADDR      0x7E

// Port Expander Initial Setting
#define P_INIT          0x18  // LCD reset off, font select to 6*8          

// Port Expander Bit Masks
#define P_HEAT          0x01
#define P_FAN           0x02
#define P_COOL          0x04
#define P_RESET         0x08
#define P_FONTSEL       0x10
#define P_6             0x20
#define P_7             0x40
#define P_8             0x80

// Global data variables
extern unsigned char PORTX_DATA;

// Port expander functions
void initPortX(void);
void setBitsPortX(unsigned char);
void clearBitsPortX(unsigned char);

// I2C Address
#define RTC_ADDR    0xA2

// Real Time Clock Registers
#define C_CONTROL1      0x00
#define C_CONTROL2      0x01
#define C_SECONDS       0x02
#define C_MINUTES       0x03
#define C_HOURS         0x04
#define C_DAYS          0x05
#define C_WEEKDAYS      0x06
#define C_MONTHS        0x07
#define C_YEARS         0x08
#define C_ALARM_MINUTE  0x09
#define C_ALARM_HOUR    0x0A
#define C_ALARM_DAY     0x0B
#define C_ALARM_WEEKDAY 0x0C
#define C_CTRL_CLKOUT   0x0D
#define C_CTRL_TIMER    0x0E
#define C_TIMER         0x0F

// Real Time Clock Settings
#define C_SETTING1      0x00
#define C_SETTING2      0x02 // Alarm interrupts enabled

// Global Data Variables
extern unsigned char DEVICE_TIME[7];
extern unsigned char DEVICE_ALARM[4];

// Real time clock functions
void configureClock(void);
void clearAI(void);
void clearTI(void);
void setAlarm(void);
void getTime(void);
void setTime(void);

// I2C Address
#define TEMPSENSE_ADDR  0x9E

// Temperature Sensor Commands
#define T_TEMP          0xAA
#define T_COUNTER       0xA8   
#define T_SLOPE         0xA9
#define T_START         0xEE
#define T_STOP          0x22
#define T_TH            0xA1
#define T_TL            0xA2
#define T_CONFIG        0xAC

// Temperature Sensor Setting
#define T_SETTING       0x02
#define T_MARGIN     1

// Temperature sensor functions
void configureTemp(void);
void startTemp(void);
void stopTemp(void);
unsigned char getTemp(void);
void setTemp(unsigned char);

