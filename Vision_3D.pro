#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T15:44:22
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++0x
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += link_pkgconfig
PKGCONFIG += opencv

LIBS +=-L/opt/local/stow/opencv-2.4.13/lib -lopencv_nonfree

INCLUDEPATH +=/opt/local/stow/opencv-2.4.13/include

TARGET = Vision_3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageprocessor.cpp \
    movablebutton.cpp \
    dynamicframe.cpp

HEADERS  += mainwindow.h \
    imageprocessor.h \
    movablebutton.h \
    dynamicframe.h
