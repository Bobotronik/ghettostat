#define TIME_POSITION
#define CURRENT_TEMP_POSITION
#define SET_TEMP_POSITION

#define OFF         0
#define HEAT        1
#define COOL        2
#define CIRCULATE   3

#define MORNING     0
#define WORK        1
#define EVENING     2
#define NIGHT       3

#define NUM_PERIODS   4
#define NUM_PROGRAMS  4

#define MAIN              0
#define PROGRAM_TAB       1
#define DAY_TAB           2
#define SETTINGS          3

#define NONE              0
#define ROOM_MENU         1 
#define FAN_MENU          2
#define MODE_MENU         3
#define SET_TO_MENU       4

#define PROGRAMS_MENU     5
#define DAYS_MENU         5

#define HOURS_MENU
#define MINUTES_MENU
#define AM_PM_MENU
#define TEMP_MENU

void initializeThermostat();

void displayTime();
void displayTemps();

void updatePeriods();
void updateThermometer();

void createProgram();
void deleteProgram();

void drawButton(unsigned char, unsigned char, unsigned char, unsigned char);
void drawSolidButton(unsigned char, unsigned char, unsigned char, unsigned char);

unsigned char isButtonTouched(unsigned char, unsigned char, unsigned char*);

void drawTopBar();
void drawLeftBar();

void drawMainScreen();
void drawMainSetToMenu();
void drawMainRoomMenu();
void drawMainFanMenu();
void drawMainModeMenu();

void drawProgramsTab();
void drawDaysTab();
void drawProgrammingScreen();
void drawProgramDaysScreen();

void drawSettingsScreen();

