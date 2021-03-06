//
//  ofMorphRender.cpp
//  kinectTracking
//
//  Created by Radames Ajna on 29/09/14.
//
//

#include "ofMorphRender.h"

ofMorphRender::ofMorphRender() {
    
}


void ofMorphRender::setup(ofFbo *_screen1, ofFbo *_screen2, ofFbo *_screen3, int _kinect_width, int _kinect_height){
    
    render_type = SPIKES;//RenderType(random() % 3);
    screen1 = _screen1;
    screen2 = _screen2;
    screen3 = _screen3;
    kinect_width = _kinect_width;
    kinect_height = _kinect_height;

    generalGUI.setName("General Params");
    generalGUI.add(minNoiseCorner.set("min noise corner", 0,10,100));
    generalGUI.add(maxNoiseCorner.set("max noise corne", 0,10,100));
    generalGUI.add(cHeight.set("objects height pos",480,0,960));
    generalGUI.add(blockWidth.set("block width", 20, 10, 200));
    generalGUI.add(blockHeight.set("block height", 50, 10, 400));
    generalGUI.add(smooth_value.set("movement smooth",3, 0, 10));
    gradientGUI.setName("Gradient");
    
    gradientGUI.add(gradient_min_width.set("min width",0.2, 0,5));
    gradientGUI.add(gradient_max_width.set("max width",3, 0,5));
    gradientGUI.add(gradient_time_frames.set("time per frame", 10, 0, 1000));
    gradientGUI.add(gradient_change_per_level.set("size change per level", 0.05, 0, 2));
    gradientGUI.add(gradient_animation_speed.set("animation speed", 5, 0, 100));
    gradientGUI.add(gradient_animation_max_time.set("animation max time", 3, 0, 100));
    
    barsGUI.setName("Bars");
    barsGUI.add(bars_min_width.set("bar width min",0,10,300));
    barsGUI.add(bars_max_width.set("bar width max",0,10,300));
    
    spikesGUI.setName("Spikes");
    spikesGUI.add(maxSpikesRandPoints.set("max num spikes", 10,2,100));
    spikesGUI.add(spikeMinSize.set("min Size", 40, 5,200));
    spikesGUI.add(spikeMaxSize.set("max Size", 100, 5,200));
    spikesGUI.add(spike_angle.set("spikes angle",30, 20,60));
    spikesGUI.add(half_base.set("base len", 5, 2, 25));
    
    

    morphs.clear();

}

void ofMorphRender::draw() {
    if (render_type == ALL_GRADIENT) {
        for (int i = 0; i < 3; ++i) {
            draw_all_gradient(&morphs, i);
        }
        return;
    }
    for(unordered_map<unsigned int, ofMorph>::iterator it = morphs.begin(); it != morphs.end(); it++){
        for (int i = 0; i < 3; ++i) {
            if (it->second.screens[i]) {
                switch (render_type) {
                    case BARS:
                        draw_bar(it->second, i);
                        break;
                    case GRADIENT:
                        draw_gradient(&it->second, i);
                        break; 
                    case SPIKES:
                        draw_spikes(it->second, i);
                        break;
                }
            }
        }
    }
}

void ofMorphRender::draw_all_gradient(unordered_map<unsigned int, ofMorph> *morphs, int screen_i) {
    ofFbo *screen;
        
    float scaleH, dir, posx, posy = cHeight;
    long long now = ofGetElapsedTimeMillis();

    for(unordered_map<unsigned int, ofMorph>::iterator it = morphs->begin(); it != morphs->end(); it++){
        ofMorph *m = &it->second;

        if (now - m->last_time > gradient_time_frames) {
            m->dt += gradient_animation_speed/1000;
            if (m->dt > gradient_animation_max_time)
                m->dt = -gradient_animation_max_time;
        
            if (m->grad_added < m->grad_max) {
                while (m->grad_added < m->grad_max) {
                    gradient_data g;
                    g.posx = m->x;
                    g.posy = m->y;
                    m->gradient_slices[m->grad_added] = g;
                    m->last_time = now;
                    ++m->grad_added;
                }
                m->grad_i = 1;
            } else {
                gradient_data g;
                g.posx = m->x;
                g.posy = m->y;
                m->gradient_slices[m->grad_i] = g;
                m->grad_i = (m->grad_i + 1) % m->grad_max;
                m->last_time = now;
            }
        
        }
    }
    
    for (int j = 0; j < 15; ++j) {
        for(unordered_map<unsigned int, ofMorph>::iterator it = morphs->begin(); it != morphs->end(); it++){
            ofPath poly;
            ofMorph *m = &it->second;
        
                if (screen_i == 0) {
                    screen = screen1;
                } else if (screen_i == 1) {
                    screen = screen2;
                } else {
                    screen = screen3;
                }
                
                screen->begin();
                gradient_data g = m->gradient_slices[(m->grad_i + j) % m->grad_max];
                
                switch (screen_i) {
                    case 0:
                        scaleH = ofMap(g.posx, kinect_width, 0, gradient_min_width, gradient_max_width);
                        posx = ofMap(g.posy, kinect_height, 0, 0, CWIDTH1);
                        break;
                    case 1:
                        scaleH = ofMap(g.posy, kinect_height, 0, gradient_min_width, gradient_max_width);
                        posx = ofMap(g.posx, 0, kinect_width, 0, CWIDTH2);
                        break;
                    case 2:
                        scaleH = ofMap(g.posx, 0, kinect_width, gradient_min_width, gradient_max_width);
                        posx = ofMap(g.posy, 0, kinect_height, 0, CWIDTH3);
                        break;
                }
            
                
                ofPushStyle();
                ofPushMatrix();
            ofTranslate(posx, posy);
            float s = scaleH - j*(gradient_change_per_level) + abs(m->dt)*(m->grad_added - j - 1)/m->grad_added;
            //float s = scaleH - j*(0.06);
            s = (s < (float)gradient_min_width)?(float)gradient_min_width:s;
            ofScale(s, s);
         
            /*
            ofPolyline pl;
            pl.addVertex(-m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1]);
            pl.addVertex(m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3]);
            pl.addVertex(m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5]);
            pl.addVertex(-m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7]);
            
            ofPoint pt = pl.getCentroid2D(); */
            
            float x[4], y[4];
            float a, cx, cy, t;
            int i, i1;
            
            x[0] = -m->w/2 + m->random_delta[0];
            y[0] = -m->h/2 + m->random_delta[1];
            x[1] = m->w/2 + m->random_delta[2];
            y[1] = -m->h/2 + m->random_delta[3];
            x[2] = m->w/2 + m->random_delta[4];
            y[2] = m->h/2 + m->random_delta[5];
            x[3] = -m->w/2 + m->random_delta[6];
            y[3] = m->h/2 + m->random_delta[7];
            
            /* First calculate the polygon's signed area A */
            
            a = 0.0;
            i1 = 1;
            for (i=0; i<4; i++) {
                a += x[i] * y[i1] - x[i1] * y[i];
                i1 = (i1 + 1) % 4;
            }
            a *= 0.5;
            
            /* Now calculate the centroid coordinates Cx and Cy */
            
            cx = cy = 0.0;
            i1 = 1;
            for (i=0; i<4; i++) {
                t = x[i]*y[i1] - x[i1]*y[i];
                cx += (x[i]+x[i1]) * t;
                cy += (y[i]+y[i1]) * t;
                i1 = (i1 + 1) % 4;
            }
            cx = cx / (6.0 * a);
            cy = cy / (6.0 * a);
            
                /*
                 
                 ofSetLineWidth(4);
                 ofSetColor(0,0,0);
                 ofLine(-m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1], m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3]);
                 ofLine(m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3], m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5]);
                 ofLine(m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5], -m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7]);
                 ofLine(-m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7], -m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1]);
                 ofFill();
                 
                 */
                
                // ofSetColor(255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added);
                // poly.setStrokeWidth(4);
                
                poly.setStrokeColor(ofColor(0,0,0));
                poly.setFillColor(ofColor(255*(m->grad_added - j)/m->grad_added, 255*(m->grad_added - j)/m->grad_added, 255*(m->grad_added - j)/m->grad_added));
                poly.lineTo(-m->w/2 + m->random_delta[0] - cx, -m->h/2 + m->random_delta[1] - cy);
                poly.lineTo(m->w/2 + m->random_delta[2] - cx, -m->h/2 + m->random_delta[3] - cy);
                poly.lineTo(m->w/2 + m->random_delta[4] - cx, m->h/2 + m->random_delta[5] - cy);
                poly.lineTo(-m->w/2 + m->random_delta[6] - cx, m->h/2 + m->random_delta[7] - cy);
                poly.close();
                poly.draw();

              //  ofPushMatrix();
            
                /*
                 ofSetColor(255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added);
                 ofBeginShape();
                 ofVertex(-m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1]);
                 ofVertex(m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3]);
                 ofVertex(m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5]);
                 ofVertex(-m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7]);
                 ofEndShape();
                 */
              //  ofPopMatrix();
                ofPopMatrix();
                ofPopStyle();
                
            
            screen->end();
        }
    }
}

void ofMorphRender::draw_bar(ofMorph m, int screen_i) {
    ofFbo *screen;
    float scaleH, posx, posy = CHEIGHT/2; //bars don't change the height position
    
    switch (screen_i) {
        case 0:
            scaleH = ofMap(m.x, kinect_width, 0, bars_min_width, bars_max_width);
            posx = ofMap(m.y, kinect_height, 0, 0, CWIDTH1);
            screen = screen1;
            break;
        case 1:
            scaleH = ofMap(m.y, kinect_height, 0, bars_min_width, bars_max_width);
            posx = ofMap(m.x, 0, kinect_width, 0, CWIDTH2);
            screen = screen2;
            break;
        case 2:
            scaleH = ofMap(m.x, 0, kinect_width, bars_min_width, bars_max_width);
            posx = ofMap(m.y, 0, kinect_height, 0, CWIDTH3);
            screen = screen3;
            break;
    }
    
    screen->begin();
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(0, 0, 0);
    ofFill();
    ofDrawRectangle(posx, posy, scaleH, CHEIGHT);
    ofPopStyle();
    screen->end();

}

void ofMorphRender::draw_spikes(ofMorph m, int screen_i) {
    float rx;
    ofFbo *screen;
    float scaleH, posx, posy = cHeight;
    
    switch (screen_i) {
        case 0:
            scaleH = ofMap(m.x, kinect_width, 0, 0.5, 1);
            posx = ofMap(m.y, kinect_height, 0, 0, CWIDTH1);
            rx = CWIDTH1;
            screen = screen1;
            break;
        case 1:
            scaleH = ofMap(m.y, kinect_height, 0, 0.5, 1);
            posx = ofMap(m.x, 0, kinect_width, 0, CWIDTH2);
            rx = CWIDTH2;

            screen = screen2;
            break;
        case 2:
            scaleH = ofMap(m.x, 0, kinect_width, 0.5, 1);
            posx = ofMap(m.y, 0, kinect_height, 0, CWIDTH3);
            
            rx = CWIDTH3;

            screen = screen3;
            break;
    }
    


    screen->begin();
    
    ofPushStyle();
    ofPushMatrix();
    //ofTranslate(posx, posy);
    ofTranslate(posx, posy);
    ofScale(scaleH, scaleH);
    ofSetColor(0,0,0);
    
    ofFill();

    
        ofBeginShape();
            ofVertex(-m.w/2 + m.random_delta[0], -m.h/2 + m.random_delta[1]);
            ofVertex(m.w/2 + m.random_delta[2], -m.h/2 + m.random_delta[3]);
            ofVertex(m.w/2 + m.random_delta[4], m.h/2 + m.random_delta[5]);
            ofVertex(-m.w/2 + m.random_delta[6], m.h/2 + m.random_delta[7]);
        ofEndShape();


    
        for (int i = 0; i < m.spikesPoints.size(); ++i) {
            for (int j = 0; j < m.spikesPoints[i].size(); ++j) {
                ofSetColor(0,0,0);
                ofFill();
                ofDrawTriangle(m.spikesPoints[i][j].esq1, m.spikesPoints[i][j].esq2,
                           m.spikesPoints[i][j].pt1/scaleH - (posx-rx/2)/scaleH, m.spikesPoints[i][j].pt2/scaleH - (posy-cHeight)/scaleH,
                           m.spikesPoints[i][j].dir1,m.spikesPoints[i][j].dir2);
            }
        }
    
    
    

    
    ofPopMatrix();
    ofPopStyle();
    

    screen->end();
    
    
}

void ofMorphRender::draw_gradient(ofMorph *m, int screen_i) {
    ofFbo *screen;
    ofPath poly;
    
    float scaleH, dir, posx, posy = cHeight;
    long long now = ofGetElapsedTimeMillis();

    if (now - m->last_time > gradient_time_frames) {
        cout << m->last_time << endl;
        m->dt += gradient_animation_speed/1000;
        if (m->dt > gradient_animation_max_time)
            m->dt = 0;

        gradient_data g;
        g.posx = m->x;
        g.posy = m->y;
        m->gradient_slices[m->grad_i] = g;
        m->grad_i = (m->grad_i + 1) % m->grad_max;
        m->last_time = now;
        if (m->grad_added < m->grad_max) {
            ++m->grad_added;
        }
    }

    if (screen_i == 0) {
        screen = screen1;
    } else if (screen_i ==1) {
        screen = screen2;
    } else {
        screen = screen3;
    }
    
    screen->begin();

    for (int j = 0; j < m->grad_added; ++j) {
        gradient_data g = m->gradient_slices[(m->grad_i + j) % m->grad_max];

        switch (screen_i) {
            case 0:
                scaleH = ofMap(g.posx, kinect_width, 0, gradient_min_width, gradient_max_width);
                posx = ofMap(g.posy, kinect_height, 0, 0, CWIDTH1);
                break;
            case 1:
                scaleH = ofMap(g.posy, kinect_height, 0, gradient_min_width, gradient_max_width);
                posx = ofMap(g.posx, 0, kinect_width, 0, CWIDTH2);
                break;
            case 2:
                scaleH = ofMap(g.posx, 0, kinect_width, gradient_min_width, gradient_max_width);
                posx = CWIDTH3-ofMap(g.posy, 0, kinect_width, 0, CWIDTH3);
                break;
        }
        
        ofPushStyle();
            ofPushMatrix();        
                ofTranslate(posx, posy);
                float s = scaleH - j*(gradient_change_per_level) + m->dt*(m->grad_added - j - 1)/m->grad_added;
                //float s = scaleH - j*(0.06);
                s = (s < 0)?0.5:s;
                ofScale(s, s);
                ofPushMatrix();
        
                /*
                
                ofSetLineWidth(4);
                ofSetColor(0,0,0);
                ofLine(-m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1], m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3]);
                ofLine(m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3], m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5]);
                ofLine(m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5], -m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7]);
                ofLine(-m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7], -m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1]);
                ofFill();
                 
                */
        
                // ofSetColor(255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added);
                // poly.setStrokeWidth(4);
        /*
                ofPolyline pl;
                pl.addVertex(-m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1]);
                pl.addVertex(m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3]);
                pl.addVertex(m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5]);
                pl.addVertex(-m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7]);
                ofPoint pt = pl.getCentroid2D();*/
        
        float x[4], y[4];
        float a, cx, cy, t;
        int i, i1;

        x[0] = -m->w/2 + m->random_delta[0];
        y[0] = -m->h/2 + m->random_delta[1];
        x[1] = m->w/2 + m->random_delta[2];
        y[1] = -m->h/2 + m->random_delta[3];
        x[2] = m->w/2 + m->random_delta[4];
        y[2] = m->h/2 + m->random_delta[5];
        x[3] = -m->w/2 + m->random_delta[6];
        y[3] = m->h/2 + m->random_delta[7];
        
        /* First calculate the polygon's signed area A */
        
        a = 0.0;
        i1 = 1;
        for (i=0; i<4; i++) {
            a += x[i] * y[i1] - x[i1] * y[i];
            i1 = (i1 + 1) % 4;
        }
        a *= 0.5;
        
        /* Now calculate the centroid coordinates Cx and Cy */
        
        cx = cy = 0.0;
        i1 = 1;
        for (i=0; i<4; i++) {
            t = x[i]*y[i1] - x[i1]*y[i];
            cx += (x[i]+x[i1]) * t;
            cy += (y[i]+y[i1]) * t;
            i1 = (i1 + 1) % 4;
        }
        cx = cx / (6.0 * a);
        cy = cy / (6.0 * a);
        
                poly.setStrokeColor(ofColor(0,0,0));
                poly.setFillColor(ofColor(255*(m->grad_added - j)/m->grad_added, 255*(m->grad_added - j)/m->grad_added, 255*(m->grad_added - j)/m->grad_added));
                poly.lineTo(-m->w/2 + m->random_delta[0] - cx, -m->h/2 + m->random_delta[1] - cy);
                poly.lineTo(m->w/2 + m->random_delta[2] - cx, -m->h/2 + m->random_delta[3] - cy);
                poly.lineTo(m->w/2 + m->random_delta[4] - cx, m->h/2 + m->random_delta[5] - cy);
                poly.lineTo(-m->w/2 + m->random_delta[6] - cx, m->h/2 + m->random_delta[7] - cy);
        poly.close();
                poly.draw();
        
                /*
                    ofSetColor(255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added, 255*(grad_added - j)/grad_added);
                    ofBeginShape();
                        ofVertex(-m->w/2 + m->random_delta[0], -m->h/2 + m->random_delta[1]);
                        ofVertex(m->w/2 + m->random_delta[2], -m->h/2 + m->random_delta[3]);
                        ofVertex(m->w/2 + m->random_delta[4], m->h/2 + m->random_delta[5]);
                        ofVertex(-m->w/2 + m->random_delta[6], m->h/2 + m->random_delta[7]);
                    ofEndShape();
                */
                ofPopMatrix();
            ofPopMatrix();
        ofPopStyle();

    }
    screen->end();
}


// morph add function, needs an unique ID and star X and Y position
void ofMorphRender::addMorph(float x, float y, int id){
    ofMorph *m = new ofMorph();
    m->setup(x, y, maxSpikesRandPoints, minNoiseCorner, maxNoiseCorner, blockWidth, blockHeight, spikeMinSize, spikeMaxSize, spike_angle, half_base, smooth_value);

    
    morphs.insert(make_pair(id, *m));
}

// morph delete function
void ofMorphRender::deleteMorph(int id){
        morphs.erase(id);
}

//update kinect Area
void ofMorphRender::setKinectArea(int _kinectWidth,int _kinectHeight){
    kinect_height = _kinectHeight;
    kinect_width = _kinectWidth;
    
}
