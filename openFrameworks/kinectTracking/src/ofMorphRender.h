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
#include "ofMorph.h"

class ofMorphRender {
    
    enum RenderType {
        BARS,
        SPIKES,
        GRADIENT
    };
    
public:
    ofMorphRender();
    
    void setup(ofFbo *screen1, ofFbo *screen2, ofFbo *screen3);
    void draw();
    void draw_morph(ofMorph m, int screen_i);
    void draw_bar(ofMorph m, int screen_i);
    void draw_spikes(ofMorph m, int screen_i);
    void draw_gradient(ofMorph m, int screen_i);

    vector<ofMorph> morphs;
    int screen_type;
    ofFbo *screen1, *screen2, *screen3;
    
    ofParameterGroup parameters;
    ofParameter<float> size;
    ofParameter<int> number;
    ofParameter<ofVec2f> position;
    ofParameter<ofColor> color;
    
    // Kinect Parameters
    ofParameter<float> kinect_width;
    ofParameter<float> kinect_height;
    
    // Tunnel Renderer Parameters
    ofParameter<float> tunnel_max_width;
    ofParameter<float> tunnel_min_width;
    ofParameter<int> tunnel_num_layers;
    ofParameter<ofColor> tunnel_lower_color;
    ofParameter<ofColor> tunnel_higher_color;

    // Bars Renderer Parameters
    ofParameter<float> bars_min_width;
    ofParameter<float> bars_max_width;
    
    // Spikes Parameters
    ofParameter<int> spikes_min_num;
    ofParameter<int> spikes_max_num;
    
    static const int CHEIGHT = 960; //canvas height
    static const int CWIDTH1 = 768; //canvas width 1
    static const int CWIDTH2 = 960; //canvas width 2
    static const int CWIDTH3 = 768; //canvas width 3
};

#endif /* OFMORPHRENDER_H_*/
