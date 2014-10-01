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
    
    render_type = SPIKES;//RenderType(random() % 3);
    screen1 = _screen1;
    screen2 = _screen2;
    screen3 = _screen3;
    kinect_width = _kinect_width;
    kinect_height = _kinect_height;

    generalGUI.setName("General Params");
    generalGUI.add(minNoiseCorner.set("min Noise Corner", 0,10,100));
    generalGUI.add(maxNoiseCorner.set("max Noise Corne", 0,10,100));
    
    gradientGUI.setName("Gradient");
    
    gradientGUI.add(gradient_min_width.set("min Width",0, 10,200));
    gradientGUI.add(gradient_max_width.set("max Width",0, 10,200));
    gradientGUI.add(gradient_lower_color.set("lower Color",ofColor(127),ofColor(0,0),ofColor(255)));
    gradientGUI.add(gradient_higher_color.set("higher Color", ofColor(127),ofColor(0,0),ofColor(255)));


    
    barsGUI.setName("Bars");
    barsGUI.add(bars_min_width.set("bar width min",0,10,300));
    barsGUI.add(bars_max_width.set("bar width max",0,10,300));
    
    spikesGUI.setName("Spikes");
    spikesGUI.add(minSpikesRandPoints.set("min Rand Points", 0,3,10));
    spikesGUI.add(maxSpikesRandPoints.set("max Rand Points", 0,20,50));
    spikesGUI.add(spikes_min_num.set("min num",0,0,20));
    spikesGUI.add(spikes_max_num.set("max num",0,0,20));



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
    ofFbo *screen;
    float scaleH, posx, posy = CHEIGHT/2;
    
    switch (screen_i) {
        case 0:
            scaleH = ofMap(m.x, kinect_width, 0, bars_min_width, bars_max_width);
            posx = ofMap(m.y, kinect_height, 0, 0, CWIDTH1);
            screen = screen1;
            break;
        case 1:
            scaleH = ofMap(m.y, kinect_height, 0, bars_min_width, bars_max_width);
            posx = ofMap(m.x, 0, kinect_width, 0, CWIDTH2);
            screen = screen2;
            break;
        case 2:
            scaleH = ofMap(m.x, 0, kinect_width, 0, bars_min_width, bars_max_width);
            posx = ofMap(m.x, 0, kinect_width, 0, bars_min_width, bars_max_width);
            screen = screen3;
            break;
    }
    
    screen->begin();
    ofClear(255,255,255);
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0, 0, 0);
    ofFill();
    ofRect(posx, posy, scaleH, CHEIGHT);
    ofPopStyle();
    screen->end();

}

void ofMorphRender::draw_spikes(ofMorph m, int screen_i) {
    
    ofFbo *screen;
    float scaleH, posx, posy = CHEIGHT/2;
    
    switch (screen_i) {
        case 0:
            scaleH = ofMap(m.x, kinect_width, 0, 0.5, 3);
            posx = ofMap(m.y, kinect_height, 0, 0, CWIDTH1);
            screen = screen1;
            break;
        case 1:
            scaleH = ofMap(m.y, kinect_height, 0, 0.5, 3);
            posx = ofMap(m.x, 0, kinect_width, 0, CWIDTH2);
            screen = screen2;
            break;
        case 2:
            scaleH = ofMap(m.x, 0, kinect_width, 0, 3, 0.5);
            posx = ofMap(m.x, 0, kinect_width, 0, 0.5, 3);
            screen = screen3;
            break;
    }
    
    

    screen->begin();
    
    ofClear(255,255,255);
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(posx, posy);
    ofScale(scaleH, scaleH);
    ofSetColor(0,0,0);
    ofFill();
        ofPushMatrix();
            ofBeginShape();
            ofVertex(-m.w/2 + m.random_delta[0], -m.h/2 + m.random_delta[1]);
            ofVertex(m.w/2 + m.random_delta[2], -m.h/2 + m.random_delta[3]);
            ofVertex(m.w/2 + m.random_delta[4], m.h/2 + m.random_delta[5]);
            ofVertex(-m.w/2 + m.random_delta[6], m.h/2 + m.random_delta[7]);
            ofEndShape();
        ofPopMatrix();
    ofPopMatrix();
    ofPopStyle();
    

    screen->end();
    
    
}

void ofMorphRender::draw_gradient(ofMorph m, int screen_i) {
    ofFbo *screen;
    float scaleH, dir, posx, posy = CHEIGHT/2;
    long long now = ofGetElapsedTimeMillis();

    if (now - last_time > 50) {
        dt += 0.03;
        if (dt > 3)
            dt = 0;
        gradient_data g;
        g.posx = m.x;
        g.posy = m.y;
        g.scaleH = scaleH;
        gradient_slices[grad_i] = g;
        grad_i = (grad_i + 1) % grad_max;
        last_time = now;
        if (grad_added < grad_max) {
            ++grad_added;
        }
    }

    if (screen_i == 0) {
        screen = screen1;
    } else if (screen_i ==1) {
        screen = screen2;
    } else {
        screen = screen3;
    }
    
    screen->begin();
    ofClear(255,255,255);

    for (int j = 0; j < grad_added; ++j) {
        gradient_data g = gradient_slices[(grad_i + j) % grad_max];

        switch (screen_i) {
            case 0:
                scaleH = ofMap(g.posx, kinect_width, 0, 0.5, 5);
                posx = ofMap(g.posy, kinect_height, 0, 0, CWIDTH1);
                break;
            case 1:
                scaleH = ofMap(g.posy, kinect_height, 0, 0.5, 5);
                posx = ofMap(g.posx, 0, kinect_width, 0, CWIDTH2);
                break;
            case 2:
                scaleH = ofMap(g.posx, 0, kinect_width, 0.5, 5);
                posx = ofMap(g.posy, 0, kinect_width, 0, CWIDTH3);
                break;
        }
        
        ofPushStyle();
            ofPushMatrix();        
                ofTranslate(posx, posy);
                float s = scaleH - j*(0.06) + dt*(grad_added - j - 1)/grad_added;
                //float s = scaleH - j*(0.06);
                s = (s < 0)?0.5:s;
                ofScale(s, s);
                ofSetColor(255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added);
                ofFill();
                ofPushMatrix();
                    ofBeginShape();
                        ofVertex(-m.w/2 + m.random_delta[0], -m.h/2 + m.random_delta[1]);
                        ofVertex(m.w/2 + m.random_delta[2], -m.h/2 + m.random_delta[3]);
                        ofVertex(m.w/2 + m.random_delta[4], m.h/2 + m.random_delta[5]);
                        ofVertex(-m.w/2 + m.random_delta[6], m.h/2 + m.random_delta[7]);
                    ofEndShape();
                ofPopMatrix();
            ofPopMatrix();
        ofPopStyle();

    }
    screen->end();
}

void ofMorphRender::addMorph(float x, float y, int id){
    ofMorph *m = new ofMorph();
    m->setup(x, y, minSpikesRandPoints, maxSpikesRandPoints, minNoiseCorner, maxNoiseCorner);
    morphs[id] = *m;
}


void deleteMorph(int id){
    
}
