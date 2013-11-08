#ifndef _INDUSTRIAL_ROBOT_DATA
#define _INDUSTRIAL_ROBOT_DATA

#include "ofxVectorMath.h"
#include "ofxIndustrialRobotCoreData.h"
#include "ofxIndustrialRobotArmHelper.h"
#include "ofxIndustrialRobotTimeline.h"
#include "ofMain.h"

class ofxIndustrialRobotController {
public:

	ofxIndustrialRobotController(ofxIndustrialRobotCoreData * _data, ofxIndustrialRobotArmHelper * _helper, ofxIndustrialRobotTimeline * _timeline);
	
	void step(int framerate);
	
	float time;
	
	ofxVec3f targetPosition, lastTargetPosition;
	ofxVec3f targetDir;
	
	ofxVec3f vTarget;
	float getMaxToolDistance();
	
private:
	ofxIndustrialRobotCoreData * data;
	ofxIndustrialRobotArmHelper * helper;
	ofxIndustrialRobotTimeline * timeline;

	void setArmToPosition(ofxVec3f position, double rotation1, double rotation2);
	void setHandToPosition(ofxVec3f position, ofxVec3f dir);
};

#endif
