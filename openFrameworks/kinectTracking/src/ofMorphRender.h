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
#include <tr1/unordered_map>

class ofMorphRender {
    
    enum RenderType {
        BARS,
        SPIKES,
        GRADIENT,
        ALL_GRADIENT
    };
    
public:
    ofMorphRender();
    
    void setup(ofFbo *screen1, ofFbo *screen2, ofFbo *screen3, int kinect_width, int kinect_height);
    void draw();
    void draw_morph(ofMorph m, int screen_i);
    void draw_bar(ofMorph m, int screen_i);
    void draw_spikes(ofMorph m, int screen_i);
    void draw_gradient(ofMorph *m, int screen_i);
    void draw_all_gradient();
    void addMorph(float x, float y, int id);
    void deleteMorph(int id);
    
    void setKinectArea(int _kinectWidth,int _kinectHeight);
    
    tr1::unordered_map <unsigned int, ofMorph> morphs;

    int render_type;
    
    ofParameterGroup generalGUI;
    ofParameterGroup spikesGUI;
    ofParameterGroup barsGUI;
    ofParameterGroup gradientGUI;

    
    // Kinect Parameters
    int kinect_width;
    int kinect_height;
    
    // Gradient Renderer Parameters
    ofParameter<float> gradient_max_width;
    ofParameter<float> gradient_min_width;
    ofParameter<int> gradient_num_layers;
    ofParameter<float> gradient_time_frames;
    ofParameter<float> gradient_change_per_level;
    ofParameter<float> gradient_animation_speed;
    ofParameter<float> gradient_animation_max_time;
    
    // Bars Renderer Parameters
    ofParameter<float> bars_min_width = 10;
    ofParameter<float> bars_max_width = 150;
    
    // Spikes Parameters
    ofParameter<int> spikes_min_num;
    ofParameter<int> spikes_max_num;
    ofParameter<int> minSpikesRandPoints;
    ofParameter<int> maxSpikesRandPoints;
    
    //General Parameters
    ofParameter<int> minNoiseCorner;
    ofParameter<int> maxNoiseCorner;

    static const int CHEIGHT = 960; //canvas height
    static const int CWIDTH1 = 768; //canvas width 1
    static const int CWIDTH2 = 960; //canvas width 2
    static const int CWIDTH3 = 768; //canvas width 3

protected:
    ofFbo *screen1, *screen2, *screen3;

};

#endif /* OFMORPHRENDER_H_*/
