#ifndef CUTSPHERE_H
#define CUTSPHERE_H

#include "figurageometrica.h"

class CutSphere : public FiguraGeometrica {
private:
    int x, y, z, radius;

public:
    CutSphere(int x, int y, int z, int radius);
    void draw(Sculptor &escultor) override;
};

#endif
