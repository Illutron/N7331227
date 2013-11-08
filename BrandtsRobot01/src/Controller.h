/*
 *  Controller.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "BasePlugin.h"
#include "FaceTracker.h"
#include "Robot.h"
#include "InteractionDesign.h"

#include "Data.h"

class Controller {
public:
	Controller(myData * _data);
	myData * data;
	
	void update();
	void draw();
	void addPlugin(BasePlugin * plugin);
	list<BasePlugin*> plugins;	
};