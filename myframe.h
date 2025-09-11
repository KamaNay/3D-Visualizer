#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QPainter>
#include "df.h"
#include <QMouseEvent>

class MyFrame : public QFrame {
    Q_OBJECT

private:
    DisplayFile df;

public:
    DisplayFile& getDisplayFile() { return df; }

    explicit MyFrame(QWidget *parent = nullptr);

    void addObject(Object* obj);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif
