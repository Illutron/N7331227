
#include "ofxIndustrialRobotPositionCue.h"


ofxIndustrialRobotPositionCue::ofxIndustrialRobotPositionCue(int _speed, ofxVec3f _position, ofxVec3f _hand){
	position = _position;
	hand = _hand;
	speed = _speed;
	isPositionCue = true;
}