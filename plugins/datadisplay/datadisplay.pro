#-------------------------------------------------
#
# Project created by QtCreator 2018-11-26T17:19:29
#
#-------------------------------------------------
include(../viewframeplugin.pri)

QT       += core gui
QT       += network
QT       += xml charts

TARGET = datadisplay

HEADERS += \
    allplusegraphics.h \
    allplusetable.h \
    datadisplay.h \
    table.h \
    DisplayModel/dateview_superstruct.h \
    DisplayModel/model_cover.h \
    DisplayModel/model_details.h \
    DisplayModel/model_history.h \
    Graphics/energybar.h \
    Graphics/histogram.h \
    Graphics/scatterdiagram.h \
    Graphics/superparent.h \
    Graphics/waveform.h

SOURCES += \
    allplusegraphics.cpp \
    allplusetable.cpp \
    datadisplay.cpp \
    table.cpp \
    DisplayModel/dateview_superstruct.cpp \
    DisplayModel/model_cover.cpp \
    DisplayModel/model_details.cpp \
    DisplayModel/model_history.cpp \
    Graphics/energybar.cpp \
    Graphics/histogram.cpp \
    Graphics/scatterdiagram.cpp \
    Graphics/superparent.cpp \
    Graphics/waveform.cpp
