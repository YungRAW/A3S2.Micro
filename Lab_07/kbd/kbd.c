#include "defs.h"

#define DELAY 200

int main(){
   
   char code_ante = NOKEY;
   char code_now  = NOKEY; 
 
   unsigned char kbhit = 0;
   char kbcode; 
 
   unsigned char loop_cnt=0;
   
   sysinit();

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
         putchLCD(kbcode);
      }       //……       
      
      //PORTB &= 0<<1;

      loop_cnt++;
   } //end while 
} //end main 
