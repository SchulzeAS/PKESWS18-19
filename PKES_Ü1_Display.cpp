#include "Display.h"
#include <avr/io.h>

int bitat(int8_t arr, int pos){
	
	return (arr >> pos) & 1;
}

void clearDisplay(){
	uint8_t clear[3] = {0b00000001,0b0000001,0b00000001};
	writeToDisplay(clear);
}
void initDisplay(){
										//representation: dp,
	uint8_t idle[3] = {0b01000010,0b11101100,0b11100110};
	//setup, set Data as outs, clck, data, latch i.o
	DDRB |= (1<<DDB1);
	DDRB |= (1<<DDB2);
	DDRD |= (1<<DDD4);
	DDRC |= (1<<DDC6);
	DDRC |= (1<<DDC7);
	
	writeToDisplay(idle);
	//now set idle pattern, somehow. please help.
	
}
void writeToDisplay(uint8_t data[3]){
	//push single bits to data, clock after each one. latch at the end.
	
    	
    	for(int i = 0; i < 24; i++){
    		//try to do something else than printing the correct bit
    		pushbit(bitat(data[i/8],i%8));
    	  clk();}
		latch();
}
void pushbit(int bit){
	// snippet for low PORTC &= ~(1<<PORTC6);
	 //snipppet for high PORTC |= (1<<PORTC6);
        if(bit == 0){
        	PORTB &= ~(1<<PORTB2);
        } else {
        	//must be 1 else me stupid
        	PORTB |= (1<<PORTB2);
        }
        //theoretically bit is now in data, just clock afterwards, hopefully.
}
void clk(){
	PORTB |= (1<<PORTB1);
	PORTB &= ~(1<<PORTB1);
}
void latch(){
	PORTD |= (1<<PORTD4);
	PORTD &= ~(1<<PORTD4); 
	
}
//interprete int
void display(int kes){
	uint8_t first = 0b00000000;
	uint8_t second = 0b00000000;
	uint8_t third = 0b00000000;
	int minus = 0;
	if(kes<0){
		minus = 1;
		kes *= -1;
	}
	//"Setup done"
		if(kes>0 && kes <10){
		//display first= msb, with comma, then next
			kes *=100;
			 first = represent(((int)kes)%10);
	kes/=10;
	 second = represent(((int)kes)%10);
	kes /= 10;
	 third = represent(kes);

		
	} else if(kes>=10 && kes <100){
		//display first&second, comma one digit
		kes *=10;
			 first = represent(((int)kes)%10);
	kes/=10;
	 second = represent(((int)kes)%10);
	kes /= 10;
	 third = represent(kes);
	
	} else if(kes>=100 && kes <1000){
		//display normally, add comma to last digit.//should work.
		 first = represent(((int)kes)%10);
	kes/=10;
	 second = represent(((int)kes)%10);
	kes /= 10;
	 third = represent(kes);} else if(kes < 0 && kes > -10){
	 	kes *= -1;
	 	first = 0b10000000;
	 	second = represent(((int)kes)%10);
	 	kes /= 10;
	 	third = represent(((int)kes));
	 	
	 } 
	 if(minus == 1){
		if(minus == 1 && third == 0b01000010){
			third = 0b11000010;
		} else {
		first = second;
		second = third;
		third = 0b10000000;}
		
  	
	
}
uint8_t data[3] = {third,second,first};
	writeToDisplay({data});
}
//doesnt work
void display(float kes){
	//keine -float
	uint8_t first = 0b00000000;
	uint8_t second = 0b00000000;
	uint8_t third = 0b00000000;
	///////fcgvhjkhgfdxcghvjklhgfgxdhguiuztdfxgzuilkuzftdxui vcbn,kjchxfghjhklhgjfgchx
	int minus = 0;
	if(kes<0){
		minus = 1;
		kes *= -1;
	}
	if(kes>0 && kes <10){
		//display first= msb, with comma, then next
			kes *=100;
			 first = represent(((int)kes)%10);
	kes/=10;
	 second = represent(((int)kes)%10);
	kes /= 10;
	 third = representdot(kes);

		
	} else if(kes>=10 && kes <100){
		//display first&second, comma one digit
		kes *=10;
			 first = represent(((int)kes)%10);
	kes/=10;
	 second = representdot(((int)kes)%10);
	kes /= 10;
	 third = represent(kes);
	
	} else if(kes>=100 && kes <1000){
		//display normally, add comma to last digit.//should work.
		 first = representdot(((int)kes)%10);
	kes/=10;
	 second = represent(((int)kes)%10);
	kes /= 10;
	 third = represent(kes);} else if(kes < 0 && kes > -10){
	 	kes *= -1;
	 	first = 0b10000000;
	 	second = represent(((int)kes)%10);
	 	kes /= 10;
	 	third = represent(((int)kes));
	 	
	 } 
if(minus == 1){
		if(minus == 1 && third == 0b01000010){
			third = 0b11000010;
		} else {
		first = second;
		second = third;
		third = 0b10000000;}
  	
	

}
	 
	 
	 
	uint8_t data[3] = {third,second,first};
	writeToDisplay({data});
}

void toggleLamp(int n){
	if(n==1){
		// snippet for low PORTC &= ~(1<<PORTC6);
	 //snipppet for high PORTC |= (1<<PORTC6); 44
	 PORTC |= (PORTC6);
	 PORTC |= (PORTC7);
		//lamps on
	} 
	if(n==0){
 	 PORTC &= ~(1<<PORTC6);
 	 PORTC &= ~(1<<PORTC7);
		//lamps off
	}
	
}
uint8_t represent(int n){
	switch ( n )  
      {  
         case 0: return 0b01111110; 
              
            break;  
        		case 1:  return 0b01000010;
              
            break;  
            case 2:  return 0b11101100;
              
            break; 
            case 3:  return 0b11100110;
              
            break; 
            case 4:  return 0b11010010;
              
            break; 
            case 5:  return 0b10110110;
              
            break; 
            case 6:  return 0b10111110;
              
            break; 
            case 7:  return 0b01110010;
              
            break; 
            case 8:  return 0b11111110;
              
            break; 
            case 9:  return 0b11110110;
              
            break; 
         
              
      }  
      
}
uint8_t representdot(int n){
	switch ( n )  
      {  
         case 0: return 0b01111111; 
              
            break;  
        		case 1:  return 0b01000011;
              
            break;  
            case 2:  return 0b11101101;
              
            break; 
            case 3:  return 0b11100111;
              
            break; 
            case 4:  return 0b11010011;
              
            break; 
            case 5:  return 0b10110111;
              
            break; 
            case 6:  return 0b10111111;
              
            break; 
            case 7:  return 0b01110011;
              
            break; 
            case 8:  return 0b11111111;
              
            break; 
            case 9:  return 0b11110111;
              
            break; 
         
              
      }  
      
}
static int x = 0;
      void toggle(){
      	if(x == 0){x = 1;} else{x=0;}
      	
      }
      int getToggle(){
      	return x;
      }

// TODO: Write a Driver for the LED-Display
