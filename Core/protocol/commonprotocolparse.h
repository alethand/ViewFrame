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
 *      20181109:wey:调整协议格式字段，支持对多协议的支持；调整对位操作的解析；
 *
 *
 <protocol  name = "健康管理" type="0xCCDD" start="0x1ACF" end="0x0000FC1D" >
    <items count="1" memoryByte="0" length="50">
        <item>
            <name>回放数据类型</name>
            <bytes>4</bytes>
            <bits>
                <bit>
                    <name>回放速度</name>
                    <start>0</start>
                    <last>1</last>
                    <unit>度</unit>
                    <weight>1.0</weight>
                    <precision>1.0</precision>
                    <list>
                        <string>辐射源</string>
                        <string>全脉冲</string>
                        <string>中频采集</string>
                        <string>频谱</string>
                        <string>设备状态</string>
                    </list>
                </bit>
            </bits>
        </item>
    </items>

    <items count="-1" memoryByte="4" length="50">
        <item />
    </items>
</protocol>
 */
#ifndef COMMONPROTOCOLPARSE_H
#define COMMONPROTOCOLPARSE_H

#include "Base/util/fileutils.h"
#include "datastruct.h"
//!应使用内部类，仅由ProtocolManager进行调用，拒绝所有的其他外部使用者

namespace Core{

class CommonProtocolParse : public Base::RXmlParseMethod
{
public:
    CommonProtocolParse();

    bool  startParse(QDomNode & rootNode);
    Datastruct::BaseProtocol getProtocol(){return parsedProtocol;}

    static void parseBits(QDomNode &node, Datastruct::Data_Word &data);

private:
    bool parseItems(QDomNode & itemsNode);

    void parseItem(QDomNode &node, Datastruct::Data_Word &fieldData);
    void parseFieldType(QDomNode &node, Datastruct::Data_Word &data);
    void parseGroup(QDomNode &node, Datastruct::Data_Word &data);

    static void parseBitType(QDomNode &node, Datastruct::Data_Bit &data);

private:
    Datastruct::BaseProtocol parsedProtocol;
};

}

#endif // COMMONPROTOCOLPARSE_H
