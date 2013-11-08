#include "ofxIndustrialRobotTimeline.h"
#include "ofxVectorMath.h"

ofxIndustrialRobotTimeline::ofxIndustrialRobotTimeline(){
//	cues.push_back(new ofxIndustrialRobotPositionCue(10, ofxVec3f(300.0,400.0,0.0)));
//	cues.push_back(new ofxIndustrialRobotPositionCue(10, ofxVec3f(200.0,20.0,0.0)));
//	cues.push_back(new ofxIndustrialRobotPositionCue(10, ofxVec3f(200.0,0.0,200.0)));
//	cues.push_back(new ofxIndustrialRobotPositionCue(10, ofxVec3f(200.0,100.0,0.0)));
//	cues.push_back(new ofxIndustrialRobotPositionCue(10, ofxVec3f(0.0,300.0,-200.0)));
//	cues.push_back(new ofxIndustrialRobotPositionCue(10, ofxVec3f(-100.0,300.0,-200.0)));
	cues.push_back(new ofxIndustrialRobotPositionCue(200, ofxVec3f(-200.0,100.0,-200.0), ofxVec3f(-1,1,1)));
	cues.push_back(new ofxIndustrialRobotPositionCue(100, ofxVec3f(000.0,100.0,-300.0), ofxVec3f(-1,-1,1)));
	cues.push_back(new ofxIndustrialRobotPositionCue(50, ofxVec3f(100.0,300.0,-300.0), ofxVec3f(-1,-1,-1)));
	cues.push_back(new ofxIndustrialRobotPositionCue(100, ofxVec3f(300.0,100.0,-300.0), ofxVec3f(1,-1,-1)));
	generateSpline(0,0);
	currentCue = 0;
	currentCueMaxSpeed = 0;
	splineTime = 0;
};

float fade(float percent, float start, float stop){
	float a = (stop-start);
//	cout<<percent<<endl;
	float percent2 = (float)(50.0*(sin((PI*percent)-HALF_PI)+1.0))/100.0;
	//cout<<a*percent + start<<endl;
	return a*percent2 + start;
}

float fadeDistance(float start, float stop, float time){
	float t = (time/1000.0);
	float a = (stop-start)/t;
	return 0.5*a*t*t+start*t;
}

float calculateDistanceInSpline(ofxMSASpline3D spline, float start, float end, int resolution){
	float ret;
	ret = 0;
	float pDist = (float)end-start;
	if(start < 0 || start > 1.0)
		return NULL;
	ofxVec3f p1 = spline.sampleAt(start);				
	for(int i=1 ; i<=resolution ; i++){
		float s = start+(float)pDist*i/resolution;
		if(s < 0 || s > 1.0)
			return NULL;
		ofxVec3f p2 = spline.sampleAt(s);
		ret += (p2-p1).length();
		p1 = p2;
	}	
	return ret;
}


void ofxIndustrialRobotTimeline::step(ofxVec3f v, int framerate){
	if(framerate > 0){ //we have to check so we don't get divide by zero
	//	cout<<"fps"<<framerate<<endl;
		float dt = 1.0 / framerate;
		if(cues.size() > 3){ //the spline can only be generated if there are at least 4 cues
			//Check if we are at a cue point
			if(splineTime * (nCuesInSpline-1) >= currentCue-splineStart && currentCue < getLastPositionCue() ){
				splineStart = (currentCue > 1)? currentCue - 1 : 0;
				if(numberPositionCues()-splineStart < 4){
					splineStart = numberPositionCues() - 4;
				}
				if(splineStart < 0){
					splineStart = 0;	
				}
 				generateSpline(splineStart,0); //every time we are in a cue point we can generate a new spline (with the new points eventually added)
				cout<<"cue ++"<<endl;  
				
				currentCue ++;
				cueStartTime = ofGetElapsedTimeMillis();
				nCuesInSpline = numberPositionCues()-splineStart;
				splineTime = (float)(currentCue-1-splineStart) / (nCuesInSpline-1); //Calculate where in the spline we are (0.0-1.0)
				currentCueMaxSpeed = v.length(); timeStartFadeDown = 0;
				avgSpeedReq = getPositionCue(currentCue)->speed;
//				avgSpeedReq = v.length() + 10;;
				
				//float v1 = v.length();
				//float v2 = avgSpeedReq - (v1 - avgSpeedReq);
				//a = (v2-v1)/(getPositionCue(currentCue)->time);
			}
			
			//Check if we should step forward in spline
			
			
			if(splineTime * (nCuesInSpline-1) < currentCue-splineStart){
				int inTime = 1000;
				int outTime = 1000;
				int curTime = ofGetElapsedTimeMillis()-cueStartTime;
			
				float reql = getPositionCue(currentCue)->speed*dt;

//				cout<<splineTime<<"   "<<(float)(currentCue-splineStart) / (nCuesInSpline-1)<<endl;
				lengthToNextCue = calculateDistanceInSpline(spline, splineTime, (float)(currentCue-splineStart) / (nCuesInSpline-1), 2000);
				float reqFadeDistance = fadeDistance(v.length(), 0, outTime) ;
				float dstSpeed; 
				if(currentCue < getLastPositionCue()){
					dstSpeed = getPositionCue(currentCue+1)->speed;
				} else {
					dstSpeed = 0.0;
				}

				if(timeStartFadeDown > 0){					
					reqFadeDistance = fadeDistance(currentCueMaxSpeed, dstSpeed, outTime) ;
				} else {
					reqFadeDistance = fadeDistance(v.length(), dstSpeed, outTime) ;

				}
				if(reqFadeDistance > lengthToNextCue && v.length() > 0){
					if(timeStartFadeDown == 0){
						timeStartFadeDown = ofGetElapsedTimeMillis();
						currentCueMaxSpeed = v.length();
						tmp = 0;

					}
				//	cout<<"fade down  "<<(float)lengthToNextCue/reqFadeDistance<<endl;
				//	cout<<endl<<endl;
					reql = fade(((float)(ofGetElapsedTimeMillis()-timeStartFadeDown)/outTime), currentCueMaxSpeed,dstSpeed)*dt;
					//cout<<fade(((float)(ofGetElapsedTimeMillis()-timeStartFadeDown)/outTime), currentCueMaxSpeed,dstSpeed)<<endl;
					tmp += reql;
					//		cout<<reql<<endl;
				} else if(curTime <= inTime){
					//	cout<<"fade up      "<<(float)curTime/inTime<<"   =   "<<fade((float)curTime/inTime, 0.0, 1.0)<<endl;
					reql = fade((float)curTime/inTime, currentCueMaxSpeed, getPositionCue(currentCue)->speed)*dt;
				}	
				
			//	cout<<ofGetElapsedTimeMillis()-cueStartTime<<endl;
			//	cout<<"a: "<<lengthToNextCue<<endl;
			//	cout<<"b: "<<calculateDistanceInSpline(spline, splineTime, (float)(currentCue-splineStart) / (nCuesInSpline-1), 2000)<<endl;
				float l = 0;

		//		cout<<"v="<<v.length()<<" "<<getPositionCue(currentCue)->speed <<" "<<dt<<" reql="<<reql<<" "<<((getPositionCue(currentCue)->speed - v.length())*0.1)<<endl;
//				cout<<a<<" "<<v.length()<<" "<<reql<<endl;
				//float reql = (float)(v.length()+1000*a) * dt;

				float dSt = 0;
				bool end = false;
				ofxVec3f p1 = spline.sampleAt(splineTime);
				while(l < reql && end == false){
					dSt += 0.000001;
					if(dSt + splineTime <= 1.0 ){
						ofxVec3f p2 = spline.sampleAt(splineTime+dSt);
						l += (p2-p1).length();
						p1 = p2;
					} else {
						end = true;
					}
				//	if(dSt > 0.5)
				//		break;
				}
				splineTime += dSt; //Step forward in the splines time
				if(splineTime > 1.0){					
					splineTime = 1.0;
				}
			}			
		}
	}
}

ofxVec3f ofxIndustrialRobotTimeline::getPosition(){
	return 	spline.sampleAt(splineTime);
}

ofxVec3f ofxIndustrialRobotTimeline::getHandPosition(){
	return 	handSpline.sampleAt(splineTime);
}

int ofxIndustrialRobotTimeline::numberCues(){
	return cues.size();
}

int ofxIndustrialRobotTimeline::numberPositionCues(){
	int n = 0;
	ofxIndustrialRobotPositionCue * c;
	for(int i=0;i<cues.size();i++){
		if(cues[i]->isPositionCue)
			n++;
	}
	return n;
}

int ofxIndustrialRobotTimeline::getLastPositionCue(){
	for(int i=cues.size()-1;i>=0;i--){
		if(cues[i]->isPositionCue)
			return i;			
	}

}

ofxIndustrialRobotPositionCue* ofxIndustrialRobotTimeline::getPositionCue(int n){
	int c = 0;
	for(int i=0;i<numberCues();i++){
		if(cues[i]->isPositionCue){
			if(c == n){
				return (ofxIndustrialRobotPositionCue*)cues[i];
			}
			c++;
		}
	}
	return NULL;
}

void ofxIndustrialRobotTimeline::generateSpline(int first, int last){
	spline.clear();
	handSpline.clear();
	spline.setInterpolation(OFX_MSA_SPLINE_CUBIC);
	handSpline.setInterpolation(OFX_MSA_SPLINE_CUBIC);
	//spline.setInterpolation(OFX_MSA_SPLINE_LINEAR);
	//spline.setUseDistance(true);
	int n= numberPositionCues()-last;
	for(int i=first;i<n;i++){
		spline.push_back(getPositionCue(i)->position);
		handSpline.push_back(getPositionCue(i)->hand);
	}
}
