TARGET = $$IDLE_LIB_NAME

include(viewframe.pri)

win32 {
    DLLDESTDIR = $$IDE_APP_PATH
}

DESTDIR = $$IDE_LIBRARY_PATH

TEMPLATE = lib
CONFIG += shared dll
