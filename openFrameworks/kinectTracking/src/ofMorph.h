#ifndef _OF_MORPH // if this class hasn't been defined, the program can define it
#define _OF_MORPH // by using this if statement you prevent the class to be called more
// than once which would confuse the compiler

#include "ofMain.h"

struct gradient_data {
    float posx, posy;
};


struct pointpointTuple {
    float esq1,esq2;
    float dir1,dir2;
    float pt1,pt2;
};

class ofMorph {
    
public: // place public functions or variables declarations here
    // methods, equivalent to specific functions of your class objects
    void update();  // update method, used to refresh your objects properties
    void updatePosition( float _x, float _y);
    void draw();    // draw method, this where you'll do the object's drawing
    void setup( float _x, float _y, int maxSpikesRandPoints, int minNoiseCorner, int maxNoiseCorner, float blockWidth, float blockHeight,
                float spike_min_size, float spike_max_size, float spike_angle, float half_base, float _smooth_value);
    float x, y;
    float center_x, center_y;
    float w, h;
    float smooth_value;
    
    float random_delta[8];

    //spikes
    vector<vector<pointpointTuple> > spikesPoints;
    
    bool screens[3];
    ofColor c;
    
    long long last_time = 0;
    gradient_data gradient_slices[15];
    int grad_i = 0;
    int grad_added = 0;
    int grad_max = 15;
    float dt = 0;
    
private:
    
    void ofRandomSpikes(int maxSpikesRandPoints, float spike_min_size, float spike_max_size,float spike_angle, float half_base);
    
    
};

#endif