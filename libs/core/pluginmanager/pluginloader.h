/*!
 *  @brief     模块关系维护
 *  @details   临时拥有PluginManager的职责，待两者合并:20181108;
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "pluginxmlparse.h"

namespace Core{

class PluginLoader
{
public:
    explicit PluginLoader();

    void loadPlugin();

    bool initConfigFile();
    bool saveConfigFile();

    ModuleList * getModules(){return parsemethod->getModules();}
    NetworkMap * getNetworks(){return parsemethod->getNetworks();}
    PluginMap * getPlugins(){return parsemethod->getPlugins();}

    Datastruct::NetworkInfo getNetwork(QString networkId,bool & existed);

private:
    PluginXmlParse * parsemethod;

};

}

#endif // PLUGINLOADER_H
