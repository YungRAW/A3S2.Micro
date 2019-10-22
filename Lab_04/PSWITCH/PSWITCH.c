#include <avr/io.h>

int main(){
   unsigned char inputs;

   DDRA = 0xff;
   DDRB = 0;

   while(1){
      inputs = PINB & 0b00000111;

      switch (inputs) {
      
      case 0 : PORTA = 0b10000001;
               break;
      case 1 : PORTA = 0b11001111;
               break;
      case 2 : PORTA = 0b10010010;
               break;
      case 3 : PORTA = 0b10000110;
               break;
      case 4 : PORTA = 0b11001100;
               break;
      case 5 : PORTA = 0b10100100;
               break;
      case 6 : PORTA = 0b10100000;
               break;
      case 7 : PORTA = 0b10001111;
               break;
      }
   }
}
