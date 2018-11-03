#-------------------------------------------------
#
# Project created by QtCreator 2018-08-26T09:54:14
#
#-------------------------------------------------

QT       += core gui xml charts
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViewFrame
TEMPLATE = app

#RC_ICONS += $${TARGET}.ico

CONFIG(debug, debug|release) {

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

SOURCES += main.cpp \
    widgets/mainwindow.cpp \
    protocol/datastruct.cpp \
    protocol/protocoldata.cpp \
    file/globalconfigfile.cpp \
    global.cpp \
    widgets/taskcontrol/modelview/tableview.cpp \
    widgets/taskcontrol/modelview/tableviewdelegate.cpp \
    widgets/taskcontrol/modelview/tableviewmodel.cpp \
    widgets/taskcontrol/taskcontrol.cpp \
    widgets/taskcontrol/taskcontrolpanel.cpp \
    widgets/taskcontrol/bandcontroldialog.cpp \
    widgets/taskcontrol/turntablecontroldialog.cpp \
    widgets/taskcontrol/statecontroldialog.cpp \
    widgets/taskcontrol/playbackcontroldialog.cpp \
    widgets/taskcontrol/selfcheckcontroldialog.cpp \
    widgets/taskcontrol/instrumentcontroldialog.cpp \
    widgets/taskcontrol/gathercontroldialog.cpp \
    widgets/healthmanage/fileOpt.cpp \
    widgets/healthmanage/healthinfopannel.cpp \
    widgets/healthmanage/healthstate_display.cpp \
    widgets/healthmanage/layoutset.cpp \
    widgets/healthmanage/dynamiclayoutbynet.cpp \
    widgets/healthmanage/staticlayoutbyxml.cpp \
    widgets/taskcontrol/net/taskdispatcher.cpp \
    widgets/datadisplay/allplusegraphics.cpp \
    widgets/datadisplay/allplusetable.cpp \
    widgets/datadisplay/mfacquisitiongraphics.cpp \
    widgets/datadisplay/mfacquistiontable.cpp \
    widgets/datadisplay/radiasourcemap.cpp \
    widgets/datadisplay/radiationsourcetable.cpp \
    widgets/datadisplay/radiationsourcetablerenovate.cpp \
    widgets/datadisplay/radiationsourcetablerenovatedialog.cpp \
    widgets/datadisplay/spectrumgraphics.cpp \
    widgets/datadisplay/table.cpp \
    widgets/datadisplay/modelview/tableviewdata.cpp \
    widgets/datadisplay/modelview/tableviewmodelcustom.cpp \
    widgets/datadisplay/modelview/tableviewmoderradiationsource.cpp \
    widgets/shortcutsettings.cpp \
    file/programfilepath.cpp \
    widgets/taskcontrol/head.cpp \
    widgets/Util/dataexportandprint.cpp \
    widgets/Util/exporttoexcelthread.cpp \
    widgets/Util/tipdialog.cpp \
    widgets/taskcontrol/taskcntldisplay_parsexml.cpp \
    protocol/layout_byxml.cpp \
    widgets/CustomWidget/energybar.cpp \
    calculater/commonalgorithms.cpp \
    widgets/datadisplay/Graphics/scatterdiagram.cpp \
    widgets/datadisplay/filterdockpanel.cpp

HEADERS  += \
    widgets/mainwindow.h \
    protocol/datastruct.h \
    protocol/protocoldata.h \
    file/globalconfigfile.h \
    global.h \
    widgets/taskcontrol/modelview/tableview.h \
    widgets/taskcontrol/modelview/tableviewdelegate.h \
    widgets/taskcontrol/modelview/tableviewmodel.h \
    widgets/taskcontrol/taskcontrol.h \
    widgets/taskcontrol/utils.h \
    widgets/taskcontrol/taskcontrolpanel.h \
    widgets/taskcontrol/bandcontroldialog.h \
    widgets/taskcontrol/turntablecontroldialog.h \
    widgets/taskcontrol/statecontroldialog.h \
    widgets/taskcontrol/playbackcontroldialog.h \
    widgets/taskcontrol/selfcheckcontroldialog.h \
    widgets/taskcontrol/instrumentcontroldialog.h \
    widgets/taskcontrol/gathercontroldialog.h \
    widgets/taskcontrol/head.h \
    widgets/healthmanage/fileOpt.h \
    widgets/healthmanage/healthinfopannel.h \
    widgets/healthmanage/healthstate_display.h \
    widgets/healthmanage/layoutset.h \
    widgets/healthmanage/dynamiclayoutbynet.h \
    widgets/healthmanage/staticlayoutbyxml.h \
    widgets/healthmanage/head.h \
    widgets/taskcontrol/net/taskdispatcher.h \
    widgets/datadisplay/allplusegraphics.h \
    widgets/datadisplay/allplusetable.h \
    widgets/datadisplay/mfacquisitiongraphics.h \
    widgets/datadisplay/mfacquistiontable.h \
    widgets/datadisplay/radiasourcemap.h \
    widgets/datadisplay/radiationsourcetable.h \
    widgets/datadisplay/radiationsourcetablerenovate.h \
    widgets/datadisplay/radiationsourcetablerenovatedialog.h \
    widgets/datadisplay/spectrumgraphics.h \
    widgets/datadisplay/table.h \
    widgets/datadisplay/modelview/tableviewdata.h \
    widgets/datadisplay/modelview/tableviewmodelcustom.h \
    widgets/datadisplay/modelview/tableviewmoderradiationsource.h \
    widgets/shortcutsettings.h \
    file/programfilepath.h \
    widgets/Util/dataexportandprint.h \
    widgets/Util/exporttoexcelthread.h \
    widgets/Util/tipdialog.h \
    widgets/taskcontrol/taskcntldisplay_parsexml.h \
    protocol/layout_byxml.h \
    widgets/CustomWidget/energybar.h \
    calculater/commonalgorithms.h \
    widgets/datadisplay/Graphics/scatterdiagram.h \
    widgets/datadisplay/filterdockpanel.h

FORMS    += mainwindow.ui \
    widgets/taskcontrol/gathercontroldialog.ui \
    widgets/datadisplay/mfacquisitiongraphics.ui \
    widgets/datadisplay/radiasourcemap.ui \
    widgets/datadisplay/radiationsourcetable.ui \
    widgets/datadisplay/radiationsourcetablerenovate.ui \
    widgets/datadisplay/radiationsourcetablerenovatedialog.ui \
    widgets/datadisplay/spectrumgraphics.ui \
    widgets/datadisplay/filterdockpanel.ui

win32-msvc2013{
    LIBS+= ../Lib/Base.lib
    LIBS+= ../Lib/Network.lib
}

win32-g++{
    LIBS+= -L../Lib/ -lBase
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

unix{
    LIBS+= -L../Lib/ -lNetwork -lBase
}

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../../$${TARGET}/

RESOURCES += \
    res.qrc
