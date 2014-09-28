int LINE = 1;
int SQ = 0;

public class Morph {
    int x, y, w, h, vx, vy;
    int type;
    color c;

  Morph(int x, int y, int w, int h, color c) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.c = c;
    this.vx = 0;
    this.vy = 0;
    this.type = LINE;
  }

  void update() {
    this.x += this.vx;
    if (this.x <= 341) {
      this.x = 341;
    } else if (this.x + this.w >= 682) {
      this.x = 682 - this.w;
    }
    this.y += this.vy;
    if (this.y <= 384) {
      this.y = 384;
    } else if (this.y + this.h >= 768) {
      this.y = 768 - this.h;
    }

  }

  void draw() {
    pushStyle();
    fill(this.c);
    rectMode(CORNER);
    rect(this.x, this.y, this.w, this.h);
    popStyle();
  }

  void draw_line(int screen_i) { 
    float s_x, s_y, new_w;
    switch (screen_i) {
      case 1:
        s_x = (this.y - 384)/(384.0 - this.h);
        s_y = 1 - ((this.x - 341.0)/(341 - this.w));
        new_w = this.w + s_y*this.w;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_y*this.w, 384);
        popStyle();
        break;
      case 2:
        s_x = (this.y - 384)/(384.0 - this.h);
        s_y = 1 - ((this.x - 341.0)/(341 - this.w));
        new_w = this.w + s_y*this.w;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(341 + new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_x*this.w, 384);
        popStyle();
        break;
 
    }
  }

  void draw_square(int screen_i) { 
    switch (screen_i) {
      case 1:
        float s_x = (this.y - 384)/(384.0 - this.h);
        float s_y = 1 - ((this.x - 341.0)/(341 - this.w));
        float new_w = this.w + s_y*this.w;
        float new_h = this.h + s_y*this.h;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_y*this.w, this.h + s_y*this.h);
        popStyle();
        break;
    }
  }

  void draw_screen(int i) {
    switch(this.type) {
      case 0:
        draw_square(i);
        break;
      case 1:
        draw_line(i);
        break;
    }
  }

}
