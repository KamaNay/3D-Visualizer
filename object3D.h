#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "object.h"
#include <vector>
#include <QString>
#include <cmath>

struct Point3D {
    double x, y, z;
    Point3D(double X=0, double Y=0, double Z=0) : x(X), y(Y), z(Z) {}
};

class Object3D : public Object {
public:
    Object3D(QString name = "3DObject") : Object(name, POLYGON) {}

    void draw(QPainter *painter) override {
        Q_UNUSED(painter);
    }


    void addVertex(double x, double y, double z) {
        vertices.push_back(Point3D(x, y, z));
    }

    void addFace(const std::vector<int>& face) {
        faces.push_back(face);
    }

    // ---- Transformações ----
    void translate3D(double dx, double dy, double dz) {
        for (auto &v : vertices) {
            v.x += dx;
            v.y += dy;
            v.z += dz;
        }
    }

    void scale3D(double sx, double sy, double sz, const Point3D &center) {
        for (auto &v : vertices) {
            v.x = center.x + (v.x - center.x) * sx;
            v.y = center.y + (v.y - center.y) * sy;
            v.z = center.z + (v.z - center.z) * sz;
        }
    }

    void rotateX3D(double angle) {
        double rad = angle * M_PI / 180.0;
        double c = cos(rad), s = sin(rad);
        for (auto &v : vertices) {
            double y = v.y * c - v.z * s;
            double z = v.y * s + v.z * c;
            v.y = y; v.z = z;
        }
    }

    void rotateY3D(double angle) {
        double rad = angle * M_PI / 180.0;
        double c = cos(rad), s = sin(rad);
        for (auto &v : vertices) {
            double x = v.x * c + v.z * s;
            double z = -v.x * s + v.z * c;
            v.x = x; v.z = z;
        }
    }

    void rotateZ3D(double angle) {
        double rad = angle * M_PI / 180.0;
        double c = cos(rad), s = sin(rad);
        for (auto &v : vertices) {
            double x = v.x * c - v.y * s;
            double y = v.x * s + v.y * c;
            v.x = x; v.y = y;
        }
    }

    // ---- Projeção ortogonal ----
    std::vector<std::vector<Point>> projectOrthographic() const {
        std::vector<std::vector<Point>> projected;
        for (auto &face : faces) {
            std::vector<Point> poly;
            for (int idx : face) {
                const auto &v = vertices[idx - 1];
                poly.push_back(Point(v.x, -v.y)); // invertido para coordenadas Qt
            }
            projected.push_back(poly);
        }
        return projected;
    }

private:
    std::vector<Point3D> vertices;
    std::vector<std::vector<int>> faces;
};

#endif // OBJECT3D_H
