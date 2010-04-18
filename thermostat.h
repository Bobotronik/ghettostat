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
#define NUM_PROGRAMS  5

#define MAIN              0
#define MAIN_SET_TO       1
#define MAIN_ROOM         2
#define MAIN_FAN          3
#define MAIN_MODE         4

#define PROGRAM_PROGRAMS  5   
#define PROGRAM_DAYS      6
#define PROGRAM_PROGRAMMING 7
#define PROGRAM_PROGRAM_DAYS 8

#define SETTINGS          9

void initializeThermostat();

void displayTime();
void displayTemps();

void updatePeriods();
void updateThermometer();

void createProgram();
void deleteProgram();

void drawButton(unsigned char, unsigned char, unsigned char, unsigned char);

void drawTopBar();
void drawLeftBar();

void drawMainScreen();

void drawMainSetToMenu();
void drawMainRoomMenu();
void drawMainFanMenu();
void drawMainModeMenu();

void drawProgramsScreen();
void drawDaysScreen();
void drawProgrammingScreen();
void drawProgramDaysScreen();

void drawSettingsScreen();

