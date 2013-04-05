#-------------------------------------------------
#
# Project created by QtCreator 2012-11-26T16:42:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FetchExecuteCycle-desktop
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            loadprogramdialogue.cpp \
    component.cpp \
    instructionswidget.cpp

HEADERS  += mainwindow.h \
            loadprogramdialogue.h \
    component.h \
    instructionswidget.h

FORMS    += mainwindow.ui \
            loadprogramdialogue.ui \
    instructionswidget.ui

RESOURCES += \
    resources.qrc
