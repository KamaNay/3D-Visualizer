QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = DF
TEMPLATE = app
SOURCES += mainwindow.cpp myframe.cpp \
    main.cpp
HEADERS += mainwindow.h point.h line.h polygon.h myframe.h \
    df.h \
    house.h \
    object.h \
    pointobj.h \
    viewport.h \
    window.h
FORMS += mainwindow.ui
