#-------------------------------------------------
#
# Project created by QtCreator 2013-01-24T15:20:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TransferFunction
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    functiontab.cpp \
    ctoken.cpp \
    cpoly.cpp

HEADERS  += mainwindow.h \
    functiontab.h \
    ctoken.h \
    cpoly.h

FORMS    += mainwindow.ui \
    functiontab.ui
