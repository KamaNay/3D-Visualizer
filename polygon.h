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
        QPolygon poly;
        for (auto &p : points) {
            poly << QPoint(p.getX(), p.getY());
        }
        painter->drawPolygon(poly);
    }
};

#endif
