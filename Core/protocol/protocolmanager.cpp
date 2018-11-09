#include "protocolmanager.h"

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
 * @brief 根据协议名称获取对应的协议
 * @param[in]  protocolName 协议名
 * @param[in/out]  existed true：对应的协议是否存在;false：对应的协议不存在
 * @return 协议描述
 */
Datastruct::BaseProtocol ProtocolManager::getProtocol(QString protocolName, bool &existed)
{
    Datastruct::BaseProtocol bprotocol;
    existed = protocols.contains(protocolName);
    if(existed)
        bprotocol = protocols.value(protocolName);

    return bprotocol;
}

/*!
 * @brief 根据协议名称获取对应的协议
 * @param[in]  protocolType 协议类型,如0xCCDD
 * @param[in/out]  existed true：对应的协议是否存在;false：对应的协议不存在
 * @return 协议描述
 */
Datastruct::BaseProtocol ProtocolManager::getProtocol(int protocolType,bool & existed)
{
    Datastruct::BaseProtocol bprotocol;
    BaseProtocolMap::iterator iter = protocols.begin();
    while(iter != protocols.end()){
        if(iter.value().type == protocolType){
            existed = true;
            bprotocol = iter.value();
            break;
        }
        iter++;
    }

    return bprotocol;
}

}
