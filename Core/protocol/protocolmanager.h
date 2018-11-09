/*!
 *  @brief     协议管理器
 *  @details   1.负责维护所有解析产生的协议数据，每个协议之间不存在相互的影响 @n
 *             2.通过向网络数据处理以及显示模块提供解析后的字段内容 @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PROTOCOLMANAGER_H
#define PROTOCOLMANAGER_H

#include <QMap>
#include "datastruct.h"

namespace Core{

class ProtocolManager
{
public:
    ProtocolManager();

    bool parseLocalDir(QString dirname);
    bool parseLocalProtocol(QString fullFilePath);

    Datastruct::BaseProtocol getProtocol(QString protocolName,bool & existed);
    Datastruct::BaseProtocol getProtocol(int protocolType,bool & existed);

    int size(){return protocols.size();}

private:
    typedef QMap<QString,Datastruct::BaseProtocol> BaseProtocolMap;

    BaseProtocolMap protocols;
};

}

#endif // PROTOCOLMANAGER_H
