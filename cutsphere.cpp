#include "cutsphere.h"

CutSphere::CutSphere(int x, int y, int z, int radius) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = radius;
}

void CutSphere::draw(Sculptor &escultor) {
    escultor.cutSphere(x, y, z, radius);
}
