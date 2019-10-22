#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define C1 1
#define C2 2
#define C3 3

#define IFLAG 7

volatile unsigned char kbhit=0, kbcode;

unsigned int h = 23, m = 59, s = 50;

int main(){
   unsigned char stare = C1;
   char buf_pit[4];
   unsigned long int a,b,c;

   unsigned char pitagoreic = 0;

   char msg_busy[] = "Busy... ";

   sysinit();
   
   //bit 7 - FOC0 nu se programeaza, ramne 0
   //bits 3,6 - WGM01 si WGM00 = 11, adica mod CTC
   //bits 5:4 - COM01 si COM00 = 10
   //bits 2:0 - CS02, CS01 si CS00 = 100, adica clk/256 (de la prescaler)
   TCCR0 = 0b01101100;

   //bit 7 - FOC2 nu se programeaza, ramne 0
   //bits 3,6 - WGM21 si WGM20 = 10, adica mod CTC
   //bits 5:4 - COM21 si COM20 nu se programeaza, raman 0
   //bits 2:0 - CS22, CS21 si CS20 = 110, adica clk/256 (de la prescaler)
   TCCR2 = 0b00001110;

   OCR0 = 128; //50% duty cycle
   OCR2 = 250 - 1;

   setbit (DDRB, 3); //OC0 pin is output

   setbit(TIMSK, TOIE0); //am demascat cerea de intrerupere TOV0
   setbit(TIMSK, OCIE2); //am demascat cerea de intrerupere OCF2
   setbit(SREG, IFLAG); //pentru a demasca intreruperea procesorului
   
   while(1){

	  //c pitagoreic start
      if( kbhit && kbcode>='0' && kbcode<='9'){
         kbhit=0;

         switch(stare){
         	case C1:   //cifra 0
               gotoLC(1, 1);
               putsLCD("                ");
               gotoLC(1, 1);
               buf_pit[0]=kbcode;
               putchLCD(kbcode); //scrie prima cifra
               stare=C2;
               break;

            case C2:
               buf_pit[1]=kbcode;
               gotoLC(1,2);
               putchLCD(kbcode); //scrie a doua cifra
               stare=C3;
               break;
         
            case C3:
               buf_pit[2]=kbcode;
               buf_pit[3]='\0';
               gotoLC(1,3);
               putchLCD(kbcode); //scrie a treia cifra
               gotoLC(1,8);
               for(int i=0; msg_busy[i] != 0; i++){
                  putchLCD(msg_busy[i]); //scrie "Busy... ," pe linia 1, din coloana 8
                  wait(50000);
               }
               c = atoi(buf_pit); //calculeaza c

               for(a = 1; a <= c; a++){
                  for(b = 1; b <= c; b++){
                     if(a * a + b * b == c * c){ //verifica daca a^2 + b^2 = c^2
                        pitagoreic = 1;
                        goto show_result;
                     }
                  }
               }

               show_result:
               
               gotoLC(1,8);
      		   if(pitagoreic){
      		      putsLCD("PYTH    ");
      		   }
      		   else{
      		      putsLCD("nonPYTH ");
      		   }

               pitagoreic = 0;

               stare=C1;
               break;
         }//end switch     
      }// end c pitagoreic      
   }//end while
}//end main


ISR(TIMER0_OVF_vect){

   static unsigned char code_now = NOKEY, code_ante;
   static unsigned char loop_cnt = 0;

   static int lumi = 50;
   char buf_lumi[4];

   if(loop_cnt==4){ //4 sau altã valoare
      loop_cnt=0;
      code_ante = code_now;
      code_now = kbscan();

      if( code_ante == NOKEY && code_now != NOKEY){
         kbhit=1;
         kbcode=code_now;
      }
   }

   //PWM start
   if(kbhit  && (kbcode == 'C' || kbcode == 'D')){
      kbhit = 0;

      if(kbcode == 'C'){
         //lumi=//...creste luminozitatea
         lumi += 5;
         
         if(lumi > 100){
            lumi = 100;
         }
      }

      if(kbcode == 'D'){
         //lumi=//...scade luminozitatea
         lumi -= 5;

         if(lumi < 0){
            lumi = 0;
     
         }
      }

      //lumi% duty cycle. Se aplica regula de 3 simpla
      //100.......255
      //lumi......x
      //x = 255 * lumi / 100
      OCR0 = (255 * lumi) / 100;
      gotoLC(2,11);
      putsLCD("L=");
      itoa(lumi, buf_lumi, 10);
      putsLCD(buf_lumi);
      putsLCD("%  " );

   }//PWM end
   loop_cnt++;
}

ISR(TIMER2_COMP_vect){

   static unsigned int cycles = 0;

   cycles++;

   if(cycles == 125){
      cycles = 0;
      s++;

      if(s > 59){
         s = 0;
         m++;
      }

      if(m > 59){
         m = 0;
         h++;
      }

      if(h > 23){
         h = 0;
      }

      gotoLC(2,1);
      if( (h / 10) != 0 ){
         putchLCD(h / 10 + 48);
      }
      else{
         putchLCD(' ');
      }

      putchLCD(h % 10 + 48);
      putchLCD(':');
      putchLCD(m / 10 + 48);
      putchLCD(m % 10 + 48);
      putchLCD(':');
      putchLCD(s / 10 + 48);
      putchLCD(s % 10 + 48);
   }//end if
}//end ISR


