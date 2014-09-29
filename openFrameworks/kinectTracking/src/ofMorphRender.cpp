//
//  ofMorphRender.cpp
//  kinectTracking
//
//  Created by Radames Ajna on 29/09/14.
//
//

#include "ofMorphRender.h"

ofMorphRender::ofMorphRender() {
    
}


void ofMorphRender::setup(ofFbo *_screen1, ofFbo *_screen2, ofFbo *_screen3){
    
    render_type = RenderType(random() % 3);
    
    screen1 = _screen1;
    screen2 = _screen1;
    screen3 = _screen1;
    
    parameters.setName("Morphs");
    parameters.add(size.set("size",10,0,100));
    parameters.add(number.set("number",2,1,20));
    parameters.add(position.set("position",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    color.set("color",ofColor(127),ofColor(0,0),ofColor(255));
    parameters.add(color);
    

}

void ofMorphRender::draw() {
    for(tr1::unordered_map<unsigned int, ofMorph>::iterator it = morphs.begin(); it != morphs.end(); ++it)
        for (int i = 0; i < 3; ++i)
            if (it->second.screens[i])
                switch (render_type) {
                    case BARS:
                        draw_bar(it->second, i);
                        break;
                    case GRADIENT:
                        draw_gradient(it->second, i);
                        break;
                    case SPIKES:
                        draw_spikes(it->second, i);
                        break;
                }
}

void ofMorphRender::draw_morph(ofMorph m, int screen_i) {
    float scaleH1 = ofMap(m.x, kinect_width,0, 0, 1);
    float posx1 = ofMap(m.y, kinect_height, 0, 0, CWIDTH1);
    float posy1 = CHEIGHT/2;
    
    float scaleH2 = ofMap(m.y, kinect_height, 0, 0, 1);
    float posx2 = ofMap(m.x, 0, kinect_width, 0, CWIDTH2);
    float posy2 = CHEIGHT/2;
    
    float scaleH3 = ofMap(m.x, 0, kinect_width, 0, 1);
    float posx3 = ofMap(m.y, 0, kinect_width, 0, CWIDTH3);
    float posy3 = CHEIGHT/2;
    
    screen1->begin();
    ofClear(255,255,255);
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(posx1, posy1, 30*scaleH1*10, 60*scaleH1*10);
    ofPopStyle();
    screen1->end();
}

void ofMorphRender::draw_bar(ofMorph m, int screen_i) {
    
}

void ofMorphRender::draw_spikes(ofMorph m, int screen_i) {
    
}

void ofMorphRender::draw_gradient(ofMorph m, int screen_i) {
    
}

void ofMorphRender::addMorph(float x, float y, int id){
    ofMorph *m = new ofMorph();
    m->setup(x, y);
    morphs[id] = *m;
}


void deleteMorph(int id){
    
}
