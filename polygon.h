#ifndef POLYGON_H
#define POLYGON_H

#include <QVector>
#include "point.h"

class Polygon {
public:
    QString name;
    QVector<Point> points;

    Polygon(QString name, QVector<Point> points);
};

#endif // POLYGON_H
