#include "ofAppQtWindow.h"
#include "ofBaseApp.h"
#include "ofMain.h"

// glut works with static callbacks UGH, so we need static variables here:




//----------------------------------------------------------
ofAppQtWindow::ofAppQtWindow(){
	timeNow				= 0;
	timeThen			= 0;
	fps					= 60; //give a realistic starting value - win32 issues
	windowMode			= OF_WINDOW;
	bNewScreenMode		= true;
	nFramesForFPS		= 0;
	nFrameCount			= 0;
	buttonInUse			= 0;
	bEnableSetupScreen	= true;
	bFrameRateSet		= false;
	millisForFrame		= 0;
	prevMillis			= 0;
	diffMillis			= 0;
	requestedWidth		= 0;
	requestedHeight		= 0;
	nonFullScreenX		= -1;
	nonFullScreenY		= -1;
	mouseX				= 0;
	mouseY				= 0;
	
}

//------------------------------------------------------------
void ofAppQtWindow::setupOpenGL(int w, int h, int screenMode){
	
	
	int argc = 1;
	char *argv = "openframeworks";
	char **vptr = &argv;
	qtApp = new QApplication(argc, vptr);


/*	glutInit(&argc, vptr);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
	
	windowMode = screenMode;
	bNewScreenMode = true;
	
	if (windowMode != OF_GAME_MODE){
		glutInitDisplayString("rgb double depth>=16 alpha samples<=4");
		
		glutInitWindowSize(w, h);
		glutCreateWindow("");
		
		//Default colors etc are now in ofGraphics - ofSetupGraphicDefaults
		ofSetupGraphicDefaults();
	*/	
		/*
		 ofBackground(200,200,200);		// default bg color
		 ofSetColor(0xFFFFFF); 			// default draw color
		 // used to be black, but
		 // black + texture = black
		 // so maybe grey bg
		 // and "white" fg color
		 // as default works the best...
		 */
/*		
		requestedWidth  = glutGet(GLUT_WINDOW_WIDTH);
		requestedHeight = glutGet(GLUT_WINDOW_HEIGHT);
	} else {
		glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
    	// w x h, 32bit pixel depth, 60Hz refresh rate
		char gameStr[64];
		sprintf( gameStr, "%dx%d:%d@%d", w, h, 32, 60 );
		
    	glutGameModeString(gameStr);
		
    	if (!glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
    		ofLog(OF_LOG_ERROR,"game mode error: selected format (%s) not available \n", gameStr);
    	}
    	// start fullscreen game mode
    	glutEnterGameMode();
	}*/
}

//------------------------------------------------------------
void ofAppQtWindow::initializeWindow(){
	
	
	//----------------------
	// setup the callbacks
	
	/*glutMouseFunc(mouse_cb);
	glutMotionFunc(motion_cb);
	glutPassiveMotionFunc(passive_motion_cb);
	glutIdleFunc(idle_cb);
	glutDisplayFunc(display);
	
	glutKeyboardFunc(keyboard_cb);
	glutKeyboardUpFunc(keyboard_up_cb);
	glutSpecialFunc(special_key_cb);
	glutSpecialUpFunc(special_key_up_cb);
	
	glutReshapeFunc(resize_cb);
	*/
}

//------------------------------------------------------------
void ofAppQtWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	static ofEventArgs voidEventArgs;

	ofAppPtr = (testApp*)appPtr;

	window = new Window(ofAppPtr);

	if(ofAppPtr){
		ofAppPtr->setup(window,window->viewCombo);
		ofAppPtr->update();
	}
	window->loadSettings();
	
    window->show();	
//	window->glWidget->appSet = true;

	ofSetDataPathRoot("../../../data/");

	
#ifdef OF_USING_POCO
	ofNotifyEvent( ofEvents.setup, voidEventArgs );
	ofNotifyEvent( ofEvents.update, voidEventArgs );
#endif
	
	qtApp->exec();



	
	
/*	glutMainLoop();*/
}


void ofAppQtWindow::setStatusMessage(string s){

	window->statusBar()->showMessage(QString(s.c_str()), 2000);
	
}

//------------------------------------------------------------
void ofAppQtWindow::exitApp(){
	
	//  -- This already exists in ofExitCallback
	
	//	static ofEventArgs voidEventArgs;
	//
	//	if(ofAppPtr)ofAppPtr->exit();
	//
	//	#ifdef OF_USING_POCO
	//		ofNotifyEvent( ofEvents.exit, voidEventArgs );
	//	#endif
	
	ofLog(OF_LOG_VERBOSE,"GLUT OF app is being terminated!");
	
	OF_EXIT_APP(0);
}

//------------------------------------------------------------
float ofAppQtWindow::getFrameRate(){
	return window->getGlFrameRate();
}

//------------------------------------------------------------
int ofAppQtWindow::getFrameNum(){
	return nFrameCount;
}

//------------------------------------------------------------
void ofAppQtWindow::setWindowTitle(string title){
	glutSetWindowTitle(title.c_str());
}

//------------------------------------------------------------
ofPoint ofAppQtWindow::getWindowSize(){
//	int width = glutGet(GLUT_WINDOW_WIDTH);
//	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int width = window->getGlWidth();
	int height = window->getGlHeight();
	return ofPoint(width, height,0);
}

//------------------------------------------------------------
ofPoint ofAppQtWindow::getWindowPosition(){
	int x = glutGet(GLUT_WINDOW_X);
	int y = glutGet(GLUT_WINDOW_Y);
	return ofPoint(x,y,0);
}

//------------------------------------------------------------
ofPoint ofAppQtWindow::getScreenSize(){
	int width = glutGet(GLUT_SCREEN_WIDTH);
	int height = glutGet(GLUT_SCREEN_HEIGHT);
	return ofPoint(width, height,0);
}

//------------------------------------------------------------
void ofAppQtWindow::setWindowPosition(int x, int y){
	glutPositionWindow(x,y);
}

//------------------------------------------------------------
void ofAppQtWindow::setWindowShape(int w, int h){
	glutReshapeWindow(w, h);
	// this is useful, esp if we are in the first frame (setup):
	requestedWidth  = w;
	requestedHeight = h;
}

//------------------------------------------------------------
void ofAppQtWindow::hideCursor(){
	glutSetCursor(GLUT_CURSOR_NONE);
}

//------------------------------------------------------------
void ofAppQtWindow::showCursor(){
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}

//------------------------------------------------------------
void ofAppQtWindow::setFrameRate(float targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?
	
	// --- > f / s
	
	if (targetRate == 0){
		bFrameRateSet = false;
		return;
	}
	
	bFrameRateSet 			= true;
	float durationOfFrame 	= 1.0f / (float)targetRate;
	millisForFrame 			= (int)(1000.0f * durationOfFrame);
	
	//frameRate				= targetRate;
	
}

//------------------------------------------------------------
int ofAppQtWindow::getWindowMode(){
	return windowMode;
}

//------------------------------------------------------------
void ofAppQtWindow::toggleFullscreen(){
	if( windowMode == OF_GAME_MODE)return;
	
	if( windowMode == OF_WINDOW ){
		windowMode = OF_FULLSCREEN;
	}else{
		windowMode = OF_WINDOW;
	}
	
	bNewScreenMode = true;
}

//------------------------------------------------------------
void ofAppQtWindow::setFullscreen(bool fullscreen){
    if( windowMode == OF_GAME_MODE)return;
	
    if(fullscreen && windowMode != OF_FULLSCREEN){
        bNewScreenMode  = true;
        windowMode      = OF_FULLSCREEN;
    }else if(!fullscreen && windowMode != OF_WINDOW) {
        bNewScreenMode  = true;
        windowMode      = OF_WINDOW;
    }
}

//------------------------------------------------------------
void ofAppQtWindow::enableSetupScreen(){
	bEnableSetupScreen = true;
}

//------------------------------------------------------------
void ofAppQtWindow::disableSetupScreen(){
	bEnableSetupScreen = false;
}

//------------------------------------------------------------
/*void ofAppQtWindow::display(void){
	static ofEventArgs voidEventArgs;
	
	//--------------------------------
	// when I had "glutFullScreen()"
	// in the initOpenGl, I was gettings a "heap" allocation error
	// when debugging via visual studio.  putting it here, changes that.
	// maybe it's voodoo, or I am getting rid of the problem
	// by removing something unrelated, but everything seems
	// to work if I put fullscreen on the first frame of display.
	
	if (windowMode != OF_GAME_MODE){
		if ( bNewScreenMode ){
			if( windowMode == OF_FULLSCREEN){
				
				//----------------------------------------------------
				// before we go fullscreen, take a snapshot of where we are:
				nonFullScreenX = glutGet(GLUT_WINDOW_X);
				nonFullScreenY = glutGet(GLUT_WINDOW_Y);
				//----------------------------------------------------
				
				glutFullScreen();
				
#ifdef TARGET_OSX
				SetSystemUIMode(kUIModeAllHidden,NULL);
#endif
				
			}else if( windowMode == OF_WINDOW ){
				
				glutReshapeWindow(requestedWidth, requestedHeight);
				
				//----------------------------------------------------
				// if we have recorded the screen posion, put it there
				// if not, better to let the system do it (and put it where it wants)
				if (nFrameCount > 0){
					glutPositionWindow(nonFullScreenX,nonFullScreenY);
				}
				//----------------------------------------------------
				
#ifdef TARGET_OSX
				SetSystemUIMode(kUIModeNormal,NULL);
#endif
			}
			bNewScreenMode = false;
		}
	}
	
	int width, height;
	
	width  = ofGetWidth();
	height = ofGetHeight();
	
	height = height > 0 ? height : 1;
	// set viewport, clear the screen
	glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();
	
	// I don't know why, I need more than one frame at the start in fullscreen mode
	// also, in non-fullscreen mode, windows/intel graphics, this bClearAuto just fails.
	// I seem to have 2 buffers, alot of flickering
	// and don't accumulate the way I expect.
	// with this line:   if ((bClearAuto == true) || nFrameCount < 3){
	// we do nFrameCount < 3, so that the buffers are cleared at the start of the app
	// or else we have video memory garbage to draw on to...
	
#ifdef TARGET_WIN32
	//windows doesn't get accumulation in window mode
	if ((bClearAuto == true || windowMode == OF_WINDOW) || nFrameCount < 3){
#else
		//mac and linux does :)
		if ( bClearAuto == true || nFrameCount < 3){
#endif
			glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		
		if( bEnableSetupScreen )ofSetupScreen();
		
		if(ofAppPtr)
			ofAppPtr->draw();
		
#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.draw, voidEventArgs );
#endif
		
		glutSwapBuffers();
		
		// -------------- fps calculation:
		timeNow = ofGetElapsedTimef();
		if( (timeNow-timeThen) > 0.05f || nFramesForFPS == 0 ) {
			fps = (double)nFramesForFPS / (timeNow-timeThen);
			timeThen = timeNow;
			nFramesForFPS = 0;
			
			//hack for windows - was getting NAN - maybe unitialized vars???
			if( nFrameCount < 5) frameRate = fps;
			else frameRate = 0.9f * frameRate + 0.1f * fps;
		}
		nFramesForFPS++;
		// --------------
		
		nFrameCount++;		// increase the overall frame count
		
		//setFrameNum(nFrameCount); // get this info to ofUtils for people to access
		
	}
	
	//------------------------------------------------------------
	void ofAppQtWindow::mouse_cb(int button, int state, int x, int y) {
		static ofMouseEventArgs mouseEventArgs;
		
		if (nFrameCount > 0){
			if(ofAppPtr){
				ofAppPtr->mouseX = x;
				ofAppPtr->mouseY = y;
			}
			
			if (state == GLUT_DOWN) {
				if(ofAppPtr)
					ofAppPtr->mousePressed(x,y,button);
				
#ifdef OF_USING_POCO
				mouseEventArgs.x = x;
				mouseEventArgs.y = y;
				mouseEventArgs.button = button;
				ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
#endif
			} else if (state == GLUT_UP) {
				if(ofAppPtr){
					ofAppPtr->mouseReleased(x,y,button);
					ofAppPtr->mouseReleased();
				}
				
#ifdef OF_USING_POCO
				mouseEventArgs.x = x;
				mouseEventArgs.y = y;
				mouseEventArgs.button = button;
				ofNotifyEvent( ofEvents.mouseReleased, mouseEventArgs );
#endif
			}
			buttonInUse = button;
		}
	}
	
	//------------------------------------------------------------
	void ofAppQtWindow::motion_cb(int x, int y) {
		static ofMouseEventArgs mouseEventArgs;
		
		if (nFrameCount > 0){
			if(ofAppPtr){
				ofAppPtr->mouseX = x;
				ofAppPtr->mouseY = y;
				ofAppPtr->mouseDragged(x,y,buttonInUse);
			}
			
#ifdef OF_USING_POCO
			mouseEventArgs.x = x;
			mouseEventArgs.y = y;
			mouseEventArgs.button = buttonInUse;
			ofNotifyEvent( ofEvents.mouseDragged, mouseEventArgs );
#endif
		}
		
	}
	
	//------------------------------------------------------------
	void ofAppQtWindow::passive_motion_cb(int x, int y) {
		static ofMouseEventArgs mouseEventArgs;
		
		if (nFrameCount > 0){
			if(ofAppPtr){
				ofAppPtr->mouseX = x;
				ofAppPtr->mouseY = y;
				ofAppPtr->mouseMoved(x,y);
			}
			
#ifdef OF_USING_POCO
			mouseEventArgs.x = x;
			mouseEventArgs.y = y;
			ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
#endif
		}
	}
	
	
	//------------------------------------------------------------
	void ofAppQtWindow::idle_cb(void) {
		static ofEventArgs voidEventArgs;
		
		//	thanks to jorge for the fix:
		//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate
		
		if (nFrameCount != 0 && bFrameRateSet == true){
			diffMillis = ofGetElapsedTimeMillis() - prevMillis;
			if (diffMillis > millisForFrame){
				; // we do nothing, we are already slower than target frame
			} else {
				int waitMillis = millisForFrame - diffMillis;
#ifdef TARGET_WIN32
				Sleep(waitMillis);         //windows sleep in milliseconds
#else
				usleep(waitMillis * 1000);   //mac sleep in microseconds - cooler :)
#endif
			}
		}
		prevMillis = ofGetElapsedTimeMillis(); // you have to measure here
		
		if(ofAppPtr)
			ofAppPtr->update();
		
#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.update, voidEventArgs);
#endif
		
		glutPostRedisplay();
	}
	
	
	//------------------------------------------------------------
	void ofAppQtWindow::keyboard_cb(unsigned char key, int x, int y) {
		static ofKeyEventArgs keyEventArgs;
		
		if(ofAppPtr)
			ofAppPtr->keyPressed(key);
		
#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
#endif
		
		if (key == OF_KEY_ESC){				// "escape"
			exitApp();
		}
	}
	
	//------------------------------------------------------------
	void ofAppQtWindow::keyboard_up_cb(unsigned char key, int x, int y) {
		static ofKeyEventArgs keyEventArgs;
		
		if(ofAppPtr)
			ofAppPtr->keyReleased(key);
		
#ifdef OF_USING_POCO
		keyEventArgs.key = key;
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
#endif
	}
	
	//------------------------------------------------------
	void ofAppQtWindow::special_key_cb(int key, int x, int y) {
		static ofKeyEventArgs keyEventArgs;
		
		if(ofAppPtr)
			ofAppPtr->keyPressed(key | OF_KEY_MODIFIER);
		
#ifdef OF_USING_POCO
		keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyPressed, keyEventArgs );
#endif
	}
	
	//------------------------------------------------------------
	void ofAppQtWindow::special_key_up_cb(int key, int x, int y) {
		static ofKeyEventArgs keyEventArgs;
		
		if(ofAppPtr)
			ofAppPtr->keyReleased(key | OF_KEY_MODIFIER);
		
#ifdef OF_USING_POCO
		keyEventArgs.key = (key | OF_KEY_MODIFIER);
		ofNotifyEvent( ofEvents.keyReleased, keyEventArgs );
#endif
	}
	
	//------------------------------------------------------------
	void ofAppQtWindow::resize_cb(int w, int h) {
		static ofResizeEventArgs resizeEventArgs;
		
		if(ofAppPtr)
			ofAppPtr->windowResized(w,h);
		
#ifdef OF_USING_POCO
		resizeEventArgs.width = w;
		resizeEventArgs.height = h;
		ofNotifyEvent( ofEvents.windowResized, resizeEventArgs );
#endif
	}
*/