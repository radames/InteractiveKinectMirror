//
//  ofMorphRender.cpp
//  kinectTracking
//
//  Created by Radames Ajna on 29/09/14.
//
//

#include "ofMorphRender.h"



ofMorphRender::ofMorphRender(){
    
    

}


void ofMorphRender::setup(){
    
    parameters.setName("Morphs");
    parameters.add(size.set("size",10,0,100));
    parameters.add(number.set("number",2,1,20));
    parameters.add(position.set("position",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    
    color.set("color",ofColor(127),ofColor(0,0),ofColor(255));
    
    parameters.add(color);
    

}
