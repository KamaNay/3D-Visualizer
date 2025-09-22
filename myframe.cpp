#include "myframe.h"
#include "window.h"
#include "viewport.h"

MyFrame::MyFrame(QWidget *parent) : QFrame(parent) {
    // Define uma window inicial (coordenadas do mundo)
    window = new Window(0, 400, 0, 300);
    // Define uma viewport (em pixels, mesma do frame inicialmente)
    viewport = new Viewport(0, width(), 0, height());
}

void MyFrame::addObject(Object* obj) {
    df.addObject(obj);
    update();
}

// Desenha todos os objetos do DisplayFile
void MyFrame::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // Para cada objeto no DisplayFile
    for (auto obj : df.getObjects()) {
        auto pts = obj->getPoints();
        std::vector<QPoint> transformed;

        // Converte mundo → SCN → viewport
        for (auto &p : pts) {
            Point pn = window->worldToNormalized(p);
            Point pv = viewport->normalizedToViewport(pn);
            transformed.push_back(QPoint(pv.getX(), pv.getY()));
        }

        // Ajusta cor dependendo da seleção
        if (obj->isSelected())
            painter.setPen(QPen(Qt::blue, 3)); // selecionado: azul mais grosso
        else
            painter.setPen(QPen(Qt::red, 2));  // não selecionado: vermelho normal

        // Desenha conforme o tipo do objeto
        if (obj->getType() == POINT) {
            painter.drawPoint(transformed[0]);

        } else if (obj->getType() == LINE) {
            painter.drawLine(transformed[0], transformed[1]);

        } else if (obj->getType() == POLYGON) {
            QPolygon poly;
            for (auto &pt : transformed)
                poly << pt;
            painter.drawPolygon(poly);
        }
    }
}

// Detecta clique do mouse para selecionar objetos
void MyFrame::mousePressEvent(QMouseEvent *event) {
    QPoint click = event->pos();
    const int tol = 5; // tolerância em pixels

    // Converte clique da viewport → SCN
    double nx = (double)(click.x() - viewport->getXMin()) /
                    (viewport->getXMax() - viewport->getXMin()) * 2.0 - 1.0;

    double ny = 1.0 - (double)(click.y() - viewport->getYMin()) /
                          (viewport->getYMax() - viewport->getYMin()) * 2.0;

    // Converte SCN → mundo
    double wx = window->getXMin() + (nx + 1.0) / 2.0 * (window->getXMax() - window->getXMin());
    double wy = window->getYMin() + (ny + 1.0) / 2.0 * (window->getYMax() - window->getYMin());

    QPointF clickWorld(wx, wy);

    for (auto obj : df.getObjects()) {
        bool selected = false;
        auto pts = obj->getPoints();

        if (obj->getType() == POINT) {
            Point p = pts[0];
            if (std::abs(clickWorld.x() - p.getX()) <= tol &&
                std::abs(clickWorld.y() - p.getY()) <= tol) {
                selected = true;
            }

        } else if (obj->getType() == LINE) {
            Point p1 = pts[0], p2 = pts[1];
            double x0 = clickWorld.x(), y0 = clickWorld.y();
            double x1 = p1.getX(), y1 = p1.getY();
            double x2 = p2.getX(), y2 = p2.getY();

            double A = x0 - x1, B = y0 - y1;
            double C = x2 - x1, D = y2 - y1;
            double dot = A*C + B*D;
            double len_sq = C*C + D*D;
            double param = (len_sq != 0) ? dot / len_sq : -1;

            double xx, yy;
            if (param < 0)      { xx = x1; yy = y1; }
            else if (param > 1) { xx = x2; yy = y2; }
            else                { xx = x1 + param*C; yy = y1 + param*D; }

            double dist = std::hypot(x0 - xx, y0 - yy);
            if (dist <= tol) selected = true;

        } else if (obj->getType() == POLYGON) {
            // Verifica se ponto está dentro do polígono (em coordenadas do mundo)
            QPolygonF poly;
            for (auto &p : pts)
                poly << QPointF(p.getX(), p.getY());
            if (poly.containsPoint(clickWorld, Qt::OddEvenFill))
                selected = true;
        }

        if (selected) {
            for (auto o : df.getObjects()) o->setSelected(false);
            obj->setSelected(true);
            break;
        }
    }

    update();
}
