#include <avr/io.h> 
 
int main(){    
	// valorile memorate ale intrarilor    
	unsigned char inputs;    
	// Daca nu va place unsigned char puteti folosi uint8_t definit in stdint.h    
	// uint8_t este definit ca: typedef unsigned char uint8_t    
	// Variabila xi, (i=0,1,2) memoreaza in bitul i valoarea intrarii xi.     
	// Ceilalti biti sunt 0.    
	unsigned char x3, x2, x1, x0; 
 
    // in variabila outs se asambleaza iesirile astfel:    
    // bit   7 6 5 4   3  2  1  0    
    // outs= - - - -   -  f2 f1 f0    
    unsigned char outs;    
    unsigned char temp; 
   
    DDRA = 0b11111111; //sau DDRA = 0xff;
    DDRB = 0b00000000; //sau DDRB = 0x00;
   
    while (1){
		//memoreaza valorile variabilelor de intrare       
		//bit      7654 3 2  1  0       
		//inputs =        x2  x1  x0
	    inputs = PINB;
	    inputs &= 0b00001111;
	   
	    x0 = inputs & 1<<0;
	    x1 = (inputs>>1) & 0b00000001;
	    x2 = (inputs>>2) & 0b00000001;
       x3 = (inputs>>3) & 0b00000001;	   

	    outs = 0;
		
	    //f0
	    temp = x3 ^ x2 ^ x1 ^ x0;
	    if (temp & 1<<0)
		    outs |= 0b00000001;
	   
	    //f1
	    if (inputs < 3)
			 outs |= 0b00000010;
		
		//f2
		//temp = (x2 & x0) | (~x2 & x1);
		if (inputs == 2 || inputs == 3 || inputs == 5 || inputs == 7 || inputs == 11 || inputs == 13)
			outs |= 0b00000100;

      if (inputs == 7 || inputs == 11 || inputs > 12)
         outs |= 0b00001000;
		
		PORTA = outs;
	}  
}
