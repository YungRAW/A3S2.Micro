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
   
   DDRD = 0b00000000;

   if (vRS == 0)
      clrbit(PORTB, RS); //RS = '0'
   else
      setbit(PORTB, RS); //RS = '1'

   setbit(PORTB, RW); //RW = '1'

   nop;

   setbit(PORTB, E); //E = '1'

   nop; nop; nop; nop; nop;

   data = PIND;

   clrbit(PORTB, E); //E = '0'
   
   nop;

   DDRD = 0b11111111;

   return data;
}

void wr_LCDreg(unsigned char vRS, unsigned char data){
   
   if (vRS == 0)
      clrbit(PORTB, RS); //RS = '0'
   else
      setbit(PORTB, RS); //RS = '1'

   clrbit(PORTB, RW); //RW = '0'

   PORTD = data;

   nop;

   setbit(PORTB, E); //E = '1'

   nop; nop; nop; nop; nop;

   clrbit(PORTB, E); //E = '0'
   
   nop;
}


void ckbf(){
   
   while(rd_LCDreg(SReg) & 1<<7){}
}

void initLCD(){
   ckbf();
   // Function set
   //bit 7 6 5 4   3 2 1 0
   //    0 0 1 DL  N F — — 
   //    0 0 1 1   1 - - -
   wr_LCDreg(IReg, 0b00111000);

   ckbf();
   // Entry mode set
   //bit 7 6 5 4   3 2   1 0
   //    0 0 0 0   0 1 I/D S
   //    0 0 0 0   0 1   1 0
   wr_LCDreg(IReg, 0b00000110);

   ckbf();
   // Display on/off control
   //bit 7 6 5 4   3 2 1 0
   //    0 0 0 0   1 D C B
   //    0 0 0 0   1 1 1 1
   wr_LCDreg(IReg, 0b00001111);

   ckbf();
   //Clear display
   //bit 7 6 5 4   3 2 1 0
   //    0 0 0 0   0 0 0 1
   wr_LCDreg(IReg, 0x01);
}

void sysinit(){
   PORTB = 0b00000000;
   DDRB  = 0b11100000;
   DDRD  = 0b11111111;
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
