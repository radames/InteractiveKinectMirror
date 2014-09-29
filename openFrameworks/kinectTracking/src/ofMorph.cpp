#include "ofMorph.h"

ofMorph::ofMorph(float _x, float _y, float _w, float _h, ofColor _c)
{
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    c = _c;
    
    for (int i; i < 3; ++i) {
        screens[i] = (ofRandom(0, 1) >= 0.5)?true:false;
    }
    

}

void ofMorph::update(){

}

void ofMorph::draw(){

}