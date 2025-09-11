#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "line.h"
#include "polygon.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //Linha
    Object* l = new Line("L1", Point(150, 100), Point(300, 100));

    //Triangulo
    std::vector<Point> pts = {
        Point(220, 200),
        Point(250, 240),
        Point(220, 280)
    };
    Object* poly = new Polygon("Poly1", pts);

    //Pentagono
    std::vector<Point> pts2 = {
        Point(150, 150),
        Point(193, 185),
        Point(181, 241),
        Point(121, 241),
        Point(103, 185)
    };
    Object* poly2 = new Polygon("Poly2", pts2);

    ui->frame->setFixedSize(400, 300);
    ui->frame->setStyleSheet("background-color: white;");


    // Adiciona no frame (interface)
    ui->frame->addObject(l);
    ui->frame->addObject(poly);
    ui->frame->addObject(poly2);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // pega objeto selecionado
    Object* selected = nullptr;
    for (auto obj : ui->frame->getDisplayFile().getObjects()) {
        if (obj->isSelected()) {
            selected = obj;
            break;
        }
    }

    if (!selected) return;

    switch (event->key()) {
    case Qt::Key_Left:
        selected->translate(-10, 0);
        break;
    case Qt::Key_Right:
        selected->translate(10, 0);
        break;
    case Qt::Key_Up:
        selected->translate(0, -10);
        break;
    case Qt::Key_Down:
        selected->translate(0, 10);
        break;
    case Qt::Key_Plus: { // escala maior
        Point c = selected->getCenter();
        selected->scale(1.1, 1.1, c.getX(), c.getY());
        break;
    }
    case Qt::Key_Minus: { // escala menor
        Point c = selected->getCenter();
        selected->scale(0.9, 0.9, c.getX(), c.getY());
        break;
    }
    case Qt::Key_Q: { // rotaciona anti-horário
        Point c = selected->getCenter();
        selected->rotate(-10, c.getX(), c.getY());
        break;
    }
    case Qt::Key_E: { // rotaciona horário
        Point c = selected->getCenter();
        selected->rotate(10, c.getX(), c.getY());
        break;
    }
    }

    ui->frame->update();
}

