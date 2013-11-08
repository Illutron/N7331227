#include "ofxIndustrialRobotSerial.h"
#define DATAGRAM_LENGTH 24


ofxIndustrialRobotSerial::ofxIndustrialRobotSerial(){
	serial.setVerbose(false);
	connected = serial.setup("/dev/tty.usbserial-A9005esv", 115200);	
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
}


void ofxIndustrialRobotSerial::sendMsg(float val){
	if(connected){
		unsigned char outbuf[DATAGRAM_LENGTH];
		//((float*)outbuf)[3]=(float)(sin(ofGetElapsedTimeMillis()/1000.0)*TWO_PI);
		((float*)outbuf)[3]=(float)val;
		
		outbuf[DATAGRAM_LENGTH-4]='*';
		outbuf[DATAGRAM_LENGTH-3]=0xaa;
		outbuf[DATAGRAM_LENGTH-2]=0x55;
		outbuf[DATAGRAM_LENGTH-1]=0x77;
		
		//	for(int i=0;i<DATAGRAM_LENGTH;i++)
		//	{
		//	ApdComPort1->PutChar(outbuf[i]);
		serial.writeBytes(outbuf, DATAGRAM_LENGTH);
	}
	
}
void ofxIndustrialRobotSerial::step(){
	if(connected){
		nTimesRead = 0;
		nBytesRead = 0;
		int nRead  = 0;  // a temp variable to keep count per read
		
		unsigned char bytesReturned[1];
		
		//		memset(bytesReadString, 0, 4);
		//		memset(bytesReturned, 0, 1);
		
		//	while( (nRead = serial.readBytes( bytesReturned, 1)) > 0){
		//	nTimesRead++;	
		//		nBytesRead = nRead;
		//			cout<<"ret "<<bytesReturned<<endl;
		//	};
		//		memcpy(bytesReadString, bytesReturned, 1);
		serial.flush(true, false);
		
		//        Sleep(1);
		//	}
	}
	
}


