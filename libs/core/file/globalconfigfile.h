/*!
 *  @brief     软件启动配置文件
 *  @details   启动时解析/config/config.ini文件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef GLOBALCONFIGFILE_H
#define GLOBALCONFIGFILE_H

#include <QSettings>

#include "Base/util/rlog.h"
#include <core/protocol/datastruct.h>
#include <core/core_global.h>

namespace Core{

class CORESHARED_EXPORT GlobalConfigFile
{
public:
    GlobalConfigFile();

    void setSettings(QSettings * settings){this->settings = settings;}

    bool parseFile();
    void saveFile();

public:
    RLog::LogConfig logConfig;
    Datastruct::DatabaseConfigInfo databaseConfig;
    Datastruct::SystemConfigInfo systemConfigInfo;

private:
    QVariant getGlobalValue(const QString & group,const QString &key, const QVariant & defaultValue);

private:
    QSettings * settings;

};

} //namespace Core

#endif // GLOBALCONFIGFILE_H
