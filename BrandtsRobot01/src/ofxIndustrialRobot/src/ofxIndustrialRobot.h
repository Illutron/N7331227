#ifndef _INDUSTRIAL_ROBOT
#define _INDUSTRIAL_ROBOT

#include "ofConstants.h"

#include "ofxVectorMath.h"
#include "ofxThread.h"

#include "ofxIndustrialRobotThread.h"

#include "ofxIndustrialRobotVisualizer.h"


class ofxIndustrialRobot {
public:
	ofxIndustrialRobot(ofBaseApp * app);
	void drawVisualizer(int x, int y, int w, int h);
	
	ofxIndustrialRobotThread thread;
	ofxIndustrialRobotVisualizer * visualizer;
	ofBaseApp * app;
};


#endif
