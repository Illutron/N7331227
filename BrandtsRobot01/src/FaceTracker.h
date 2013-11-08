/*
 *  FaceTracker.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "ofxCvHaarFinder.h"
#include "ofxVectorMath.h"


class FaceTracker : public BasePlugin {
	public:
	virtual void init(myData * _data);
	virtual void update();
	virtual void draw();
	virtual void threadedFunction();
	bool tryLock();
	
	bool isLocked(pthread_mutex_t * _mutex);
	ofxVec3f *  getBiggestFace();
	
	
	myData * data;
	pthread_mutex_t  newdataMutex;
	ofxCvGrayscaleImage srcImgGrayFacial;
	ofxCvHaarFinder	haarFinder;;
	bool newData;
	vector<ofxCvBlob> blobs;
	
	
	
	};
