#include "ofMorph.h"

void ofMorph::setup(float _x, float _y)
{
    x = _x;
    y = _y;
    
    for (int i; i < 3; ++i) {
        screens[i] = (ofRandom(0, 1) >= 0.5)?true:false;
    }
    
}

void ofMorph::updatePosition(float _x, float _y){
    x = _x;
    y = _y;
}

void ofMorph::draw(){

}