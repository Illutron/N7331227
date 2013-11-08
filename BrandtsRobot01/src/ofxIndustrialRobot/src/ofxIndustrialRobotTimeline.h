#ifndef _INDUSTRIAL_ROBOT_TIMELINE
#define _INDUSTRIAL_ROBOT_TIMELINE

#include "ofxIndustrialRobotCue.h"
#include "ofxIndustrialRobotPositionCue.h"
#include "ofxMSASpline.h"

class ofxIndustrialRobotTimeline{
public:
	
	ofxMSASpline3D spline;
	ofxMSASpline3D handSpline;
	
	float splineTime;
	int currentCue;
	
	float lengthToNextCue;
	float avgSpeedReq;
	float a ;
	int nCuesInSpline, splineStart;

	
	unsigned long cueStartTime;
	
	void step(ofxVec3f v, int framerate);
	ofxVec3f getPosition();
	ofxVec3f getHandPosition();
	
	
	ofxIndustrialRobotTimeline();
	
	int numberCues();
	int numberPositionCues();
	int getLastPositionCue();
	
	void generateSpline(int first, int last);
	
	ofxIndustrialRobotPositionCue* getPositionCue(int n);
	vector<ofxIndustrialRobotCue*> cues;

private:
	float currentCueMaxSpeed; //
	unsigned long timeStartFadeDown;
	float tmp;
};


#endif