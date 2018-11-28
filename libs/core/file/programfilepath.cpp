#include "programfilepath.h"

#include <QDir>
#include <QApplication>

namespace Core{

ProgramFilePath::ProgramFilePath()
{
    programPath = qApp->applicationDirPath();
    configPath = programPath + QDir::separator() + QString("../config");
    translationPath = configPath + QDir::separator() + QString("translations");
    shortcutPath = configPath + QDir::separator() + QString("schemes");
    pluginPath = programPath + QDir::separator() + QString("../lib/plugins");

    taskControlPath = configPath + QDir::separator() + QString("protocol/taskcontrol/");
    healthManagePath = configPath + QDir::separator() + QString("protocol/healmanage/");
    dataViewPath = configPath + QDir::separator() + QString("protocol/dataview/");

    configFile = configPath + QDir::separator() + QString("config.ini");
    pluginFile = configPath + QDir::separator() + QString("plugins.xml");
    layoutFile = configPath + QDir::separator() + QString("layout.bin");
}

} //namespace Core
