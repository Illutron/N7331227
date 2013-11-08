/*
 *  BasePlugin.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "Data.h"
#include "ofxThread.h"

class BasePlugin : public ofxThread {
public:
	virtual void init(myData * _data);
	virtual void update();
	virtual void draw();
	bool tryLock();
	bool isLocked(pthread_mutex_t * _mutex);
	bool isLocked();
	BasePlugin* BasePlugin::getFilter(string _name);
	myData * data;
	

};