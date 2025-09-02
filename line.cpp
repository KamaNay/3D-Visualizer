#include "line.h"

Line::Line(QString name, Point p1, Point p2) : name(name) {
    points.append(p1);
    points.append(p2);
}
