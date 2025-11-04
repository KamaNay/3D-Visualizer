#include "myframe.h"
#include "window.h"
#include "viewport.h"
#include "house.h"
#include "object3D.h"
#include <cmath>
#include <QSizePolicy>

// ---- Cohen–Sutherland ----
enum OutCode { INSIDE = 0, LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8 };

static int computeOutCode(double x, double y, double xmin, double xmax, double ymin, double ymax) {
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

static bool clipLineCohenSutherland(Point &p1, Point &p2, double xmin, double xmax, double ymin, double ymax) {
    double x1 = p1.getX(), y1 = p1.getY();
    double x2 = p2.getX(), y2 = p2.getY();

    int out1 = computeOutCode(x1, y1, xmin, xmax, ymin, ymax);
    int out2 = computeOutCode(x2, y2, xmin, xmax, ymin, ymax);

    bool accept = false;
    while (true) {
        if (!(out1 | out2)) { accept = true; break; }
        else if (out1 & out2) { break; }
        else {
            double x, y;
            int out = out1 ? out1 : out2;
            if (out & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (out & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (out & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (out == out1) {
                x1 = x; y1 = y;
                out1 = computeOutCode(x1, y1, xmin, xmax, ymin, ymax);
            } else {
                x2 = x; y2 = y;
                out2 = computeOutCode(x2, y2, xmin, xmax, ymin, ymax);
            }
        }
    }

    if (accept) {
        p1 = Point(x1, y1);
        p2 = Point(x2, y2);
    }
    return accept;
}

// ---- MyFrame ----
MyFrame::MyFrame(QWidget *parent) : QFrame(parent) {
    window = new Window(-500, 500, -500, 500);
    viewport = new Viewport(0, width(), 0, height());

    setAutoFillBackground(true);
    setStyleSheet("background-color: white;");
    setMinimumSize(800, 600);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MyFrame::addObject(Object* obj) {
    df.addObject(obj);
    update();
}

void MyFrame::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // --- Define viewport com margens ---
    int marginX = static_cast<int>(width() * 0.05);
    int marginY = static_cast<int>(height() * 0.05);
    int vLeft = marginX;
    int vTop = marginY;
    int vWidth = width() - 2 * marginX;
    int vHeight = height() - 2 * marginY;
    viewport->resize(vLeft, vLeft + vWidth, vTop, vTop + vHeight);

    painter.save();
    painter.setClipRect(vLeft, vTop, vWidth, vHeight);

    for (auto obj : df.getObjects()) {

        auto obj3D = dynamic_cast<Object3D*>(obj);
        if (obj3D) {
            auto polys = obj3D->projectOrthographic();
            QColor edgeColor = obj->isSelected() ? Qt::blue : Qt::darkGray;
            QColor fillColor = obj->isSelected() ? QColor(173, 216, 230, 100) : QColor(200, 200, 200, 80);

            for (auto &polyPts : polys) {
                QPolygonF poly;
                for (auto &p : polyPts) {
                    Point pn = window->worldToNormalized(p);
                    Point pv = viewport->normalizedToViewport(pn);
                    poly << QPointF(pv.getX(), pv.getY());
                }

                painter.setPen(QPen(edgeColor, obj->isSelected() ? 2 : 1));
                painter.setBrush(fillColor);
                painter.drawPolygon(poly);
            }
            continue;
        }

        auto house = dynamic_cast<House*>(obj);
        if (house) {
            auto pts = house->getPoints();
            if (pts.size() < 5) continue;

            // Converte pontos da casa (transformados) para viewport
            QPolygonF poly;
            bool anyInside = false;
            for (auto &p : pts) {
                Point pn = window->worldToNormalized(p);
                if (pn.getX() >= -1.0 && pn.getX() <= 1.0 && pn.getY() >= -1.0 && pn.getY() <= 1.0)
                    anyInside = true;
                Point pv = viewport->normalizedToViewport(pn);
                poly << QPointF(pv.getX(), pv.getY());
            }
            if (!anyInside) continue;

            painter.setPen(obj->isSelected() ? QPen(Qt::blue, 3) : QPen(Qt::darkRed, 2));
            painter.setBrush(Qt::lightGray);
            painter.drawPolygon(poly);

            // --- desenha porta e janelas rotacionadas ---
            // base da casa = pts[0] (bottom-left original) e pts[4] (bottom-right original)
            Point baseL = pts[0], baseR = pts[4];
            double cxWorld = (baseL.getX() + baseR.getX()) / 2.0;
            double cyBase = (baseL.getY() + baseR.getY()) / 2.0;
            double dx = baseR.getX() - baseL.getX();
            double dy = baseR.getY() - baseL.getY();
            double angle = atan2(dy, dx); // orientação da base
            double widthWorld = std::hypot(dx, dy);

            double bodyHWorld = std::hypot(pts[1].getX() - pts[0].getX(), pts[1].getY() - pts[0].getY());

            // Unidade de direção: along base e perpendicular (up)
            double cosA = cos(angle);
            double sinA = sin(angle);
            double perpX = -sinA;  // Perpendicular "up" (sentido anti-horário relativo à base)
            double perpY = cosA;

            auto drawRotRectWorld = [&](double cxW, double cyW, double wW, double hW, const QColor &col){
                double cosA = cos(angle), sinA = sin(angle);
                // vértices locais (em coordenadas do mundo) do retângulo (em ordem)
                std::vector<Point> vworld;
                vworld.push_back(Point(cxW - wW/2, cyW));
                vworld.push_back(Point(cxW - wW/2, cyW + hW));
                vworld.push_back(Point(cxW + wW/2, cyW + hW));
                vworld.push_back(Point(cxW + wW/2, cyW));
                // rotaciona em torno do centro (cxW, cyW)
                QPolygonF polyR;
                for (auto &lp : vworld) {
                    // translate para centro, rotaciona, volta (usamos formula direta)
                    double lx = lp.getX() - cxW;
                    double ly = lp.getY() - cyW;
                    double rx = lx * cosA - ly * sinA;
                    double ry = lx * sinA + ly * cosA;
                    double xw = cxW + rx;
                    double yw = cyW + ry;
                    Point pn = window->worldToNormalized(Point(xw, yw));
                    Point pv = viewport->normalizedToViewport(pn);
                    polyR << QPointF(pv.getX(), pv.getY());
                }
                painter.setBrush(col);
                painter.drawPolygon(polyR);
            };

            // Função auxiliar para calcular cxW e cyW com offsets locais
            auto calcAnchor = [&](double offset_along, double offset_up) -> std::pair<double, double> {
                double cxW = cxWorld + offset_along * cosA + offset_up * perpX;
                double cyW = cyBase + offset_along * sinA + offset_up * perpY;
                return {cxW, cyW};
            };

            // Porta (centro na base, sobe do chão: offset_along=0, offset_up=0 para bottom)
            auto [cxDoor, cyDoor] = calcAnchor(0.0, 0.0);
            drawRotRectWorld(cxDoor, cyDoor, widthWorld * 0.2, bodyHWorld * 0.36, Qt::darkGray);

            // Janela esquerda (offset ao longo da base -0.3, up 0.3*bodyH para bottom da janela)
            auto [cxWinLeft, cyWinLeft] = calcAnchor(-widthWorld * 0.3, bodyHWorld * 0.3);
            drawRotRectWorld(cxWinLeft, cyWinLeft, widthWorld * 0.2, bodyHWorld * 0.2, Qt::cyan);

            // Janela direita (offset ao longo da base +0.3, up 0.3*bodyH para bottom da janela)
            auto [cxWinRight, cyWinRight] = calcAnchor(widthWorld * 0.3, bodyHWorld * 0.3);
            drawRotRectWorld(cxWinRight, cyWinRight, widthWorld * 0.2, bodyHWorld * 0.2, Qt::cyan);

            continue;
        }

        // demais objetos (mantém seu código original)
        auto pts = obj->getPoints();
        if (obj->getType() == LINE) {
            Point pn1 = window->worldToNormalized(pts[0]);
            Point pn2 = window->worldToNormalized(pts[1]);
            if (!clipLineCohenSutherland(pn1, pn2, -1.0, 1.0, -1.0, 1.0)) continue;
            Point pv1 = viewport->normalizedToViewport(pn1);
            Point pv2 = viewport->normalizedToViewport(pn2);
            painter.setPen(obj->isSelected() ? QPen(Qt::blue,3) : QPen(Qt::red,2));
            painter.drawLine(QPointF(pv1.getX(), pv1.getY()), QPointF(pv2.getX(), pv2.getY()));
        }
        else if (obj->getType() == POLYGON) {
            QPolygon poly;
            bool anyInside = false;
            for (auto &p : pts) {
                Point pn = window->worldToNormalized(p);
                if (pn.getX() >= -1.0 && pn.getX() <= 1.0 && pn.getY() >= -1.0 && pn.getY() <= 1.0)
                    anyInside = true;
                Point pv = viewport->normalizedToViewport(pn);
                poly << QPoint(static_cast<int>(pv.getX()), static_cast<int>(pv.getY()));
            }
            if (!anyInside) continue;
            painter.setPen(obj->isSelected() ? QPen(Qt::blue,3) : QPen(Qt::red,2));
            painter.drawPolygon(poly);
        }
        else if (obj->getType() == POINT) {
            Point pn = window->worldToNormalized(pts[0]);
            if (pn.getX() < -1.0 || pn.getX() > 1.0 || pn.getY() < -1.0 || pn.getY() > 1.0) continue;
            Point pv = viewport->normalizedToViewport(pn);
            painter.setPen(obj->isSelected() ? QPen(Qt::blue,3) : QPen(Qt::red,2));
            painter.drawPoint(QPoint(static_cast<int>(pv.getX()), static_cast<int>(pv.getY())));
        }
    }

    painter.restore();
    painter.setPen(QPen(Qt::black,1,Qt::DashLine));
    painter.drawRect(vLeft, vTop, vWidth, vHeight);

}

// ---- Mouse Selection ----
void MyFrame::mousePressEvent(QMouseEvent *event) {
    QPoint click = event->pos();
    const int tol = 5;

    // Converte clique para coordenadas normalizadas e depois mundo
    double nx = (double)(click.x() - viewport->getXMin()) /
                    (viewport->getXMax() - viewport->getXMin()) * 2.0 - 1.0;
    double ny = 1.0 - (double)(click.y() - viewport->getYMin()) /
                          (viewport->getYMax() - viewport->getYMin()) * 2.0;

    double wx = window->getXMin() + (nx + 1.0) / 2.0 * (window->getXMax() - window->getXMin());
    double wy = window->getYMin() + (ny + 1.0) / 2.0 * (window->getYMax() - window->getYMin());
    QPointF clickWorld(wx, wy);

    for (auto obj : df.getObjects()) {
        bool selected = false;

        // --- Caso Object3D ---
        if (auto obj3D = dynamic_cast<Object3D*>(obj)) {
            auto projected = obj3D->projectOrthographic();
            for (auto &face : projected) {
                QPolygonF poly;
                for (auto &p : face) {
                    // converte ponto do modelo para coordenadas do window
                    Point pn = window->worldToNormalized(p);
                    Point pv = viewport->normalizedToViewport(pn);
                    // reconverte viewport -> mundo (igual ao cliqueWorld)
                    double nxx = (pv.getX() - viewport->getXMin()) /
                                     (viewport->getXMax() - viewport->getXMin()) * 2.0 - 1.0;
                    double nyy = 1.0 - (pv.getY() - viewport->getYMin()) /
                                           (viewport->getYMax() - viewport->getYMin()) * 2.0;
                    double wxp = window->getXMin() + (nxx + 1.0) / 2.0 *
                                                         (window->getXMax() - window->getXMin());
                    double wyp = window->getYMin() + (nyy + 1.0) / 2.0 *
                                                         (window->getYMax() - window->getYMin());
                    poly << QPointF(wxp, wyp);
                }
                if (poly.containsPoint(clickWorld, Qt::OddEvenFill)) {
                    selected = true;
                    break;
                }
            }
        }

        // --- Caso 2D padrão ---
        else {
            auto pts = obj->getPoints();
            if (obj->getType() == POINT) {
                Point p = pts[0];
                if (std::abs(clickWorld.x() - p.getX()) <= tol &&
                    std::abs(clickWorld.y() - p.getY()) <= tol)
                    selected = true;
            }
            else if (obj->getType() == LINE) {
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
            }
            else if (obj->getType() == POLYGON) {
                QPolygonF poly;
                for (auto &p : pts)
                    poly << QPointF(p.getX(), p.getY());
                if (poly.containsPoint(clickWorld, Qt::OddEvenFill))
                    selected = true;
            }
        }

        // Seleciona o objeto e desmarca os outros
        if (selected) {
            for (auto o : df.getObjects()) o->setSelected(false);
            obj->setSelected(true);
            break;
        }
    }

    update();
}

