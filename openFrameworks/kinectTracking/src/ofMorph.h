#ifndef _OF_MORPH // if this class hasn't been defined, the program can define it
#define _OF_MORPH // by using this if statement you prevent the class to be called more
// than once which would confuse the compiler

#include "ofMain.h"

class ofMorph {
    
public: // place public functions or variables declarations here
    
    // methods, equivalent to specific functions of your class objects
    void update();  // update method, used to refresh your objects properties
    void draw();    // draw method, this where you'll do the object's drawing
    ofMorph(float _x, float _y, float _w, float _h, ofColor _c);
    float x, y, h, w;
    bool screens[3];
    ofColor c;

};

#endif