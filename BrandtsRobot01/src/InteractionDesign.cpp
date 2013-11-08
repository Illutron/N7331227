/*
 *  InteractionDesign.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "InteractionDesign.h"


void InteractionDesign::init(myData * _data)
{
	
	
	data = _data;
	industrialRobot = new ofxIndustrialRobot(data->baseApp);
}

void InteractionDesign::update()
{
	
	
	if(ofRandom(1,100) < 2)
	{
		ofxVec3f * vec =  faceTracker->getBiggestFace();
		cout << "x: " << vec->x;
		if (vec->x > 0 && vec->y > 0)
		{
			
			//robot->setPosition(100, *vec,ofxVec3f(-1,-1,1));
		}
	}
	
	
}



void InteractionDesign::draw()
{

	
}