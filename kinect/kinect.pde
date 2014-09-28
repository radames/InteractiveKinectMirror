int screen_width = 1024;
int screen_height = 768;
ArrayList<Morph> morphs = new ArrayList<Morph>();;

void setup () {
  size(screen_width, screen_height);
  frameRate(60);
  //  smooth(2);
  noCursor();

  morphs.add(new Morph(341, 384, 50, 80, color(0,0,0)));
  morphs.add(new Morph(541, 484, 50, 80, color(255,0,0)));

}

void draw_screens() {

  // Screen 1
  pushStyle();
  rect(0, 0, 341.0, 384.0);
  popStyle();

  for (Morph m : morphs) {
  }

  // Screen 2
  pushStyle();
  rect(341, 0, 341.0, 384.0);
  popStyle();


  for (Morph m : morphs) {
  }

  // Screen 3
  pushStyle();
  rect(682, 0, 341.0, 384.0);
  popStyle();


  for (Morph m : morphs) {
  }



}

void draw() {

  rect(341.0, 384.0, 341.0, 384.0);

  draw_screens();

  for (Morph m : morphs) {
    m.update();
    m.draw();
    m.draw_screen(1);
    m.draw_screen(2);
     m.draw_screen(3);
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
