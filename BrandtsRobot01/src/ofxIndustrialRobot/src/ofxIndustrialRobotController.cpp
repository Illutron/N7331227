
#include "ofxIndustrialRobotController.h"

ofxIndustrialRobotController::ofxIndustrialRobotController(ofxIndustrialRobotCoreData* _data, ofxIndustrialRobotArmHelper * _helper, ofxIndustrialRobotTimeline * _timeline){
	data = _data;
	helper = _helper;
	timeline = _timeline;
	
	targetPosition = ofxVec3f();
	lastTargetPosition = ofxVec3f();
}

void ofxIndustrialRobotController::step(int framerate){
	//	setArmToPosition(targetPosition);
	if(framerate > 0){
		timeline->step(vTarget, framerate);
		//setHandToPosition(timeline->spline.sampleAt(time/100.0), ofxVec3f(1.0,0.0,0.0));
		lastTargetPosition = targetPosition;
		targetPosition = timeline->getPosition();
		vTarget = targetPosition - lastTargetPosition;
	//	cout<<"v1: "<<vTarget.length()<<" "<<framerate<<endl;
		vTarget *= framerate;
		if(lastTargetPosition.length() == 0){
			vTarget = 0;
		}
		setHandToPosition(targetPosition, timeline->getHandPosition());
			data->armSpeedHistory.push_back(vTarget);
//		data->armSpeedHistory.push_back(ofxVec3f(timeline->lengthToNextCue,0,0));
		
	
		
	}
}

void ofxIndustrialRobotController::setHandToPosition(ofxVec3f toolEnd, ofxVec3f dir){	
	ofxVec3f toolStart = toolEnd - dir.normalized()*data->tool->l;
	ofxVec3f dist = toolStart - helper->getEndPosition(0);
	//	cout<<getMaxToolDistance()-dist.length()<<" "<<endl;
	double a = atan2(toolStart.x,toolStart.z)-HALF_PI;
	ofxVec3f toolStartRotated = toolStart.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	ofxVec3f dirRotated = dir.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	
	double a1 = -180*atan2(dirRotated.x, dirRotated.y)/PI+180;
	bool reverse = false;
	if(a1 < 0){
		reverse = true;
		a1 += 180;
	}
	
	ofxVec3f hand1 =  ofxVec3f(1.0,0.0,0.0)*(data->arms[3].length+data->arms[4].length);
	hand1.rotate(a1, ofxVec3f(0.0,0.0,1.0));
	hand1 = hand1.getRotatedRad(a, ofxVec3f(0.0,1.0,0.0));
	
	//	cout<<hand1.length()<<endl;
	
	ofxVec3f armPosition = toolStart - hand1;
	
	//The roll
	//	cout<<atan2(dirRotated.y,dirRotated.x)<<endl;
	ofxVec3f dirRotated2 = dirRotated.getRotatedRad(-atan2(dirRotated.y,dirRotated.x), ofxVec3f(0.0,0.0,1.0));
	double a2 = -180*atan2(dirRotated2.z, dirRotated2.x)/PI;
	if(reverse)
		a2 = -a2+180;
	//	cout<<atan2(dirRotated2.z, dirRotated2.x)<<endl;
	//	cout<<a2<<endl;
	//	cout<<a1<<endl;
	
	
	//targetPosition = toolEnd;
	setArmToPosition(armPosition, a1, a2);
	
	
	//We now have to find out how to ratate the hand wrist to get into the direction of the tool
	
}

void ofxIndustrialRobotController::setArmToPosition(ofxVec3f position, double rotation1, double rotation2){
	double a = atan2(position.x,position.z)-HALF_PI;	
	
	ofxVec3f rotatedTarget = position.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	ofxVec3f rotatedStart = helper->getEndPosition(0).getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	
	float b = sqrt((rotatedTarget.x-rotatedStart.x)*(rotatedTarget.x-rotatedStart.x) + (rotatedTarget.y-rotatedStart.y)*(rotatedTarget.y-rotatedStart.y));
	
	float base = (data->arms[1].length*data->arms[1].length  - data->arms[2].length*data->arms[2].length + b*b)/(2.0*b);
	float h = sqrt(data->arms[1].length*data->arms[1].length-base*base);
	float px=rotatedStart.x+base*(rotatedTarget.x-rotatedStart.x)/b;
	float py=rotatedStart.y+base*(rotatedTarget.y-rotatedStart.y)/b;
	
	float q1x=px+h*(rotatedTarget.y-rotatedStart.y)/b;
	float q1y=py-h*(rotatedTarget.x-rotatedStart.x)/b;
	float q2x=px-h*(rotatedTarget.y-rotatedStart.y)/b;
	float q2y=py+h*(rotatedTarget.x-rotatedStart.x)/b;
	
	
	float a1 = atan2(q2y-rotatedStart.y, q2x-rotatedStart.x);
	float a2 = atan2(rotatedTarget.y-q2y, rotatedTarget.x-q2x);
	
	data->arms[0].rotation = (180.0*a/PI);	
	data->arms[1].rotation = -(180.0*a1/PI-90);
	data->arms[2].rotation = -(180.0*a2/PI-90)+(180.0*a1/PI-90);
	data->arms[3].rotation = -(180.0*(-a2)/PI)-rotation1;
	data->arms[4].rotation = rotation2;
	
	//	data->arms[3].rotation = -90;
	
	//Base rotation
}

float ofxIndustrialRobotController::getMaxToolDistance(){
	return (data->arms[1].length + data->arms[2].length);
}