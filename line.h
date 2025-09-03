#ifndef LINE_H
#define LINE_H

#include "object.h"

class Line : public Object {
public:
    Line(QString n, const Point &p1, const Point &p2) : Object(n, LINE) {
        points.push_back(p1);
        points.push_back(p2);
    }

    void draw(QPainter *painter) override {
        painter->drawLine(points[0].getX(), points[0].getY(),
                          points[1].getX(), points[1].getY());
    }
};

#endif
