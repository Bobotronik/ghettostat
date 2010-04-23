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
#define PROGRAMS_TAB      1
#define DAYS_TAB          2
#define PROGRAMMING_SCREEN  3
#define SETTINGS          4

// For menuState
#define NONE              0

#define ROOM_MENU         1 
#define FAN_MENU          2
#define MAIN_MODE_MENU    3
#define SET_TO_MENU       4

#define MONDAY            5
#define TUESDAY           6
#define WEDNESDAY         7
#define THURSDAY          8
#define FRIDAY            9
#define SATURDAY          10
#define SUNDAY            11 

#define HOURS_MENU        12
#define MINUTES_MENU      13
#define TEMP_MENU         14
#define PROGRAM_MODE_MENU 15

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
extern const unsigned char roomMenu[];
extern const unsigned char fanMenu[];
extern const unsigned char modeMenu[];

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
extern const unsigned char monMenu[];
extern const unsigned char tueMenu[];
extern const unsigned char wedMenu[];
extern const unsigned char thuMenu[];
extern const unsigned char friMenu[];
extern const unsigned char satMenu[];
extern const unsigned char sunMenu[]; 

void initializeThermostat();

void displayTime();
void displayTemps();

void updatePeriods();
void updateThermometer();

void createProgram();
void deleteProgram();

void printTopBorder();
void printBottomBorder();

void drawButton(unsigned char*);
void drawSolidButton(unsigned char*);

void drawVerticalMenu(unsigned char*);
void drawHorizontalMenu(unsigned char*);

unsigned char isButtonTouched(unsigned char, unsigned char, unsigned char*);
unsigned char isVerticalMenuButtonTouched(unsigned char, unsigned char, unsigned char*);
unsigned char isHorizontalMenuButtonTouched(unsigned char, unsigned char, unsigned char*);

void drawTopBar();
void drawLeftBar();

void drawMainScreen();
void drawMainSetToMenu();
void drawMainRoomMenu();
void drawMainFanMenu();
void drawMainModeMenu();

void drawProgramsTab();

void drawDaysTab();
void drawDaysMenu(unsigned char*);

void drawProgrammingScreen(unsigned char);

unsigned char determineHoursMenu(unsigned char);
void drawHoursMenu(unsigned char);

void drawMinutesMenu();
void toggleAmPm(unsigned char, unsigned char);

unsigned char determineTempMenu(unsigned char);
void drawProgrammingTempMenu();

void drawProgrammingModeMenu();

void drawSettingsScreen();

