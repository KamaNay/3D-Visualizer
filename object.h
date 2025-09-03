#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include <QPainter>
#include <vector>
#include "point.h"

enum ObjectType {
    POINT,
    LINE,
    POLYGON
};

class Object {
protected:
    QString name;
    ObjectType type;
    std::vector<Point> points;

public:
    Object(QString n, ObjectType t) : name(n), type(t) {}
    virtual ~Object() {}

    QString getName() const { return name; }
    ObjectType getType() const { return type; }
    std::vector<Point> getPoints() const { return points; }

    virtual void draw(QPainter *painter) = 0;
};

#endif
