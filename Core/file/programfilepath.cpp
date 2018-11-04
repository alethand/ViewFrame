#include "programfilepath.h"

#include <QDir>
#include <QApplication>

ProgramFilePath::ProgramFilePath()
{
    programPath = qApp->applicationDirPath();
    configPath = programPath + QDir::separator() + QString("../config");
    translationPath = configPath + QDir::separator() + QString("translations");
    protocolPath = configPath + QDir::separator() + QString("protocol");
    shortcutPath = configPath + QDir::separator() + QString("schemes");

    configFile = configPath + QDir::separator() + QString("config.ini");
}
