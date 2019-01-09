#include <FrameStream.h>
#include <Frameiterator.h>
#include <avr/io.h>
#include "IMU.h"
#include "Distance.h"
#include "Display.h"
#include "Motor.h"


#define OUTPUT__BAUD_RATE 9600
#define DASH Serial.print("|");
#define BREAK Serial.print("\n");
int border = 1;
FrameStream frm(Serial1);

// Forward declarations
void InitGUI();
float currRotation = 0.0f;


// hierarchical runnerlist, that connects the gui elements with
// callback methods
declarerunnerlist(GUI);

// First level will be called by frm.run (if a frame is recived)
beginrunnerlist();
fwdrunner(!g, GUIrunnerlist); //forward !g to the second level (GUI)
callrunner(!!, InitGUI);
fwdrunner(ST, stickdata);
endrunnerlist();

// GUI level
beginrunnerlist(GUI);
callrunner(re, CallbackREAD);
callrunner(es, CallbackSTOP);
callrunner(ms, CallbackSTART);
endrunnerlist();

int8_t left;
int8_t right;

void stickdata(char* str, size_t length)
{char* left = strtok(str, ",");
    char* right = strtok(NULL, ",");
    setMotors(atoi(left),atoi(right));
    // str contains a string of two numbers ranging from -127 to +127
    // indicating left and right motor values
    // TODO: interprete str, set motors
}

void CallbackREAD(){
//	Serial.print(readADC(0b00000001)); Serial.print(" ");
//	Serial.print(readADC(0b00000010)); Serial.print("IMU --> ");
	IMUdataf c = {0,0,0,0,0,0} ;
	readIMUscale(c);
	Serial.print(readLR()); DASH Serial.print(readSR()); BREAK
//	Serial.print(linearizeSR(readADC(0)));DASH Serial.print(linearizeSR(readADC(0)));DASH 	Serial.print(linearizeSR(readADC(0)));BREAK
//	Serial.print(linearizeLR(readADC(0)));DASH 	Serial.print(linearizeLR(readADC(1)));DASH Serial.print(linearizeLR(readADC(2)));BREAK
	//Serial.print("\n(SR:");Serial.print(readSR()); Serial.print(" | LR:"); Serial.print(readLR()); Serial.print(")");
//	Serial.print(readADC(0)); Serial.print("<-0 | 1->");Serial.print(readADC(1)); Serial.print("<-1 | 2-> ");Serial.print(readADC(2)); Serial.print("<-2\n");
/*	Serial.print(c.accel_z);Serial.print( " ");
	Serial.print(c.gyro_x);Serial.print( " ");
	Serial.print(c.gyro_y);Serial.print( " ");
	Serial.print(c.gyro_z);Serial.print( " | \n ");*/
	
}


void CallbackSTOP()
{deactivateMotors();
    // call deactivateMotors();
}


void CallbackSTART()
{activateMotors();
    // call activateMotors
}

/*
 * @brief initialises the GUI of ArduinoView
 *
 * In this function, the GUI, is configured. For this, Arduinoview shorthand,
 * HTML as well as embedded JavaScript can be used.
 */
void InitGUI()
{
    delay(500);

    frm.print(F("!h<h1>PKeS Exercise 2</h1>"));
    frm.end();

    frm.print(F("!SbesvSTOP"));
    frm.end();
    frm.print(F("!SbrevREAD"));
    frm.end();
    frm.print(F("!SbmsvSTART"));
    frm.end();
    frm.print(F("!SGgrw80%!h300px"));//LRSENSOR
    frm.end();
    frm.print(F("!SGgaw80%!h300px"));//LRSENSOR
    frm.end();
    
    //this implements a joystick field using HTML SVG and JS
    frm.print(F("!H"
      "<div><style>svg * { pointer-events: none; }</style>\n"
      "<div style='display:inline-block'> <div id='state'> </div>\n"
      "<svg id='stick' width='300' height='300' viewBox='-150 -150 300 300' style='background:rgb(200,200,255)' >\n"
      "<line id='pxy' x1='0' y1='0' x2='100' y2='100' style='stroke:rgb(255,0,0);stroke-width:3' />\n"
      "<circle id='cc' cx='0' cy='0' r='3'  style='stroke:rgb(0,0,0);stroke-width:3' />\n"
      "</svg></div><div style='display:inline-block'><div id='info0'></div><div id='info1'></div><div id='info2'></div></div></div>\n"
      "<script>\n"
        "var getEl=function(x){return document.getElementById(x)};\n"
        "var setEl=function(el,attr,val){(el).setAttributeNS(null,attr,val)};\n"
        "var stick=getEl('stick');\n"
        "function sticktransform(x,y){\n"
          "x = x-150;\n"
          "y = -(y-150);\n"
          "setstick(x,y);\n"
        "}\n"
        "function setstick(x,y){\n"
          "setpointer(x,y);\n"
          "l = Math.floor(Math.min(127,Math.max(-127,y + x/2)));\n"
          "r = Math.floor(Math.min(127,Math.max(-127,y - x/2)));\n"
          "setStateDisplay(x,y,l,r);\n"
          "sendframe(\"ST\"+l +\",\"+r);\n"
        "}\n"
        "function setStateDisplay(x,y,l,r){\n"
          "msg=getEl('state');\n"
          "msg.innerHTML= 'x= '+ x +' y= '+ y +' l= '+ l +' r= '+ r ;\n"
        "}\n"
        "function setpointer(x,y){\n"
          "pxy=getEl('pxy');\n"
          "setEl(pxy,'x2',x);\n"
          "setEl(pxy,'y2',-y);\n"
        "}\n"
        "stick.onmousemove=function(e){\n"
          "if( e.buttons == 1 ){\n"
            "sticktransform(e.offsetX,e.offsetY);\n"
        "}};\n"
        "stick.onmousedown=function(e){\n"
          "if( e.buttons == 1 ){\n"
            "sticktransform(e.offsetX,e.offsetY);\n"
        "}};\n"
        "stick.onmouseup=function(e){\n"
          "setstick(0,0);\n"
        "};\n"
        "stick.onmouseleave=function(e){\n"
          "setstick(0,0);\n"
        "};\n"
        "function sendframe(msg){\n"
          "ArduinoView.sendMessage(ArduinoView._createSerialFrame(msg))\n"
        "}\n"
        "setstick(0,0);\n"
      "</script>\n"));
    frm.end();
}

/*
 * @brief Initialisation
 *
 * Implement basic initialisation of the program.
 */
IMUdataf imuoffset;

void setup()
{
	
    delay(1000);
		DDRF &= ~(1<<PF6);
		DDRF &= ~(1<<PF7);
		DDRE &= ~(1<<PE6); 
    //prepare Serial interfaces
    Serial.begin(OUTPUT__BAUD_RATE);
    Serial1.begin(OUTPUT__BAUD_RATE);

    Serial.println(F("Willkommen zur PKeS Übung"));
		configADC();
		initializeIMU();
		initDisplay();
    //request reset of GUI
    frm.print("!!");
    frm.end();

    delay(500);

    // initialise Motors, 8-Segment-Display, ADCs and IMU
}

/*
 *  @brief Main loop
 *
 *  This function will be called repeatedly and shall therefore implement the
 *  main behavior of the program.
 */
#define samplesize 100
void loop()
{  
	static float measures[samplesize];
	static float measuresacc[samplesize];
 static int n;
	char* c[10];
char* d[10];
		float speed = 0.0f;
		//read IMUdataf +2 (static to save speed)
	   static IMUdataf h;
	    readIMUscale(h);
    // read & run ArduinoView Frames
    while(frm.run());
    //scaling accel-readings and current vector velocity +4
    h.accel_x = (h.accel_x * 9.81) / 16384;
    h.accel_y = (h.accel_y * 9.81) / 16384;
    measures[n%samplesize] = sqrt(powf(abs(h.accel_x),2)+ powf(abs(h.accel_y),2));
    n++;
    //calculating avg; +2
    static float avg = 0.0f;
    static float avgacc = 0.0f;
    if(n==samplesize && border){
    	
    	for(int i = 0;i< (samplesize);i++){
    		avg += measures[i];
    		avgacc += measuresacc[i];
    	}
    	avgacc /= samplesize;
    	avg/= samplesize;
    	border = 0;
    	Serial.print("            AVG->"); Serial.print(avg); Serial.print(" "); Serial.print(avgacc);BREAK
    }
    speed = 0;
    for(int i=n%samplesize;i<25;i++){
    	speed+= measuresacc[i%samplesize];
    	
    	
    }
    
    
    
    Serial.print(speed); DASH
    Serial.print(measures[(n%samplesize)-1] - avg);DASH 
    //rotation handling

    
    float gyro = h.gyro_z;
    if(gyro>0.63||gyro<-0.63){currRotation += gyro;}
    currRotation = (float)(((int)currRotation) % 360);
    if(currRotation < 0){
    	currRotation = 360 - currRotation;
    }
    currRotation = (float)((int)currRotation%360);
   
    
   
	Serial.print(gyro); BREAK
	delay(40);
    /* everytime.h example
    every(500){
        frm.print(F("!jgetEl('info2').innerHTML='"));
        frm.print(millis());
        frm.print("';");
        frm.end();
    }
    */
}

