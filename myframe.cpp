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

    // Add all points individually to display them
    df.addPoint(Point("P1", 10.0, 10.0));

    df.addLine(Line("L1", Point("P2", 20.0, 20.0), Point("P3", 80.0, 20.0)));

    QVector<Point> polyPoints = {
        Point("P4", 50.0, 30.0),
        Point("P5", 80.0, 30.0),
        Point("P6", 80.0, 65.0),
        Point("P7", 50.0, 65.0)
    };

    df.addPolygon(Polygon("Poly1", polyPoints));

    setMinimumSize(400, 400);
}

void MyFrame::paintEvent([[maybe_unused]] QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    double scaleX = width() / worldWidth;
    double scaleY = height() / worldHeight;

    // Desenhar pontos
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    for (const Point &p : df.points) {
        int px = static_cast<int>(p.x * scaleX);
        int py = static_cast<int>((worldHeight - p.y) * scaleY); // Inverter Y
        painter.drawEllipse(px - 3, py - 3, 6, 6);
        painter.drawText(px + 5, py + 5, p.name);
    }

    // Desenhar retas
    painter.setPen(Qt::blue);
    for (const Line &l : df.lines) {
        int x1 = static_cast<int>(l.points[0].x * scaleX);
        int y1 = static_cast<int>((worldHeight - l.points[0].y) * scaleY);
        int x2 = static_cast<int>(l.points[1].x * scaleX);
        int y2 = static_cast<int>((worldHeight - l.points[1].y) * scaleY);
        painter.drawLine(x1, y1, x2, y2);
        painter.drawText(x1 + 5, y1 - 5, l.name);
    }

    // Desenhar polígonos
    painter.setPen(Qt::green);
    painter.setBrush(QBrush(Qt::green, Qt::DiagCrossPattern));
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
