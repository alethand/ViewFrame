include(../../ViewFrame.pri)

QT += core network

CONFIG += c++11

TARGET = network
CONFIG += console
CONFIG -= app_bundle

DEFINES += NETWORK_LIBRARY

TEMPLATE = lib

CONFIG(debug, debug|release) {
#  TARGET = $$join(TARGET,,,d)           #为debug版本生成的文件增加d的后缀
  contains(TEMPLATE, "lib") {
    DESTDIR = $$IDE_LIBRARY_PATH
    DLLDESTDIR = $$IDE_BIN_PATH
  } else {
    DESTDIR = $$IDE_BIN_PATH
  }
} else {
  contains(TEMPLATE, "lib") {
    DESTDIR = $$IDE_LIBRARY_PATH
    DLLDESTDIR = $$IDE_BIN_PATH
  } else {
    DESTDIR = $$IDE_BIN_PATH
  }
}

HEADERS += \
    netglobal.h \
    network_global.h \
    head.h \
    multitransmits/basetransmit.h \
    dataprocess/handler.h \
    dataprocess/sockdatahandler.h \
    connection/seriesconnection.h \
    connection/tcpclient.h \
    wraprule/wraprule.h \
    multitransmits/transmits.h \
    base/abstractserver.h \
    base/socket.h \
    wraprule/tcp_iocpdatapacketrule.h \
    multitransmits/tcptransmit.h \
    wraprule/tcp_bytedatapacketrule.h \
    multitransmits/udptransmit.h \
    wraprule/udp_bytedatapacketrule.h

SOURCES += \
    netglobal.cpp \
    multitransmits/basetransmit.cpp \
    dataprocess/handler.cpp \
    dataprocess/sockdatahandler.cpp \
    connection/seriesconnection.cpp \
    connection/tcpclient.cpp \
    wraprule/wraprule.cpp \
    multitransmits/transmits.cpp \
    base/abstractserver.cpp \
    base/socket.cpp \
    wraprule/tcp_iocpdatapacketrule.cpp \
    multitransmits/tcptransmit.cpp \
    wraprule/tcp_bytedatapacketrule.cpp \
    multitransmits/udptransmit.cpp \
    wraprule/udp_bytedatapacketrule.cpp

win32-msvc2013{
    HEADERS+=   win32iocp/tcpserver.h \
        win32iocp/SharedIocpData.h \
        win32iocp/workthread.h \
        win32iocp/netutils.h \
        win32iocp/iopacket.h \
        win32iocp/iocpcontext.h \
        multitransmits/iocptransmit.h

    SOURCES += win32iocp/SharedIocpData.cpp \
        win32iocp/workthread.cpp \
        win32iocp/netutils.cpp \
        win32iocp/tcpserver.cpp \
        win32iocp/iopacket.cpp \
        win32iocp/iocpcontext.cpp \
        multitransmits/iocptransmit.cpp

    LIBS+=$$IDE_LIBRARY_PATH/base.lib
}
