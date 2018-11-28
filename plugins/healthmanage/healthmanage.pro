#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T17:19:29
#
#-------------------------------------------------
include(../viewframeplugin.pri)

QT       += core gui
QT       += network
QT       += xml

TARGET = healthmanage

HEADERS += \
    dynamiclayoutbynet.h \
    fileOpt.h \
    head.h \
    healthinfopannel.h \
    healthstate_display.h \
    layoutset.h \
    staticlayoutbyxml.h

SOURCES += \
    dynamiclayoutbynet.cpp \
    fileOpt.cpp \
    healthinfopannel.cpp \
    healthstate_display.cpp \
    layoutset.cpp \
    staticlayoutbyxml.cpp
