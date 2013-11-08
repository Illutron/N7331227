/*
 *  Controller.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Controller.h"

Controller::Controller(myData * _data){
	data = _data;
	//InteractionDesign * tmpID = new InteractionDesign();
	 FaceTracker *  tmpFaceTracker = new FaceTracker();
	//Robot * tmpRobot = new Robot();
	
	//tmpID->robot = tmpRobot;
	//tmpID->faceTracker = tmpFaceTracker;
	
	addPlugin(tmpFaceTracker);
	//addPlugin(tmpRobot);
	//addPlugin(tmpID);
	
}

void Controller::update()
{	
	for(list<BasePlugin*>::iterator it=plugins.begin(); it!=plugins.end(); it++){
		(*it)->update();
	}	
}


void Controller::draw()
{	
	for(list<BasePlugin*>::iterator it=plugins.begin(); it!=plugins.end(); it++){
			(*it)->draw();
	}	
}

void Controller::addPlugin(BasePlugin* plugin){
	plugins.push_back(plugin);
	plugin->init(data);
}

