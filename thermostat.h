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
#define PROGRAMS_TAB       1
#define DAYS_TAB           2
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

// Left Bar
extern const unsigned char leftBarX;
extern const unsigned char leftBarWidth;
extern const unsigned char leftBarHeight;
extern const unsigned char homeButton[];
extern const unsigned char programButton[];
extern const unsigned char settingsButton[];

// Main Screen
extern const unsigned char setToButton[];
extern const unsigned char rightBarX;
extern const unsigned char rightBarWidth;
extern const unsigned char rightBarHeight;
extern const unsigned char roomButton[];
extern const unsigned char fanButton[];
extern const unsigned char modeButton[];

// Program Screen
extern const unsigned char tabY;
extern const unsigned char tabWidth;
extern const unsigned char tabHeight;
extern const unsigned char programsTab[];
extern const unsigned char daysTab[];

extern const unsigned char programsButtonX;
extern const unsigned char programsButtonWidth;
extern const unsigned char programsButtonHeight;

extern const unsigned char program1Button[];
extern const unsigned char program2Button[];
extern const unsigned char program3Button[];
extern const unsigned char program4Button[];

extern const unsigned char daysButtonY;
extern const unsigned char daysButtonWidth;
extern const unsigned char daysButtonHeight;
extern const unsigned char monButton[];
extern const unsigned char wedButton[];
extern const unsigned char friButton[];
extern const unsigned char tueButton[];
extern const unsigned char thuButton[];
extern const unsigned char satButton[];
extern const unsigned char sunButton[];

void initializeThermostat();

void displayTime();
void displayTemps();

void updatePeriods();
void updateThermometer();

void createProgram();
void deleteProgram();

void drawButton(unsigned char*);
void drawSolidButton(unsigned char*);

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
void drawDaysMenu(unsigned char*);

void drawSettingsScreen();

