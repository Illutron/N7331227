#pragma once

#include "ofxVectorMath.h"

class ofxIndustrialRobotArmData {
public:
	float rotation;
	float length;
	ofxVec3f axis;  //rotation axis
	ofxVec3f offset; //The offset added to the end of the arm. Has a length
};
