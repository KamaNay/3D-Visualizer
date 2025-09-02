#ifndef POINT_H
#define POINT_H

#include <QString>

class Point
{
public:
    QString name;
    double x, y;

    Point(QString name, double x, double y);
};

#endif // POINT_H
