/*!
 *  @brief     插件xml文件解析
 *  @details   1.解析plugins.xml文件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINXMLPARSE_H
#define PLUGINXMLPARSE_H

#include "datastruct.h"
#include "Base/util/fileutils.h"

#include <QMap>

namespace Core{

typedef QMap<QString,Datastruct::ModuleInfo> ModuleMap;
typedef QMap<QString,Datastruct::NetworkInfo> NetworkMap;
typedef QMap<QString,Datastruct::PluginInfo> PluginMap;

class PluginXmlParse : public Base::RXmlParseMethod
{
public:
    PluginXmlParse();

    bool  startParse(QDomNode & rootNode);

    ModuleMap * getModules(){return this->modules;}
    NetworkMap * getNetworks(){return this->networks;}
    PluginMap * getPlugins(){return this->plugins;}

private:
    void parsePlugin(QDomNode & pluginsNode);
    void parseNetwork(QDomNode & networkNode);
    void parseModule(QDomNode & moduleNode);

private:
    ModuleMap * modules;
    NetworkMap * networks;
    PluginMap * plugins;
};

}

#endif // PLUGINXMLPARSE_H
