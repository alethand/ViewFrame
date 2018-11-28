#include "pluginloader.h"

#include "file/programfilepath.h"
#include <base/util/rsingleton.h>
#include <core/pluginmanager/pluginmanager.h>
#include <core/pluginmanager/rcomponent.h>
#include <file/programfilepath.h>

#include <QFileInfo>
#include <QDir>
#include <QPluginLoader>
#include <QDebug>

namespace Core{

PluginLoader::PluginLoader():parsemethod(NULL)
{
    parsemethod = new PluginXmlParse;
}

/*!
 * @brief 从lib/plugins/目录下加载可用的dll文件
 */
void PluginLoader::loadPlugin()
{
    ProgramFilePath filePath;
    QDir dir(filePath.pluginPath);
    QStringList filters;
    filters<<"*.dll";
    QFileInfoList fileList = dir.entryInfoList(filters);

    std::for_each(fileList.begin(),fileList.end(),[&](const QFileInfo & info){
        QPluginLoader loader(info.absoluteFilePath());
        RComponent * component = qobject_cast<RComponent *>(loader.instance());
        if(component){
            RSingleton<PluginManager>::instance()->addAvailblePlugin(component);
        }
    });
}

bool PluginLoader::initConfigFile()
{
    ProgramFilePath filePath;
    Base::RXmlFile file(filePath.pluginFile);
    file.setParseMethod(parsemethod,false);
    return file.startParse();
}

bool PluginLoader::saveConfigFile()
{
    ProgramFilePath filePath;
    Base::RXmlFile file(filePath.pluginFile);
    file.setParseMethod(parsemethod,false);
    return file.startSave();
}

Datastruct::NetworkInfo PluginLoader::getNetwork(QString networkId, bool &existed)
{
    existed = false;
    Datastruct::NetworkInfo info;

    NetworkMap * networkMap = parsemethod->getNetworks();
    if(networkMap && networkMap->contains(networkId)){
        info = networkMap->value(networkId);
        existed = true;
    }
    return info;
}

}
