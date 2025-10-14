#ifndef HOUSE_H
#define HOUSE_H

#include "object.h"
#include <QPainter>

class House : public Object {
public:
    House(QString n, double cx, double cy, double width = 60, double height = 60)
        : Object(n, POLYGON)
    {
        double halfW = width / 2.0;
        double bodyH = height * 0.6;
        double roofH = height * 0.4;

        // Contorno original (guarda no vetor points, usado pelas transformações)
        points = {
            Point(cx - halfW, cy - bodyH),     // bottom-left
            Point(cx - halfW, cy),             // top-left
            Point(cx,         cy + roofH),     // topo do telhado
            Point(cx + halfW, cy),             // top-right
            Point(cx + halfW, cy - bodyH)      // bottom-right
        };
    }

    void draw(QPainter *painter) override {
        if (points.size() < 5) return;

        // Corpo e telhado (transformados)
        QPolygonF corpoTelhado;
        for (auto &p : points)
            corpoTelhado << QPointF(p.getX(), p.getY());

        painter->setPen(QPen(Qt::darkRed, 2));
        painter->setBrush(Qt::lightGray);
        painter->drawPolygon(corpoTelhado);

        // Cálculos baseados nos pontos atuais
        double minX = points[0].getX(), maxX = points[0].getX();
        double minY = points[0].getY(), maxY = points[0].getY();
        for (auto &p : points) {
            if (p.getX() < minX) minX = p.getX();
            if (p.getX() > maxX) maxX = p.getX();
            if (p.getY() < minY) minY = p.getY();
            if (p.getY() > maxY) maxY = p.getY();
        }

        double bodyH = maxY - minY;
        double width = maxX - minX;
        double cx = (minX + maxX) / 2.0;

        // --- Porta ---
        double doorW = width * 0.2;
        double doorH = bodyH * 0.36;
        double doorX = cx - doorW / 2.0;
        double doorY = minY;
        QRectF porta(doorX, doorY, doorW, doorH);
        painter->setBrush(Qt::darkGray);
        painter->drawRect(porta);

        // --- Janelas ---
        double winW = width * 0.2;
        double winH = bodyH * 0.2;
        double winY = minY + bodyH * 0.4;
        QRectF janelaEsq(cx - width * 0.35 - winW / 2.0, winY, winW, winH);
        QRectF janelaDir(cx + width * 0.35 - winW / 2.0, winY, winW, winH);
        painter->setBrush(Qt::cyan);
        painter->drawRect(janelaEsq);
        painter->drawRect(janelaDir);
    }
};

#endif // HOUSE_H
