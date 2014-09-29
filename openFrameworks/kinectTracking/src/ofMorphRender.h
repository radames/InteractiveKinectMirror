/*
 * CirclesRenderer.h
 *
 *  Created on: 11/07/2012
 *      Author: arturo
 */

#ifndef OFMORPHRENDER_H_
#define OFMORPHRENDER_H_

#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofMain.h"


class ofMorphRender {
public:
    ofMorphRender();
    
    void setup();
    void draw();
    
    ofParameterGroup parameters;
    ofParameter<float> size;
    ofParameter<int> number;
    ofParameter<ofVec2f> position;
    ofParameter<ofColor> color;
    
};

#endif /* OFMORPHRENDER_H_*/
