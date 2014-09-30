#include "ofMorph.h"

void ofMorph::setup(float _x, float _y)
{
    x = _x;
    y = _y;
    w = 100;
    h = 300;
    
    for (int i = 0; i < 3; ++i) {
        screens[i] = true; //(ofRandom(0, 1) >= 0.5)?true:false;
    }
    
    for (int i = 0; i < 8; ++ i) {
        random_delta[i] = ofRandom(5, 15);
    }
    
}

void ofMorph::updatePosition(float _x, float _y){
    x = _x;
    y = _y;
}

void ofMorph::draw(){

}