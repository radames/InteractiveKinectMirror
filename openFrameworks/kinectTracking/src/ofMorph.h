#ifndef _OF_MORPH // if this class hasn't been defined, the program can define it
#define _OF_MORPH // by using this if statement you prevent the class to be called more
// than once which would confuse the compiler

#include "ofMain.h"

class ofMorph {
    
public: // place public functions or variables declarations here
    // methods, equivalent to specific functions of your class objects
    void update();  // update method, used to refresh your objects properties
    void updatePosition(float _x, float _y);
    void draw();    // draw method, this where you'll do the object's drawing
    void setup(float _x, float _y, int minSpikesRandPoints, int maxSpikesRandPoints, int minNoiseCorner, int maxNoiseCorner);
    float x, y;
    float w, h;
    float random_delta[8];
    vector<ofPoint> random_points;
    bool screens[3];
    ofColor c;

};

#endif