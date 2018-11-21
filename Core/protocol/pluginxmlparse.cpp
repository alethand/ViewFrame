#include "pluginxmlparse.h"

#include <QDebug>

#include "Base/util/rutil.h"

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
    MoudleNode():node("moudle"),id("id"),closeable("closeable"),visible("visible"),floatable("floatable"),name("name"),protocols("protocols"),
            pluginId("plugin"),networkId("network"),layout("layout"),geometry("geometry"),x("x"),y("y"),w("w"),h("h"){}

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
    QString geometry;
    QString x;
    QString y;
    QString w;
    QString h;
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

bool PluginXmlParse::startSave(QDomDocument &doc)
{
    QDomElement rootElement = doc.createElement("root");

    //plugin
    QDomElement plugins = doc.createElement("plugins");
    savePlugin(doc,plugins);
    rootElement.appendChild(plugins);

    //network
    QDomElement nets = doc.createElement("net");
    saveNetwork(doc,nets);
    rootElement.appendChild(nets);

    //modules
    QDomElement modules = doc.createElement("moudles");
    saveModule(doc,modules);
    rootElement.appendChild(modules);

    doc.appendChild(rootElement);

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
    modules = new ModuleList;
    for(int i = 0; i < nodes.size();i++){
        QDomNode child = nodes.at(i).firstChild();
        Datastruct::ModuleInfo minfo;
        while(!child.isNull()){
            QDomElement childElement = child.toElement();
            QString tagname = childElement.tagName();
            if(tagname == pnode.closeable)
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
            else if(tagname == pnode.geometry)
            {
                QDomNode geometryChild = child.firstChild();
                while(!geometryChild.isNull()){
                    QDomElement geometryElement = geometryChild.toElement();
                    QString gname = geometryElement.tagName();
                    if(gname == pnode.x)
                    {
                        minfo.geometry.setX(getRectSize(R_X,geometryElement.text()));
                    }
                    else if(gname == pnode.y)
                    {
                        minfo.geometry.setY(getRectSize(R_Y,geometryElement.text()));
                    }
                    else if(gname == pnode.w)
                    {
                        minfo.geometry.setWidth(getRectSize(R_W,geometryElement.text()));
                    }
                    else if(gname == pnode.h)
                    {
                        minfo.geometry.setHeight(getRectSize(R_H,geometryElement.text()));
                    }
                    geometryChild = geometryChild.nextSibling();
                }
            }
            child = child.nextSibling();
        }
        modules->push_back(minfo);
    }
}

void PluginXmlParse::savePlugin(QDomDocument &doc, QDomElement &pluginNodes)
{
    PluginMap::iterator iter = plugins->begin();
    PluginNode pnode;
    while(iter != plugins->end()){
        QDomElement plugin = doc.createElement(pnode.node);

        QDomElement id = doc.createElement(pnode.id);
        id.appendChild(doc.createTextNode(iter.value().id));
        QDomElement name = doc.createElement(pnode.name);
        name.appendChild(doc.createTextNode(iter.value().name));

        plugin.appendChild(id);
        plugin.appendChild(name);

        pluginNodes.appendChild(plugin);
        iter++;
    }
}

void PluginXmlParse::saveNetwork(QDomDocument &doc, QDomElement &network)
{
    NetworkMap::iterator iter = networks->begin();
    NetworkNode pnode;
    while(iter != networks->end()){
        QDomElement netNode = doc.createElement(pnode.node);

        QDomElement id = doc.createElement(pnode.id);
        id.appendChild(doc.createTextNode(iter.value().id));

        QDomElement ip = doc.createElement(pnode.ip);
        ip.appendChild(doc.createTextNode(iter.value().baseInfo.ip));

        QDomElement port = doc.createElement(pnode.port);
        port.appendChild(doc.createTextNode(QString::number(iter.value().baseInfo.port)));

        QString protocolText;
        switch(iter.value().protocol){
            case Datastruct::N_TCP:
                protocolText = "tcp";break;
            case Datastruct::N_UDP:
                protocolText = "udp";break;
            default:break;
        }
        QDomElement protocol = doc.createElement(pnode.protocol);
        protocol.appendChild(doc.createTextNode(protocolText));

        QString typeText;
        switch(iter.value().baseInfo.connectionType){
            case Datastruct::N_Client:
                typeText = "client";break;
            case Datastruct::N_Server:
                typeText = "server";break;
            default:break;
        }

        QDomElement type = doc.createElement(pnode.type);
        type.appendChild(doc.createTextNode(typeText));

        netNode.appendChild(id);
        netNode.appendChild(ip);
        netNode.appendChild(port);
        netNode.appendChild(protocol);
        netNode.appendChild(type);

        if(iter.value().protocol == Datastruct::N_UDP){
            QDomElement isMulti = doc.createElement(pnode.multicast);
            isMulti.appendChild(doc.createTextNode(QString::number((int)iter.value().baseInfo.multicast)));

            QDomElement multiIp = doc.createElement(pnode.multiIp);
            multiIp.appendChild(doc.createTextNode(iter.value().baseInfo.multicastIp));

            netNode.appendChild(isMulti);
            netNode.appendChild(multiIp);
        }
        network.appendChild(netNode);
        iter++;
    }
}

void PluginXmlParse::saveModule(QDomDocument &doc, QDomElement &moduleNodess)
{
    ModuleList::iterator iter = modules->begin();
    MoudleNode pnode;
    while(iter != modules->end()){
        QDomElement moduleNode = doc.createElement(pnode.node);
        Datastruct::ModuleInfo info = *iter;

        //layout
        QDomElement layout = doc.createElement(pnode.layout);
        QString layoutText;

        switch(info.layout){
            case Datastruct::TOP:layoutText = "top"; break;
            case Datastruct::BOTTOM:layoutText = "bottom"; break;
            case Datastruct::LEFT:layoutText = "left"; break;
            case Datastruct::RIGHT:layoutText = "right"; break;
            default:break;
        }
        layout.appendChild(doc.createTextNode(layoutText));

        //geometry
        QDomElement geometry = doc.createElement(pnode.geometry);
        QDomElement x = doc.createElement(pnode.x);
        x.appendChild(doc.createTextNode(QString::number(info.geometry.x())));
        QDomElement y = doc.createElement(pnode.y);
        y.appendChild(doc.createTextNode(QString::number(info.geometry.y())));
        QDomElement w = doc.createElement(pnode.w);
        w.appendChild(doc.createTextNode(QString::number(info.geometry.width())));
        QDomElement h = doc.createElement(pnode.h);
        h.appendChild(doc.createTextNode(QString::number(info.geometry.height())));

        geometry.appendChild(x);
        geometry.appendChild(y);
        geometry.appendChild(w);
        geometry.appendChild(h);

        //close
        QDomElement closeable = doc.createElement(pnode.closeable);
        closeable.appendChild(doc.createTextNode(QString::number((int)info.closeable)));

        //visible
        QDomElement visible = doc.createElement(pnode.visible);
        visible.appendChild(doc.createTextNode(QString::number((int)info.visible)));

        //floatable
        QDomElement floatable = doc.createElement(pnode.floatable);
        floatable.appendChild(doc.createTextNode(QString::number((int)info.floatable)));

        //name
        QDomElement name = doc.createElement(pnode.name);
        name.appendChild(doc.createTextNode(info.name));

        //protocols
        QDomElement protocols = doc.createElement(pnode.protocols);
        protocols.appendChild(doc.createTextNode(info.protocols.join(";")));

        //plugin
        QDomElement pluginId = doc.createElement(pnode.pluginId);
        pluginId.appendChild(doc.createTextNode(info.pluginId));

        //protocols
        QDomElement networkId = doc.createElement(pnode.networkId);
        networkId.appendChild(doc.createTextNode(info.networkId));


        moduleNode.appendChild(layout);
        moduleNode.appendChild(geometry);
        moduleNode.appendChild(closeable);
        moduleNode.appendChild(visible);
        moduleNode.appendChild(floatable);
        moduleNode.appendChild(name);
        moduleNode.appendChild(protocols);
        moduleNode.appendChild(pluginId);
        moduleNode.appendChild(networkId);

        moduleNodess.appendChild(moduleNode);
        iter++;
    }
}

int PluginXmlParse::getRectSize(RectPos pos,QString content)
{
    if(content.indexOf("%") >= 0){
        int percentInt = content.left(content.indexOf("%")).toInt();
        if(percentInt < 0 || percentInt >100)
            percentInt = 0;

        double percent = (double)percentInt/100;
        switch(pos){
            case R_X:
            case R_W:
                    return RUtil::screenGeometry().width() * percent;
                break;

            case R_Y:
            case R_H:
                    return RUtil::screenGeometry().height() * percent;
                break;

            default:
                break;
        }
    }else{
        return content.toInt();
    }

    return 0;
}

}
