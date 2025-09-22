#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QPainter>
#include "df.h"
#include <QMouseEvent>
#include "window.h"
#include "viewport.h"

class MyFrame : public QFrame {
    Q_OBJECT

private:
    DisplayFile df;
    Window* window;
    Viewport* viewport;

public:
public:
    DisplayFile& getDisplayFile() { return df; }
    Window& getWindow() { return *window; }
    Viewport& getViewport() { return *viewport; }

    explicit MyFrame(QWidget *parent = nullptr);

    void addObject(Object* obj);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif
