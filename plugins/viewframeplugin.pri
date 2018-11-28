include(../viewframe.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $$IDE_PLUGIN_PATH

TEMPLATE = lib

win32-msvc2013{
    LIBS+= $$IDE_LIBRARY_PATH/base.lib
    LIBS+= $$IDE_LIBRARY_PATH/network.lib
    LIBS+= $$IDE_LIBRARY_PATH/core.lib
}
