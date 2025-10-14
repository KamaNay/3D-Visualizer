#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "house.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // faz o frame ser o widget central da MainWindow (ocupa todo o espaço)
    // ⚠️ Certifique-se de que você não adicionou o frame em outro layout no .ui
    setCentralWidget(ui->frame);
    ui->frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->frame->setMinimumSize(400, 300); // opcional

    // Adiciona casas (mantenha como antes)
    ui->frame->addObject(new House("Casa1", 100, 250, 80, 80));
    ui->frame->addObject(new House("Casa2", 200, 250, 80, 80));
    ui->frame->addObject(new House("Casa3", 300, 250, 80, 80));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    Object* selected = nullptr;

    for (auto obj : ui->frame->getDisplayFile().getObjects()) {
        if (obj->isSelected()) {
            selected = obj;
            break;
        }
    }

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

    auto& window = ui->frame->getWindow();
    switch (event->key()) {
    case Qt::Key_W:  window.translate(0, -10); break;
    case Qt::Key_S:  window.translate(0, 10);  break;
    case Qt::Key_A:  window.translate(-10, 0); break;
    case Qt::Key_D:  window.translate(10, 0);  break;
    case Qt::Key_Z:  window.zoom(0.9);         break;
    case Qt::Key_X:  window.zoom(1.1);         break;
    case Qt::Key_C:  window.rotate(-5);        break;
    case Qt::Key_V:  window.rotate(5);         break;
    default: break;
    }

    ui->frame->update();
}
