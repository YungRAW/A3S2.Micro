#include "defs.h"

#define E  7
#define RW 6
#define RS 5

#define IReg 0
#define SReg 0
#define DReg 1

volatile unsigned long int delay;

void wait(unsigned long int val){
   for(delay=0;delay<val;delay++){}
}

unsigned char rd_LCDreg(unsigned char vRS){
   unsigned char data;
   .....;
   return data;
}

void wr_LCDreg(unsigned char vRS, unsigned char data){
.....
}


void ckbf(){
.......
}

void initLCD(){
   ckbf();
   // Function set
   //bit 7 6 5 4   3 2 1 0
   //    0 0 1 DL  N F — — 
   //    0 0 1  ...
   wr_LCDreg(...);

   ckbf();
   // entry mode set
   //bit 7 6 5 4   3 2 1 0
   //...     
   //    
   wr_LCDreg(...);

   ckbf();
   // Display on/off control
   //bit 7 6 5 4   3 2 1 0
   // ...    
   //    
   wr_LCDreg();

   ckbf();
   //Clear display
   wr_LCDreg(....);
}

void sysinit(){
   PORTB = ...;
   DDRB  = ...;
   DDRD  = ...;
   wait(120000UL);
   initLCD();
}

/*
void putchLCD( char ch){
.....
}

void putsLCD( char* ch){
......
}

void clrLCD(){
.......
}

*/
