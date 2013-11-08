

#include "ofxIndustrialRobot.h"

ofxIndustrialRobot::ofxIndustrialRobot(ofBaseApp * _app){
	app = _app;
	thread.timeline = new ofxIndustrialRobotTimeline();
	thread.helper = new ofxIndustrialRobotArmHelper(&thread.coreData);
	thread.controller = new ofxIndustrialRobotController(&thread.coreData, thread.helper, thread.timeline);
	visualizer = new ofxIndustrialRobotVisualizer(thread.helper, thread.controller, _app, thread.timeline);
	thread.serial = new ofxIndustrialRobotSerial();

	thread.start();
	

}

//---

void ofxIndustrialRobot::drawVisualizer(int x, int y, int w, int h){
	if(thread.lock()){
		ofDrawBitmapString("robot thread fps: "+ofToString(thread.framerate, 0), 15, 45);


		thread.unlock();
	}
}

//---



//---
