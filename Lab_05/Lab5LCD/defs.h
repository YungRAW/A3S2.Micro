#include <avr/io.h>

#define clrbit(var,bit) var &= ~(1<<bit)
#define setbit(var,bit) var |= 1<<bit

#define nop asm("nop"::);

void wait(unsigned long int val);
void sysinit();
void putchLCD(char ch);
void putsLCD( char* ch);
void clrLCD();
