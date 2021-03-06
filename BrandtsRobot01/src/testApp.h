#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "ofxEasyGui.h"

#include "Data.h"
#include "Defines.h"
#include "Controller.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		

		
		ofVideoGrabber 		vidGrabber;
	Controller * controller;
		myData * data;
	
	
	void testFunction(int setting);
	
	
	//************************
	// TEST GUI
	//************************
	
/*	bool saveXml;
	bool loadXml;
	
	GuiHandler*         gui;
	
	bool    myValue1;
	bool    myValue2;
	float   myValue3;
	string  myString1;
	string  myString2;
	string  myString3;
	int     myValue4;
	bool    myValue5;
	int     myValue6;
	
	int testValue1;
	
	bool toggle1a;
	bool toggle1b;*/
	

};

#endif
