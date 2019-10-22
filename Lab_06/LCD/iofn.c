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

//Pasul 1
void putchLCD( char ch){
   ckbf();
   wr_LCDreg(DReg, ch);
}

//Pasul 2
/*
void putsLCD( char* ch){
   while(*ch){
      ckbf();
      wr_LCDreg(DReg, *ch);
      ch++;
   }
}
*/

//Pasul 3
void clrLCD(){
   ckbf();
   //Clear display
   //bit 7 6 5 4   3 2 1 0
   //    0 0 0 0   0 0 0 1
   wr_LCDreg(IReg, 0x01);
}

//Pasul 4
void gotoLC(unsigned char line, unsigned char col){
   unsigned char position = 0;

   if(line == 1){
      position |= 1<<7;
      position |= col - 1;

      ckbf();
      // Set DDRAM address
      //bit 7   6   5   4     3   2   1   0
      //    1 ADD ADD ADD   ADD ADD ADD ADD
      //    1   0   0   0   col col col col
      wr_LCDreg(IReg, position);
   }
   else if(line == 2){
      position |= 1<<6 | 1<<7;
      position |= col - 1;

      ckbf();
      // Set DDRAM address
      //bit 7   6   5   4     3   2   1   0
      //    1 ADD ADD ADD   ADD ADD ADD ADD
      //    1   1   0   0   col col col col
      wr_LCDreg(IReg, position);
   }
}

//Pasul 5: Wrap
void putsLCD( char* ch){
   while(*ch){
      ckbf();
      wr_LCDreg(DReg, *ch++);
      
      ckbf();
      _delay_us(8);

      //0x10 == 16
      if(rd_LCDreg(SReg) == 0x10)
         gotoLC(2,1);

      //0x50 == 80
      if(rd_LCDreg(SReg) == 0x50)
         gotoLC(1,1);
   }
}

//Pasul 6: Smiley
void smiley(){
   ckbf();
   // Set CGRAM address
   //bit 7 6   5   4     3   2   1   0
   //    0 1 ACG ACG   ACG ACG ACG ACG
   //    0 1   0   1     0   0   0   0
   wr_LCDreg(IReg, 0b01010000);

   ckbf();
   wr_LCDreg(DReg, 0b01000000); //line 1

   ckbf();
   wr_LCDreg(DReg, 0b01001010); //line 2

   ckbf();
   wr_LCDreg(DReg, 0b01000000); //line 3

   ckbf();
   wr_LCDreg(DReg, 0b01000100); //line 4

   ckbf();
   wr_LCDreg(DReg, 0b01010001); //line 5

   ckbf();
   wr_LCDreg(DReg, 0b01001110); //line 6

   ckbf();
   wr_LCDreg(DReg, 0b01000000); //line 7

   ckbf();
   wr_LCDreg(DReg, 0b01000000); //line 8

}
