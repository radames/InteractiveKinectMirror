#include "ofApp.h"
#include "ofxSyphon.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {

    screenSetup(); //screen and some OF setups
    kinectSetup(); //kinetic setup
    morphRender.setup(&screen1, &screen2, &screen3, kinectWidth, kinectHeight); //inicializo os parametros

    guiSetup(); //GUI Setup

}

//--------------------------------------------------------------
void ofApp::update() {

    ofEnableAlphaBlending();
	ofBackground(255, 255, 255);
    kinectUpdate();

    //varre os blobs, checa
    RectTracker& tracker = contourFinder.getTracker();
//    for(int i = 0; i < contourFinder.size(); i++) {
//        unsigned int label = contourFinder.getLabel(i);
//
//           if(tracker.existsPrevious(label)) {
//               //caso o tracker j‡ existe checa qual o novo ID
//
//               const cv::Rect& previous = tracker.getPrevious(label);
//               const cv::Rect& current = tracker.getCurrent(label);
//               //tracker.getPre
//               //atualiza o hash com a posicao dos morphs
//
//               morphRender.morphs[label].updatePosition(current.x, current.y);
//
//           }
//
//    }

    const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
    const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
    const vector<unsigned int>& newLabels = tracker.getNewLabels();
    const vector<unsigned int>& deadLabels = tracker.getDeadLabels();


    //varrer deadLabels e procurar morphs e KILL them
    /*

    for(int i = 0; i < contourFinder.size(); i++) {
        unsigned int label = contourFinder.getLabel(i);

       // int label = currentLabels[i];
        const cv::Rect& current = tracker.getCurrent(label);


        if(tracker.existsPrevious(label)) {
           // update position Morph
            if (morphRender.morphs.count(label) > 0) {
                morphRender.morphs[label].updatePosition(current.x, current.y);
            }
        } 
    }*/

    for(int i = 0; i < deadLabels.size(); i++) {
        if (morphRender.morphs.count(deadLabels[i]) > 0) {
            morphRender.deleteMorph(deadLabels[i]);
        }
    }

    for(int i = 0; i < currentLabels.size(); i++) {
        int label = newLabels[i];
        const cv::Rect& current = tracker.getCurrent(label);

        if(tracker.existsPrevious(label)) {
            if (morphRender.morphs.count(label) > 0) {
                morphRender.morphs[label].updatePosition(applyOffsetX(current.x), applyOffsetY(current.y));
            }
        }
    }
    
    for(int i = 0; i < newLabels.size(); i++) {
        int label = newLabels[i];
        const cv::Rect& current = tracker.getCurrent(label);
        morphRender.addMorph(applyOffsetX(current.x), applyOffsetY(current.y), label);
    }


}

//--------------------------------------------------------------
void ofApp::draw() {

    if(bDebugMode){ debugMode(); }//draw debug mode

    // cleaning alls screens
    screen1.begin();
    ofClear(255,255,255, 0);
    screen1.end();
    screen2.begin();
    ofClear(255,255,255, 0);
    screen2.end();
    screen3.begin();
    ofClear(255,255,255, 0);
    screen3.end();


    morphRender.draw();
    screen1.draw(0,0);
    screen2.draw(CWIDTH1,0);
    screen3.draw(CWIDTH1+CWIDTH2,0);

    syphonServer.publishScreen(); //syphon screen

}


///DEBUG-MODE

void ofApp::debugMode(){

    //showing kinect stuffs
    ofPushStyle();

        ofSetColor(255, 130, 0);
        ofFill();		// draw "filled shapes"
        ofRect(0, 0, CHEIGHT, CHEIGHT);
        ofRect(CWIDTH1 + CWIDTH2, 0, 768, CHEIGHT);
        ofSetColor(255,0,0);
        ofFill();
        ofRect(CWIDTH3, 0, 960, CHEIGHT);
        ofSetColor(255, 255, 255);


        // draw from the live kinect
        kinect.draw(0, 200, 300, 200);
        kinect.drawDepth(0, 0, 300, 200);

    ofPopStyle();

    ofPushMatrix();

        ofTranslate(0,400);
        ofScale(300.0/kinect.width,200.0/kinect.height);

        grayImage.draw(0,0);

            ofPushMatrix();
                ofTranslate(kinect.width/2,kinect.height/2);
                ofPushStyle();
                //drawing the actual kinect area, used to the positions calculation inside de morphrender funciton
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofSetColor(255,0,0,100);
                ofFill();
                ofRect(0,0, kinectWidth, kinectHeight);

                ofPopStyle();
            ofPopMatrix();
        contourFinder.draw();

    ofPushStyle();
    //draw simuled blob
    ofSetColor(0,255,0);
    ofFill();
    ofCircle(blobx ,bloby, 5);

    ofPopStyle();

    ofPopMatrix();

    //loop through all blobs detected and draw the centroid and lables

    RectTracker& tracker = contourFinder.getTracker();

    for(int i=0; i < contourFinder.size(); i++){
        unsigned int label = contourFinder.getLabel(i);

        if(tracker.existsPrevious(label)) {

            ofPoint center = toOf(contourFinder.getCenter(i));
            ofPushStyle();
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
            ofPopStyle();
        }

    }

    // draw instructions
    ofPushStyle();

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
    << "set near threshold " << nearThreshold << " (press: + -)" << endl
    << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.getContours().size()
    << ", fps: " << ofGetFrameRate() << endl
    << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;


    ofDrawBitmapString(reportStream.str(), 20, 652);
    ofPopStyle();
    gui.draw();




}


//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();


}

void ofApp::screenSetup(){

    syphonServer.setName("kinectTracking");

    ofSetWindowShape(CWIDTH1*2 + CWIDTH2,CHEIGHT); //set windowSize the same as the


    //starting FBO buffers for each screen
    screen1.allocate(CWIDTH1, CHEIGHT,  GL_RGBA32F_ARB);
    screen2.allocate(CWIDTH2, CHEIGHT,  GL_RGBA32F_ARB);
    screen3.allocate(CWIDTH3, CHEIGHT,  GL_RGBA32F_ARB);

    screen1.begin();
    ofClear(255,255,255, 0);
    screen1.end();
    screen2.begin();
    ofClear(255,255,255, 0);
    screen2.end();
    screen3.begin();
    ofClear(255,255,255, 0);
    screen3.end();


    ofEnableSmoothing();
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);

}

void ofApp::kinectUpdate(){

    kinect.update();

    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {

        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);

        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);


        // update the cv images
        grayImage.flagImageChanged();

        contourFinder.setMinAreaRadius(minBlobSize);
        contourFinder.setMaxAreaRadius(maxBlobSize);
        contourFinder.findContours(grayImage);

    }

}
void ofApp::kinectSetup(){

    // enable depth.video image calibration
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


    ofSetFrameRate(60);
    // zero the tilt on startup
    kinect.setCameraTiltAngle(0);
    bDebugMode = true;
    nearThreshold = 255;


    //blob tracking system parameter
    contourFinder.getTracker().setPersistence(100);
    contourFinder.getTracker().setMaximumDistance(32);


}



void ofApp::guiSetup(){





    gui.setup("Settings", "settings.xml", 310,100);

    gui.add(enableMouse.set("Mouse DEBUG",true));
    parametersKinect.setName("Kinect");
    parametersKinect.add(kinectWidth.set("Width",640, 540,740));
    parametersKinect.add(kinectHeight.set("Height",480, 380,580));



    parametersKinect.add(farThreshold.set("Far Threshold", 0,0, 255 ));
    parametersKinect.add(numMaxBlobs.set("Num Max Blos",10,0,15));
    parametersKinect.add(maxBlobSize.set("max Blob Size",0,0,500));
    parametersKinect.add(minBlobSize.set("min Blob Size",0,0,500));

    parametersKinect.add(offsetX.set("Offset X", 0,-200, 200 ));
    parametersKinect.add(offsetY.set("Offset Y", 0,-200, 200 ));



    gui.add(parametersKinect);
    gui.add(morphRender.generalGUI);
    gui.add(morphRender.gradientGUI);
    gui.add(morphRender.barsGUI);
    gui.add(morphRender.spikesGUI);


    gui.minimizeAll();
    // events for change in paramenters on ofpp application
    kinectWidth.addListener(this,&ofApp::kinectUpdateAreaW);
    kinectHeight.addListener(this,&ofApp::kinectUpdateAreaH);

    gui.loadFromFile("settings.xml");

}
//Gui events for kinect Area
void ofApp::kinectUpdateAreaW(int& kinectWidth){
    morphRender.setKinectArea(kinectWidth, kinectHeight);

}

void ofApp::kinectUpdateAreaH(int& kinectHeight){
    morphRender.setKinectArea(kinectWidth, kinectHeight);

}




float ofApp::applyOffsetX(float _x){
    return _x + offsetX;

}
float ofApp::applyOffsetY(float _y){
    return _y + offsetY;
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
		case 'm':
            enableMouse = !enableMouse;
			break;

		case OF_KEY_UP:
            bloby-=10;

			break;

		case OF_KEY_DOWN:
            bloby+=10;
			break;

        case OF_KEY_LEFT:
            blobx-=10;
            break;

        case OF_KEY_RIGHT:
            blobx+=10;
            break;

        case 'z':
            //if not exist Add
            if(!morphRender.morphs.count(0)){
                morphRender.addMorph(kinectWidth/2,ofRandom(kinectHeight),0);
            }
            break;
        case 'x':
            //if exists delete
            if(morphRender.morphs.count(0)){
                morphRender.deleteMorph(0);
            }
            break;


	}


    //if debugMode and not mouse move the blob with the keyboard
    if(!enableMouse){
        if(morphRender.morphs.size() > 0){

            morphRender.morphs[0].x = applyOffsetX(blobx);
            morphRender.morphs[0].y = applyOffsetY(bloby);

        }
    }



}
void ofApp::mouseMoved(int x, int y){


    if(enableMouse){
        if(morphRender.morphs.size() > 0){
            blobx = ofMap(ofGetMouseX(), 0, ofGetScreenWidth(),  0, kinectWidth);
            bloby = ofMap(ofGetMouseY(), 0, ofGetScreenHeight(), 0, kinectHeight);

            morphRender.morphs[0].x = applyOffsetX(blobx);
            morphRender.morphs[0].y = applyOffsetX(bloby);
        }
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
