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


void ofMorphRender::setup(ofFbo *_screen1, ofFbo *_screen2, ofFbo *_screen3, float _kinect_width, float _kinect_height){
    
    render_type = BARS;//RenderType(random() % 3);
    screen1 = _screen1;
    screen2 = _screen2;
    screen3 = _screen3;
    kinect_width = _kinect_width;
    kinect_height = _kinect_height;
    parameters.setName("Morphs");
    parameters.add(size.set("size",10,0,100));
    parameters.add(number.set("number",2,1,20));
    parameters.add(position.set("position",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    color.set("color",ofColor(127),ofColor(0,0),ofColor(255));
    parameters.add(color);

    parameters.add(bars_min_width.set("bar width min",0,100,500));
    parameters.add(bars_max_width.set("bar width max",0,100,500));


}

void ofMorphRender::draw() {
    for(tr1::unordered_map<unsigned int, ofMorph>::iterator it = morphs.begin(); it != morphs.end(); it++)
        for (int i = 0; i < 3; ++i) {
            if (it->second.screens[i]) {
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
        }
}

void ofMorphRender::draw_bar(ofMorph m, int screen_i) {
    float scaleH1 = ofMap(m.x, kinect_width, 0, bars_min_width, bars_max_width);
    float posx1 = ofMap(m.y, kinect_height, 0, 0, CWIDTH1);
    float posy1 = CHEIGHT/2;
    
    float scaleH2 = ofMap(m.y, kinect_height, 0, bars_min_width, bars_max_width);
    float posx2 = ofMap(m.x, 0, kinect_width, 0, CWIDTH2);
    float posy2 = CHEIGHT/2;
    
    float scaleH3 = ofMap(m.x, 0, kinect_width, 0, bars_min_width, bars_max_width);
    float posx3 = ofMap(m.y, 0, kinect_width, 0, CWIDTH3);
    float posy3 = CHEIGHT/2;
    
    switch (screen_i) {
        case 0:
            screen1->begin();
            ofClear(255,255,255);
            ofPushStyle();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(0, 0, 0);
            ofFill();
            ofRect(posx1, posy1, scaleH1, CHEIGHT);
            ofPopStyle();
            screen1->end();
            break;
        case 1:
            screen2->begin();
            ofClear(255,255,255);
            ofPushStyle();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(0, 0, 0);
            ofFill();
            ofRect(posx2,posy2, scaleH2, CHEIGHT);
            ofPopStyle();
            screen2->end();
            break;            
        case 2:
            screen3->begin();
            ofClear(255,255,255);
            ofPushStyle();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(0, 0, 0);
            ofFill();
            ofRect(posx3,posy3, scaleH3, CHEIGHT);
            ofPopStyle();
            screen3->end();
            break;
    }

    
}

void ofMorphRender::draw_spikes(ofMorph m, int screen_i) {
    
}

void ofMorphRender::draw_gradient(ofMorph m, int screen_i) {
    float scaleH1 = ofMap(m.x, kinect_width, 0, 0.5, 3);
    float dir1 = ofMap(m.y, kinect_height, 0, -1, 1);
    float posx1 = ofMap(m.y, kinect_height, 0, 0, CWIDTH1);
    float posy1 = CHEIGHT/2;
    
    float scaleH2 = ofMap(m.y, kinect_height, 0, 0.5, 3);
    float dir2 = ofMap(m.x, 0, kinect_width, -1, 1);
    float posx2 = ofMap(m.x, 0, kinect_width, 0, CWIDTH2);
    float posy2 = CHEIGHT/2;
    
    float scaleH3 = ofMap(m.x, 0, kinect_width, 0, 3, 0.5);
    float dir3 = ofMap(m.y, kinect_height, 0, 1, -1);
    float posx3 = ofMap(m.y, 0, kinect_width, 0, CWIDTH3);
    float posy3 = CHEIGHT/2;
    
    switch (screen_i) {
        case 0:
            screen1->begin();
            ofClear(255,255,255);
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(posx1, posy1);
            for (int j = 0; j < 10; ++j) {
                
                ofScale(0.9, 0.9);
                ofSetColor((10 - j)*10, (10 - j)*10, (10 - j)*10);
                ofFill();
                ofPushMatrix();
                    ofTranslate(dir1*j, 0);

                    ofBeginShape();
                    ofVertex(-m.w/2 + m.random_delta[0], -m.h/2 + m.random_delta[1]);
                    ofVertex(m.w/2 + m.random_delta[2], -m.h/2 + m.random_delta[3]);
                    ofVertex(m.w/2 + m.random_delta[4], m.h/2 + m.random_delta[5]);
                    ofVertex(-m.w/2 + m.random_delta[6], m.h/2 + m.random_delta[7]);
                    ofEndShape();
                ofPopMatrix();
            }
            
            ofPopMatrix();
            ofPopStyle();
            screen1->end();
            break;
        case 1:
            screen2->begin();
            ofClear(255,255,255);
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(posx2, posy2);
            ofScale(scaleH2, scaleH2);
            for (int j = 0; j < 10; ++j) {
                ofScale(0.9, 0.9);
                ofSetColor((10 - j)*10, (10 - j)*10, (10 - j)*10);
                ofFill();
                ofPushMatrix();
                    ofTranslate(dir2*100, 0);
                    ofBeginShape();
                    ofVertex(-m.w/2 + m.random_delta[0], -m.h/2 + m.random_delta[1]);
                    ofVertex(m.w/2 + m.random_delta[2], -m.h/2 + m.random_delta[3]);
                    ofVertex(m.w/2 + m.random_delta[4], m.h/2 + m.random_delta[5]);
                    ofVertex(-m.w/2 + m.random_delta[6], m.h/2 + m.random_delta[7]);
                    ofEndShape();
                ofPopMatrix();
            }
            ofPopMatrix();
            ofPopStyle();
            screen2->end();
            break;
        case 2:
            screen3->begin();
            ofClear(255,255,255);
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(posx3, posy3);
            ofScale(scaleH3, scaleH3);
            for (int j = 0; j < 10; ++j) {
                ofScale(0.9, 0.9);
                ofSetColor((10 - j)*10, (10 - j)*10, (10 - j)*10);
                ofFill();
                ofPushMatrix();
                    ofTranslate(dir3*j, 0);
                    ofBeginShape();
                    ofVertex(-m.w/2 + m.random_delta[0], -m.h/2 + m.random_delta[1]);
                    ofVertex(m.w/2 + m.random_delta[2], -m.h/2 + m.random_delta[3]);
                    ofVertex(m.w/2 + m.random_delta[4], m.h/2 + m.random_delta[5]);
                    ofVertex(-m.w/2 + m.random_delta[6], m.h/2 + m.random_delta[7]);
                    ofEndShape();
                ofPopMatrix();
            }
            ofPopMatrix();
            ofPopStyle();
            screen3->end();
            break;
    }
}

void ofMorphRender::addMorph(float x, float y, int id){
    ofMorph *m = new ofMorph();
    m->setup(x, y);
    morphs[id] = *m;
}


void deleteMorph(int id){
    
}
