/*
 *  Robot.h
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "BasePlugin.h"
#include "Data.h"
#include "ofxIndustrialRobot.h"

class Robot : public BasePlugin {
public:
	virtual void init(myData * _data);
	virtual void update();
	virtual void draw();


	virtual void threadedFunction();
	void setPosition(int speed, ofxVec3f vecArm, ofxVec3f vecHand);
	bool tryLock();
	bool isLocked(pthread_mutex_t * _mutex);
	ofxIndustrialRobot * industrialRobot;
	
	myData * data;
	
	
	
	
};