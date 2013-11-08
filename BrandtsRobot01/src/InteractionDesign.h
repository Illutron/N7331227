/*
 *  InteractionDesign.h
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "Robot.h"
#include "FaceTracker.h"
#include "ofxIndustrialRobot.h"

class InteractionDesign : public BasePlugin {
public:
	virtual void init(myData * _data);
	virtual void update();
	virtual void draw();
	

	ofxIndustrialRobot * industrialRobot;
	myData * data;
	Robot * robot;
	FaceTracker * faceTracker;
	
	
	
};