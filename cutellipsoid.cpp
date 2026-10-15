#include "cutellipsoid.h"

CutEllipsoid::CutEllipsoid(int x, int y, int z, int rx, int ry, int rz) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->rx = rx;
    this->ry = ry;
    this->rz = rz;
}

void CutEllipsoid::draw(Sculptor &escultor) {
    escultor.cutEllipsoid(x, y, z, rx, ry, rz);
}
