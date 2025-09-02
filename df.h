#ifndef DF_H
#define DF_H

#include <QVector>
#include "point.h"
#include "line.h"
#include "polygon.h"

class DF
{
public:
    QVector<Point> points;
    QVector<Line> lines;
    QVector<Polygon> polygons;

    void addPoint(Point p);
    void addLine(Line l);
    void addPolygon(Polygon p);
};

#endif // DF_H
