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

    void draw(QPainter *painter) override { Q_UNUSED(painter); }

    void addVertex(double x, double y, double z) {
        vertices.emplace_back(x, y, z);
    }

    void addFace(const std::vector<int>& face) {
        faces.push_back(face);
    }

    // retorna faces como listas de Point3D (sem projeção)
    std::vector<std::vector<Point3D>> getFaces3D() const {
        std::vector<std::vector<Point3D>> out;
        for (const auto &face : faces) {
            std::vector<Point3D> poly;
            poly.reserve(face.size());
            for (int idx : face) {
                poly.push_back(vertices[idx - 1]);
            }
            out.push_back(std::move(poly));
        }
        return out;
    }

    // transformações 3D
    void translate3D(double dx, double dy, double dz) override {
        for (auto &v : vertices) { v.x += dx; v.y += dy; v.z += dz; }
    }

    void scale3D(double sx, double sy, double sz, const Point3D &center) {
        for (auto &v : vertices) {
            v.x = center.x + (v.x - center.x) * sx;
            v.y = center.y + (v.y - center.y) * sy;
            v.z = center.z + (v.z - center.z) * sz;
        }
    }

    void rotateX3D(double angle) {
        Point3D c = getCenter();
        double rad = angle * M_PI/180.0;
        double co = cos(rad), si = sin(rad);
        for (auto &v : vertices) {
            double y = v.y - c.y, z = v.z - c.z;
            double ny = y * co - z * si;
            double nz = y * si + z * co;
            v.y = c.y + ny; v.z = c.z + nz;
        }
    }
    void rotateY3D(double angle) {
        Point3D c = getCenter();
        double rad = angle * M_PI/180.0;
        double co = cos(rad), si = sin(rad);
        for (auto &v : vertices) {
            double x = v.x - c.x, z = v.z - c.z;
            double nx = x * co + z * si;
            double nz = -x * si + z * co;
            v.x = c.x + nx; v.z = c.z + nz;
        }
    }
    void rotateZ3D(double angle) {
        Point3D c = getCenter();
        double rad = angle * M_PI/180.0;
        double co = cos(rad), si = sin(rad);
        for (auto &v : vertices) {
            double x = v.x - c.x, y = v.y - c.y;
            double nx = x * co - y * si;
            double ny = x * si + y * co;
            v.x = c.x + nx; v.y = c.y + ny;
        }
    }

    Point3D getCenter() const {
        double cx=0, cy=0, cz=0;
        if (vertices.empty()) return Point3D(0,0,0);
        for (const auto &v : vertices) { cx += v.x; cy += v.y; cz += v.z; }
        double n = (double)vertices.size();
        return Point3D(cx/n, cy/n, cz/n);
    }

    // Normaliza (centraliza e escala para um tamanho padrão)
    void normalizeModel() {
        if (vertices.empty()) return;
        double minX=1e9, minY=1e9, minZ=1e9;
        double maxX=-1e9, maxY=-1e9, maxZ=-1e9;
        for (auto &v : vertices) {
            minX = std::min(minX, v.x); minY = std::min(minY, v.y); minZ = std::min(minZ, v.z);
            maxX = std::max(maxX, v.x); maxY = std::max(maxY, v.y); maxZ = std::max(maxZ, v.z);
        }
        double cx = (minX + maxX) / 2.0;
        double cy = (minY + maxY) / 2.0;
        double cz = (minZ + maxZ) / 2.0;
        double size = std::max({maxX-minX, maxY-minY, maxZ-minZ});
        if (size <= 0.0) size = 1.0;
        double scale = 200.0 / size; // heurística (ajuste se quiser)
        for (auto &v : vertices) {
            v.x = (v.x - cx) * scale;
            v.y = (v.y - cy) * scale;
            v.z = (v.z - cz) * scale;
        }
    }

    double computeDepth(const Window &window) const override {
        double avgZ = 0.0;
        int count = 0;
        for (const auto &v : vertices) {
            double tx = v.x, ty = v.y, tz = v.z;
            ((Window&)window).applyCameraTransformTemp(tx, ty, tz);
            avgZ += tz;
            ++count;
        }
        return (count > 0) ? (avgZ / count) : 1e9;
    }

private:
    std::vector<Point3D> vertices;
    std::vector<std::vector<int>> faces;
};

#endif // OBJECT3D_H
