#ifndef POINTOBJ_H
#define POINTOBJ_H

#include "object.h"

class PointObj : public Object {
public:
    PointObj(QString n, double x, double y) : Object(n, POINT) {
        points.push_back(Point(x, y));
    }

    void draw(QPainter *painter) override {
        painter->drawPoint(points[0].getX(), points[0].getY());
    }
};

#endif // POINTOBJ_H
