
#include "ofxIndustrialRobotThread.h"


ofxIndustrialRobotThread::ofxIndustrialRobotThread(){
	lastTimeMillist = 0;
	framerate = 0;
	
}


void ofxIndustrialRobotThread::step(){
	//timeline->generateSpline();
	controller->step(framerate);
//	serial->step();

	if(serial->lock()){
		serial->sendMsg(coreData.arms[0].rotation);
		serial->unlock();
	}
	
	/*	if(app->mouseDown){
	 timeline->cues.push_back(new ofxIndustrialRobotPositionCue(1000, ofxVec3f(app->mouseX,app->mouseY,-200.0)));
	 }*/
}