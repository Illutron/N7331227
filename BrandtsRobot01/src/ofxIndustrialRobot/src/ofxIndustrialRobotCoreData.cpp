#include "ofxIndustrialRobotCoreData.h"

ofxIndustrialRobotCoreData::ofxIndustrialRobotCoreData(){
	arms[0].rotation = 0;
	arms[0].length = 200;
	arms[0].offset = ofxVec3f(00,10,0);
	arms[0].axis = ofxVec3f(0.0,1.0,0.0);
	
	arms[1].rotation = 70;
	arms[1].length = 200;
	arms[1].axis = ofxVec3f(0.0,0.0,1.0);
	
	arms[2].rotation = 20;
	arms[2].length = 200;
	arms[2].axis = ofxVec3f(0.0,0.0,1.0);

	arms[3].rotation = 20;
	arms[3].length = 100;
	arms[3].axis = ofxVec3f(0.0,0.0,1.0);
	
	arms[4].rotation = 20;
	arms[4].length = 50;
	arms[4].axis = ofxVec3f(0.0,1.0,0.0);
	
	tool = new ofxIndustrialRobotTool(ofxVec3f(1.0,0.0,0.0), 100, 10, 10);
}