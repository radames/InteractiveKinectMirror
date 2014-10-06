#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofMorph.h"
#include "ofMorphRender.h"

class ofApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void mouseMoved(int x, int y);
    
    void screenSetup();
    void kinectSetup();
    void kinectUpdate();
    void debugMode();
    

    void guiSetup();
    
    //gui Change events
    void kinectUpdateAreaW(int& kinectWidth);
    void kinectUpdateAreaH(int& kinectHeight);
    
    
    // blob Offset detais
    float applyOffsetX(float _x);
    float applyOffsetY(float _y);


    
	ofxKinect kinect;
    ofxSyphonServer syphonServer;
    ofxCv::ContourFinder contourFinder;


	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    //GUI
    ofxPanel gui; //

    ofParameterGroup parametersKinect;
    

    ofParameter<bool> enableMouse;
    ofParameter<int> farThreshold = 143;
    ofParameter<float> offsetX;
    ofParameter<float> offsetY;
    ofParameter<int> numMaxBlobs = 15;
    ofParameter<int> minBlobSize = 22;
    ofParameter<int> maxBlobSize = 354;
    ofParameter<int> kinectWidth = 640;
    ofParameter<int> kinectHeight = 480;
    
    
    //options variables
    bool bDebugMode = false;
	int nearThreshold;
	
    float blobx,bloby;
        
    static const int CHEIGHT = 960; //canvas height
    static const int CWIDTH1 = 768; //canvas width 1
    static const int CWIDTH2 = 960; //canvas width 2
    static const int CWIDTH3 = 768; //canvas width 3
    
    //ofscreen buffers
    ofFbo screen1;
    ofFbo screen2;
    ofFbo screen3;
    
    ofMorphRender morphRender;
    
    vector<ofMorph> morphs;
        


};
