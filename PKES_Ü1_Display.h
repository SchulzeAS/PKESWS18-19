#ifndef DISPLAY_H
#define DISPLAY_H

#include <inttypes.h>


//TODO: make Display.cpp implement this interface

// you may create as much helper functions inside
// your Display.cpp as you like
// ADVICE: all values are displayed as decimal (Base 10)
// if you like you may ADD functions that display values
// as hexadecimal (Base 16)

//initialises Display
//Setup Data Direction
//write some default pattern or empty to Display



// writes 3 data-bytes to Display
// these bytes/bits should represent the 7 Segments and Dot
// may be used to implement the pattern or clearing of initDisplay()
// and the writeValue Functions
// prepare to discuss your bitorder
void writeToDisplay(uint8_t data[3]);

//writes an integer value to display
void writeValueToDisplay(int value);
uint8_t representdot(int n);
void pushbit(int bit);
void initDisplay();
void clk();
void latch();
uint8_t interprete(int kes);
uint8_t represent(int n);
void display(int kes);
void display(float kes);
void clearDisplay();
void CallbackFT();

//writes float value to display
//display as many significant digits as possible
//eg. 5.87 ; 14.5; 124.; -12.4
void writeValueToDisplay(float value, int x);
int getToggle();
void toggle();
void toggleLamp(int n);

#endif
