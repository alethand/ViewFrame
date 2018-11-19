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
    file/globalconfigfile.cpp \
    global.cpp \
    widgets/taskcontrol/modelview/tableview.cpp \
    widgets/taskcontrol/modelview/tableviewdelegate.cpp \
    widgets/taskcontrol/modelview/tableviewmodel.cpp \
    widgets/taskcontrol/taskcontrol.cpp \
    widgets/taskcontrol/taskcontrolpanel.cpp \
    widgets/healthmanage/fileOpt.cpp \
    widgets/healthmanage/healthinfopannel.cpp \
    widgets/healthmanage/healthstate_display.cpp \
    widgets/healthmanage/layoutset.cpp \
    widgets/healthmanage/dynamiclayoutbynet.cpp \
    widgets/healthmanage/staticlayoutbyxml.cpp \
    widgets/taskcontrol/net/taskdispatcher.cpp \
    widgets/datadisplay/allplusegraphics.cpp \
    widgets/datadisplay/allplusetable.cpp \
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
    widgets/taskcontrol/tasklayout.cpp \
    widgets/taskcontrol/taskdialogproxy.cpp \
    widgets/taskcontrol/taskparsedmethod.cpp \
    calculater/commonalgorithms.cpp \
    widgets/datadisplay/Graphics/scatterdiagram.cpp \
    widgets/datadisplay/Graphics/waveform.cpp \
    widgets/datadisplay/Graphics/superparent.cpp \
    widgets/datadisplay/Graphics/histogram.cpp \
    widgets/datadisplay/Graphics/energybar.cpp \
    protocol/commonprotocolparse.cpp \
    protocol/protocolmanager.cpp \
    protocol/pluginxmlparse.cpp \
    protocol/pluginloader.cpp \
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
    widgets/datadisplay/radiusSource/panel.cpp \
    widgets/datadisplay/radiusSource/radiussrcdisplay.cpp \
    widgets/datadisplay/DisplayModel/model_cover.cpp \
    widgets/datadisplay/DisplayModel/model_details.cpp \
    widgets/datadisplay/DisplayModel/model_history.cpp \
    widgets/datadisplay/datadisplay.cpp \
    network/netparseinfo.cpp \
    widgets/datadisplay/DisplayModel/dateview_superstruct.cpp

HEADERS  += \
    widgets/mainwindow.h \
    protocol/datastruct.h \
    file/globalconfigfile.h \
    global.h \
    widgets/taskcontrol/modelview/tableview.h \
    widgets/taskcontrol/modelview/tableviewdelegate.h \
    widgets/taskcontrol/modelview/tableviewmodel.h \
    widgets/taskcontrol/taskcontrol.h \
    widgets/taskcontrol/utils.h \
    widgets/taskcontrol/taskcontrolpanel.h \
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
    widgets/datadisplay/table.h \
    widgets/datadisplay/modelview/tableviewdata.h \
    widgets/datadisplay/modelview/tableviewmodelcustom.h \
    widgets/datadisplay/modelview/tableviewmoderradiationsource.h \
    widgets/shortcutsettings.h \
    file/programfilepath.h \
    widgets/Util/dataexportandprint.h \
    widgets/Util/exporttoexcelthread.h \
    widgets/Util/tipdialog.h \
    widgets/taskcontrol/tasklayout.h \
    widgets/taskcontrol/taskdialogproxy.h \
    widgets/taskcontrol/taskparsedmethod.h \
    calculater/commonalgorithms.h \
    widgets/datadisplay/Graphics/scatterdiagram.h \
    widgets/datadisplay/Graphics/waveform.h \
    widgets/datadisplay/Graphics/superparent.h \
    widgets/datadisplay/Graphics/histogram.h \
    widgets/datadisplay/Graphics/energybar.h \
    protocol/commonprotocolparse.h \
    protocol/protocolmanager.h \
    protocol/pluginxmlparse.h \
    protocol/pluginloader.h \
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
    widgets/datadisplay/radiusSource/panel.h \
    widgets/datadisplay/radiusSource/radiussrcdisplay.h \
    widgets/datadisplay/DisplayModel/dateview_superstruct.h \
    widgets/datadisplay/DisplayModel/model_cover.h \
    widgets/datadisplay/DisplayModel/model_details.h \
    widgets/datadisplay/DisplayModel/model_history.h \
    widgets/datadisplay/datadisplay.h \
    network/netparseinfo.h

FORMS    += mainwindow.ui \
    widgets/datadisplay/mfacquisitiongraphics.ui \
    widgets/datadisplay/radiusSource/radiationsourcetable.ui \
    widgets/datadisplay/radiusSource/radiationsourcetablerenovatedialog.ui \
    widgets/datadisplay/radiationsourcetablerenovate.ui \
    widgets/datadisplay/spectrumgraphics.ui \
    widgets/datadisplay/filterdockpanel.ui \
    widgets/datadisplay/radiasourcemap.ui \
    widgets/datadisplay/radiationsourcetable.ui \
    widgets/datadisplay/radiationsourcetablerenovatedialog.ui

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
