#include "myframe.h"

MyFrame::MyFrame(QWidget *parent)
    : QFrame(parent)
{
    // Set background to black and enable auto-filling to clear artifacts
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);

    worldWidth = 100.0;
    worldHeight = 100.0;

    df.addPoint(Point("P1", 20.0, 15.0));
    df.addPoint(Point("P2", 40.0, 50.0));
    df.addPoint(Point("P3", 20.0, 80.0));
    df.addPoint(Point("P4", 3.0, 50.0));
    df.addPoint(Point("P5", 50.0, 30.0));
    df.addPoint(Point("P6", 80.0, 30.0));
    df.addPoint(Point("P7", 65.0, 60.0));

    QVector<Point> polyPoints = {
        Point("P1", 20.0, 15.0),
        Point("P2", 40.0, 50.0),
        Point("P3", 20.0, 80.0),
        Point("P4", 3.0, 50.0)
    };

    QVector<Point> polyPoints2 = {
        Point("P5", 50.0, 30.0),
        Point("P6", 80.0, 30.0),
        Point("P7", 65.0, 60.0),
    };

    df.addPolygon(Polygon("Poly1", polyPoints));
    df.addPolygon(Polygon("Poly2", polyPoints2));

    setMinimumSize(400, 400);
}

void MyFrame::paintEvent([[maybe_unused]] QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    double scaleX = width() / worldWidth;
    double scaleY = height() / worldHeight;

    for(const Point &p : df.points){
        int px = static_cast<int>(p.x * scaleX);
        int py = static_cast<int>((worldHeight - p.y) * scaleY); // Inverter Y
        painter.drawEllipse(px - 3, py - 3, 6, 6);
        QRect textRect(px - 5, py - 10, 50, 20); // Retângulo para o texto
        painter.drawText(textRect, Qt::AlignCenter, p.name);
    }

    // Desenhar polígonos
    painter.setPen(Qt::blue);
    for (const Polygon &poly : df.polygons) {
        QPolygonF qpoly;
        for (const Point &p : poly.points) {
            int px = static_cast<int>(p.x * scaleX);
            int py = static_cast<int>((worldHeight - p.y) * scaleY);
            qpoly << QPointF(px, py);
        }
        painter.drawPolygon(qpoly);
        // Desenhar nome no primeiro ponto
        int px = static_cast<int>(poly.points[0].x * scaleX);
        int py = static_cast<int>((worldHeight - poly.points[0].y) * scaleY);
        painter.drawText(px + 5, py + 12, poly.name);
    }
}
