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
    bool selected = false;

public:
    Object(QString n, ObjectType t) : name(n), type(t) {}
    virtual ~Object() {}

    QString getName() const { return name; }
    ObjectType getType() const { return type; }
    std::vector<Point> getPoints() const { return points; }

    bool isSelected() const { return selected; }
    void setSelected(bool s) { selected = s; }

    virtual void draw(QPainter *painter) = 0;

    Point getCenter() const {
        double sumX = 0, sumY = 0;
        for (auto &p : points) {
            sumX += p.getX();
            sumY += p.getY();
        }
        return Point(sumX / points.size(), sumY / points.size());
    }

    // Translação
    void translate(double dx, double dy) {
        for (auto &p : points) {
            p.setX(p.getX() + dx);
            p.setY(p.getY() + dy);
        }
    }

    // Escalonamento (em relação a um ponto fixo)
    void scale(double sx, double sy, double cx = 0, double cy = 0) {
        for (auto &p : points) {
            double nx = cx + (p.getX() - cx) * sx;
            double ny = cy + (p.getY() - cy) * sy;
            p.setX(nx);
            p.setY(ny);
        }
    }

    // Rotação (em torno de um ponto fixo)
    void rotate(double angleDeg, double cx = 0, double cy = 0) {
        double angleRad = angleDeg * M_PI / 180.0;
        double cosA = cos(angleRad);
        double sinA = sin(angleRad);

        for (auto &p : points) {
            double x = p.getX() - cx;
            double y = p.getY() - cy;
            double nx = x * cosA - y * sinA;
            double ny = x * sinA + y * cosA;
            p.setX(nx + cx);
            p.setY(ny + cy);
        }
    }
};

#endif
