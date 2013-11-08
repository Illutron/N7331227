
#include "ofxIndustrialRobotVisualizer.h"

ofxIndustrialRobotVisualizer::ofxIndustrialRobotVisualizer(ofxIndustrialRobotArmHelper * _helper,  ofxIndustrialRobotController * _controller, ofBaseApp * _app, ofxIndustrialRobotTimeline * _timeline){
	helper = _helper;
	controller = _controller;
	app = _app;
	timeline = _timeline;

	rotationSpeed = 0.3;	
	camera.position(ofGetWidth()/2.0, ofGetHeight()/2.00+100, 800);
	light1.specular(255, 255, 255);
}

//---

void ofxIndustrialRobotVisualizer::drawGeneric(){
	//Base
	ofSetColor(0, 255, 255);	
	glTranslated(0, -25, 0);
	//ofxBox(ofxVec3f(0,25,0), ofxVec3f(200, 50, 200));	
	
	glRotated(180, 0.0, 0.0, -1.0); 
	glPushMatrix();
	drawArm0();
	drawArm1();
	drawArm2();
	drawArm3();
	drawArm4();
	//Draw tool
	ofSetColor(255, 0, 0);
	float l = helper->getTool()->l; //The length of the tool
	ofxVec3f rotation = helper->getTool()->getRotation();
	glRotated(rotation.z, 0.0, 0.0, 1.0);
	ofxBox(ofxVec3f(helper->getTool()->h/2.0,l/2,0), ofxVec3f(helper->getTool()->h, l, helper->getTool()->w), ofxVec3f(0.0, 0.0, 0.0));
//	ofxVec3f axis = helper->getAxis(i); //The axis the arm rotates around
//	ofxVec3f offset = helper->getOffset(i);
//	glRotated(helper->getRotationAngle(i), axis.x, axis.y, axis.z); // Rotate it around the axis
//	glTranslated(0, l, 0);
//	ofxBox(ofxVec3f(0,-l/2.0,0), ofxVec3f(60, l, 60), ofxVec3f(0.0, 0.0, 0.0));
//	glTranslated(-offset.y, offset.x, offset.z);	
	
	glPopMatrix();
	
	//Target
	glRotated(180, 0.0, 1.0, 0.0); 
	ofSetColor(255, 0, 0);
	ofxSphere(controller->targetPosition, ofxVec3f(20, 20, 20));
	
	
	glPushMatrix();
	ofSetColor(0, 255, 0);
	for(int i=0;i<timeline->numberPositionCues(); i++){
		ofxVec3f vec = timeline->getPositionCue(i)->position;
		ofxSphere(vec, ofxVec3f(10, 10, 10));	
	}
	timeline->spline.drawRaw(10, 5);
	ofSetColor(255, 255, 0, 100);
	timeline->spline.drawSmooth(50, 0, 10);
	glLineWidth(1);
	glPopMatrix();
}

void ofxIndustrialRobotVisualizer::draw3d(int x, int y, int w, int h){	
//	camera.orbitAround(ofxVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0), ofxVec3f(0.0,1.0,0.0), rotationSpeed);
	light1.directionalLight(255, 255, 255, 0.0, 0.0, 1.0);

	glPushMatrix();
	glTranslated(x, y, 0);
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 20);
	ofRect(0, 0, w, h);

	
	glPushMatrix();
	glViewport(x, ofGetHeight()-h-y, w, h);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	camera.place();
	glTranslated(0, 200, 0);
	ofxLightsOn();
	ofFill();
	ofSetColor(0, 255, 0);	
	ofxSphere(ofGetWidth()/2.0, ofGetHeight()/2.0, 0, 500, 10, 500);
	ofSetColor(0, 255, 255);	
	//ofxSphere(ofGetWidth()/2.0, ofGetHeight()/2.0, 0, 20, 10, 20);

	glTranslated(ofGetWidth()/2.0, ofGetHeight()/2.0, 0);

	drawGeneric();
	
	glColor3f(255, 255, 255);	
	ofxLightsOff();
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();
	
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, w, h);
	ofDrawBitmapString("Robot Visualizer", x+10, y+15);
	
	glPopMatrix();	
}


void ofxIndustrialRobotVisualizer::drawtop(int x, int y, int w, int h){	
	topView.setPosAndSize(x,y,w, h);
	float aspect = (float)ofGetWidth()/ofGetHeight(); //1.333...
	float aspect2 = (float)w/h; //1.0000
	float mult = (float)aspect/aspect2; //Factor to multiply x coordinates with
	float scale = 0.5; // Same as to zoom
	ofxVec3f translate = ofxVec3f((ofGetWidth()/2.0)/mult, ofGetHeight()/2.0,0);
	
	if(topView.click){
		float cx = -(1.0- 2*((float)app->mouseX - x)/w);
		float cy = 1.0- 2*((float)app->mouseY - y)/h;
		float a1 = -atan2(cx,cy);
		float a2 = atan2(controller->targetPosition.x,controller->targetPosition.z)-HALF_PI;
//		controller->targetPosition = ofxVec3f(((cx*ofGetWidth()/mult)/scale)/2.0, controller->targetPosition.y, (cy*ofGetHeight()/scale)/2.0);
		ofxVec3f target = ofxVec3f(((cx*ofGetWidth()/mult)/scale)/2.0, controller->targetPosition.y, (cy*ofGetHeight()/scale)/2.0);
		timeline->cues.push_back(new ofxIndustrialRobotPositionCue(100, target, (target-ofxVec3f(0,target.y,0)).normalized()));
		topView.click = false;
	}
	glPushMatrix();
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(0.0, (float)ofGetWidth(), 0.0, (float)ofGetHeight(), 10.0, 2500);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1000,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);

	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
	
	glViewport(x, ofGetHeight()- y-h, w, h);
	ofFill();
	ofSetColor(255, 255, 0);
	
	glScaled(1.0*mult, 1.0, 1.0);	
	glTranslated(0, -ofGetHeight(), 0);

	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 50);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofSetRectMode(1);
	glTranslated(translate.x,translate.y, 0);
	glScaled(scale, scale, scale);
	
	glRotated(-90, 1.0, 0, 0.0);

	drawGeneric();
	
	ofSetRectMode(0);
	
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Top view", x+10, y+15);

	glPopMatrix();	
}

void ofxIndustrialRobotVisualizer::drawside(int x, int y, int w, int h){	
	sideView.setPosAndSize(x,y,w, h);

	float aspect = (float)ofGetWidth()/ofGetHeight(); //1.333...
	float aspect2 = (float)w/h; //1.0000
	float mult = (float)aspect/aspect2; //Factor to multiply x coordinates with
	float scale = 0.9; // Same as to zoom
	ofxVec3f translate = ofxVec3f((ofGetWidth()/2.0)/mult-200, ofGetHeight()/2.0+300,0);
	
	if(sideView.click){
		float cx = ((float)app->mouseX - x)/w;
		float cy = ((float)app->mouseY - y)/h;
		float a = atan2(controller->targetPosition.x,controller->targetPosition.z)-HALF_PI;
		ofxVec3f target = ofxVec3f(((cx*ofGetWidth()/(float)mult)-translate.x)/scale, -((cy*ofGetHeight())-translate.y)/scale, 0);
		target.rotateRad(a, ofxVec3f(0.0,1.0,0.0));

		timeline->cues.push_back(new ofxIndustrialRobotPositionCue(100, target, (target-ofxVec3f(0,target.y,0)).normalized()));

		sideView.click = false;
	}
	
	glPushMatrix();
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(0.0, (float)ofGetWidth(), 0.0, (float)ofGetHeight(), 10.0, 2500);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1000,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
	
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.

	glViewport(x, ofGetHeight()- y-h, w, h);

	ofFill();
	ofSetColor(255, 255, 0);
	

	glScaled(1.0*mult, 1.0, 1.0);	
	glTranslated(0, -ofGetHeight(), 0);
	
	glPushMatrix();
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 50);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofSetRectMode(1);
	
	glTranslated(translate.x, translate.y, 0);
	glScaled(scale, scale, scale);
	glRotated(helper->getRotationAngle(0), 0.0,1.0,0.0); // Rotate it around the axis
	
	drawGeneric();

	ofFill();
	glRotated(helper->getRotationAngle(0), 0.0,1.0,0.0); // Rotate it around the axis
	ofSetColor(255, 255, 255);
	ofLine(-1000, 0, 1000, 0);
	ofLine(0,-1000, 0, 1000);
	ofSetRectMode(0);	

	glPopMatrix();	
	
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Side view", x+10, y+15);
	
	glPopMatrix();	

}


void ofxIndustrialRobotVisualizer::drawGraphs(int x, int y, int w, int h){
	float aspect = (float)ofGetWidth()/ofGetHeight(); //1.333...
	float aspect2 = (float)w/h; //1.0000
	float mult = (float)aspect/aspect2; //Factor to multiply x coordinates with
	
	glPushMatrix();
	glTranslated(x, y, 0);

	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 50);
	ofRect(0, 0, w, h);
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, w, h);
	
	
	glBegin(GL_LINE_STRIP);
	int start = helper->getCoreData()->armSpeedHistory.size() - w;
	if(start < 0)
		start = 0;
	for(int i=start;i<helper->getCoreData()->armSpeedHistory.size();i++){
		glVertex3d(i-start, h-helper->getCoreData()->armSpeedHistory[i].length(),0);
	}
	
	glEnd();
	
	ofDrawBitmapString(ofToString(helper->getCoreData()->armSpeedHistory.size(), 0), x+10, y+30);
	
	glPopMatrix();

	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, w, h);
	ofDrawBitmapString("Robot Graphs", x+10, y+15);
	
}


void ofxIndustrialRobotVisualizer::drawArm(int i, int w, int h){
	float l = helper->getLength(i); //The length of the arm
	ofxVec3f axis = helper->getAxis(i); //The axis the arm rotates around
	ofxVec3f offset = helper->getOffset(i);
	glRotated(helper->getRotationAngle(i), axis.x, axis.y, axis.z); // Rotate it around the axis
	glTranslated(0, l, 0);
	ofxBox(ofxVec3f(0,-l/2.0,0), ofxVec3f(60, l, 60), ofxVec3f(0.0, 0.0, 0.0));
	glTranslated(-offset.y, offset.x, offset.z);	
}

void ofxIndustrialRobotVisualizer::drawArm0(){
	ofSetColor(255, 0, 0);
	drawArm(0, 80, 80);
}
void ofxIndustrialRobotVisualizer::drawArm1(){
	ofSetColor(255, 255, 0);
	drawArm(1, 60, 60);
}

void ofxIndustrialRobotVisualizer::drawArm2(){
	ofSetColor(0, 255, 0);
	drawArm(2, 60, 60);
}

void ofxIndustrialRobotVisualizer::drawArm3(){
	ofSetColor(0, 255, 255);
	drawArm(3, 60, 60);
}

void ofxIndustrialRobotVisualizer::drawArm4(){
	ofSetColor(0, 0, 255);
	drawArm(4, 60, 60);
}