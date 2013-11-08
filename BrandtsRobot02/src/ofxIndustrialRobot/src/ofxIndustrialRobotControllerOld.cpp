
#include "ofxIndustrialRobotController.h"

ofxIndustrialRobotController::ofxIndustrialRobotController(ofxIndustrialRobotCoreData* _data, ofxIndustrialRobotArmHelper * _helper, ofxIndustrialRobotTimeline * _timeline, ofxIndustrialRobotPlotter * _plotter){
	data = _data;
	helper = _helper;
	timeline = _timeline;
	plotter = _plotter;
	
	input = Slider;
	targetPosition = ofxVec3f();
	targetDir = ofxVec3f(0.0, -1.0, 0.0);
	lastTargetPosition = ofxVec3f();
	helper->data->reverseHeadPercentGoal = 1.0;
	helper->data->reverseHeadPercentTmp = 1.0;
	gravityTarget = ofxVec3f();
	gravityV = ofxVec3f();
	gravityVDir = ofxVec3f();
	gravityTargetDir = ofxVec3f();
	gravityMaxSpeed = 0.0;
	gravityForce = 1.0;
	mode = 0;
}

void ofxIndustrialRobotController::setInput(int _input){
	input = _input;
	if(input == Gravity){
		gravityV = ofxVec3f(0.0,0.0,0.0);
		gravityVDir = ofxVec3f(0.0,0.0,0.0);
		gravityTarget = targetPosition;
		gravityTargetDir = targetDir;
		
	} if(input == Timeline){
		timeline->clear();
		//	timeline->cues.push_back(new ofxIndustrialRobotPositionCue(100, targetPosition, (targetPosition-ofxVec3f(0,targetPosition.y,0)).normalized()));
		addCue(100, targetPosition, targetDir);
	}
	
	if(!legalPosition(targetPosition,targetDir, helper->data->reverseHeadPercentGoal)){
		float ot = 0.0;
		if( helper->data->reverseHeadPercentGoal == 0.0)
			ot = 1.0;
		if(legalPosition(targetPosition, targetDir, ot)){
			helper->data->reverseHeadPercentGoal = ot;
			helper->data->reverseHeadPercent = ot;
			helper->data->reverseHeadPercentTmp = ot;
		}
	}	
}

void ofxIndustrialRobotController::gotoResetPosition(){
	for(int i=0;i<5;i++){
		data->arms[i].rotation = converter->convertO(i, 0);
	}
	targetPosition = helper->getEndPosition(5);
	targetDir = helper->getTargetDir();
	
	gravityV = ofxVec3f(0.0,0.0,0.0);
	gravityVDir = ofxVec3f(0.0,0.0,0.0);
	gravityTarget = targetPosition;
	gravityTargetDir = targetDir;
	
	timeline->clear();
	addCue(100, targetPosition, targetDir);
	
	if(!legalPosition(targetPosition,targetDir, helper->data->reverseHeadPercentGoal)){
		float ot = 0.0;
		if( helper->data->reverseHeadPercentGoal == 0.0)
			ot = 1.0;
		if(legalPosition(targetPosition, targetDir, ot)){
			helper->data->reverseHeadPercentGoal = ot;
			helper->data->reverseHeadPercent = ot;
			helper->data->reverseHeadPercentTmp = ot;
		}
	}		
	
}

void ofxIndustrialRobotController::step(float framerate){
	if(framerate > 0){// && ofGetElapsedTimeMillis() % 5 == 0){	
		
		//		cout<<helper->data->reverseHeadPercent<<endl;
		//Calculate new reverse head value
		if(helper->data->reverseHeadPercentGoal == 1.0 && helper->data->reverseHeadPercentTmp < 1.0){
			helper->data->reverseHeadPercentTmp += 0.1*(1.0/framerate);
		}
		if(helper->data->reverseHeadPercentGoal == 0.0 && helper->data->reverseHeadPercentTmp > 0.0){
			helper->data->reverseHeadPercentTmp -= 0.1*(1.0/framerate);
		}
		helper->data->reverseHeadPercent = (sin(helper->data->reverseHeadPercentTmp*PI-HALF_PI)+1.0)/2.0;		
		//		helper->data->reverseHeadPercent += (helper->data->reverseHeadPercentGoal - helper->data->reverseHeadPercent)*0.01;
		lastTargetPosition = targetPosition;		
		
		if(input == Timeline){
			timeline->step(vTarget, framerate);
			targetPosition = timeline->getPosition();
			targetDir = timeline->getHandPosition();
			
			if(!legalPosition(timeline->getNextCuePosition(), timeline->getNextCueHandPosition(), helper->data->reverseHeadPercentGoal)){
				cout<<"Not legal"<<endl;
				float ot = 0.0;
				if( helper->data->reverseHeadPercentGoal == 0.0)
					ot = 1.0;
				if(legalPosition(timeline->getNextCuePosition(), timeline->getNextCueHandPosition(), ot)){
					cout<<"Reverse"<<endl;
					helper->data->reverseHeadPercentGoal = ot;
				}
			}
			setHandToPosition(targetPosition, targetDir,helper->data->reverseHeadPercent);
		}	
		
		if(input == ManualPosition){	
			variant = ofxIndustrialRobotDefines::Auto;
			gravityMaxSpeed = 1.0;
			gravityForce = 0.03;
		}
		
		if(input == Gravity || input == ManualPosition){
			//Constants
			double a  = gravityForce*0.01;
			double maxSpeed = gravityMaxSpeed;
			
			//Variables recalculated every frame
			double goalDist = (gravityTarget - targetPosition).length();
			double curSpeed = gravityV.length();
			double curAcc = gravityA.length();
			if( (gravityA*-1.0 + (gravityTarget - targetPosition)).length() >= (gravityA + (gravityTarget - targetPosition)).length()){
				curAcc *= -1.0;
			}
			
			double tmt = (sqrt(curAcc*curAcc/a + 2.0* curSpeed)/(sqrt(2.0) *sqrt(a)));
			double amt = -(sqrt(a) * sqrt((curAcc*curAcc +  a * 2.0 * curSpeed)/a))/sqrt(2);
			
			double distanceAtBrake = (curAcc*curAcc*curAcc)/(3.0 *a*a) + (curAcc*curAcc * tmt)/a + (amt*tmt*tmt)/2.0 - (a * tmt*tmt*tmt)/2.0 + (curAcc * curSpeed)/a + 2.0 * tmt * curSpeed;
			//	double distanceAtAcc = curAcc*curAcc*curAcc/(3.0 *  a*a) - (curAcc * curSpeed)/a;
			//			double distanceAtAcc =	curAcc*curAcc*curAcc/(3.0 * a*a) - (curAcc* curSpeed)/a - (curAcc*curAcc*sqrt(curAcc*curAcc - 2.0 *a* curSpeed))/(3.0 * a*a) + (2.0 * curSpeed * sqrt(curAcc*curAcc - 2* a* curSpeed))/(3.0 * a);
			double speedIfAccelerate = curAcc * (-curAcc)/a + (a * ((-curAcc)/a)*((-curAcc)/a))/2.0 + curSpeed;
			
			double speedIfDeac = 0.5*curAcc*curAcc/a + curSpeed;
			
			if(!gravityV.normalized().match((gravityTarget - targetPosition).normalized()) ){
				gravityV += ((gravityTarget - targetPosition).normalized() - gravityV.normalized())*0.05;
			}
			
			mode = 0;
			if(goalDist > 0){
				mode = 1;
			}
			if(speedIfDeac > maxSpeed){
				mode = -1;
				if(curAcc < 0){
					mode = 0;
					gravityA= ofxVec3f();
				}
			}
			if(distanceAtBrake >= goalDist){
				mode = -1;
			}
			if(speedIfAccelerate <= 0  && curAcc < 0){
				mode = 1;
			}
			
			//cout<<"mode "<<mode<<endl;
			if(goalDist < 0.2){
				targetPosition = gravityTarget;
				gravityA = ofxVec3f();
				gravityV = ofxVec3f();
			} else {
				gravityA += (gravityTarget - targetPosition).normalized() * a * (float)mode;
			}
		
			gravityV += gravityA*(125.0/framerate);
			if( (gravityA*-1.0 + gravityV).length() <= (gravityA + gravityV).length()){
				data->armAccHistory.push_back((gravityA.length()+0.2)*6.0);
			} else {
				data->armAccHistory.push_back((-gravityA.length()+0.2)*6.0);	
			}
							
			
			gravityVDir *= 0.92;
			gravityVDir += (gravityTargetDir - targetDir )*0.0050*gravityForce;
			
			targetPosition += gravityV*(125.0/framerate);
			targetDir += gravityVDir*(125.0/framerate);

			
			
			

			
			/*	gravityV *= 0.95;
			 if(gravityMaxSpeed == 0){
			 gravityV += (gravityTarget - targetPosition)*0.0010*gravityForce;
			 } else {
			 gravityV += (gravityTarget - targetPosition)*0.0010*gravityForce*((gravityMaxSpeed-gravityV.length())/gravityMaxSpeed);
			 }
			 gravityVDir *= 0.92;
			 gravityVDir += (gravityTargetDir - targetDir)*0.0010*gravityForce;
			 
			 *//*	if(gravityV.length() > gravityMaxSpeed && gravityMaxSpeed != 0.0){
			  float div =(float)gravityMaxSpeed / gravityV.length();
			  gravityV *= div;
			  gravityVDir *= div;				
			  }*/
			//gravityV *= 0.95;
			
			
			
	/*		targetDir += gravityVDir*(125.0/framerate);
			targetDir.normalize();
			
			
			*/if(variant == ofxIndustrialRobotDefines::Auto){
				if(!legalPosition(gravityTarget,gravityTargetDir, helper->data->reverseHeadPercentGoal)){
					float ot = 0.0;
					if( helper->data->reverseHeadPercentGoal == 0.0)
						ot = 1.0;
					if(legalPosition(gravityTarget, gravityTargetDir, ot)){
						helper->data->reverseHeadPercentGoal = ot;
						setHandToPosition(targetPosition,  targetDir);			
					}
				} else {
					setHandToPosition(targetPosition,  targetDir, helper->data->reverseHeadPercent);			
				}
				
			} else {
				if(variant == ofxIndustrialRobotDefines::Up){
					helper->data->reverseHeadPercentGoal = 1.0;
				}
				if(variant == ofxIndustrialRobotDefines::Down){
					helper->data->reverseHeadPercentGoal = 0.0;
				}
				if(legalPosition(gravityTarget,gravityTargetDir, helper->data->reverseHeadPercentGoal)){
					setHandToPosition(targetPosition,  targetDir, helper->data->reverseHeadPercent);		
				}
			}
		}
		
		if(input == Slider){
			float rotations[5];
			getArmRotations(targetPosition, targetDir, 0.0, rotations, false);
			targetPosition = helper->getEndPosition(5);
			targetDir = helper->getTargetDir();
			if(!legalPosition(targetPosition, targetDir, helper->data->reverseHeadPercentGoal)){
				float ot = 0.0;
				if( helper->data->reverseHeadPercentGoal == 0.0){
					ot = 1.0;
				}
				if(legalPosition(targetPosition, targetDir, ot)){
					helper->data->reverseHeadPercentGoal = ot;
					helper->data->reverseHeadPercentTmp = ot;
					helper->data->reverseHeadPercent = ot;
				}
			}
		}
		
		vTarget = targetPosition - lastTargetPosition;
		vTarget *= framerate;
		if(lastTargetPosition.length() == 0){
			vTarget = 0;
		}
		data->armSpeedHistory.push_back(vTarget*6);
		
		
	}
}

bool ofxIndustrialRobotController::getArmRotations(ofxVec3f toolEnd, ofxVec3f dir, float variant, float* ret,  bool limitMotors){
	ofxVec3f toolStart = toolEnd - dir.normalized()*data->tool->l;
	ofxVec3f dist = toolStart - helper->getEndPosition(0);
	double a = atan2(toolStart.x,toolStart.z)-HALF_PI;
	if(a < -PI)
		a += TWO_PI;
	ofxVec3f toolStartRotated = toolStart.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	ofxVec3f dirRotated = dir.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	
	double a1 = atan2(dirRotated.y,dirRotated.x) - HALF_PI + variant*PI;
	
	ofxVec3f h = -ofxVec3f(cos(a1), sin(a1))*(data->arms[3].length+data->arms[4].length + data->tool->h/2.0);
	
	dirRotated = dirRotated.getRotatedRad(HALF_PI - (a1) + variant*PI, ofxVec3f(0.0,0.0,1.0));
	//cout<<dirRotated.x<<"  "<<dirRotated.y<<"   "<<dirRotated.z<<endl;
	
	double a2 =  (atan2(dirRotated.x,dirRotated.z)-HALF_PI - variant*PI) * (1.0 + variant*-2.0)-TWO_PI*variant;
	//	cout<<"a2: "<<a2<<"    "<<dirRotated.x<<"    "<<dir.x<<endl;
	
	/*	if(dirRotated.z < 0){
	 a2 *= -1.0;
	 }
	 */	
	//	cout<<"   "<<a2<<endl;
	ofxVec3f armPosition = toolStart + h.getRotatedRad(a, ofxVec3f(0.0,1.0,0.0));
	
	
	float ret2[5]; 
	
	bool ok = getArmRotations2(armPosition, 360.0*a1/TWO_PI, 360.0*a2/TWO_PI, ret2, limitMotors);
	
	for(int i=0;i<5;i++){
		ret[i] = ret2[i];
	}
	return ok;
}

bool ofxIndustrialRobotController::getArmRotations2(ofxVec3f position, double rotation1, double rotation2,  float* ret,  bool limitMotors){
	double a = atan2(position.x,position.z)-HALF_PI;
	if(a < 0)
		a += TWO_PI;
	
	if((position-helper->getEndPosition(0)).length()>getMaxToolDistance()){
		//		cout<<"To far away"<<endl;
		return false;
		position -= (position-helper->getEndPosition(0)).normalized()*((float)(position-helper->getEndPosition(0)).length()-getMaxToolDistance()+0.001);
	}
	
	
	ofxVec3f rotatedTarget = position.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	ofxVec3f rotatedStart = helper->getEndPosition(0).getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	
	float b = sqrt((rotatedTarget.x-rotatedStart.x)*(rotatedTarget.x-rotatedStart.x) + (rotatedTarget.y-rotatedStart.y)*(rotatedTarget.y-rotatedStart.y));
	
	float base = (data->arms[1].length*data->arms[1].length  - data->arms[2].length*data->arms[2].length + b*b)/(2.0*b);
	float h = sqrt(data->arms[1].length*data->arms[1].length-base*base);
	float px=rotatedStart.x+base*(rotatedTarget.x-rotatedStart.x)/b;
	float py=rotatedStart.y+base*(rotatedTarget.y-rotatedStart.y)/b;
	
	float q2x=px-h*(rotatedTarget.y-rotatedStart.y)/b;
	float q2y=py+h*(rotatedTarget.x-rotatedStart.x)/b;
	
	float a1 = atan2(q2y-rotatedStart.y, q2x-rotatedStart.x);
	float a2 = atan2(rotatedTarget.y-q2y, rotatedTarget.x-q2x);
	float r[5];
	if(!isnan((180.0*a/PI))&&!isnan(-(180.0*a1/PI-90)) && !isnan(-(180.0*a2/PI-90)+(180.0*a1/PI-90))&&!isnan(-(180.0*(-a2)/PI)-rotation1)){
		r[0] = fmod(180.0*a/PI, 360);	
		r[1] = fmod(-(180.0*a1/PI-90),360);
		r[2] = fmod(-(180.0*a2/PI-90)+(180.0*a1/PI-90),360);
		r[3] = fmod(-(180.0*(-a2)/PI)-rotation1,360);
		r[4] = fmod(rotation2,360);
		
		if(r[0] > 180){
			r[0] -= 360;
		}
	} else {
		cout<<"NaN"<<endl;
		return false;
	}
	if(limitMotors){
		for(int i=0;i<5;i++){
			if(fmod(r[i],360) < fmod(converter->getMinRotation(i, r),360) || fmod(r[i],360) > fmod(converter->getMaxRotation(i, r),360)){
				//	cout<<"Error!!!"<<endl;
				if(fmod(r[i],360) + 360 > fmod(converter->getMinRotation(i, r),360) && fmod(r[i],360) + 360 < fmod(converter->getMaxRotation(i, r),360)){			
					//		cout<<"Solved 1"<<endl;
					r[i] = fmod(r[i],360) + 360;
				}
				if(fmod(r[i],360) - 360 > fmod(converter->getMinRotation(i, r),360) && fmod(r[i],360) - 360 < fmod(converter->getMaxRotation(i, r),360)){			
					//		cout<<"Solved 2"<<endl;
					r[i] = fmod(r[i],360) - 360;
				}
			}
			//cout<<i<<": "<<r[i]<<"  "<<fmod(r[i],360)<<endl;
		}
		
		
		for(int i=0;i<5;i++){
			if(fmod(r[i],360) <= fmod(converter->getMinRotation(i, r),360)){
				if(i == 4){
					if((r[i]) <= converter->getMinRotation(i, r)){
						//			cout<<"less then min at axis "<<i<<"     "<<fmod(r[i],360)<<" < "<<fmod(converter->getMinRotation(i, r),360)<<endl;
						return false;
					}
				} else {
					//		cout<<"less then min at axis "<<i<<"     "<<(r[i])<<" < "<<(converter->getMinRotation(i, r))<<endl;
					return false;
				} 
			} else if(fmod(r[i],360) >= fmod(converter->getMaxRotation(i, r),360)){
				if(i == 4){
					if((r[i]) >= converter->getMaxRotation(i, r)){
						//		cout<<"more then max at axis "<<i<<"     "<<(r[i])<<" > "<<converter->getMaxRotation(i, r)<<endl;
						return false;
					}
				} else {
					//		cout<<"more then max at axis "<<i<<"     "<<(r[i])<<" > "<<converter->getMaxRotation(i, r)<<endl;
					return false;
				}
			}
		}
	}
	//cout<<"OK"<<endl;
	for(int i=0;i<5;i++){
		ret[i] = r[i];	
	}
	return true;
}


bool ofxIndustrialRobotController::legalPosition(ofxVec3f toolEnd, ofxVec3f dir, float variant){
	float b[5];
	return getArmRotations(toolEnd, dir, variant, b);	
}

bool ofxIndustrialRobotController::legalArmPosition(ofxVec3f position, double rotation1, double rotation2){
	return true;
}

void ofxIndustrialRobotController::setHandToPosition(ofxVec3f toolEnd, ofxVec3f dir, float variant){	
	//	cout<<variant<<endl;
	float b[5];
	if(variant == -1){
		variant = helper->data->reverseHeadPercent;
	}
	
	getArmRotations(toolEnd, dir, variant, b, false);
	float min = converter->getMinRotation(4, b);
	float max = converter->getMaxRotation(4, b);
	
	if(b[4] < min){
		if(b[4] + 360.0 > min && b[4] + 360.0 < max){
			b[4] += 360.0;
		}
	}
	if(b[4] > max){
		if(b[4] - 360.0 > min && b[4] - 360.0 < max){
			b[4] -= 360.0;
		}
	}
	
	for(int i=0;i<5;i++){
		if(b[i] < converter->getMinRotation(i, b)){
			b[i] = converter->getMinRotation(i, b);
		}
		if(b[i] > converter->getMaxRotation(i, b)){
			b[i] = converter->getMaxRotation(i, b);
		}
		
		data->arms[i].rotation = b[i];
	}
	
	
}

void ofxIndustrialRobotController::setArmToPosition(ofxVec3f position, double rotation1, double rotation2){
	
}

float ofxIndustrialRobotController::getMaxToolDistance(){
	float r[5];
	r[0] = 0;
	r[1] = converter->getMaxRotation(1, r);
	ofxVec3f v1 = ofxVec3f(data->arms[1].length,0,0);
	ofxVec3f v2 = ofxVec3f(data->arms[2].length,0,0).rotated(converter->getMinRotation(2, r),ofxVec3f(0,0,1));
	return (v1+v2).length();
}

bool ofxIndustrialRobotController::addCue(float speed, ofxVec3f target, ofxVec3f dir,  bool cubicSpline,  bool endPause,  bool endInZeroSpeed){
	if(legalPosition(target,  dir, 1.0) || legalPosition(target,  dir, 0.0)){
		timeline->cues.push_back(new ofxIndustrialRobotPositionCue(speed, target, dir, cubicSpline, endPause,endInZeroSpeed));
		return true;
	} else return false;
	
}

void ofxIndustrialRobotController::mousePressed(ofxVec3f target){
	if(input == Timeline){
		if(addCue(100, target, targetDir)){
			cout<<"Added "<<target.x<<", "<<target.y<<", "<<target.z<<endl;
		}
	} else if(input == Gravity){
		if(legalPosition(target, targetDir, 0.0) || legalPosition(target, targetDir, 1.0)){
			gravityTarget = target;
			gravityTargetDir = targetDir;
			gravityForce = 0.1;
			gravityMaxSpeed = 1.5;
			variant = ofxIndustrialRobotDefines::Auto;
			
		}
	}
	
}