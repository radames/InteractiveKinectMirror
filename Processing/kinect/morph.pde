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
    if (this.x <= area_x) {
      this.x = floor(area_x);
    } else if (this.x + this.w >= area_x + area_w) {
      this.x = floor(area_x + area_w - this.w);
    }
    this.y += this.vy;
    if (this.y <= area_y) {
      this.y = floor(area_y);
    } else if (this.y + this.h >= area_y + area_h) {
      this.y = floor(area_y + area_h - this.h);
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
        s_x = (this.y - area_y)/(area_y - this.h);
        s_y = 1 - ((this.x - area_x)/(area_x - this.w));
        new_w = this.w + s_y*this.w;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_y*this.w, 384);
        popStyle();
        break;
      case 2:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = 1 - (this.x - area_x)/(area_x - this.w);
        new_w = this.w + s_x*this.w;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(area_x + new_w/2 + (area_x - new_w)*(1 - s_y), 192, new_w, 384);
        popStyle();
        break;
       case 3:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = (this.x - area_x)/(area_x - this.w);
        new_w = this.w + s_y*this.w;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(2*area_x + new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_y*this.w, 384);
        popStyle();
       break;
 
    }
  }

  void draw_square(int screen_i) { 
    float s_x, s_y, new_w, new_h;
    switch (screen_i) {
      case 1:
        s_x = (this.y - 384)/(384.0 - this.h);
        s_y = 1 - ((this.x - 341.0)/(341 - this.w));
        new_w = this.w + s_y*this.w;
        new_h = this.h + s_y*this.h;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_y*this.w, this.h + s_y*this.h);
        popStyle();
        break;
      case 2:
        s_x = (this.y - 384)/(384.0 - this.h);
        s_y = 1 - ((this.x - 341.0)/(341 - this.w));
        new_w = this.w + s_y*this.w;
        new_h = this.h + s_y*this.h;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_y*this.w, this.h + s_y*this.h);
        popStyle();
        break;
       case 3:
        s_x = (this.y - 384)/(384.0 - this.h);
        s_y = 1 - ((this.x - 341.0)/(341 - this.w));
        new_w = this.w + s_y*this.w;
        new_h = this.h + s_y*this.h;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(new_w/2 + (341 - new_w)*(1 - s_x), 192, this.w + s_y*this.w, this.h + s_y*this.h);
        popStyle();
        break;
 
    }
  }

  void draw_screen(int i) {
    switch(screen_type[i]) {
      case 0:
        draw_square(i+1);
        break;
      case 1:
        draw_line(i+1);
        break;
    }
  }

}
