#include "commonprotocolparse.h"

#include <QDebug>

namespace Core{

class ProtocolStruct{
public:
    ProtocolStruct():name("name"),type("type"),startCode("start"),endCode("end"){}
    QString name;
    QString type;
    QString startCode;
    QString endCode;
};

CommonProtocolParse::CommonProtocolParse()
{

}

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

    if(rootElement.hasAttribute(pstruct.endCode))
        parsedProtocol.endCode = rootElement.attribute(pstruct.endCode).toInt();

    //解析字段信息
    if(rootElement.childNodes().size() != 1 )
        return false;

    return parseItems(rootElement.childNodes().at(0));
}

/*!
 * @brief 解析items节点
 * @param[in] QDomNode 待解析的items节点
 */
bool CommonProtocolParse::parseItems(QDomNode & itemsNode)
{
    QDomNodeList items = itemsNode.childNodes();
    for(int i = 0; i < items.size(); i++){
        QDomNode itemNode = items.at(i);
        parseItem(itemNode.firstChild());
    }
    return true;
}

/*!
 * @brief 解析item节点中子节点信息
 * @param[in] itemNode 待解析的item节点
 */
void CommonProtocolParse::parseItem(QDomNode &node)
{
    Datastruct::FieldData fieldData;
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
            if(fieldData.bytes !=0)
                fieldData.isSigned = (bool)domElem->text().toUShort();
            else
                qDebug()<<"error1";
        }
        else if(nodeInfo.itemBits == domElem->tagName())
        {
            fieldData.bytes = domElem->text().toUShort();
        }
        else if(nodeInfo.itemOffset == domElem->tagName()){

            if(fieldData.bits !=0)
               fieldData.offset = domElem->text().toUShort();
            else
                qDebug()<<"error2";
        }
        else if(nodeInfo.itemText == domElem->tagName())
        {
            fieldData.displayText = domElem->text();
        }
        else if(nodeInfo.itemWeight == domElem->tagName())
        {
            fieldData.weight = domElem->text().toFloat();
        }
        else if(nodeInfo.itemPrecision == domElem->tagName())
        {
            fieldData.precision = domElem->text().toFloat();
        }
        else if(nodeInfo.itemUnit == domElem->tagName())
        {
            fieldData.unit = domElem->text();
        }
        else  if(nodeInfo.itemType == domElem->tagName())
        {
              parseType(node,fieldData);
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
             {   nod =& (nodeList.at(i) );
                 if(nod->toElement().tagName() == "max"){
//                      fieldData.maxValue = nod->toElement().text().toUShort();
                 }
                 else  if(nod->toElement().tagName() == "min"){
//                      fieldData.minValue = nod->toElement().text().toUShort();
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

    parsedProtocol.protocol.fields.append(fieldData);
}

/*!
 * @brief 解析控件显示类型
 * @param[in] node 待解析类型节点
 * @param[in] data 保存解析后的协议信息
 */
void CommonProtocolParse::parseType(QDomNode &node, Datastruct::FieldData &data)
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
}

}
