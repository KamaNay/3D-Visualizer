#ifndef POLYGON_H
#define POLYGON_H

#include "object.h"
#include <QPolygon>

class Polygon : public Object {
public:
    Polygon(QString n, const std::vector<Point> &pts) : Object(n, POLYGON) {
        points = pts;
    }

    // Desenha um poligono na tela
    void draw(QPainter *painter) override {
        QPen pen = painter->pen();
        if (isSelected())
            painter->setPen(QPen(Qt::blue, 3));
        else
            painter->setPen(QPen(Qt::red, 2));

        QPolygon poly;
        for (auto &p : points) {
            poly << QPoint(p.getX(), p.getY());
        }
        painter->drawPolygon(poly);
    }
};

#endif
