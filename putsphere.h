#ifndef PUTSPHERE_H
#define PUTSPHERE_H

#include "figurageometrica.h"

class PutSphere : public FiguraGeometrica {
private:
    int x, y, z, radius;
    float r, g, b, a;

public:
    PutSphere(int x, int y, int z, int radius,
              float r, float g, float b, float a);

    void draw(Sculptor &escultor) override;
};

#endif
