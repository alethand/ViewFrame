﻿/*!
 *  @brief     插件xml文件解析
 *  @details   1.解析plugins.xml文件
 *             2.保存修改后的plugin.xml文件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINXMLPARSE_H
#define PLUGINXMLPARSE_H

#include <core/protocol/datastruct.h>
#include <base/util/fileutils.h>

#include <QMap>

namespace Core{

typedef QList<Datastruct::ModuleInfo> ModuleList;
typedef QMap<QString,Datastruct::NetworkInfo> NetworkMap;
typedef QMap<QString,Datastruct::PluginInfo> PluginMap;

class PluginXmlParse : public Base::RXmlParseMethod
{
public:
    PluginXmlParse();

    bool  startParse(QDomNode & rootNode);
    bool  startSave(QDomDocument & doc);

    enum RectPos{
        R_X,
        R_Y,
        R_W,
        R_H
    };

    ModuleList * getModules(){return this->modules;}
    NetworkMap * getNetworks(){return this->networks;}
    PluginMap * getPlugins(){return this->plugins;}

private:
    void parsePlugin(QDomNode & pluginsNode);
    void parseNetwork(QDomNode & networkNode);
    void parseModule(QDomNode & moduleNode);

    void savePlugin(QDomDocument & doc, QDomElement & pluginNodes);
    void saveNetwork(QDomDocument & doc,QDomElement & network);
    void saveModule(QDomDocument & doc, QDomElement & moduleNodess);

    int getRectSize(RectPos, QString content);

private:
    ModuleList * modules;
    NetworkMap * networks;
    PluginMap * plugins;
};

}

#endif // PLUGINXMLPARSE_H
