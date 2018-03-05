#-------------------------------------------------
#
# Project created by QtCreator 2016-07-12T13:44:58
#
#-------------------------------------------------

QT       += core gui
QT       += core
QT       += sql
QT       +=xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UIFF
TEMPLATE = app


SOURCES += main.cpp\
        mainform.cpp \
    tablewidget.cpp \
    form.cpp \
    sqltablemodel.cpp \
    plotter.cpp

HEADERS  += mainform.h \
   tablewidget.h \
    form.h \
    sqltablemodel.h \
    plotter.h

FORMS += \  
    form.ui

RESOURCES += \
    resource.qrc \
    plotter.qrc
