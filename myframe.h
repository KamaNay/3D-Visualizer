#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QPainter>
#include "df.h"

class MyFrame : public QFrame {
private:
    DF df;
    double worldWidth, worldHeight;

public:
    explicit MyFrame(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MYFRAME_H
