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

    // ---- Calcula o centro geométrico do objeto ----
    Point3D getCenter() const {
        double cx = 0, cy = 0, cz = 0;
        for (auto &v : vertices) {
            cx += v.x;
            cy += v.y;
            cz += v.z;
        }
        int n = vertices.size();
        if (n == 0) return Point3D(0, 0, 0);
        return Point3D(cx / n, cy / n, cz / n);
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

    // ---- Rotaciona em torno do próprio centro ----
    void rotateX3D(double angle) {
        Point3D center = getCenter();
        double rad = angle * M_PI / 180.0;
        double c = cos(rad), s = sin(rad);
        for (auto &v : vertices) {
            double y = v.y - center.y;
            double z = v.z - center.z;
            double newY = y * c - z * s;
            double newZ = y * s + z * c;
            v.y = center.y + newY;
            v.z = center.z + newZ;
        }
    }

    void rotateY3D(double angle) {
        Point3D center = getCenter();
        double rad = angle * M_PI / 180.0;
        double c = cos(rad), s = sin(rad);
        for (auto &v : vertices) {
            double x = v.x - center.x;
            double z = v.z - center.z;
            double newX = x * c + z * s;
            double newZ = -x * s + z * c;
            v.x = center.x + newX;
            v.z = center.z + newZ;
        }
    }

    void rotateZ3D(double angle) {
        Point3D center = getCenter();
        double rad = angle * M_PI / 180.0;
        double c = cos(rad), s = sin(rad);
        for (auto &v : vertices) {
            double x = v.x - center.x;
            double y = v.y - center.y;
            double newX = x * c - y * s;
            double newY = x * s + y * c;
            v.x = center.x + newX;
            v.y = center.y + newY;
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

    // ---- Projeção perspectiva ----
    std::vector<std::vector<Point>> projectPerspective(double d = 500.0) const {
        std::vector<std::vector<Point>> projected;

        for (auto &face : faces) {
            std::vector<Point> poly;
            for (int idx : face) {
                const auto &v = vertices[idx - 1];

                // Evita divisão por zero (ponto atrás da câmera)
                double zc = (v.z == 0) ? 0.0001 : v.z;

                // Projeção perspectiva simples (câmera em Z = 0 olhando para -Z)
                double xp = (v.x * d) / (d - zc);
                double yp = (v.y * d) / (d - zc);

                poly.push_back(Point(xp, -yp)); // inverte Y para coordenadas Qt
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
