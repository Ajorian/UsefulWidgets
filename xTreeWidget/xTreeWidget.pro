#-------------------------------------------------
#
# Project created by QtCreator 2018-12-06T14:30:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xTreeWidget
TEMPLATE = app

SOURCES += main.cpp\
        test.cpp \
    xTreeWidget.cpp \
    xml/gxp.cpp \
    xml/pugi/pugixml.cpp

HEADERS  += test.h \
    xTreeWidget.h \
    xml/gxp.h \
    xml/pugi/pugiconfig.hpp \
    xml/pugi/pugixml.hpp

FORMS    += test.ui \
    xTreeWidget.ui

RESOURCES += \
    xTreeWidget.qrc

