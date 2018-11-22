// -----------------------------------------------------------------
// Exercise 1
// -----------------------------------------------------------------

#include <FrameStream.h>
#include <Frameiterator.h>
#include <avr/io.h>
#include "Display.h"


#define OUTPUT__BAUD_RATE 57600
FrameStream frm(Serial1);

// Forward declarations
void InitGUI();

// hierarchical runnerlist, that connects the gui elements with
// callback methods
declarerunnerlist(GUI);

// First level will be called by frm.run (if a frame is recived)
beginrunnerlist();
fwdrunner(!g, GUIrunnerlist); //forward !g to the second level (GUI)
callrunner(!!, InitGUI);
endrunnerlist();

// second level
// SL the slider and Ft the checkbox are registerd here
beginrunnerlist(GUI);
fwdrunner(SL, CallbackSL);
fwdrunner(FT, CallbackFT);
endrunnerlist();


/*
 * this is the callback of the Slider SL
 * slider callback str will contain
 * a number ranging from -200 to 1000 in a string
 */
 
void CallbackFT(char* str, size_t length){
	toggle();
	
		Serial.print(getToggle());
		
	}

void CallbackSL(char* str, size_t length)
{
    // TODO interprete string str as integer
    // TODO map integer to
    // show current number string in Arduinoview
    frm.print("!jdocument.getElementById(\"info_val\").innerText=");
    frm.print(str);
    float x = atof(str);
    int z = atoi(str);
    if(getToggle() == 1){
    	display(x);
    	Serial.print("float");
    } else {
    	display(z);
    	Serial.print("int");
    }
    
   
    
    frm.end();
}


/*
 * @brief initialises the GUI of ArduinoView
 *
 * In this function, the GUI, is configured. For this, Arduinoview shorthand,
 * HTML as well as embedded JavaScript can be used.
 */
void InitGUI()
{
    frm.print(F("!h<h1>PKeS Exercise 1</h1>"));
    frm.end();

    // Generating the Slider SL
    frm.print("!SsSL");
    frm.end();
    frm.print("!ScFT");
		frm.end();
    // modify the Slider using JavaScript
    frm.print("!jvar x=document.getElementById(\"_SD_SL\");");
    frm.print("x.max= 1000;");
    frm.print("x.min=-200;");
    frm.print("x.step=.01;");
    frm.print("x.style.width=\"100%\";");
    frm.end();

    // generate some Space to display Information
    frm.print(F("!H<div>Slider value: <span id=info_val></span></div>"));
    frm.end();
}

/*
 * @brief Initialisation
 *
 * Implement basic initialisation of the program.
 */
void setup()
{
    // give the Web-Interface a bit time to connect
    // to all outputs.
    delay(1000);
	
    //prepare Serial interfaces
    Serial.begin(OUTPUT__BAUD_RATE);
    Serial1.begin(OUTPUT__BAUD_RATE);

    Serial.println(F("Willkommen zur PKeS Übung"));
			
    //request reset of GUI
    frm.print("!!");
    frm.end();

    //TODO initialise Display through initDisplay() here
    delay(500);
    
    initDisplay();
    toggleLamp(1);
    delay(1000);
    uint8_t idle[3] = {0b1000000,0b01000000,0b00100000};
			writeToDisplay(idle);
   // display(666);
}



/*
 *  @brief Main loop
 *
 *  This function will be called repeatedly and shall therefore implement the
 *  main behavior of the program.
 */






void loop()
{
    // read & run ArduinoView Frames
    while(frm.run());
}
