#include "ofApp.h"
#include "ofxSyphon.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
    syphonServer.setName("kinectTracking");

    ofSetWindowShape(CWIDTH1*2 + CWIDTH2,CHEIGHT); //set windowSize the same as the

    ofEnableSmoothing();
    ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    

    
	bThreshWithOpenCV = true;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
    bDebugMode = true;

    nearThreshold = 255;
    
    // GUI ------
    
    
    gui.setup("Settings", "settings.xml", 310,100);
    

    parametersKinect.setName("Kinect");
    
    parametersKinect.add(farThreshold.set("Far Threshold", 0,0, 255 ));
    parametersKinect.add(numMaxBlobs.set("Num Max Blos",10,0,15));
    parametersKinect.add(maxBlobSize.set("max Blob Size",0,0,500));
    parametersKinect.add(minBlobSize.set("min Blob Size",0,0,500));

    parametersKinect.add(offsetX.set("Offset X", 0,0, 200 ));
    parametersKinect.add(offsetY.set("Offset Y", 0,0, 200 ));
    
    parametersShapes.setName("Shapes");
    

    
    gui.add(parametersKinect);
    gui.add(parametersShapes);

    gui.loadFromFile("settings.xml");
    
    
    contourFinder.getTracker().setPersistence(15);
    contourFinder.getTracker().setMaximumDistance(32);    

}

//--------------------------------------------------------------
void ofApp::update() {
	
	ofBackground(255, 255, 255);
	
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
			
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		grayImage.flagImageChanged();
        
        contourFinder.setMinAreaRadius(minBlobSize);
        contourFinder.setMaxAreaRadius(maxBlobSize);
        contourFinder.findContours(grayImage);

//
//		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
//		// also, find holes is set to true so we will get interior contours as well....
//		contourFinder.findContours(grayImage, minBlobSize, maxBlobSize, numMaxBlobs, false);
//
    }

    
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    if(bDebugMode) debugMode(); //draw debug mode
    
    syphonServer.publishScreen(); //syphon screen

}


///DEBUG-MODE

void ofApp::debugMode(){
    
    //showing kinect stuffs
    
    ofSetColor(255, 130, 0);
    ofFill();		// draw "filled shapes"
    ofRect(0, 0, CHEIGHT, CHEIGHT);
    ofRect(CWIDTH1 + CWIDTH2, 0, 768, CHEIGHT);
    ofSetColor(255,0,0);
    ofFill();
    
    ofRect(CWIDTH3, 0, 960, CHEIGHT);
    ofSetColor(255, 255, 255);
    
    
    // draw from the live kinect
    kinect.drawDepth(0, 0, 300, 200);
    kinect.draw(0, 200, 300, 200);
    ofPushMatrix();

        ofTranslate(0,400);
        ofScale(300.0/kinect.width,200.0/kinect.height);
        grayImage.draw(0,0);
        contourFinder.draw();

    ofPopMatrix();

    //loop through all blobs detected and draw the centroid and lables
    
    RectTracker& tracker = contourFinder.getTracker();

    for(int i=0; i < contourFinder.size(); i++){
        unsigned int label = contourFinder.getLabel(i);

        if(tracker.existsPrevious(label)) {
            ofPoint center = toOf(contourFinder.getCenter(i));
            ofSetColor(255,0,0);
            ofFill();
            ofPushMatrix();
                ofTranslate(0,400);
                ofScale(300.0/kinect.width,200.0/kinect.height);
                ofEllipse(center.x,center.y,10,10);
                string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
                ofDrawBitmapString(msg,center.x,center.y);
                ofVec2f velocity = toOf(contourFinder.getVelocity(i));
                ofPushMatrix();
                    ofTranslate(center.x, center.y);
                    ofScale(10, 10);
                    ofLine(0, 0, velocity.x, velocity.y);
                    ofPopMatrix();
                ofPopMatrix();
            ofPopMatrix();
        }
        
    }
    
    // draw instructions
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
        << "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
   reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
    << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
    << "set near threshold " << nearThreshold << " (press: + -)" << endl
    << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.getContours().size()
    << ", fps: " << ofGetFrameRate() << endl
    << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
    
    if(kinect.hasCamTiltControl()) {
        reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
    
    ofDrawBitmapString(reportStream.str(), 20, 652);
    
    gui.draw();
    


}


//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	

}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
	switch (key) {
    
        case 's':
            gui.saveToFile("settings.xml");

            break;
        case 'l':
            gui.loadFromFile("settings.xml");
            break;
            
        case 'f':
            ofToggleFullscreen();
            ofSetWindowShape(CWIDTH1*2 + CWIDTH2,CHEIGHT); //set windowSize the same as the

            break;
    
        case 'd':
            bDebugMode = !bDebugMode;

            break;
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			break;
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{}
