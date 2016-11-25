#include "ofMorph.h"

void ofMorph::setup(float _x, float _y, int maxSpikesRandPoints, int minNoiseCorner, int maxNoiseCorner, float blockWidth, float blockHeight,
                    float spike_min_size, float spike_max_size, float spike_angle, float half_base, float _smooth_value)
                    
{
    x = _x;
    y = _y;
    
    w = blockWidth;
    h = blockHeight;
    
    smooth_value = _smooth_value;
    for (int i = 0; i < 3; ++i) {
        screens[i] = true; (ofRandom(0, 1) >= 0.5)?true:false;
    }
    
    //sampling the noise for cornes
    for (int i = 0; i < 8; ++ i) {
        random_delta[i] = ofRandom(minNoiseCorner, maxNoiseCorner);
    }

    
    ofRandomSpikes(maxSpikesRandPoints, spike_min_size, spike_max_size, spike_angle, half_base); //initialize spikes
    
}

void ofMorph::updatePosition(float _x, float _y){
   // ofVec2f pos(_x, _y);
   // kalman.update(pos);
    
   // ofVec2f point = kalman.getEstimation();
   // x = point.x;
   // y = point.y;
    
    if (abs(_x - x) > smooth_value) {
        x = _x;
    }
    
    if (abs(_y - y) > smooth_value) {
        y = _y;
    }
    
}

void ofMorph::draw(){

}


void ofMorph::ofRandomSpikes(int maxSpikesRandPoints, float spike_min_size, float spike_max_size, float spike_angle, float half_base){
    
    
    int nspikes_side = 0; //random num max of spikes for each side
    
    float x_left, x_right, y_left, y_right, y_top, y_bottom, x_top, x_bottom;
    float rand_x, rand_y, rand_len,random_del;
    float alpha, spike_len, spike_delta;
    
    float rand_corner_min = 5;
    float rand_corner_max = 20;
    
    float pol[4][2];
    
    pol[0][0] = -w/2 + random_delta[0];
    pol[0][1] = -h/2 + random_delta[1];
    pol[1][0] = w/2 + random_delta[2];
    pol[1][1] = -h/2 + random_delta[3];
    pol[2][0] = w/2 + random_delta[4];
    pol[2][1] = h/2 + random_delta[5];
    pol[3][0] = -w/2 + random_delta[6];
    pol[3][1] = h/2 + random_delta[7];
    
    //looping throught the 4 sides of the quadrilateral
    
    for (int i = 0; i < 4; ++i) {
        
        int nspikes_side = int(ofRandom(1, maxSpikesRandPoints));
        
        vector<pointpointTuple> randSpikes;
        
        /* Create random points in polygon side */
        for (int j = 0; j < nspikes_side; ++j) {
            
            pointpointTuple spikes;
            
            spike_len = ofRandom(spike_min_size, spike_max_size);
            
            /* Is it a top/bottom side or left/right side? */
            /* Top or bottom */
            if (i == 0 || i == 1) {
                x_left = pol[i][0];
                y_left = pol[i][1];
                x_right = pol[(i + 1) % 4][0];
                y_right = pol[(i + 1) % 4][1];
            } else {
                x_left = pol[(i + 1) % 4][0];
                y_left = pol[(i + 1) % 4][1];
                x_right = pol[i][0];
                y_right = pol[i][1];
            }
            
            if ((i + 2) % 2 == 0) {
                
                rand_x = ofRandom(x_left + half_base, x_right - half_base);
                rand_y = ofMap(rand_x, x_left, x_right,  y_left, y_right);
                
                /* Ponto esquerdo da base do spike */
                spikes.esq1 = rand_x - half_base;
                spikes.esq2  = ofMap(spikes.esq1, x_left, x_right,  y_left, y_right);
                
                /* Ponto direito da base do spike */
                spikes.dir1  = rand_x + half_base;
                spikes.dir2  = ofMap(spikes.dir1, x_left, x_right,  y_left, y_right);
                
                /* Ponto mais externo do spike */
                random_del = ofRandom(-spike_angle, spike_angle);
                
                spikes.pt1 = rand_x + random_del;
                spikes.pt2 = rand_y - (1 - i)*spike_len;
                
            } else {
                
                rand_y = ofRandom(y_left + half_base, y_right - half_base);
                rand_x = ofMap(rand_y, y_left, y_right,  x_left, x_right);
                
                /* Ponto esquerdo da base do spike */
                spikes.esq2 = rand_y - half_base;
                spikes.esq1 = ofMap(spikes.esq2,  y_left, y_right,  x_left, x_right);
                
                /* Ponto direito da base do spike */
                spikes.dir2 = rand_y + half_base;
                spikes.dir1 =  ofMap(spikes.dir2,  y_left, y_right,  x_left, x_right);
                
                /* Ponto mais externo do spike */
                random_del = ofRandom(-spike_angle, spike_angle);
                
                spikes.pt1 = rand_x + i*(2 - i)/i*spike_len;
                spikes.pt2 = rand_y + random_del;
                
            }
            
            randSpikes.push_back(spikes);
            
        }
        spikesPoints.push_back(randSpikes);
    }

}