#include "putbox.h"

PutBox::PutBox(int x0, int x1, int y0, int y1,
               int z0, int z1, float r, float g, float b, float a) {
    this->x0 = x0;
    this->x1 = x1;
    this->y0 = y0;
    this->y1 = y1;
    this->z0 = z0;
    this->z1 = z1;
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void PutBox::draw(Sculptor &escultor) {
    escultor.setColor(r, g, b, a);
    escultor.putBox(x0, x1, y0, y1, z0, z1);
}
