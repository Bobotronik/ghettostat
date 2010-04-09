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

#define MAIN        0
#define PROGRAM     1
#define SETTINGS    2

void initializeThermostat();

void displayTime();
void displayTemps();

void updatePeriods();
void updateThermometer();

void createProgram();
void deleteProgram();

void drawButton();
void drawMainScreen();
void drawProgramScreen();
void drawSettingsScreen();

