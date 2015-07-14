#-------------------------------------------------
#
# Project created by QtCreator 2015-06-05T18:18:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testFPTree
TEMPLATE = app

INCLUDEPATH += algorithm

DEPENDPATH += algorithm

SOURCES += main.cpp\
        mainwindow.cpp \
        algorithm/node.cpp \
        algorithm/pattern.cpp \
        algorithm/fptree.cpp

HEADERS  += mainwindow.h \
        algorithm/node.h \
        algorithm/pattern.h \
        algorithm/fptree.h

FORMS    += mainwindow.ui
