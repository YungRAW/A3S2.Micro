#include "defs.h"

#define FMSG   0
#define WAIT   1
#define RING   2
#define OPEN   3
#define COD1   4
#define COD2   5
#define COD3   6
#define ERR    7

int main(){

   char code_now= NOKEY, code_ante;
   unsigned char kbhit=0;
   char kbcode = 0;
   
   unsigned char loop_cnt=0;
   volatile unsigned long int delay = 0;
   
   char cod[4]="123"; //sau int cod = 123;
   char buf[4] ="999";
   char stare = FMSG;

   char cmd = 0;
   unsigned char contor = 0;
      
   sysinit();
   
   while (1){
      //determina daca este tasta apasata
      if(loop_cnt==100){
         code_ante=code_now;
         code_now = kbscan();
         if(code_now!=NOKEY && code_ante==NOKEY){
            kbhit=1;
            kbcode=code_now;
         }
         loop_cnt=0;
      }
      
      //SFSM
      switch (stare){
         case FMSG:
            clrLCD();
            putsLCD("Suna la 1..8,");
            gotoLC(2,1);
            putsLCD("C pentru cod:");
            stare = WAIT;
            break;
         
         case WAIT:
            if(kbhit){
               kbhit=0;
               if ('C'==kbcode){
                  clrLCD();
                  putsLCD("Cod=");
                  stare = COD1;
               }
               if (kbcode>='1' && kbcode <= '8'){
                  clrLCD();
                  putsLCD("Sun la ");
                  putchLCD(kbcode);                  
                  stare = RING;
               }
            }               
            break;
            
         case RING:
            if( (PINB & 1<<0) == 1 ){
               cmd = 1;
            }               

            if(cmd == 1){
               contor = 0;
               cmd = 0;
               delay = 0;
               stare = OPEN;
               clrLCD();
               putsLCD("Deschis!");
               break;
            }

            switch(delay){
               case 0 : clrLCD();
                        putsLCD("Sun la ");
                        putchLCD(kbcode);
                        wait(250000UL); //asteapta 1 secunda
                        delay = 1;
                        contor++;
                        break;

               case 1 : clrLCD();
                        putsLCD("Sun la ");
                        putchLCD(kbcode);
                        putsLCD(".");
                        wait(250000UL); //asteapta 1 secunda
                        delay = 2;
                        contor++;
                        break;

               case 2 : clrLCD();
                        putsLCD("Sun la ");
                        putchLCD(kbcode);
                        putsLCD("..");
                        wait(250000UL); //asteapta 1 secunda
                        delay = 3;
                        contor++;
                        break;

               case 3 : clrLCD();
                        putsLCD("Sun la ");
                        putchLCD(kbcode);
                        putsLCD("...");
                        wait(250000UL); //asteapta 1 secunda
                        delay = 0;
                        contor++;
                        break;
            }//end switch(delay)
            
            if(contor == 15 && cmd == 0){
               delay = 0;
               contor = 0;
               stare = FMSG;
            }

            //Pasul 1
            //wait(3750000UL); //asteapta 15 secunde == 250000 x 15
            //if(cmd == 0){
            //   stare = FMSG;
            //}

            break;
            
         case COD1:
            if(kbhit){
               kbhit=0;
               if (kbcode>='0' && kbcode <= '9'){
                  buf[0]=kbcode;
                  putchLCD('*');
                  stare = COD2;               
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case COD2: 
            if(kbhit){
               kbhit=0;
               if (kbcode >= '0' && kbcode <= '9'){
                  buf[1] = kbcode;
                  putchLCD('*');
                  stare = COD3;               
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case COD3: 
            if(kbhit){
               kbhit=0;
               if (kbcode >= '0' && kbcode <= '9'){
                  buf[2] = kbcode;
                  putchLCD('*');
                  clrLCD();
                  
                  if(cod[0] == buf[0] && cod[1] == buf[1] && cod[2] == buf[2]){
                     putsLCD("Deschis!");
                     stare = OPEN;
                  }
                  else{
                     putsLCD("Cod invalid");
                     stare = ERR;
                  }
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case OPEN: 
            wait(1250000UL); //asteapta 5 secunde == 250000 x 5
            stare = FMSG;
            break;
            
         case ERR: 
            wait(1250000UL); //asteapta 5 secunde == 250000 x 5
            stare = FMSG;
            break;
            
      }//end switch
      loop_cnt++;
      
      if(stare == OPEN)
         setbit(PORTB, 1);
      else
         clrbit(PORTB, 1);

 }//end while
}//end main
