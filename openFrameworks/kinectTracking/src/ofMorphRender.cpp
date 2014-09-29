//
//  ofMorphRender.cpp
//  kinectTracking
//
//  Created by Radames Ajna on 29/09/14.
//
//

#include "ofMorphRender.h"



ofMorphRender::ofMorphRender(){
    screen_type = RenderType(random() % 3);

}


void ofMorphRender::setup(){
    
    parameters.setName("Morphs");
    parameters.add(size.set("size",10,0,100));
    parameters.add(number.set("number",2,1,20));
    parameters.add(position.set("position",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    color.set("color",ofColor(127),ofColor(0,0),ofColor(255));
    parameters.add(color);
    

}

void ofMorphRender::draw() {
    for(vector<ofMorph>::iterator it = morphs.begin(); it != morphs.end(); ++it){
        switch (screen_type) {
            case BARS:
                for (int i; i < 3; ++i) {
                    if ((*it).screens[i])
                        draw_bar((*it), i);
                }
                break;
            case GRADIENT:
                for (int i; i < 3; ++i) {
                    if ((*it).screens[i])
                        draw_gradient((*it), i);
                }
                break;
            case SPIKES:
                for (int i; i < 3; ++i) {
                    if ((*it).screens[i])
                        draw_spikes((*it), i);
                }
                break;

        }
    }
}

void ofMorphRender::draw_morph(ofMorph m, int screen_i) {
    
}

void ofMorphRender::draw_bar(ofMorph m, int screen_i) {
    
}

void ofMorphRender::draw_spikes(ofMorph m, int screen_i) {
    
}

void ofMorphRender::draw_gradient(ofMorph m, int screen_i) {
    
}


