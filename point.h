#ifndef POINT_H
#define POINT_H

// Representa um ponto 2D
class Point {
private:
    double x, y;

public:
    Point(double x = 0, double y = 0) : x(x), y(y) {}
    double getX() const { return x; }
    double getY() const { return y; }

    void setX(double nx) { x = nx; }
    void setY(double ny) { y = ny; }
};

#endif
