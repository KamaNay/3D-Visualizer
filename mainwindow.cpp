#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objreader.h"
#include "object3D.h"
#include "house.h"

/*
 * MainWindow:
 * - configura o frame central (ui->frame)
 * - adiciona alguns objetos de exemplo (casas)
 * - carrega modelos OBJ (se existirem nos caminhos)
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->frame);
    ui->frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->frame->setMinimumSize(400, 300);

    ui->frame->addObject(new House("Casa1", 100, 250, 80, 80));

    loadOBJModel("C:/Users/andre/Documents/UTFPR/CG/AP/Charizard.obj");
    loadOBJModel("C:/Users/andre/Documents/UTFPR/CG/AP/UmbreonHighPoly.obj");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadOBJModel(const QString &filePath)
{
    Object3D* model = OBJReader::loadOBJ(filePath);
    if (model) {
        ui->frame->addObject(model);
        ui->frame->update();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    bool cameraMode = event->modifiers() & Qt::ShiftModifier;

    // se SHIFT pressionado -> controla câmera
    if (cameraMode) {
        auto& w = ui->frame->getWindow();
        switch (event->key()) {
        case Qt::Key_W: w.moveForward(20); break;
        case Qt::Key_S: w.moveForward(-20); break;
        case Qt::Key_A: w.moveRight(-20); break;
        case Qt::Key_D: w.moveRight(20); break;
        case Qt::Key_Q: w.moveUp(20); break;
        case Qt::Key_E: w.moveUp(-20); break;
        case Qt::Key_I: w.pitch -= 2; break;
        case Qt::Key_K: w.pitch += 2; break;
        case Qt::Key_J: w.yaw -= 2; break;
        case Qt::Key_L: w.yaw += 2; break;
        case Qt::Key_U: w.roll -= 2; break;
        case Qt::Key_O: w.roll += 2; break;
        case Qt::Key_P: w.perspectiveMode = !w.perspectiveMode; break;
        default: break;
        }
        ui->frame->update();
        return;
    }

    // sem SHIFT -> controla objeto selecionado
    Object* selected = nullptr;
    for (auto obj : ui->frame->getDisplayFile().getObjects()) {
        if (obj->isSelected()) { selected = obj; break; }
    }

    if (selected) {
        // frente / trás no eixo Z (vírgula / ponto)
        if (event->key() == Qt::Key_Comma) {
            if (auto obj3D = dynamic_cast<Object3D*>(selected)) obj3D->translate3D(0,0,-20);
            else selected->translate3D(0,0,-20);
            ui->frame->update();
            return;
        } else if (event->key() == Qt::Key_Period) {
            if (auto obj3D = dynamic_cast<Object3D*>(selected)) obj3D->translate3D(0,0,20);
            else selected->translate3D(0,0,20);
            ui->frame->update();
            return;
        }

        // controles específicos
        if (auto obj3D = dynamic_cast<Object3D*>(selected)) {
            switch (event->key()) {
            case Qt::Key_Up: obj3D->translate3D(0,10,0); break;
            case Qt::Key_Down: obj3D->translate3D(0,-10,0); break;
            case Qt::Key_Left: obj3D->translate3D(-10,0,0); break;
            case Qt::Key_Right: obj3D->translate3D(10,0,0); break;
            case Qt::Key_PageUp: obj3D->translate3D(0,0,-10); break;
            case Qt::Key_PageDown: obj3D->translate3D(0,0,10); break;
            case Qt::Key_T: obj3D->rotateX3D(-10); break;
            case Qt::Key_G: obj3D->rotateX3D(10); break;
            case Qt::Key_F: obj3D->rotateY3D(-10); break;
            case Qt::Key_H: obj3D->rotateY3D(10); break;
            case Qt::Key_R: obj3D->rotateZ3D(-10); break;
            case Qt::Key_Y: obj3D->rotateZ3D(10); break;
            case Qt::Key_Equal: obj3D->scale3D(1.1,1.1,1.1,obj3D->getCenter()); break;
            case Qt::Key_Minus: obj3D->scale3D(0.9,0.9,0.9,obj3D->getCenter()); break;
            default: break;
            }
            ui->frame->update();
            return;
        } else {
            // objeto 2D
            switch (event->key()) {
            case Qt::Key_Left: selected->translate(-10,0); break;
            case Qt::Key_Right: selected->translate(10,0); break;
            case Qt::Key_Up: selected->translate(0,-10); break;
            case Qt::Key_Down: selected->translate(0,10); break;
            case Qt::Key_Z: {
                Point c = selected->getCenter(); selected->rotate(-10, c.getX(), c.getY()); break;
            }
            case Qt::Key_X: {
                Point c = selected->getCenter(); selected->rotate(10, c.getX(), c.getY()); break;
            }
            case Qt::Key_Equal: {
                Point c = selected->getCenter(); selected->scale(1.1,1.1,c.getX(),c.getY()); break;
            }
            case Qt::Key_Minus: {
                Point c = selected->getCenter(); selected->scale(0.9,0.9,c.getX(),c.getY()); break;
            }
            default: break;
            }
            ui->frame->update();
            return;
        }
    }
}
