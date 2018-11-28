#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T17:19:29
#
#-------------------------------------------------
include(../viewframeplugin.pri)

QT       += core gui
QT       += network
QT       += xml

TARGET = taskcontrol

HEADERS += \
    head.h \
    taskcontrol.h \
    taskcontrolpanel.h \
    taskdialogproxy.h \
    tasklayout.h \
    taskparsedmethod.h \
    modelview/tableview.h \
    modelview/tableviewdelegate.h \
    modelview/tableviewmodel.h \
    net/taskdispatcher.h

SOURCES += \
    head.cpp \
    taskcontrol.cpp \
    taskcontrolpanel.cpp \
    taskdialogproxy.cpp \
    tasklayout.cpp \
    taskparsedmethod.cpp \
    modelview/tableview.cpp \
    modelview/tableviewdelegate.cpp \
    modelview/tableviewmodel.cpp \
    net/taskdispatcher.cpp

