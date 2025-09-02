#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Criar e configurar o MyFrame
    frame = new MyFrame(this);
    frame->setMinimumSize(400, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}
