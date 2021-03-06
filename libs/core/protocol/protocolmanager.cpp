﻿#include "protocolmanager.h"

#include <QDir>
#include <QDebug>

#include "commonprotocolparse.h"

namespace Core{

ProtocolManager::ProtocolManager()
{

}

/*!
 * @brief 解析指定文件夹下的xml文件
 * @param[in] dirname 文件夹名
 * @return true 全部解析成功 false 存在解析失败的，已经跳过解析失败的文件
 */
bool ProtocolManager::parseLocalDir(QString dirname)
{
    QDir dir(dirname);
    if(!dir.exists())
        return false;

    QStringList filters;
    filters<<"*.xml";
    QFileInfoList fileList = dir.entryInfoList(filters);

    std::for_each(fileList.begin(),fileList.end(),[&](QFileInfo fileInfo){
        if(!parseLocalProtocol(fileInfo.absoluteFilePath()))
            return false;
    });
    return true;
}

/*!
 * @brief 解析指定的配置文件
 * @param[in] fullFilePath 待解析文件的全路径
 * @return true 解析成功 false 解析失败
 * @warning 此函数内部会出现2次 Datastruct::BaseProtocol的复制。可以进行优化
 */
bool ProtocolManager::parseLocalProtocol(QString fullFilePath)
{
    Base::RXmlFile xmlFile(fullFilePath);
    CommonProtocolParse parseMethod;
    xmlFile.setParseMethod(&parseMethod,false);
    if(xmlFile.startParse()){
        Datastruct::BaseProtocol bprotocol = parseMethod.getProtocol();
        protocols.insert(bprotocol.name,bprotocol);
        return true;
    }
    return false;
}

/*!
 * @brief 判断协议类型是否存在
 * @param[in] protocolType 协议类型
 * @return true 存在；false 不存在
 */
bool ProtocolManager::isExisted(int protocolType)
{
    BaseProtocolMap::iterator iter = protocols.begin();
    while(iter != protocols.end()){
        if(iter.value().type == protocolType){
            return true;
            break;
        }
        iter++;
    }

    return false;
}

/*!
 * @brief 根据协议名称获取对应的协议
 * @param[in]  protocolName 协议名
 * @param[in/out]  existed true：对应的协议是否存在;false：对应的协议不存在
 * @return 协议描述
 */
const Datastruct::BaseProtocol* ProtocolManager::getProtocol(QString protocolName, bool *existed)
{
    Datastruct::BaseProtocol bprotocol;
    *existed = protocols.contains(protocolName);
    if(*existed)
        return &(protocols.value(protocolName));

    return NULL;
}

/*!
 * @brief 根据协议名称获取对应的协议
 * @param[in]  protocolType 协议类型,如0xCCDD
 * @param[in/out]  existed true：对应的协议是否存在;false：对应的协议不存在
 * @return 协议描述
 */
const Datastruct::BaseProtocol* ProtocolManager::getProtocol(int protocolType,bool *existed)
{
    BaseProtocolMap::iterator iter = protocols.begin();
    while(iter != protocols.end()){
        if(iter.value().type == protocolType){
            *existed = true;
            return &(iter.value());
        }
        iter++;
    }

    return NULL;
}

}
