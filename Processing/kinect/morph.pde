public class Morph {
    float x, y, x_m, y_m, w, h, vx, vy;
    float [][] verts;
    int type;
    color c;

  Morph(float x, float y, float w, float h, color c, int type) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.x_m = this.x + this.w/2;
    this.y_m = this.y + this.h/2;
    this.h = h;
    this.c = c;
    this.vx = 0;
    this.vy = 0;
    this.type = type;
    init();
  }

  void init() {
    switch(this.type) {
        case 2:
          this.verts = new float[4][2];
          this.verts[0][0] = 5*random(-1, 1);
          this.verts[0][1] = 5*random(-1, 1);
          this.verts[1][0] = 5*random(-1, 1);
          this.verts[1][1] = 5*random(-1, 1);
          this.verts[2][0] = 5*random(-1, 1);
          this.verts[2][1] = 5*random(-1, 1);
          this.verts[3][0] = 5*random(-1, 1);
          this.verts[3][1] = 5*random(-1, 1);
          break;
        default:
          break;
    }
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
        rect(new_w/2 + (screen1_w - new_w)*(1 - s_x), 192, this.w + s_y*this.w, screen3_h);
        popStyle();
        break;
      case 2:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = 1 - (this.x - area_x)/(area_x - this.w);
        new_w = this.w + s_x*this.w;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(screen2_x + new_w/2 + (screen2_w - new_w)*(1 - s_y), 192, new_w, screen3_h);
        popStyle();
        break;
       case 3:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = (this.x - area_x)/(area_x - this.w);
        new_w = this.w + s_y*this.w;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(screen3_x + new_w/2 + (screen3_w - new_w)*(1 - s_x), 192, this.w + s_y*this.w, screen3_h);
        popStyle();
       break;

    }
  }

  void draw_square(int screen_i) { 
    float s_x, s_y, new_w, new_h;
    switch (screen_i) {
      case 1:
        s_x = (this.y - area_y)/(area_y - this.h);
        s_y = 1 - ((this.x - area_x)/(area_x - this.w));
        new_w = this.w + s_y*this.w;
        new_h = this.h + s_y*this.h;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(screen1_x + new_w/2 + (screen1_w - new_w)*(1 - s_x), 192, this.w + s_y*this.w, this.h + s_y*this.h);
        popStyle();
        break;
      case 2:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = 1 - ((this.x - area_x)/(area_x - this.w));
        new_w = this.w + s_x*this.w;
        new_h = this.h + s_x*this.h;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(screen2_x + new_w/2 + (screen2_w - new_w)*(1 - s_y), 192, new_w, new_h);
        popStyle();
        break;
       case 3:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = (this.x - area_x)/(area_x - this.w);
        new_w = this.w + s_y*this.w;
        new_h = this.h + s_y*this.h;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(screen3_x + new_w/2 + (screen3_w - new_w)*(1 - s_x), 192, this.w + s_y*this.w, this.h + s_y*this.h);
        popStyle();
        break;
 
    }
  }

  void draw_tunnel(int screen_i) {
    float xs_m, ys_m, s_x, s_y, new_w, new_h;
    switch (screen_i) {
      case 1:
        xs_m = map(this.y - area_h, 0, area_h, screen1_w, 0);
        ys_m = map(this.x - area_w, 0, area_w, 0, 1);
        s_x = (this.y - area_y)/(area_y - this.h);
        s_y = 2 - ((this.x - area_x)/(area_x - this.w));
        new_w = this.w + s_y*this.w;
        new_h = this.h + s_y*this.h;
        screen1.pushStyle();
        screen1.fill(this.c);

       screen1.pushMatrix();
       screen1.translate(xs_m, 192);
       screen1.scale(1 + ys_m, 1 + ys_m);
        screen1.beginShape();
        //screen1.ellipse(x_m, y_m, 10, 10);
        screen1.vertex(this.w/2,this.h/2);
        screen1.vertex(this.w/2,-this.h/2);
        screen1.vertex(-this.w/2,-this.h/2);
        screen1.vertex(-this.w/2,this.h/2);
       screen1.endShape(CLOSE);
        screen1.popMatrix();
        screen1.popStyle();
        break;
      case 2:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = 1 - ((this.x - area_x)/(area_x - this.w));
        new_w = this.w + s_x*this.w;
        new_h = this.h + s_x*this.h;
        screen2.pushStyle();
        screen2.fill(this.c);
        screen2.rectMode(CENTER);
        screen2.rect(new_w/2 + (screen2_w - new_w)*(1 - s_y), 192, new_w, new_h);
        screen2.popStyle();
        break;
       case 3:
        s_x = 1 - (this.y - area_y)/(area_y - this.h);
        s_y = (this.x - area_x)/(area_x - this.w);
        new_w = this.w + s_y*this.w;
        new_h = this.h + s_y*this.h;
        screen3.pushStyle();
        screen3.fill(this.c);
        screen3.rectMode(CENTER);
        screen3.rect(new_w/2 + (screen3_w - new_w)*(1 - s_x), 192, this.w + s_y*this.w, this.h + s_y*this.h);
        screen3.popStyle();
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
      case 2:
        draw_tunnel(i+1);
        break;
 
    }
  }

}
