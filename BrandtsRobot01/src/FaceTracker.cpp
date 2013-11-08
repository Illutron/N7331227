/*
 *  FaceTracker.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "FaceTracker.h"


 void FaceTracker::init(myData * _data)
{
	
	data = _data;
	haarFinder.setup("haarXML/haarcascade_frontalface_alt.xml");
	cout<< "init mutex:" <<  pthread_mutex_init(&newdataMutex, NULL) << "\n"; 
	srcImgGrayFacial.allocate(CAPWIDTH,CAPHEIGHT);
	srcImgGrayFacial.set(0);
	haarFinder.findHaarObjects(srcImgGrayFacial, 10, 10, 1); // HACK ELSE IT CRASHES WEIRD??
	pthread_mutex_lock(&newdataMutex);
	newData = false;
	startThread(true, false);   // blocking, verbose

}

void FaceTracker::update()
{

	//if (!newData)
	{
		cout << "hep2";
		srcImgGrayFacial = data->srcImgGray;
		pthread_mutex_unlock(&newdataMutex);
		newData = true;
	}
	
}

void FaceTracker::threadedFunction(){
	while( isThreadRunning() != 0 ){
		if (pthread_mutex_lock(&newdataMutex))
		{
			cout << "hep";
		/*	haarFinder.findHaarObjects(srcImgGrayFacial, 10, 10, 1);
			newData = false;
			if( lock())
			{
				blobs =  haarFinder.blobs;
				
				unlock();
			}*/
		}
	}
}


ofxVec3f *  FaceTracker::getBiggestFace()
{
	if( lock() )
	{
	float size = 0;
	float x = 0;
	float y = 0;
	int numFace = blobs.size();
	for(int i = 0; i < numFace; i++){
		float tmpSize = blobs[i].boundingRect.width * blobs[i].boundingRect.height;
		if (size < tmpSize)
		{
			float x = blobs[i].boundingRect.x;
			float y = blobs[i].boundingRect.y;
			size = tmpSize;
		}
		
	}
		unlock();
		return new ofxVec3f(x,y,0);
		
	}
	
}

void FaceTracker::draw()
{
	srcImgGrayFacial.draw(0,0);
	if( lock() )
	{
		
		int numFace = blobs.size();
		
		
		glPushMatrix();
		
		glTranslatef(350, 350, 0);
		
		for(int i = 0; i < numFace; i++){
			
			float x = blobs[i].boundingRect.x;
			float y = blobs[i].boundingRect.y;
			float w = blobs[i].boundingRect.width;
			float h = blobs[i].boundingRect.height;
			
			float cx = blobs[i].centroid.x;
			float cy = blobs[i].centroid.y;
			
			ofSetColor(0xFF0000);
			ofRect(x, y, w, h);
			
			ofSetColor(0xFFFFFF);
			ofDrawBitmapString("face "+ofToString(i), cx, cy);
			
		}
		
		glPopMatrix();
		unlock();
	}
}