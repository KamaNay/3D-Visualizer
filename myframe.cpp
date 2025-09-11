#include "myframe.h"

MyFrame::MyFrame(QWidget *parent) : QFrame(parent) {}

void MyFrame::addObject(Object* obj) {
    df.addObject(obj);
    update();
}

// Desenha todos os objetos do DisplayFile
void MyFrame::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 5));
    df.drawAll(&painter);
}

// Detecta clique do mouse para selecionar objetos
void MyFrame::mousePressEvent(QMouseEvent *event) {
    QPoint click = event->pos();
    const int tol = 5; // tolerância em pixels para ponto/linha

    for (auto obj : df.getObjects()) {
        bool selected = false;
        auto pts = obj->getPoints();

        if (obj->getType() == POINT) {
            // Seleção de ponto
            Point p = pts[0];
            if (std::abs(click.x() - p.getX()) <= tol &&
                std::abs(click.y() - p.getY()) <= tol) {
                selected = true;
            }

        } else if (obj->getType() == LINE) {
            // Seleção de linha (distância ponto-segmento)
            Point p1 = pts[0], p2 = pts[1];
            double x0 = click.x(), y0 = click.y();
            double x1 = p1.getX(), y1 = p1.getY();
            double x2 = p2.getX(), y2 = p2.getY();

            // Projeção do ponto no segmento
            double A = x0 - x1;
            double B = y0 - y1;
            double C = x2 - x1;
            double D = y2 - y1;

            double dot = A * C + B * D;
            double len_sq = C * C + D * D;
            double param = (len_sq != 0) ? dot / len_sq : -1;

            double xx, yy;
            if (param < 0) {
                xx = x1; yy = y1;
            } else if (param > 1) {
                xx = x2; yy = y2;
            } else {
                xx = x1 + param * C;
                yy = y1 + param * D;
            }

            double dist = std::hypot(x0 - xx, y0 - yy);
            if (dist <= tol) {
                selected = true;
            }

        } else if (obj->getType() == POLYGON) {
            // Seleção de polígono (ponto dentro)
            QPolygon poly;
            for (auto &p : pts)
                poly << QPoint(p.getX(), p.getY());

            if (poly.containsPoint(click, Qt::OddEvenFill)) {
                selected = true;
            }
        }

        // Marca como selecionado
        if (selected) {
            for (auto o : df.getObjects()) o->setSelected(false);
            obj->setSelected(true);
            break;
        }
    }

    update();
}
