﻿#include "pluginxmlparse.h"

#include <QDebug>

namespace Core{

struct PluginNode{
    PluginNode():node("plugin"),id("id"),name("name"){}

    QString node;
    QString id;
    QString name;
};

struct NetworkNode{
    NetworkNode():node("network"),id("id"),type("type"),ip("ip"),port("port"),protocol("protocol"),multicast("multicast"),
        multiIp("multiIp"){}
    QString node;
    QString id;
    QString type;
    QString ip;
    QString port;
    QString protocol;
    QString multicast;
    QString multiIp;
};

struct MoudleNode{
    MoudleNode():node("module"),id("id"),closeable("closeable"),visible("visible"),floatable("floatable"),name("name"),protocols("protocols"),
            pluginId("plugin"),networkId("network"),layout("layout"){}

    QString node;
    QString id;
    QString closeable;
    QString visible;
    QString floatable;
    QString name;
    QString protocols;
    QString pluginId;
    QString networkId;
    QString layout;
};

PluginXmlParse::PluginXmlParse()
{

}

bool PluginXmlParse::startParse(QDomNode &rootNode)
{
    //plugin
    QDomNodeList plugins = rootNode.toElement().elementsByTagName("plugins");
    if(plugins.size() != 1)
        return false;
    parsePlugin(plugins.at(0));

    //network
    QDomNodeList nets = rootNode.toElement().elementsByTagName("net");
    if(nets.size() != 1)
        return false;
    parseNetwork(nets.at(0));

    //module
    QDomNodeList moudles = rootNode.toElement().elementsByTagName("moudles");
    if(moudles.size() != 1)
        return false;
    parseModule(moudles.at(0));

    return true;
}

void PluginXmlParse::parsePlugin(QDomNode &pluginsNode)
{
    QDomNodeList nodes = pluginsNode.toElement().childNodes();
    PluginNode pnode;
    plugins = new PluginMap;
    for(int i = 0; i < nodes.size();i++){
        QDomNode child = nodes.at(i).firstChild();
        Datastruct::PluginInfo pinfo;
        while(!child.isNull()){
            QDomElement childElement = child.toElement();
            QString tagname = childElement.tagName();
            if(tagname == pnode.id)
            {
                pinfo.id = childElement.text();
            }
            else if(tagname == pnode.name)
            {
                pinfo.name = childElement.text();
            }
            child = child.nextSibling();
        }
        plugins->insert(pinfo.id,pinfo);
    }
}

void PluginXmlParse::parseNetwork(QDomNode &networkNode)
{
    QDomNodeList nodes = networkNode.toElement().childNodes();
    NetworkNode pnode;
    networks = new NetworkMap;
    for(int i = 0; i < nodes.size();i++){
        QDomNode child = nodes.at(i).firstChild();
        Datastruct::NetworkInfo netinfo;
        while(!child.isNull()){
            QDomElement childElement = child.toElement();
            QString tagname = childElement.tagName();
            if(tagname == pnode.id){
                netinfo.id = childElement.text();
            }else if(tagname == pnode.ip){
                netinfo.baseInfo.ip = childElement.text();
            }else if(tagname == pnode.port){
                netinfo.baseInfo.port = childElement.text().toUShort();
            }else if(tagname == pnode.protocol){
                if(childElement.text().toLower() == "udp"){
                    netinfo.protocol = Datastruct::N_UDP;
                }else if(childElement.text().toLower() == "tcp"){
                    netinfo.protocol = Datastruct::N_TCP;
                }
            }else if(tagname == pnode.type){
                if(childElement.text().toLower() == "client"){
                    netinfo.baseInfo.connectionType = Datastruct::N_Client;
                }else if(childElement.text().toLower() == "server"){
                    netinfo.baseInfo.connectionType = Datastruct::N_Server;
                }
            }else if(tagname == pnode.multicast){
                netinfo.baseInfo.multicast = childElement.text().toInt();
            }else if(tagname == pnode.multiIp){
                netinfo.baseInfo.multicastIp = childElement.text();
            }
            child = child.nextSibling();
        }
        networks->insert(netinfo.id,netinfo);
    }
}

void PluginXmlParse::parseModule(QDomNode &moduleNode)
{
    QDomNodeList nodes = moduleNode.toElement().childNodes();
    MoudleNode pnode;
    modules = new ModuleMap;
    for(int i = 0; i < nodes.size();i++){
        QDomNode child = nodes.at(i).firstChild();
        Datastruct::ModuleInfo minfo;
        while(!child.isNull()){
            QDomElement childElement = child.toElement();
            QString tagname = childElement.tagName();
            if(tagname == pnode.id)
            {
                minfo.id = childElement.text();
            }
            else if(tagname == pnode.closeable)
            {
                minfo.closeable = childElement.text().toInt();
            }
            else if(tagname == pnode.visible)
            {
                minfo.visible = childElement.text().toInt();
            }
            else if(tagname == pnode.floatable)
            {
                minfo.floatable = childElement.text().toInt();
            }
            else if(tagname == pnode.name)
            {
                minfo.name = childElement.text();
            }
            else if(tagname == pnode.protocols)
            {
                minfo.protocols = childElement.text().split(";");
            }
            else if(tagname == pnode.pluginId)
            {
                minfo.pluginId = childElement.text();
            }
            else if(tagname == pnode.networkId)
            {
                minfo.networkId = childElement.text();
            }
            else if(tagname == pnode.layout)
            {
                QString layout = childElement.text().toLower();
                if(layout == "left"){
                    minfo.layout = Datastruct::LEFT;
                }else if(layout == "top"){
                    minfo.layout = Datastruct::TOP;
                }else if(layout == "right"){
                    minfo.layout = Datastruct::RIGHT;
                }else if(layout == "bottom"){
                    minfo.layout = Datastruct::BOTTOM;
                }
            }
            child = child.nextSibling();
        }
        modules->insert(minfo.id,minfo);
    }
}

}
