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

}
