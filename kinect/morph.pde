public class Morph {
    int x, y, w, h, vx, vy;
    color c;

  Morph(int x, int y, int w, int h, color c) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
    this.c = c;
    this.vx = 0;
    this.vy = 0;
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
    rect(this.x, this.y, this.w, this.h);
    popStyle();
  }

  void draw_screen(int i) {
    switch(i) {
      case 1:
        float s = (this.x - 341)/341.0;
        pushStyle();
        fill(this.c);
        rectMode(CENTER);
        rect(341 + this.w/2.0 - this.y +341, 192, (1.5-s)*this.w, (1.5-s)*this.h);
        popStyle();
        break;
      default:
        break;

    }
  }

}
