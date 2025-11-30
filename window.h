#ifndef WINDOW_H
#define WINDOW_H

#include <cmath>
#include "point.h"

/*
 * Window atua como "câmera" integrada:
 * - mantém posição e orientação (camX, camY, camZ, pitch, yaw, roll)
 * - fornece worldToNormalized(X,Y,Z) que projeta pontos 3D para coordenadas normalizadas
 * - fornece applyCameraTransformTemp(X,Y,Z) que transforma world->camera space (sem alterar mundo)
 */

class Window {
private:
    double xmin, xmax, ymin, ymax;

public:
    Window(double xMin = -400, double xMax = 400, double yMin = -300, double yMax = 300)
        : xmin(xMin), xmax(xMax), ymin(yMin), ymax(yMax) {}

    double getXMin() const { return xmin; }
    double getXMax() const { return xmax; }
    double getYMin() const { return ymin; }
    double getYMax() const { return ymax; }

    // Campos da câmera
    double camX = 0.0, camY = 0.0, camZ = 600.0;
    double pitch = 0.0, yaw = 0.0, roll = 0.0;
    bool perspectiveMode = true;
    double fov = 90.0;
    double nearPlane = 1.0;
    double farPlane = 5000.0;

    // Gera matriz de visão simplificada (R e -R*C)
    void getViewMatrix(double M[4][4]) const {
        double cx = cos(pitch * M_PI/180.0);
        double sx = sin(pitch * M_PI/180.0);
        double cy = cos(yaw * M_PI/180.0);
        double sy = sin(yaw * M_PI/180.0);
        double cz = cos(roll * M_PI/180.0);
        double sz = sin(roll * M_PI/180.0);

        // combinação Rz * Rx * Ry
        double R00 = cy*cz;
        double R01 = -cy*sz;
        double R02 = sy;

        double R10 = sx*sy*cz + cx*sz;
        double R11 = -sx*sy*sz + cx*cz;
        double R12 = -sx*cy;

        double R20 = -cx*sy*cz + sx*sz;
        double R21 = cx*sy*sz + sx*cz;
        double R22 = cx*cy;

        M[0][0] = R00; M[0][1] = R01; M[0][2] = R02;
        M[1][0] = R10; M[1][1] = R11; M[1][2] = R12;
        M[2][0] = R20; M[2][1] = R21; M[2][2] = R22;

        M[0][3] = -(R00*camX + R01*camY + R02*camZ);
        M[1][3] = -(R10*camX + R11*camY + R12*camZ);
        M[2][3] = -(R20*camX + R21*camY + R22*camZ);

        M[3][0] = M[3][1] = M[3][2] = 0.0;
        M[3][3] = 1.0;
    }

    // Projeção: world (X,Y,Z) -> normalized coordinates (Xn, Yn)
    Point worldToNormalized(double X, double Y, double Z) const {
        double V[4][4];
        getViewMatrix(V);

        double xc = V[0][0]*X + V[0][1]*Y + V[0][2]*Z + V[0][3];
        double yc = V[1][0]*X + V[1][1]*Y + V[1][2]*Z + V[1][3];
        double zc = V[2][0]*X + V[2][1]*Y + V[2][2]*Z + V[2][3];

        // Se ortográfica simples: mapa linear
        if (!perspectiveMode) {
            double nx = (X - xmin) / (xmax - xmin) * 2.0 - 1.0;
            double ny = (Y - ymin) / (ymax - ymin) * 2.0 - 1.0;
            return Point(nx, ny);
        }

        // proteger divisão por zero
        if (zc >= -nearPlane) zc = -nearPlane;
        double f = 1.0 / tan((fov * 0.5) * M_PI/180.0);

        double xp = (f * xc) / -zc;
        double yp = (f * yc) / -zc;

        return Point(xp, yp);
    }

    Point worldToNormalized(const Point& p) const { return worldToNormalized(p.getX(), p.getY(), 0.0); }
    Point worldToNormalized(const Point& p, double z) const { return worldToNormalized(p.getX(), p.getY(), z); }


    /*
     * applyCameraTransformTemp:
     * transforma as coordenadas world (X,Y,Z) para camera-space (multiplica pela view matrix),
     * escrevendo o resultado em X,Y,Z. NÃO altera o mundo; usado para calcular profundidade.
     */

    void applyCameraTransformTemp(double &X, double &Y, double &Z) const {
        double V[4][4];
        getViewMatrix(V);

        double xc = V[0][0]*X + V[0][1]*Y + V[0][2]*Z + V[0][3];
        double yc = V[1][0]*X + V[1][1]*Y + V[1][2]*Z + V[1][3];
        double zc = V[2][0]*X + V[2][1]*Y + V[2][2]*Z + V[2][3];

        X = xc; Y = yc; Z = zc;
    }

    void moveForward(double d) {
        camZ -= d * cos(yaw*M_PI/180.0);
        camX += d * sin(yaw*M_PI/180.0);
    }
    void moveRight(double d) {
        camX += d * cos(yaw*M_PI/180.0);
        camZ += d * sin(yaw*M_PI/180.0);
    }
    void moveUp(double d) {
        camY += d;
    }
};

#endif
