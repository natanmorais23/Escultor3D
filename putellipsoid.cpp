#include "putellipsoid.h"

PutEllipsoid::PutEllipsoid(int x, int y, int z, int rx, int ry, int rz,
                           float r, float g, float b, float a) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->rx = rx;
    this->ry = ry;
    this->rz = rz;
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

void PutEllipsoid::draw(Sculptor &escultor) {
    escultor.setColor(r, g, b, a);
    escultor.putEllipsoid(x, y, z, rx, ry, rz);
}
