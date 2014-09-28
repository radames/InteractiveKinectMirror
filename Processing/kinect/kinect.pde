int screen_width = 1024;
int screen_height = 768;
float area_x = 341, area_y = 384, area_w = 341, area_h = 384;
// float screen1_x = 0, screen1_y = 0, screen1_w = 768, screen1_h = 960;
// float screen2_x = screen1_x + screen1_w, screen2_y = 0, screen2_w = 960, screen2_h = 960;
// float screen3_x = screen2_x + screen2_w, screen3_y = 0, screen3_w = 768, screen3_h = 960;
float screen1_x = 0, screen1_y = 0, screen1_w = 341, screen1_h = 384;
float screen2_x = screen1_x + screen1_w, screen2_y = 0, screen2_w = 341, screen2_h = 384;
float screen3_x = screen2_x + screen2_w, screen3_y = 0, screen3_w = 341, screen3_h = 384;
ArrayList<Morph> morphs = new ArrayList<Morph>();
int LINE = 1;
int SQ = 0;
int []screen_type = new int[3];

void setup () {
  size(screen_width, screen_height);
  frameRate(60);
  noCursor();

  morphs.add(new Morph(341, 384, 50, 80, color(0,0,0)));
  morphs.add(new Morph(541, 484, 50, 80, color(255,0,0)));

  screen_type[0] = SQ;
  screen_type[1] = SQ;
  screen_type[2] = SQ;

}

void draw_screens() {

  // Screen 1
  pushStyle();
  rect(screen1_x, screen1_y, screen1_w, screen1_h);
  popStyle();

  for (Morph m : morphs) {
  }

  // Screen 2
  pushStyle();
  rect(screen2_x, screen2_y, screen2_w, screen2_h);
  popStyle();


  for (Morph m : morphs) {
  }

  // Screen 3
  pushStyle();
  rect(screen3_x, screen3_y, screen3_w, screen3_h);
  popStyle();


  for (Morph m : morphs) {
  }



}

void draw() {

  // Draw area simulating kinect capture
  rect(area_x, area_y, area_w, area_h);

  draw_screens();

  for (Morph m : morphs) {
    m.update();
    m.draw();
    m.draw_screen(0);
    m.draw_screen(1);
     m.draw_screen(2);
  }
}

void keyPressed() {
  switch(key) {
    default:
        switch(keyCode) {
          case UP:
            for (Morph m: morphs) {
                if (m.vy >= 0) {
                    m.vy -= 5;
                }
            }
            break;
          case DOWN:
            for (Morph m: morphs) {
                if (m.vy <= 0) {
                    m.vy += 5;
                }
            }
            break;
           case LEFT:
            for (Morph m: morphs) {
                if (m.vx >= 0) {
                    m.vx -= 5;
                }
            }
            break;
          case RIGHT:
            for (Morph m: morphs) {
                if (m.vx <= 0) {
                    m.vx += 5;
                }
            }
            break;
 
        }
      break;
  }
}

void keyReleased() {
  switch(key) {
    case ' ':
      
      break;
    
    default:
      switch(keyCode) {
        case UP:
            for (Morph m: morphs) {
                m.vy = 0;
            }
            break;
        case DOWN:
            for (Morph m: morphs) {
                m.vy = 0;
            }
            break;
        case LEFT:
            for (Morph m: morphs) {
                m.vx = 0;
            }
            break;
        case RIGHT:
            for (Morph m: morphs) {
                m.vx = 0;
            }
            break;
      }
      break;
  }
}
