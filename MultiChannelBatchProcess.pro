#-------------------------------------------------
#
# Project created by QtCreator 2016-05-30T17:19:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MultiChannelBatchProcess#ProcessSchedule
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    schedulealgo.cpp \
    memalgo.cpp

HEADERS  += mainwindow.h \
    schedulealgo.h \
    memalgo.h \
    datastruct.h

FORMS    += mainwindow.ui
CONFIG += c++11
