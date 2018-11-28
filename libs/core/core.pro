#-------------------------------------------------
#
# Project created by QtCreator 2018-08-26T09:54:14
#
#-------------------------------------------------

QT       += core gui xml
QT       += network

DEFINES += CORE_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = core
TEMPLATE = lib

#RC_ICONS += $${TARGET}.ico

include(../../viewframe.pri)

CONFIG(debug, debug|release) {

  contains(TEMPLATE, "lib") {
    DESTDIR = $$IDE_LIBRARY_PATH
    DLLDESTDIR = $$IDE_BIN_PATH
  } else {
    DESTDIR = $$IDE_LIBRARY_PATH
  }
} else {
  contains(TEMPLATE, "lib") {
    DESTDIR = $$IDE_LIBRARY_PATH
    DLLDESTDIR = $$IDE_BIN_PATH
  } else {
    DESTDIR = $$IDE_LIBRARY_PATH
  }
}

SOURCES += widgets/mainwindow.cpp \
    protocol/datastruct.cpp \
    file/globalconfigfile.cpp \
    global.cpp \
    widgets/shortcutsettings.cpp \
    file/programfilepath.cpp \
    calculater/commonalgorithms.cpp \
    protocol/commonprotocolparse.cpp \
    protocol/protocolmanager.cpp \
    network/networkmanager.cpp \
    network/tcpserver.cpp \
    network/rtask.cpp \
    pluginmanager/observer.cpp \
    pluginmanager/pluginmanager.cpp \
    pluginmanager/rcomponent.cpp \
    pluginmanager/subject.cpp \
    widgets/pageinfo.cpp \
    network/protocolparsethread.cpp \
    others/googlemap.cpp \
    others/utility.cpp \
    widgets/mapview.cpp \
    network/taskmanager.cpp \
    selfwidget/mydockwidgettitlebutton.cpp \
    selfwidget/mydockwidget.cpp \
    selfwidget/widget.cpp \
    selfwidget/modulesetting.cpp \
    selfwidget/titlebar.cpp \
    network/netparseinfo.cpp \
    file/layoutparsemethod.cpp \
    selfwidget/networksettingdialog.cpp \
    network/udpserver.cpp \
    network/udpclient.cpp \
    pluginmanager/pluginloader.cpp \
    pluginmanager/pluginxmlparse.cpp

HEADERS  += \
    widgets/mainwindow.h \
    protocol/datastruct.h \
    file/globalconfigfile.h \
    global.h \
    widgets/shortcutsettings.h \
    file/programfilepath.h \
    calculater/commonalgorithms.h \
    protocol/commonprotocolparse.h \
    protocol/protocolmanager.h \
    network/networkmanager.h \
    network/tcpserver.h \
    network/rtask.h \
    pluginmanager/observer.h \
    pluginmanager/pluginmanager.h \
    pluginmanager/rcomponent.h \
    pluginmanager/subject.h \
    widgets/pageinfo.h \
    network/protocolparsethread.h \
    others/googlemap.h \
    others/CommonHeader.h \
    others/utility.h \
    widgets/mapview.h \
    network/taskmanager.h \
    selfwidget/mydockwidgettitlebutton.h \
    selfwidget/mydockwidget.h \
    selfwidget/widget.h \
    selfwidget/modulesetting.h \
    selfwidget/titlebar.h \
    network/netparseinfo.h \
    file/layoutparsemethod.h \
    selfwidget/networksettingdialog.h \
    network/udpserver.h \
    network/udpclient.h \
    core_global.h \
    pluginmanager/pluginloader.h \
    pluginmanager/pluginxmlparse.h

FORMS    += mainwindow.ui

win32-msvc2013{
    LIBS+= $$IDE_LIBRARY_PATH/base.lib
    LIBS+= $$IDE_LIBRARY_PATH/network.lib
}

win32-g++{
    LIBS+= -L$$IDE_LIBRARY_PATH -lbase
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

unix{
    LIBS+= -L$$IDE_LIBRARY_PATH -lnetwork -lbase
}

RESOURCES += \
    res.qrc
