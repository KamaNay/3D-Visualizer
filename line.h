#ifndef LINE_H
#define LINE_H

#include <QVector>
#include "point.h"

class Line
{
public:
    QString name;
    QVector<Point> points;

    Line(QString name, Point p1, Point p2);
};

#endif // LINE_H
