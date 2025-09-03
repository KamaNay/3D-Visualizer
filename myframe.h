#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QPainter>
#include "df.h"

class MyFrame : public QFrame {
    Q_OBJECT

private:
    DisplayFile df;

public:
    explicit MyFrame(QWidget *parent = nullptr);

    void addObject(Object* obj);

protected:
    void paintEvent(QPaintEvent *) override;
};

#endif
