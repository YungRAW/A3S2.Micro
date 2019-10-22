#include "defs.h"

#define S2 250000UL

int main(){

   //unsigned char k;

   sysinit();
   //Pasul 1
   //putchLCD('H');

   //Pasul 2
   //putsLCD("Hello Micro");
   smiley();
   
   while(1){

      //Pasul 6
      clrLCD();
      wait(S2);
      putchLCD(2);
      putsLCD(" is Smiley!");
      wait(S2);      

      
      //Pasul 5
      /*
      clrLCD();                     // Ce se vede pe LCD
      wait(S2);
      putsLCD("0123456789abcdef");  //0123456789abcdef
      wait(S2);                     //_

      putchLCD('g');                //0123456789abcdef
      wait(S2);                     //g_

      clrLCD();                     //_
      wait(S2);                     //

      putsLCD("0123456789abcdefg"); //0123456789abcdef
      wait(S2);                     //g_

      putsLCD("hijklmnopqrstuv");   //0123456789abcdef
      wait(S2);                     //ghijklmnopqrstuv

      putchLCD('w');                //w123456789abcdef
      wait(S2);                     //ghijklmnopqrstuv   
      */
      
         

      //Pasul 3
      /*
      putsLCD("Hello Micro!");
      wait(S2);
      clrLCD();
      wait(S2);
      */

      //Pasul 4
      /*
      putsLCD("0123456789abcdef");
      gotoLC(2,1);
      putsLCD("ghijklmnopqrstuv");

      for(k=16;k>0;k--){
         gotoLC(2,k);
         wait(S2);
      }

      for(k=16;k>0;k--){
         gotoLC(1,k);
         wait(S2);
      }

      clrLCD();
      wait(S2);
      */
   }
}

