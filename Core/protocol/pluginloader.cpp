#include "pluginloader.h"

#include "file/programfilepath.h"

namespace Core{

PluginLoader::PluginLoader():parsemethod(NULL)
{
    parsemethod = new PluginXmlParse;
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
