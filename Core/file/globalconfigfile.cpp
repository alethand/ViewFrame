#include "globalconfigfile.h"

#include "Base/util/rutil.h"
#include "Base/util/rlog.h"
#include "Base/util/rsingleton.h"
#include "Base/constants.h"

GlobalConfigFile::GlobalConfigFile():settings(nullptr)
{

}

bool GlobalConfigFile::parseFile()
{
    if(settings == nullptr)
        return false;

    //解析日志模块
    logConfig.isRecord2File = RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_RECORDFILE,true).toBool();
    logConfig.level = RUtil::getGlobalValue(Constant::LOG_LOG,Constant::LOG_LOG_LEVEL,RLog::RINFO).toInt();

    //数据库配置
    databaseConfig.hostName = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_HOST,databaseConfig.hostName).toString();
    databaseConfig.dbName = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_DATABASE_NAME,databaseConfig.dbName).toString();
    databaseConfig.dbUser = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_USERNAME,databaseConfig.dbUser).toString();
    databaseConfig.dbPass = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PASSWORD,databaseConfig.dbPass).toString();
    databaseConfig.port = RUtil::getGlobalValue(Constant::DB_SETTING,Constant::DB_PORT,databaseConfig.port).toInt();

    //系统配置
    systemConfigInfo.defaultKeySchemes = RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_DEFAULT_SCHEMES,systemConfigInfo.defaultKeySchemes).toBool();
    systemConfigInfo.userKeySchemesName = RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_USER_SCHEMES_NAME,systemConfigInfo.userKeySchemesName).toString();
    systemConfigInfo.locale = RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_LOCALE,systemConfigInfo.locale).toString();
    systemConfigInfo.style = RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_STYLE,systemConfigInfo.style).toString();
    systemConfigInfo.fullscreen = RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_FULLSCREEN,systemConfigInfo.fullscreen).toBool();
    systemConfigInfo.topHint = RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_TOPHINT,systemConfigInfo.topHint).toBool();
    systemConfigInfo.menubarVisible = RUtil::getGlobalValue(Constant::SYSTEM_SETTING,Constant::SYSTEM_MENUBAR,systemConfigInfo.menubarVisible).toBool();

    return true;
}

void GlobalConfigFile::saveFile()
{
    if(settings == nullptr)
        return;

    settings->beginGroup(Constant::LOG_LOG);
    settings->setValue(Constant::LOG_LOG_RECORDFILE,logConfig.isRecord2File);
    settings->setValue(Constant::LOG_LOG_LEVEL,logConfig.level);
    settings->endGroup();

    settings->beginGroup(Constant::DB_SETTING);
    settings->setValue(Constant::DB_HOST,databaseConfig.hostName);
    settings->setValue(Constant::DB_DATABASE_NAME,databaseConfig.dbName);
    settings->setValue(Constant::DB_USERNAME,databaseConfig.dbUser);
    settings->setValue(Constant::DB_PASSWORD,databaseConfig.dbPass);
    settings->setValue(Constant::DB_PORT,databaseConfig.port);
    settings->endGroup();

    settings->beginGroup(Constant::SYSTEM_SETTING);
    settings->setValue(Constant::SYSTEM_DEFAULT_SCHEMES,systemConfigInfo.defaultKeySchemes);
    settings->setValue(Constant::SYSTEM_USER_SCHEMES_NAME,systemConfigInfo.userKeySchemesName);
    settings->setValue(Constant::SYSTEM_LOCALE,systemConfigInfo.locale);
    settings->setValue(Constant::SYSTEM_STYLE,systemConfigInfo.style);
    settings->setValue(Constant::SYSTEM_FULLSCREEN,systemConfigInfo.fullscreen);
    settings->setValue(Constant::SYSTEM_TOPHINT,systemConfigInfo.topHint);
    settings->setValue(Constant::SYSTEM_MENUBAR,systemConfigInfo.menubarVisible);
    settings->endGroup();
}
