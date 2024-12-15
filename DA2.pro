#-------------------------------------------------
#
# Project created by QtCreator 2024-08-26T07:40:18
#
#-------------------------------------------------

QT       += core gui
QT += core gui widgets
QT += core
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DA2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagewidget.cpp

HEADERS  += mainwindow.h \
    imagewidget.h

FORMS    += mainwindow.ui
