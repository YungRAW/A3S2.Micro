#include "defs.h"

#define DELAY 200

#define SPACE_CHARACTER 10 // must be any value <0 or >9

#define STANDARD_MODE 1
#define FIRST_A_PRESS 2
#define SECOND_A_PRESS 3
#define FIRST_B_PRESS 4
#define SECOND_B_PRESS 5

int main(){

   char code_ante = NOKEY;
   char code_now  = NOKEY; 
 
   unsigned char kbhit = 0;
   char kbcode = 0; 
 
   unsigned char loop_cnt=0;

   unsigned int cycles = 0;

   unsigned int seconds = 0; //numar de secunde
   unsigned int minutes = 56; //numar de minute
   unsigned int hours = 8; //numar de ore

   unsigned int left_seconds = 0; //valoarea secundei din partea stanga
   unsigned int left_minutes = 0; //valoarea minutului din partea stanga
   unsigned int left_hours = 0; //valoarea orei din partea stanga

   unsigned int right_seconds = 0; //valoarea secundei din partea dreapta
   unsigned int right_minutes = 0; //valoarea minutului din partea dreapta
   unsigned int right_hours = 0; //valoarea orei din partea dreapta 
   
   char stare = STANDARD_MODE;

   sysinit();

   //bit 7 - FOC2 nu se programeaza, ramne 0
   //bits 3,6 - WGM21 si WGM20 = 10, adica mod CTC
   //bits 5:4 - COM21 si COM20 nu se programeaza, raman 0
   //bits 2:0 - CS22, CS21 si CS20 = 110, adica clk/256 (de la prescaler)
   TCCR2 = 0b00001110;

   //numarator modulo 250
   OCR2 = 250 - 1; //N = 250

   while(1){  //bucla principala

      if(loop_cnt==DELAY){ //citirile se fac din 15 ms in 15 ms
         loop_cnt=0; 
 
         code_ante = code_now;
         code_now  = kbscan();

         if( code_ante == NOKEY && code_now != NOKEY){
            kbhit=1;
            kbcode=code_now;
         }
      }
      
      //consuma codul
      if(kbhit){
         kbhit=0;
         gotoLC(1,1); //seteaza cursorul pe linia 1, coloana 1          
         putchLCD(kbcode);
      }       //……       
      
      //PORTB &= 0<<1;

      loop_cnt++;
	  
	  switch(stare){
		  case STANDARD_MODE:
			if(TIFR & 1<<OCF2){

				TIFR |= 1<<OCF2;
				cycles++;
	
				if(cycles == 62){ //k/2
					gotoLC(2,3);
					putchLCD(32);
					gotoLC(2,6);
					putchLCD(32);
				}
		
				if(cycles == 125){ //k = 125
					cycles = 0;
					seconds++;
		
					if(seconds > 59){
						seconds = 0;
						minutes++;
					}
		
					if(minutes > 59){
						minutes = 0;
						hours++;
					}
		
					if(hours > 23){
						hours = 0;
					}
		
					gotoLC(2,1); //seteaza cursorul pe linia 2, coloana 1 
		
					left_hours = hours / 10;
					
					if(left_hours == 0){
						left_hours = SPACE_CHARACTER;
					}
					
					right_hours = hours % 10;
					putchLCD( int_to_char(left_hours) );
					putchLCD( int_to_char(right_hours) );
					putchLCD(':');
		
					left_minutes = minutes / 10;
					right_minutes = minutes % 10;
		
					putchLCD( int_to_char(left_minutes) );
					putchLCD( int_to_char(right_minutes) );
					putchLCD(':');
		
					left_seconds = seconds / 10;
					right_seconds = seconds % 10;
					
					putchLCD( int_to_char(left_seconds) );
					putchLCD( int_to_char(right_seconds) );
		
					if(TIFR & 1<<OCF2){
						clrLCD();
						putsLCD("Eroare ceas!");
						while(1){}
					}
				}
			}

         if(kbcode == 'A'){
            stare = FIRST_A_PRESS;
            kbcode = 0;

         }
		
			break;
		
		  case FIRST_A_PRESS:
			seconds = 0;
			
			gotoLC(2,1);
			putchLCD( int_to_char(left_hours) );
			putchLCD( int_to_char(right_hours) );
			putchLCD(':');
			
			putchLCD( int_to_char(left_minutes) );
			putchLCD( int_to_char(right_minutes) );
			putchLCD(':');
			
			putchLCD('0');
			putchLCD('0');
			
			if(kbcode == 'B'){
				stare = FIRST_B_PRESS;
            kbcode = 0;
			}
			
			if(kbcode == 'A'){
				stare = SECOND_A_PRESS;
            kbcode = 0;
			}
			break;
			
		  case FIRST_B_PRESS:
			minutes++;
			
			if(minutes > 59){
				minutes = 0;
			}
			
			gotoLC(2,1);
			putchLCD( int_to_char(left_hours) );
			putchLCD( int_to_char(right_hours) );
			putchLCD(':');
			
			left_minutes = minutes / 10;
			right_minutes = minutes % 10;
		
			putchLCD( int_to_char(left_minutes) );
			putchLCD( int_to_char(right_minutes) );
			putchLCD(':');
			
			putchLCD('0');
			putchLCD('0');
			
			if(kbcode == 'B'){
				stare = FIRST_A_PRESS;
            kbcode = 0;
			}
			
			if(kbcode == 'A'){
				stare = SECOND_A_PRESS;
            kbcode = 0;
			}
			
			break;
		 
		 case SECOND_A_PRESS:
			gotoLC(2,1);
			putchLCD( int_to_char(left_hours) );
			putchLCD( int_to_char(right_hours) );
			putchLCD(':');
			
			putchLCD( int_to_char(left_minutes) );
			putchLCD( int_to_char(right_minutes) );
			putchLCD(':');
			
			putchLCD('0');
			putchLCD('0');
			
			if(kbcode == 'B'){
				stare = SECOND_B_PRESS;
            kbcode = 0;
			}
			
			if(kbcode == 'A'){
				stare = STANDARD_MODE;
            kbcode = 0;
			}
			
			break;
			
		 case SECOND_B_PRESS:
			hours++;
			
			if(hours > 23){
				hours = 0;
			}
			
			gotoLC(2,1);
			
			left_hours = hours / 10;
			
			if(left_hours == 0){
				left_hours = SPACE_CHARACTER;
			}
					
			right_hours = hours % 10;
			putchLCD( int_to_char(left_hours) );
			putchLCD( int_to_char(right_hours) );
			putchLCD(':');
			
			putchLCD( int_to_char(left_minutes) );
			putchLCD( int_to_char(right_minutes) );
			putchLCD(':');
			
			putchLCD('0');
			putchLCD('0');
			
			if(kbcode == 'B'){
				stare = SECOND_B_PRESS;
            kbcode = 0;
			}
			
			if(kbcode == 'A'){
				stare = STANDARD_MODE;
            kbcode = 0;
			}
			
			break;
	 }//end switch

      

   } //end while 
} //end main 
