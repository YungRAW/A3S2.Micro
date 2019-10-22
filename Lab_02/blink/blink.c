#include  <avr/io.h> 
 
#define P  125000L 
#define DF 90L 
#define TH (P*DF/100) 
//125 = 1ms 
 
int main()
{    
   volatile long i; 
        
   DDRA=0xff;    
   i=0;
              
   while(1)
   {
      if(i==0)
         PORTA=1;  //aprinde LED-ul 
 
      if(i==TH) 
         PORTA=0;  //stinge LED-ul 
 
      i++;       
      if(i==P)
         i=0;     //a trecut o secunda           
   } 
}    
