#-------------------------------------------------
#
# Project created by QtCreator 2018-11-27T10:34:20
#
#-------------------------------------------------
include(../viewframe.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
TEMPLATE = app

SOURCES += main.cpp

DESTDIR = $$IDE_BIN_PATH

LIBS += $$IDE_LIBRARY_PATH/base.lib
LIBS += $$IDE_LIBRARY_PATH/core.lib
