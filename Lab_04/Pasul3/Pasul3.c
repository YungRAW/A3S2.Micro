#include <avr/io.h>
#include <avr/pgmspace.h>

const unsigned char segLUT[] PROGMEM = {
   0b10000001, //0
   0b11001111, //1
   0b10010010, //2
   0b10000110, //3
//-----------------
   0b11001100, //4
   0b10100100, //5
   0b10100000, //6
   0b10001111, //7
//------------------
   0b10000000, //8
   0b10000100  //9
};

int main(){
   unsigned char inputs;

   DDRA = 0xff;
   DDRB = 0;

   while(1){
      inputs = PINB & 0b00001111;

      if (inputs < 10)
         PORTA = pgm_read_byte (&segLUT[inputs]);
      else
         PORTA = 0b11111110;
   }
}
