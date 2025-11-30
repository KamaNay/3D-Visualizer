#ifndef HOUSE_H
#define HOUSE_H

#include "object.h"
#include <QPainter>

// House: um objeto 2D simples representando uma casa como um polígono

class House : public Object {
public:
    House(const QString &name, double x = 100, double y = 250, double w = 80, double h = 80)
        : Object(name, POLYGON)
    {
        points.emplace_back(x, y);
        points.emplace_back(x, y + h);
        points.emplace_back(x + w/2, y + h + h*0.4);
        points.emplace_back(x + w, y + h);
        points.emplace_back(x + w, y);
    }

    void draw(QPainter *painter) override {
        Q_UNUSED(painter);
    }
};

#endif
