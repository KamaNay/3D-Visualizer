#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "point.h"

class Viewport {
private:
    int xmin, xmax, ymin, ymax; // limites em pixels

public:
    Viewport(int xMin, int xMax, int yMin, int yMax)
        : xmin(xMin), xmax(xMax), ymin(yMin), ymax(yMax) {}

    // Getters
    int getXMin() const { return xmin; }
    int getXMax() const { return xmax; }
    int getYMin() const { return ymin; }
    int getYMax() const { return ymax; }

    // Redefinir viewport
    void resize(int xMin, int xMax, int yMin, int yMax) {
        xmin = xMin; xmax = xMax;
        ymin = yMin; ymax = yMax;
    }

    // Converte SCN [-1,1] para coordenadas da tela
    Point normalizedToViewport(const Point& p) const {
        double vx = (p.getX() + 1) / 2.0 * (xmax - xmin) + xmin;
        double vy = (1 - (p.getY() + 1) / 2.0) * (ymax - ymin) + ymin;
        return Point(vx, vy);
    }
};

#endif
