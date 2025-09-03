#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pointobj.h"
#include "line.h"
#include "polygon.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Ponto
    Object* p = new PointObj("P1", 100, 100);

    //Linha
    Object* l = new Line("L1", Point(150, 150), Point(300, 150));

    //Poligono
    std::vector<Point> pts = {
        Point(220, 200),
        Point(250, 240),
        Point(220, 280)
    };
    Object* poly = new Polygon("Poly1", pts);

    ui->frame->setFixedSize(400, 300);
    ui->frame->setStyleSheet("background-color: white;");


    // Adiciona no frame (interface)
    ui->frame->addObject(p);
    ui->frame->addObject(l);
    ui->frame->addObject(poly);

}

MainWindow::~MainWindow() {
    delete ui;
}
