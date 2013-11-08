#pragma once

#include "ofMain.h"
#include "ofxThread.h"
#include "ofxVectorMath.h"

#include "ofxIndustrialRobotController.h"
#include "ofxIndustrialRobotTimeline.h"
#include "ofxIndustrialRobotSerial.h"

class ofxIndustrialRobotThread : public ofxThread {
public:
	int framerate;
	int lastTimeMillist;
	//--------------------------
	ofxIndustrialRobotThread();
	void start(){
		startThread(true, false);   // blocking, verbose
		serial->start();
	}
	
	void stop(){
		stopThread();
	}
	
	//--------------------------
	void threadedFunction(){
		
		while( isThreadRunning() != 0 ){
		//	if((ofGetElapsedTimeMillis()-lastTimeMillist) > 1000.0/40.0){
				if( lock() ){
					framerate =(int) 1000/(ofGetElapsedTimeMillis() - lastTimeMillist);
					if(framerate == 1000){
						framerate = 0;
					}
					lastTimeMillist = ofGetElapsedTimeMillis();
					if(framerate > 0)
						step();
					unlock();
					int wait = 1000/40-(ofGetElapsedTimeMillis()-lastTimeMillist);
					if(wait < 0){
						wait = 0;
						cout<<" HAHA"<<endl;
					}
					
					
					ofSleepMillis(wait);
				}
			}
		//}
	}
	
	void step();
	
	ofxIndustrialRobotArmHelper * helper;
	ofxIndustrialRobotController * controller;
	ofxIndustrialRobotTimeline * timeline;
	ofxIndustrialRobotSerial * serial;
	ofBaseApp * app;
	ofxIndustrialRobotCoreData coreData;
private:
};