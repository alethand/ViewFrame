#include "pluginloader.h"

#include "file/programfilepath.h"

namespace Core{

PluginLoader::PluginLoader():parsemethod(NULL)
{

}

bool PluginLoader::initConfigFile()
{
    ProgramFilePath filePath;
    Base::RXmlFile file(filePath.pluginFile);
    parsemethod = new PluginXmlParse;
    file.setParseMethod(parsemethod,false);
    return file.startParse();
}

}
