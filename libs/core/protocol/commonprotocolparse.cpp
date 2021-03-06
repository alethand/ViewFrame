﻿#include "commonprotocolparse.h"

#include <QDebug>

namespace Core{

class ProtocolStruct{
public:
    ProtocolStruct():name("name"),type("type"),startCode("start"),endCode("end"),startLen("startLen"),count("count")
      ,memoryBytes("memoryBytes"),endLen("endLen"),itemsCount("count"),
                itemsMemoryByte("memoryByte"),itemsLen("length"){}
    QString name;
    QString type;
    QString startCode;
    QString startLen;
    QString count;
    QString memoryBytes;
    QString endLen;
    QString endCode;

    QString itemsCount;
    QString itemsMemoryByte;
    QString itemsLen;
};

CommonProtocolParse::CommonProtocolParse()
{

}

/*!
 * \brief CommonProtocolParse::startParse
 * \param rootNode
 * \attention parsedProtocol中协议族列表中的个数为 Protocol下的子节点个数
 * \return
 */
bool CommonProtocolParse::startParse(QDomNode &rootNode)
{
    ProtocolStruct pstruct;
    QDomElement rootElement = rootNode.toElement();

    //解析协议信息
    if(rootElement.hasAttribute(pstruct.name))
        parsedProtocol.name = rootElement.attribute(pstruct.name);

    if(rootElement.hasAttribute(pstruct.type))
        parsedProtocol.type = rootElement.attribute(pstruct.type).toInt();

    if(rootElement.hasAttribute(pstruct.startCode))
        parsedProtocol.startCode = rootElement.attribute(pstruct.startCode).toInt();

    if(rootElement.hasAttribute(pstruct.startLen))
        parsedProtocol.startLen = rootElement.attribute(pstruct.startLen).toInt();

    if(rootElement.hasAttribute(pstruct.count))
        parsedProtocol.count = rootElement.attribute(pstruct.count).toInt();

    if(rootElement.hasAttribute(pstruct.memoryBytes))
        parsedProtocol.memoryBytes = rootElement.attribute(pstruct.memoryBytes).toInt();

    if(rootElement.hasAttribute(pstruct.endLen))
        parsedProtocol.endLen = rootElement.attribute(pstruct.endLen).toInt();

    if(rootElement.hasAttribute(pstruct.endCode))
        parsedProtocol.endCode = rootElement.attribute(pstruct.endCode).toInt();

    //解析Items信息
    if(rootElement.childNodes().size() < 1 )
        return false;

    bool result = false;
    for(int i = 0;i< rootElement.childNodes().size();i++){
        result = parseItems(rootElement.childNodes().at(i));
        if(!result)
           break;
    }

    return result;
}

/*!
 * @brief 解析items节点
 * @param[in] QDomNode 待解析的items节点
 */
bool CommonProtocolParse::parseItems(QDomNode & itemsNode)
{
    ProtocolStruct pstruct;
    Datastruct::SingleProtocol protocol;

    QDomElement itemsElement = itemsNode.toElement();
    if(itemsElement.hasAttribute(pstruct.itemsCount))
        protocol.count = itemsElement.attribute(pstruct.itemsCount).toInt();

    if(itemsElement.hasAttribute(pstruct.itemsMemoryByte))
        protocol.memoryBytes = itemsElement.attribute(pstruct.itemsMemoryByte).toInt();

    if(itemsElement.hasAttribute(pstruct.itemsLen))
        protocol.length = itemsElement.attribute(pstruct.itemsLen).toInt();
    if(itemsElement.hasAttribute("isProtocol"))
        protocol.isProtocol = true;
    else
         protocol.isProtocol = false;

    QDomNodeList items = itemsNode.childNodes();

    for(int i = 0; i < items.size(); i++)
    {
        Datastruct::Data_Word fieldData;
        fieldData.index = i;

        QDomNode itemNode = items.at(i);
        parseItem(itemNode.firstChild(),fieldData);

        protocol.fields.append(fieldData);
    }

    parsedProtocol.contents.append(protocol);
    return true;
}

/*!
 * @brief 解析item节点中子节点信息
 * @details  1.若item中有位操作，需要单独解析bits节点。
 * @param[in] itemNode 待解析的item节点
 */
void CommonProtocolParse::parseItem(QDomNode &node,Datastruct::Data_Word & fieldData)
{
    bool hasAtttribut = true;
    QDomElement * domElem = NULL;
    Datastruct::NodeInfo nodeInfo;

    while(!node.isNull() && hasAtttribut)
    {
        domElem = &(node.toElement());
        if(nodeInfo.itemName == domElem->tagName())
        {
            fieldData.name = domElem->text();
        }
        else if(nodeInfo.itemBytes == domElem->tagName())
        {
            fieldData.bytes = domElem->text().toUShort();
        }
        else if(nodeInfo.itemSigned == domElem->tagName() )
        {
            fieldData.isSigned = (bool)domElem->text().toUShort();
        }
        else if(nodeInfo.itemBits == domElem->tagName())
        {
            fieldData.bitOperator = true;
            parseBits(node,fieldData);
        }
        else if(nodeInfo.itemText == domElem->tagName())
        {
            fieldData.displayText = domElem->text();
        }
        else if(nodeInfo.itemEnable == domElem->tagName())
        {
            fieldData.enable = domElem->text().toInt();
        }
        else if(nodeInfo.itemVisible == domElem->tagName())
        {
            fieldData.visible = domElem->text().toInt();
        }
        else if(nodeInfo.itemWeight == domElem->tagName())
        {
            fieldData.weight = domElem->text().toFloat();
        }
        else if(nodeInfo.itemPrecision == domElem->tagName())
        {
            fieldData.precision = domElem->text().toFloat();
        }
        else if(nodeInfo.itemRepeat == domElem->tagName())
        {
            fieldData.repeat = domElem->text().toInt();
        }
        else if(nodeInfo.itemUnit == domElem->tagName())
        {
            fieldData.unit = domElem->text();
        }
        else  if(nodeInfo.itemType == domElem->tagName())
        {
              parseFieldType(node,fieldData);
        }
        else if(nodeInfo.itemDefaultValue == domElem->tagName())
        {
            fieldData.defaultValue = domElem->text();
        }
        else if(nodeInfo.itemComboxList == domElem->tagName())
        {
            QDomNodeList nodeList = node.childNodes();
            for(int i = 0;i < nodeList.count();i++)
                fieldData.list<<nodeList.at(i).toElement().text();
        }
        else if(nodeInfo.itemRange == domElem->tagName())
        {
             QDomNode *nod = NULL;
             QDomNodeList nodeList = node.childNodes();
             for(int i = 0;i < nodeList.count();i++)
             {
                 nod = &(nodeList.at(i));
                 if(nod->toElement().tagName() == nodeInfo.itemMax){
                      fieldData.range.maxValue = nod->toElement().text();
                 }
                 else  if(nod->toElement().tagName() == nodeInfo.itemMin){
                      fieldData.range.minValue = nod->toElement().text();
                 }
             }
        }
        else
        {
           hasAtttribut = false;
           continue;
        }
        node = node.nextSibling();
    }
}

/*!
 * @brief 解析控件显示类型
 * @param[in] node 待解析类型节点
 * @param[in] data 保存解析后的协议信息
 */
void CommonProtocolParse::parseFieldType(QDomNode &node, Datastruct::Data_Word &data)
{
    Datastruct::WidgetType wtype;
    QDomElement *domElem = NULL;
    domElem = &(node.toElement());

    if(domElem->text()== wtype.combox)
    {
        data.type = Datastruct::ControlType::ComboBox;
    }
    else if(domElem->text() == wtype.checkBox)
    {
        data.type = Datastruct::ControlType::CheckBox;
    }
    else if(domElem->text() == wtype.radioButton)
    {
        data.type = Datastruct::ControlType::RadioButton;
    }
    else if(domElem->text() == wtype.textEdit)
    {
        data.type = Datastruct::ControlType::TextEdit;
    }
    else if(domElem->text() == wtype.valueint)
    {
        data.type = Datastruct::ControlType::ValueIntEdit;
    }
    else if(domElem->text() == wtype.valuefloat)
    {
        data.type = Datastruct::ControlType::ValueFloatEdit;
    }
    else if(domElem->text() == wtype.dateEdit)
    {
        data.type = Datastruct::ControlType::DateEdit;
    }
    else if(domElem->text() == wtype.timeEdit)
    {
        data.type = Datastruct::ControlType::TimeEdit;
    }
    else if(domElem->text() == wtype.dialog)
    {
        data.type = Datastruct::ControlType::Dialog;
    }
    else if(domElem->text() == wtype.widget)
    {
        data.type = Datastruct::ControlType::Widget;
    }
    else if(domElem->text() == wtype.table)
    {
        data.type = Datastruct::ControlType::Table;
    }
    else if(domElem->text() == wtype.list)
    {
        data.type = Datastruct::ControlType::List;
    }
    else if(domElem->text() == wtype.count)
    {
        data.type = Datastruct::ControlType::Count;
    }
}


void CommonProtocolParse::parseGroup(QDomNode &node, Datastruct::Data_Word &data)
{
    QDomElement *domElem = NULL;
    domElem = &(node.toElement());
    if(domElem->tagName() == "Group")
    {

    }
}

/*!
 * @brief 解析位字段描述信息
 * @param[in] node 待解析类型节点
 * @param[in] data 保存解析后的协议信息
 */
void CommonProtocolParse::parseBits(QDomNode &node, Datastruct::Data_Word &data)
{
    QDomNodeList bits = node.childNodes();
    for(int i = 0; i < bits.size();i++)
    {
        QDomNode bitNode = bits.at(i);

        bool hasAtttribut = true;
        Datastruct::NodeInfo nodeInfo;

        Datastruct::Data_Bit bitData;
        bitData.index = i;

        QDomNode childNode = bitNode.firstChild();
        while(!childNode.isNull() && hasAtttribut)
        {
            QDomElement childElement = childNode.toElement();

            if(nodeInfo.itemName == childElement.tagName())
            {
                bitData.name = childElement.text();
            }
            else if(nodeInfo.bitStart == childElement.tagName())
            {
                bitData.startPos = childElement.text().toInt();
            }
            else if(nodeInfo.bitLast == childElement.tagName() )
            {
                bitData.last = childElement.text().toInt();
            }
            else if(nodeInfo.itemText == childElement.tagName())
            {
                bitData.displayText = childElement.text();
            }
            else if(nodeInfo.itemEnable == childElement.tagName())
            {
                bitData.enable = childElement.text().toInt();
            }
            else if(nodeInfo.itemVisible == childElement.tagName())
            {
                bitData.visible = childElement.text().toInt();
            }
            else if(nodeInfo.itemWeight == childElement.tagName())
            {
                bitData.weight = childElement.text().toFloat();
            }
            else if(nodeInfo.itemPrecision == childElement.tagName())
            {
                bitData.precision = childElement.text().toFloat();
            }
            else if(nodeInfo.itemUnit == childElement.tagName())
            {
                bitData.unit = childElement.text();
            }
            else if(nodeInfo.itemType == childElement.tagName())
            {
                  parseBitType(node,bitData);
            }
            else if(nodeInfo.itemDefaultValue == childElement.tagName())
            {
                bitData.defaultValue = childElement.text();
            }
            else if(nodeInfo.itemComboxList == childElement.tagName())
            {
                QDomNodeList nodeList = node.childNodes();
                for(int i = 0;i < nodeList.count();i++)
                    bitData.list<<nodeList.at(i).toElement().text();
            }
            else
            {
               hasAtttribut = false;
               continue;
            }
            childNode = childNode.nextSibling();
        }

        data.bits.bitList.append(bitData);
    }
}

/*!
 * @brief 解析bit位所显示的控件类型
 * @details 因bit位特殊，一般不作为浮点、日期等的显示，常用于整形数据显示。因此其解析的字段类型比FieldData较少。
 */
void CommonProtocolParse::parseBitType(QDomNode &node, Datastruct::Data_Bit &data)
{
    Datastruct::WidgetType wtype;
    QDomElement *domElem = NULL;
    domElem = &(node.toElement());

    if(domElem->text()== wtype.combox)
    {
        data.type = Datastruct::ControlType::ComboBox;
    }
    else if(domElem->text() == wtype.checkBox)
    {
        data.type = Datastruct::ControlType::CheckBox;
    }
    else if(domElem->text() == wtype.radioButton)
    {
        data.type = Datastruct::ControlType::RadioButton;
    }
    else if(domElem->text() == wtype.valueint)
    {
        data.type = Datastruct::ControlType::ValueIntEdit;
    }
}

}
