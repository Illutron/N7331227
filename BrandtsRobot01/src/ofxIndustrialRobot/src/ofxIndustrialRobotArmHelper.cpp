#include "ofxIndustrialRobotArmHelper.h"

ofxIndustrialRobotArmHelper::ofxIndustrialRobotArmHelper(ofxIndustrialRobotCoreData * _data){
	data = _data;
}

float ofxIndustrialRobotArmHelper::getLength(int arm){
	return data->arms[arm].length;
}

float ofxIndustrialRobotArmHelper::getRotationAngle(int arm){
	return data->arms[arm].rotation;	
}

ofxVec3f ofxIndustrialRobotArmHelper::getAxis(int arm){
	return data->arms[arm].axis;
}

ofxVec3f ofxIndustrialRobotArmHelper::getOffset(int arm){
	return data->arms[arm].offset;
}


ofxVec3f ofxIndustrialRobotArmHelper::getEndPosition(int arm){
	ofxVec3f v;
	for(int i=0;i<=arm;i++){
		v += ofxVec3f(0.0,1.0,0.0).rotated(data->arms[arm].rotation, data->arms[arm].axis) * data->arms[arm].length - data->arms[arm].offset.rotated(90, ofxVec3f(0,0,1)).rotated(data->arms[arm].rotation, data->arms[arm].axis) ;
	}
	
	return v;
										
}
ofxVec3f ofxIndustrialRobotArmHelper::getStartPosition(int arm){
	ofxVec3f v;
	for(int i=0;i<arm;i++){
		v += ofxVec3f(1.0,0.0,0.0).rotate(data->arms[arm].rotation, data->arms[arm].axis).rotate(90, ofxVec3f(0,0,1)) * data->arms[arm].length  - data->arms[arm].offset.rotated(90, ofxVec3f(0,0,1));
	}
	return v;
	
}

ofxIndustrialRobotTool * ofxIndustrialRobotArmHelper::getTool(){
	return data->tool;
}

ofxIndustrialRobotCoreData* ofxIndustrialRobotArmHelper::getCoreData(){
	return data;

}