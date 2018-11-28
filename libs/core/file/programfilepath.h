/*!
 *  @brief     系统文件配置信息
 *  @details   定义了系统中各个文件夹路径、文件名全路径等信息
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.28
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PROGRAMFILEPATH_H
#define PROGRAMFILEPATH_H

#include <QObject>
#include <core/core_global.h>

namespace Core{

class CORESHARED_EXPORT ProgramFilePath
{
public:
    ProgramFilePath();

    //目录信息
    QString programPath;
    QString configPath;
    QString libPath;
    QString translationPath;
    QString shortcutPath;
    QString pluginPath;

    //子模块路径信息
    QString taskControlPath;
    QString healthManagePath;
    QString dataViewPath;

    //配置文件信息
    QString configFile;
    QString pluginFile;
    QString layoutFile;
};

} //namespace Core

#endif // PROGRAMFILEPATH_H
