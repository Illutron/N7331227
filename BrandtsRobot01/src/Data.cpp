/*
 *  data.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 25/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Data.h"


myData::myData(ofBaseApp * _baseApp){
	srcImg.allocate(CAPWIDTH,CAPHEIGHT);
	baseApp = _baseApp;
	srcImgGray.allocate(CAPWIDTH,CAPHEIGHT);
	

	
}
void myData::setSrcImg(ofxCvColorImage _srcImg)
{
	srcImg = _srcImg;
	srcImgGray = _srcImg;
}

