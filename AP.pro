QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = DF
TEMPLATE = app
SOURCES += main.cpp mainwindow.cpp point.cpp line.cpp polygon.cpp df.cpp myframe.cpp
HEADERS += mainwindow.h point.h line.h polygon.h df.h myframe.h
FORMS += mainwindow.ui
