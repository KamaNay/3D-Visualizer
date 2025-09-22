#ifndef WINDOW_H
#define WINDOW_H

#include "point.h"
#include <cmath>

class Window {
private:
    double xmin, xmax, ymin, ymax; // limites da window
    double angle = 0.0;            // ângulo de rotação em graus

public:
    Window(double xMin, double xMax, double yMin, double yMax)
        : xmin(xMin), xmax(xMax), ymin(yMin), ymax(yMax) {}

    // Getters
    double getXMin() const { return xmin; }
    double getXMax() const { return xmax; }
    double getYMin() const { return ymin; }
    double getYMax() const { return ymax; }
    double getAngle() const { return angle; }

    // Pan (translação da window)
    void translate(double dx, double dy) {
        xmin += dx; xmax += dx;
        ymin += dy; ymax += dy;
    }

    // Zoom (escala)
    void zoom(double factor) {
        double cx = (xmin + xmax) / 2.0;
        double cy = (ymin + ymax) / 2.0;
        double w = (xmax - xmin) * factor / 2.0;
        double h = (ymax - ymin) * factor / 2.0;
        xmin = cx - w; xmax = cx + w;
        ymin = cy - h; ymax = cy + h;
    }

    // Rotação da window (ângulo acumulado)
    void rotate(double deltaAngle) {
        angle += deltaAngle;
    }

    // Converte ponto do mundo → SCN (considerando rotação da window)
    Point worldToNormalized(const Point& p) const {
        // Centro da window
        double cx = (xmin + xmax) / 2.0;
        double cy = (ymin + ymax) / 2.0;

        // Translada ponto para o centro da window
        double x = p.getX() - cx;
        double y = p.getY() - cy;

        // Aplica rotação inversa da window
        double rad = -angle * M_PI / 180.0;
        double xr = x * cos(rad) - y * sin(rad);
        double yr = x * sin(rad) + y * cos(rad);

        // Normaliza para [-1,1]
        double nx = xr / ((xmax - xmin) / 2.0);
        double ny = yr / ((ymax - ymin) / 2.0);

        return Point(nx, ny);
    }
};

#endif
