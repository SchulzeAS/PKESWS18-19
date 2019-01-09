
#include <util/delay.h>
#include "Display.h"
#include <avr/io.h>
#include <stdbool.h>
#include <Arduino.h>

void initDisplay() {
	DDRD |= (1<<PD4);
	DDRB |= (1<<PB1) | (1<<PB2);
	PORTB |= (1<<PB4);
}

void writeToDisplay(uint8_t* data) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++) {
			if (data[i] & (1<<j)) {
				PORTB |= (1<<PB2);
			} else {
			  PORTB &= ~(1<<PB2);
			}

			PORTB |= (1<<PB1);
			delay(10);
			PORTB &= ~(1<<PB1);
			delay(10);
		}
	}

	PORTD &= ~(1<<PD4);
	PORTD |= (1<<PD4);
}

void convertNumberToBinary(int number,int floatPoint,bool negative, bool isInteger,uint8_t* data) {
	
	for (int i = 0; i < 3; i++) {
		data[i] = 0;
	}
	
	uint8_t numbers[10]={0b01111110,	//0
									0b01000010,	//1
									0b11101100,	//2
									0b11100110,	//3
									0b11010010,	//4
									0b10110110,	//5
									0b10111110,	//6
									0b01100010,	//7
									0b11111110,	//8
									0b11110110};//9
	if (negative) {
		data[0] |= 1<<7;		//add negative
		if (number > 199) {
			data[1] |= 1<<7;
			data[2] |= 1<<7;
		} else if ((number > 9 && number < 19) || number == 1 || number > 99) {
			if (!(isInteger && floatPoint>2)) {
				data[0] |= numbers[(number/100)%10];
			}
			if (!(isInteger && floatPoint>1)) {
				data[1] |= numbers[(number/10)%10];
			}
			if (!(isInteger && floatPoint>0)) {
				data[2] |= numbers[number%10];
			}
		} else {
			if (!(isInteger && floatPoint>1)) {
				data[1] |= numbers[(number/10)%10];
			}
			if (!(isInteger && floatPoint>0)) {
				data[2] |= numbers[number%10];
			}
		}
	} else {
		if (number > 999) {
			data[0] |= 1<<7;
			data[1] |= 1<<7;
			data[2] |= 1<<7;
		} else {
			if (!(isInteger && floatPoint>2)) {
				data[0] |= numbers[(number/100)%10];
			}
			if (!(isInteger && floatPoint>1)) {
				data[1] |= numbers[(number/10)%10];
			}
			if (!(isInteger && floatPoint>0)) {
				data[2] |= numbers[number%10];
			}
		}
	}
	if(!isInteger){
		if (floatPoint==1) {
			data[1] |= 1; //set single bit for dot
		}	else if (floatPoint==2) {
			data[0] |= 1; //set single bit for dot
		} else{
			data[2] |=1;
		}
	}
}

int calcFloatPoint(double number,bool negative,bool isInteger) {
	number= number<0 ? number*-1 : number;
	if(negative && number>=10.0 && number<20.0) {
		return 2;
	} else if(number>=100.0 )
		return 0;
	else if(number>=10.0)
		return 1;
	else
		return 2;
}

int convertNumber(double number,int floatPoint,bool negative){
	number= number<0 ? number*-1 : number;
	if(negative && number>=10.0 && number<20.0) {
			return (int)(number*10);
	}	else if(floatPoint==1){
		return (int)(number*10.0);
	} else if(floatPoint==0 || floatPoint==-1){
		return (int)number;
	} else {
		return (int)(number*100.0);
	}
}

void writeValueToDisplay(double number,bool isInteger){
	bool negative=number<0 ? true : false;
	int floatPoint=calcFloatPoint(number,negative,isInteger);
	if(negative && (((int)number*-1)%100!=1 || ((int)number*-1)%10!=1)){
		floatPoint--;
	}
	uint8_t data[3];
	convertNumberToBinary(convertNumber(number,floatPoint,negative),floatPoint,negative,isInteger, data);
	writeToDisplay(data);
}
