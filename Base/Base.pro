QT       += core widgets xml
QT       += sql

win32-msvc2013{
QT       += axcontainer
}

CONFIG += c++11

TARGET = Base
CONFIG -= app_bundle

DEFINES += BASE_LIBRARY

TEMPLATE = lib

CONFIG(debug, debug|release) {
#  TARGET = $$join(TARGET,,,d)           #为debug版本生成的文件增加d的后缀

  contains(TEMPLATE, "lib") {
    DESTDIR = ../Lib
    DLLDESTDIR = ../Bin
  } else {
    DESTDIR = ../Bin
  }
} else {
  contains(TEMPLATE, "lib") {
    DESTDIR = ../Lib
    DLLDESTDIR = ../Bin
  } else {
    DESTDIR = ../Bin
  }
}

HEADERS += \
    actionmanager/action.h \
    actionmanager/actioncontainer.h \
    actionmanager/actionmanager.h \
    actionmanager/command.h \
    actionmanager/id.h \
    base_global.h \
    messagetype.h \
    util/fileutils.h \
    util/rbuffer.h \
    util/regexp.h \
    util/rlog.h \
    util/rsingleton.h \
    util/rutil.h \
    util/scaleswitcher.h \
    selfwidget/dialogproxy.h \
    selfwidget/rbutton.h \
    common/sql/autotransaction.h \
    common/sql/database.h \
    common/sql/databasemanager.h \
    common/sql/datatable.h \
    common/sql/rpersistence.h \
    protocol/datastruct.h \
    constants.h \
    common/stylemanager.h \
    common/languagemanager.h \
    common/validator/rvalidator.h \
    common/validator/rcombinevalidator.h

SOURCES += \
    actionmanager/action.cpp \
    actionmanager/actioncontainer.cpp \
    actionmanager/actionmanager.cpp \
    actionmanager/command.cpp \
    actionmanager/id.cpp \
    util/fileutils.cpp \
    util/rbuffer.cpp \
    util/regexp.cpp \
    util/rlog.cpp \
    util/rsingleton.cpp \
    util/rutil.cpp \
    util/scaleswitcher.cpp \
    selfwidget/dialogproxy.cpp \
    selfwidget/rbutton.cpp \
    protocol/datastruct.cpp \
    common/sql/autotransaction.cpp \
    common/sql/database.cpp \
    common/sql/databasemanager.cpp \
    common/sql/datatable.cpp \
    common/sql/rpersistence.cpp \
    common/stylemanager.cpp \
    common/languagemanager.cpp \
    common/validator/rvalidator.cpp \
    common/validator/rcombinevalidator.cpp

win32-msvc2013{

HEADERS +=  \
    util/xlsfilesaver.h

SOURCES +=  \
    util/xlsfilesaver.cpp

}
