#include <avr/io.h>
#include <Arduino.h>

#define IN_M1A PB5
#define IN_M1B PB6

#define IN_M2B PD6
#define IN_M2A PD7

void activateMotors(){
	DDRB|=(1<<IN_M1A) | (1<<IN_M1B);
	DDRD|=(1<<IN_M2A) | (1<<IN_M2B);
	/*
	PORTB|= (1<<IN_M1A);
	PORTB&=~(1<<IN_M1B);
	PORTD&= ~(1<<IN_M2A);
	PORTD|= (1<<IN_M2B);
	*/
}

void deactivateMotors(){
	DDRB&=~((1<<IN_M1A) | (1<<IN_M1B));
	DDRD&=~((1<<IN_M2A) | (1<<IN_M2B));
	/*
	PORTB&= ~((1<<IN_M1A) | (1<<IN_M1B));
	PORTD&= ~((1<<IN_M2A) | (1<<IN_M2B));
	*/
}

void initMotors(){
	Serial.println("init");
	TCCR4D&=~((1<<0) & (1<<1)); 	//set WGM40..41 to zero => enable fast pwm
	TCCR4C|=(1<<0);								//enable fast pwm on M2 => PD6/PD7 => PWM4D
	TCCR4A|=(1<<0);								//enable fast pwm on M1 => PB5/PB6
	
	TCCR4C|=(1<<3);			//set compare output mode for M2 => PWM4D
	TCCR4C&=~(1<<2);
	
	TCCR4A|=(1<<5);			//set compare output mode for M1 => PWM4B
	TCCR4A&=~(1<<4);
	
	TCCR4B|=(1<<0);			//set Prescaler to CK => divide by 1
	TCCR4B&=~((1<<1)|(1<<2)|(1<<3));
	
	OCR4C|=~0;
  
  activateMotors();
}

void setMotors(int8_t left,int8_t right){
	if(left==0){
		OCR4B=0;
		PORTB&= ~((1<<IN_M1A));
	} else if(left<=0){
		OCR4B=-2*left;
		PORTB&=~(1<<IN_M1A);
	} else {
		OCR4B=255-2*left;
		PORTB|= (1<<IN_M1A);
	}
	if(right==0){
		OCR4D=0;
		PORTD&= ~(1<<IN_M2B);
	} else if(right<0){
		OCR4D=255+2*right;
		PORTD|= (1<<IN_M2B);
	} else {
		OCR4D=2*right;
		PORTD&= ~((1<<IN_M2B));
	}
}
