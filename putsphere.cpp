#include "putsphere.h"

PutSphere::PutSphere(int x, int y, int z, int radius,
                     float r, float g, float b, float a) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = radius;
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void PutSphere::draw(Sculptor &escultor) {
    escultor.setColor(r, g, b, a);
    escultor.putSphere(x, y, z, radius);
}
