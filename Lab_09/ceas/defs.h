#include <avr/io.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#define clrbit(var,bit) var &= ~(1<<bit)
#define setbit(var,bit) var |= 1<<bit

#define nop asm("nop"::);

#define NOKEY 0x7f

void wait(unsigned long int val);
void sysinit();
void putchLCD(char ch);
void putsLCD( char* ch);
void clrLCD();
void initLCD();
void gotoLC(unsigned char line, unsigned char col);
void _delay_us(double __us);
void smiley();
char kbscan();
char int_to_char(int digit);
