/*
 *  Robot.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Robot.h"

void Robot::init(myData * _data)
{

	
	data = _data;
	industrialRobot = new ofxIndustrialRobot(data->baseApp);
}

void Robot::update()
{

}

void Robot::setPosition(int speed, ofxVec3f vecArm, ofxVec3f vecHand)
{
	
	//:setPosition(int speed, ofxVec3f vecArm, ofxVec3f vecHand)
	//industrialRobot->thread.timeline->cues.push_back(new ofxIndustrialRobotPositionCue(100, ofxVec3f(000.0,100.0,-300.0), ofxVec3f(-1,-1,1)));
	
	industrialRobot->thread.timeline->cues.push_back(new ofxIndustrialRobotPositionCue(speed,vecArm,vecHand));
	
}

void Robot::threadedFunction(){

}


void Robot::draw()
{
	int x = 0, y=0, w=700, h=700;
//	industrialRobot->visualizer->draw3d(x, y, w/2.0-2, h/2.0-2);
//	industrialRobot->visualizer->drawtop(x+(w/2.0)+2, y, w/2.0-2, h/2.0-2);
//	industrialRobot->visualizer->drawside(x, y+(h/2.0)+2, w/2.0-2, h/2.0-2);
//	industrialRobot->visualizer->drawGraphs(x+(w/2.0)+2, y+(h/2.0)+2, w/2.0-2, h/2.0-2);
//	industrialRobot->drawVisualizer(0, 0, 700, 700);
	
}