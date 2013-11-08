#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel( OF_LOG_VERBOSE );
    
    ofSetVerticalSync(true);
	
    vidGrabber.setVerbose(true);
    vidGrabber.setUseTexture(true);
    vidGrabber.initGrabber(640,480);
    //vidGrabber.listDevices();
	//vidGrabber.setDeviceID(0);
	
	grayImage.allocate(640,480);
    
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);
    
	
    vidGrabber.update();
	
	if( vidGrabber.isFrameNew() ){
        grayImage.setFromPixels(vidGrabber.getPixels(), 640,480);        
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(0xffffff);
    //vidGrabber.draw( 100,100 );
	grayImage.draw(200,100);
	
	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "fps: %f", ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	switch (key){
		case ' ':
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}	

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
}

