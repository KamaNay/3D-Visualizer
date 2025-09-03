#include "myframe.h"

MyFrame::MyFrame(QWidget *parent) : QFrame(parent) {}

void MyFrame::addObject(Object* obj) {
    df.addObject(obj);
    update();
}

void MyFrame::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 5));
    df.drawAll(&painter);
}
