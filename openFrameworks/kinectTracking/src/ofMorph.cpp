#include "ofMorph.h"

void ofMorph::setup(float _x, float _y, int minSpikesRandPoints, int maxSpikesRandPoints, int minNoiseCorner, int maxNoiseCorner)
{
    x = _x;
    y = _y;
    w = 100;
    h = 300;
    
    for (int i = 0; i < 3; ++i) {
        screens[i] = true; (ofRandom(0, 1) >= 0.5)?true:false;
    }
    
    //sampling the noise for cornes
    for (int i = 0; i < 8; ++ i) {
        random_delta[i] = ofRandom(minNoiseCorner, maxNoiseCorner);
    }
    int r = ofRandom(minSpikesRandPoints, maxSpikesRandPoints);
    
    //sampling the randon points in the screen
    for(int i; i < r; i++){
        // must be outside the shape
    //    Ofpoint p;
        float rx,ry;
        while( (rx < x-w/2 || rx > x+w/2) && (ry < y-h/2 || ry > y + h/2)){
     ///       rx = ofRandom(0,)
        }
    }
    
    
}

void ofMorph::updatePosition(float _x, float _y){
    x = _x;
    y = _y;
}

void ofMorph::draw(){

}