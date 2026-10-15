
#ifndef FIGURAGEOMETRICA_H
#define FIGURAGEOMETRICA_H

#include "sculptor.h"

class FiguraGeometrica {
public:
    virtual void draw(Sculptor &escultor) = 0;
    virtual ~FiguraGeometrica() = default;
};

#endif
