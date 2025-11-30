#ifndef OBJECT_H
#define OBJECT_H

#include <QString>
#include <QPainter>
#include <vector>
#include <array>
#include <cmath>
#include "point.h"
#include "window.h"

enum ObjectType {
    POINT,
    LINE,
    POLYGON
};

// Definição de matriz 3x3 (para transformações 2D existentes)
using Matrix3x3 = std::array<std::array<double, 3>, 3>;

class Object {
protected:
    QString name;
    ObjectType type;
    std::vector<Point> points;
    bool selected = false;

    // profundidade do objeto (plano z)
    double z = 0.0;

    // Aplica matriz a um ponto 2D
    Point applyMatrix(const Matrix3x3& M, const Point& p) const {
        double x = p.getX();
        double y = p.getY();
        double nx = M[0][0]*x + M[0][1]*y + M[0][2]*1;
        double ny = M[1][0]*x + M[1][1]*y + M[1][2]*1;
        return Point(nx, ny);
    }

    // Multiplica duas matrizes 3x3
    Matrix3x3 multiplyMatrix(const Matrix3x3& A, const Matrix3x3& B) const {
        Matrix3x3 R{};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[i][j] = 0;
                for (int k = 0; k < 3; k++) {
                    R[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return R;
    }

    // Aplica matriz a todos os pontos 2D
    void applyToPoints(const Matrix3x3& M) {
        for (auto &p : points) {
            p = applyMatrix(M, p);
        }
    }

public:
    Object(QString n, ObjectType t) : name(n), type(t) {}
    virtual ~Object() {}

    QString getName() const { return name; }
    ObjectType getType() const { return type; }
    std::vector<Point> getPoints() const { return points; }

    bool isSelected() const { return selected; }
    void setSelected(bool s) { selected = s; }

    virtual void draw(QPainter *painter) = 0;

    // Z (profundidade) do objeto plano
    double getZ() const { return z; }
    void setZ(double newZ) { z = newZ; }

    // Calcula o centro geométrico 2D do objeto
    Point getCenter() const {
        double sumX = 0, sumY = 0;
        if (points.empty()) return Point(0,0);
        for (auto &p : points) {
            sumX += p.getX();
            sumY += p.getY();
        }
        return Point(sumX / points.size(), sumY / points.size());
    }

    // Translação 2D
    void translate(double dx, double dy) {
        Matrix3x3 T = {{
            {1, 0, dx},
            {0, 1, dy},
            {0, 0, 1}
        }};
        applyToPoints(T);
    }

    // Translação 3D (para objetos 2D: move X,Y nos pontos e Z no valor do objeto)
    virtual void translate3D(double dx, double dy, double dz) {
        translate(dx, dy);
        z += dz;
    }

    // Escala em torno de um ponto (cx, cy) em 2D
    void scale(double sx, double sy, double cx = 0, double cy = 0) {
        Matrix3x3 T1 = {{ {1,0,-cx}, {0,1,-cy}, {0,0,1} }};
        Matrix3x3 S  = {{ {sx,0,0}, {0,sy,0}, {0,0,1} }};
        Matrix3x3 T2 = {{ {1,0,cx}, {0,1,cy}, {0,0,1} }};

        Matrix3x3 M = multiplyMatrix(T2, multiplyMatrix(S, T1));
        applyToPoints(M);
    }

    // Rotação em torno de um ponto (cx, cy) em 2D
    void rotate(double angleDeg, double cx = 0, double cy = 0) {
        double angleRad = angleDeg * M_PI / 180.0;
        double cosA = cos(angleRad);
        double sinA = sin(angleRad);

        Matrix3x3 T1 = {{ {1,0,-cx}, {0,1,-cy}, {0,0,1} }};
        Matrix3x3 R  = {{ {cosA,-sinA,0}, {sinA,cosA,0}, {0,0,1} }};
        Matrix3x3 T2 = {{ {1,0,cx}, {0,1,cy}, {0,0,1} }};

        Matrix3x3 M = multiplyMatrix(T2, multiplyMatrix(R, T1));
        applyToPoints(M);
    }

    virtual double computeDepth(const Window &window) const {
        double avgZ = 0.0;
        int count = 0;
        for (const auto &p : points) {
            double wx = p.getX();
            double wy = p.getY();
            double wz = z;
            double tx = wx, ty = wy, tz = wz;
            ((Window&)window).applyCameraTransformTemp(tx, ty, tz);
            avgZ += tz;
            ++count;
        }
        return (count > 0) ? (avgZ / count) : 1e9;
    }
};

#endif
