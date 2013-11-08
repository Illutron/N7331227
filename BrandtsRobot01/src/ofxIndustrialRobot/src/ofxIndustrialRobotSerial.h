#pragma once
#include "ofMain.h"
#include "ofxThread.h"

class ofxIndustrialRobotSerial : public ofxThread{
public:
	ofSerial	serial;
	
	
	ofxIndustrialRobotSerial();
	void step();
	
	bool		bSendSerialMessage;			// a flag for sending serial
	char		bytesRead[1];				// data from serial, we will be trying to read 3
	char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
	int			nBytesRead;					// how much did we read?
	int			nTimesRead;					// how many times did we read?
	float		readTime;					// when did we last read?	
	
	bool connected;
	
	void start(){
		startThread(true, false);   // blocking, verbose
	}
	
	void stop(){
		stopThread();
	}
	
	void sendMsg(float val);
	
	//--------------------------
	void threadedFunction(){
		
		while( isThreadRunning() != 0 ){
			if( lock() ){
//				count++;
				step();
				unlock();
				ofSleepMillis(50);
			}
		}
	}
	
};