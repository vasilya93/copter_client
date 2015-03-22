#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T18:15:17
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = copter_client
TEMPLATE = app


SOURCES += main.cpp\
    fifo.cpp \
    qcustomplot.cpp \
    commmanager.cpp \
    SerialComm.cpp \
    ClientView.cpp \
    packetmanager.cpp \
    datakeeper.cpp \
    vec3.cpp \
    orientationindicator.cpp

HEADERS  += ClientView.h \
    fifo.h \
    qcustomplot.h \
    commmanager.h \
    SerialComm.h \
    packetmanager.h \
    datakeeper.h \
    vec3.h \
    orientationindicator.h

QMAKE_CXXFLAGS += -std=c++11
