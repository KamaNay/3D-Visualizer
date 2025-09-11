#ifndef POINTOBJ_H
#define POINTOBJ_H

#include "object.h"

// Representa um ponto gráfico
class PointObj : public Object {
public:
    PointObj(QString n, double x, double y) : Object(n, POINT) {
        points.push_back(Point(x, y));
    }

    void draw(QPainter *painter) override {
        QPen pen = painter->pen();
        if (isSelected())
            painter->setPen(QPen(Qt::blue, 3));
        else
            painter->setPen(QPen(Qt::red, 2));

        painter->drawPoint(points[0].getX(), points[0].getY());
    }
};

#endif // POINTOBJ_H
