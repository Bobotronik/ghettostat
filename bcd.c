#include "bcd.h"

//Input must be from 0 to 99
unsigned char Dec2BCD(unsigned char value)
{
     unsigned char result;

     result = value % 10;
     result += ((value - result)/10 << 4);

     return result;  // result is the BCD
}