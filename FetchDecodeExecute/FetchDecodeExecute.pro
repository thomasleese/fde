QT       += core gui widgets

TARGET = FetchDecodeExecute
TEMPLATE = app

SOURCES += main.cpp \
        mainwindow.cpp \
    component.cpp \
    instructionswidget.cpp \
    loadprogramdialogue.cpp

HEADERS  += mainwindow.h \
    component.h \
    instructionswidget.h \
    loadprogramdialogue.h

FORMS    += mainwindow.ui \
    instructionswidget.ui \
    loadprogramdialogue.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resources.qrc
