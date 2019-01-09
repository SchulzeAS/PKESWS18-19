#include "Distance.h"
#include <Arduino.h>
#include <avr/io.h>
#include <math.h>

uint16_t ADC_mV_Faktor;

/**
 * @brief Configures the ADC related to mode, clock speed, gain
 *        resolution and refrence voltage
 *
 *
 * chose the reference voltage carfully related to the output
 * range of the sensor.
 */
void configADC()
{  //clear register
  ADCSRA = 0;
  //prescaler
  ADCSRA |= (1<<ADPS2);
  //enable adc
  ADCSRA |= (1<<ADEN);  
  //clear admux, also set reference to AREF
  ADMUX = 0;

  //set aref to avcc
  ADMUX |= (1 << REFS0);

  //do first conversion to get longest conversion out of the way
  ADCSRA |= (1 << ADSC);
}

/**
 * @brief Starts a single conversion and receives a result return in mV
 */
 
int16_t currdist(){
	if(readSR()>170){
		return readLR();
	} else {
		return readSR();
	}
}
int16_t readADC(uint8_t channel)
{
    //clear admux
  ADMUX &= 0xf0;
  
  //set pin
  ADMUX |= channel;
  
	//start conversion
	ADCSRA |= (1<<ADSC);

  //clear interrupt flag
  ADCSRA |= (1 << ADIF);
  
  while(ADCSRA & (1<<ADIF));//wait for sample end

  ADCSRA |= (1 << ADIF);
  
	return ADC;//is adch, adcl
}
  
  
  

/**
 * @brief Maps the digital voltage information on a distance
 *        in mm
 */
 uint16_t readLR(){
 	readADC(0);
 	uint16_t x = readADC(1);
 	readADC(2);
 	return linearizeLR(x);
 }
 uint16_t readSR(){
 	readADC(0);
 	readADC(1);
 	uint16_t x = readADC(2);
 	return linearizeSR(x);
 }
uint16_t linearizeSR(uint16_t x)
{//y = -113.0748*x + 3465.13 sr
	return  -83.46745 + (8661.313 +83.46745)/(1 + powf((x/1.873573),0.7570678));
}

/**
 * @brief Maps the digital voltage information on a distance
 *        in mm
 */
uint16_t linearizeLR(uint16_t x)
{ return  (double)179.7001 + ((double)((double)931.2356 - (double)179.7001)/(double)((double)1 + (double)powf(((double)x/(double)317.5873),(double)2.854422)));
}
