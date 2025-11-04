#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objreader.h"
#include "object3D.h"
#include "house.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // faz o frame ser o widget central da MainWindow (ocupa todo o espaço)
    setCentralWidget(ui->frame);
    ui->frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->frame->setMinimumSize(400, 300);

    // Adiciona casas (mantenha como antes)
    ui->frame->addObject(new House("Casa1", 100, 250, 80, 80));
    ui->frame->addObject(new House("Casa2", 200, 250, 80, 80));
    ui->frame->addObject(new House("Casa3", 300, 250, 80, 80));

    loadOBJModel("C:/Users/andre/Documents/UTFPR/CG/AP/Charizard.obj");
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

    if (auto obj3D = dynamic_cast<Object3D*>(selected)) {
        Point3D center3D(0, 0, 0); // você pode depois calcular o centro real do modelo se quiser

        switch (event->key()) {
        case Qt::Key_I: obj3D->rotateX3D(-10); break;  // Rotaciona para cima
        case Qt::Key_K: obj3D->rotateX3D(10);  break;  // Rotaciona para baixo
        case Qt::Key_J: obj3D->rotateY3D(-10); break;  // Rotaciona para esquerda
        case Qt::Key_L: obj3D->rotateY3D(10);  break;  // Rotaciona para direita
        case Qt::Key_U: obj3D->rotateZ3D(-10); break;  // Roll antihorário
        case Qt::Key_O: obj3D->rotateZ3D(10);  break;  // Roll horário
        case Qt::Key_Up: obj3D->translate3D(0, 10, 0); break;   // Move para cima
        case Qt::Key_Down: obj3D->translate3D(0, -10, 0); break;  // Move para baixo
        case Qt::Key_Left: obj3D->translate3D(-10, 0, 0); break;  // Esquerda
        case Qt::Key_Right: obj3D->translate3D(10, 0, 0); break;   // Direita
        case Qt::Key_Plus:
            obj3D->scale3D(1.1, 1.1, 1.1, center3D);
            break;
        case Qt::Key_Minus:
            obj3D->scale3D(0.9, 0.9, 0.9, center3D);
            break;
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

void MainWindow::loadOBJModel(const QString &filePath) {
    Object3D* model = OBJReader::loadOBJ(filePath);
    if (model) {
        ui->frame->addObject(model);
        ui->frame->update();
    }
}
