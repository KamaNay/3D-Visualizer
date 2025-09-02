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

    // Adicionar o MyFrame ao layout do centralWidget
    QLayout *layout = ui->centralwidget->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(layout);
    }
    layout->addWidget(frame);
}

MainWindow::~MainWindow()
{
    delete ui;
}
