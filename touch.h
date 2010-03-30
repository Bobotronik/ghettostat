#define MIN_TOUCH     0x02
#define MAX_TOUCH     0xd0

unsigned char isTouched();
unsigned char getX();
unsigned char getY();
void initializeTS();
