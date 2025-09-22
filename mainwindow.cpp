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

    // Configurações visuais do frame
    ui->frame->setFixedSize(600, 400);
    ui->frame->setStyleSheet("background-color: white;");


    // Adiciona no frame
    ui->frame->addObject(l);
    ui->frame->addObject(poly);
    ui->frame->addObject(poly2);

}

MainWindow::~MainWindow() {
    delete ui;
}

// Captura teclas para mover, escalar ou rotacionar objetos selecionados
void MainWindow::keyPressEvent(QKeyEvent *event) {
    Object* selected = nullptr;

    // Procura o objeto selecionado
    for (auto obj : ui->frame->getDisplayFile().getObjects()) {
        if (obj->isSelected()) {
            selected = obj;
            break;
        }
    }

    // --- atalhos para objetos ---
    if (selected) {
        switch (event->key()) {
        case Qt::Key_Left:  selected->translate(-10, 0); break;
        case Qt::Key_Right: selected->translate(10, 0); break;
        case Qt::Key_Up:    selected->translate(0, -10); break;
        case Qt::Key_Down:  selected->translate(0, 10); break;
        case Qt::Key_Plus: {
            Point c = selected->getCenter();
            selected->scale(1.1, 1.1, c.getX(), c.getY());
            break;
        }
        case Qt::Key_Minus: {
            Point c = selected->getCenter();
            selected->scale(0.9, 0.9, c.getX(), c.getY());
            break;
        }
        case Qt::Key_Q: {
            Point c = selected->getCenter();
            selected->rotate(-10, c.getX(), c.getY());
            break;
        }
        case Qt::Key_E: {
            Point c = selected->getCenter();
            selected->rotate(10, c.getX(), c.getY());
            break;
        }
        }
    }

    // --- atalhos para window ---
    auto& window = ui->frame->getWindow();

    switch (event->key()) {
    case Qt::Key_W:  window.translate(0, -10); break;  // mover para cima
    case Qt::Key_S:  window.translate(0, 10);  break;  // mover para baixo
    case Qt::Key_A:  window.translate(-10, 0); break;  // mover para esquerda
    case Qt::Key_D:  window.translate(10, 0);  break;  // mover para direita
    case Qt::Key_Z:  window.zoom(0.9);         break; // zoom in
    case Qt::Key_X:  window.zoom(1.1);         break; // zoom out
    case Qt::Key_C:  window.rotate(-5);        break;  // rotaciona anti-horário
    case Qt::Key_V:  window.rotate(5);         break;  // rotaciona horário
    default: break;
    }

    ui->frame->update();
}

