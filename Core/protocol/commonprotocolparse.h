/*!
 *  @brief     通用网络协议解析
 *  @details   1.用于解析健康管理、数据显示模块的协议文件，解析格式为xml类型。 @n
 *               每个xml文件解析成一个单独的容器存放。由协议管理器提供对协议的管理。 @n
 *             2.网络数据接收、发送模块，也引用本协议解析生成的字段信息，对网络数据进行处理，具体的处理由数据处理类负责，@n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.07
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef COMMONPROTOCOLPARSE_H
#define COMMONPROTOCOLPARSE_H

#include "Base/util/fileutils.h"
#include "datastruct.h"

namespace Core{

class CommonProtocolParse : public Base::RXmlParseMethod
{
public:
    CommonProtocolParse();

    bool  startParse(QDomNode & rootNode);
    Datastruct::BaseProtocol getProtocol(){return parsedProtocol;}

private:
    bool parseItems(QDomNode & itemsNode);
    void parseItem(QDomNode &node);
    void parseType(QDomNode &node, Datastruct::FieldData &data);

private:
    Datastruct::BaseProtocol parsedProtocol;
};

}

#endif // COMMONPROTOCOLPARSE_H
