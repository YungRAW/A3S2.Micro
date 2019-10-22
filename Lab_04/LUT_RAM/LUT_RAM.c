#include <avr/io.h>

const unsigned char segLUT[] = {
   0b10000001, //0
   0b11001111, //1
   0b10010010, //2
   0b10000110, //3
//-----------------
   0b11001100, //4
   0b10100100, //5
   0b10100000, //6
   0b10001111  //7
};

int main(){
   unsigned char inputs;

   DDRA = 0xff;
   DDRB = 0;

   while(1){
      inputs = PINB & 0b00000111;

      PORTA = segLUT[inputs];
   }
}
