#include "tasklayout.h"
#include <QtXml/QDomNodeList>
#include <QtXml/QDomDocument>
#include <QDebug>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

namespace TaskControlModel
{

struct NodeInfo{
    NodeInfo():window("window"),name("name"),width("width"),height("height"),layout("layout")
    ,type("type"),widget("widget"),groupBox("groupbox"),item("item"),items("items"),nodeShow("show"),nodeColumn("column"),nodeEnabled("enabled"),
    itemName("name"),itemBytes("bytes"),itemSigned("signed"),itemBits("bits"),itemOffset("offset"),itemText("text"),
    itemWeight("weight"),itemPrecision("precision"),itemUnit("unit"),itemType("type"),itemComboxList("comboxList"),itemRange("range"){

    }
    QString window;
    QString name;
    QString width;
    QString height;
    QString layout;
    QString type;

    QString widget;
    QString groupBox;

    QString item;
    QString items;

    QString nodeShow;
    QString nodeColumn;
    QString nodeEnabled;

    QString itemName;
    QString itemBytes;
    QString itemSigned;
    QString itemBits;
    QString itemOffset;
    QString itemText;
    QString itemWeight;
    QString itemPrecision;
    QString itemUnit;
    QString itemType;
    QString itemComboxList;
    QString itemRange;
};

TaskLayoutParse::TaskLayoutParse():RXmlParseMethod(),windowContainer(NULL),filedIndex(0)
{

}

TaskLayoutParse::~TaskLayoutParse()
{
    if(windowContainer){
        releaseContainer(windowContainer);
    }
}

/*!
 * @brief 递归释放容器
 * @param[in] container 待释放的容器数据信息
 */
void TaskLayoutParse::releaseContainer(Container *container)
{
    if(container->childContainer.size() == 0){
        std::for_each(container->fileds.begin(),container->fileds.end(),[&](Field* filed){
            delete filed;
        });
        container->fileds.clear();
    }else{
        std::for_each(container->childContainer.begin(),container->childContainer.end(),[&](Container* cter){
            releaseContainer(cter);
        });
    }
}


bool TaskLayoutParse::startParse(QDomNode &node)
{
    NodeInfo nodeInfo;
    QDomElement *domElem = &(node.toElement());
    if(!node.isNull() && domElem != NULL)
    {
       if(domElem->tagName().toLower() == nodeInfo.window)
       {
           windowContainer = new Container();

           if(domElem->hasAttribute(nodeInfo.type) && domElem->attributeNode(nodeInfo.type).value().toLower() == "dialog")
               windowContainer->continer.widget = new QDialog();

           if(domElem->hasAttribute(nodeInfo.name))
               windowContainer->continer.widget->setWindowTitle(domElem->attributeNode(nodeInfo.name).value());

           if(domElem->hasAttribute(nodeInfo.width))
               windowContainer->continer.data.width = domElem->attributeNode(nodeInfo.width).value().toInt();

           if(domElem->hasAttribute(nodeInfo.height))
               windowContainer->continer.data.height = domElem->attributeNode(nodeInfo.height).value().toInt();

           if(domElem->hasAttribute(nodeInfo.layout))
               windowContainer->continer.data.layout = parseLayout(domElem->attributeNode(nodeInfo.layout).value().toLower());

           QDomNodeList childList = node.childNodes();
           for(int i = 0; i < childList.size();i++){
               if(childList.at(i).toElement().tagName().toLower() == nodeInfo.widget){
                    Container * childContainer = new Container();
                    parseContainer(childList.at(i),childContainer);
                    windowContainer->childContainer.append(childContainer);
               }
           }
           generateWidget(windowContainer);
       }
    }

    return true;
}

/*!
 * @brief 解析容器及内部节点
 * @param[in] node 中间容器节点
 * @param[in] element 待保存解析后的信息
 */
void TaskLayoutParse::parseContainer(QDomNode &node, Container *element)
{
    QDomNodeList childList = node.childNodes();
    if(childList.size() == 2){
        NodeInfo nodeInfo;

        QDomElement elem = node.toElement();
        if(elem.attribute(nodeInfo.type).toLower() == nodeInfo.widget){
            element->continer.widget = new QWidget();
            element->continer.type = Widget;
        }
        else if(elem.attribute(nodeInfo.type).toLower() == nodeInfo.groupBox){
            QGroupBox * groupbox = new QGroupBox();
            groupbox->setTitle(elem.attribute(nodeInfo.name));
            element->continer.widget = groupbox;
            element->continer.type = Groupbox;
        }

        for(int i = 0; i < childList.size(); i++){
            if(childList.at(i).toElement().tagName() == nodeInfo.layout){
                parseLayout(childList.at(i),element);
            }else if(childList.at(i).toElement().tagName() == nodeInfo.items){
                parseItems(childList.at(i),element);
            }
        }
    }
}

/*!
 * @brief 解析容器布局属性
 * @param[in] node 布局节点
 * @param[in] element 保存解析后的容器信息
 */
void TaskLayoutParse::parseLayout(QDomNode &node,Container *element)
{
    NodeInfo nodeInfo;

    if(node.toElement().hasAttribute(nodeInfo.type))
        element->continer.data.layout = parseLayout(node.toElement().attribute(nodeInfo.type));

    QDomNodeList nodeList = node.childNodes();
    for(int i = 0;i < nodeList.count();i++)
    {
        QDomElement domElem = nodeList.at(i).toElement();
        if(domElem.tagName() == nodeInfo.nodeShow) {
             if(domElem.text().toUShort() == 1)
                 element->continer.data.isShown = true;
             else
                  element->continer.data.isShown = false;
        }
        else if(domElem.tagName() == nodeInfo.nodeEnabled) {
             element->continer.data.isEnabled = domElem.text().toUShort();
        }
        else  if(domElem.tagName() == nodeInfo.nodeColumn){
             element->continer.data.column = domElem.text().toUShort();
             element->continer.data.column = element->continer.data.column > 2 ? 2 : (element->continer.data.column < 1 ? 1 : element->continer.data.column);
        }
    }
}

/*!
 * @brief 解析协议-属性
 * @param[in] node 待解析的节点
 * @param[in/out] element 待保存的元素
 * @return
 */
void TaskLayoutParse::parseItems(QDomNode &node,Container *element)
{
    NodeInfo nodeInfo;
    QDomNode *domNode =&( node.firstChild() );
    while(!domNode->isNull() && domNode->toElement().tagName() == nodeInfo.item)
    {
        parseItem(domNode->firstChild(),element);
        domNode = &(domNode->nextSibling());
    }
}

/*!
 * @brief 解析字段中属性信息
 * @return
 */
Field *TaskLayoutParse::parseAttributes(QDomNode &node)
{
     QDomElement * domElem = NULL;
     bool hasAtttribut = true;
     Field * newItem = new Field();

     NodeInfo nodeInfo;

     while(!node.isNull() && hasAtttribut)
     {
         domElem = &(node.toElement());
         if(nodeInfo.itemName == domElem->tagName())
         {
             newItem->data.name = domElem->text();
         }
         else if(nodeInfo.itemBytes == domElem->tagName())
            newItem->data.bytes = domElem->text().toUShort();
         else if(nodeInfo.itemSigned == domElem->tagName() )
         {
             if(newItem->data.bytes !=0)
                 newItem->data.isSigned = (bool)domElem->text().toUShort();
             else
                 qDebug()<<"error1";
         }
         else if(nodeInfo.itemBits == domElem->tagName() )
             newItem->data.bytes = domElem->text().toUShort();
         else if(nodeInfo.itemOffset == domElem->tagName())
         {
             if(newItem->data.bits !=0)
                newItem->data.offset = domElem->text().toUShort();
             else
                 qDebug()<<"error2";
         }
         else if(nodeInfo.itemText == domElem->tagName())
         {
             newItem->data.displayText = domElem->text();
         }
         else if(nodeInfo.itemWeight == domElem->tagName())
         {
             newItem->data.weight = domElem->text().toFloat();
         }
         else if(nodeInfo.itemPrecision == domElem->tagName())
             newItem->data.precision = domElem->text().toFloat();
         else if(nodeInfo.itemUnit == domElem->tagName())
             newItem->data.unit = domElem->text();
         else  if(nodeInfo.itemType == domElem->tagName()){
               parseType(node,newItem);
         }
         else if(nodeInfo.itemComboxList == domElem->tagName()&&newItem->type == ControlType::ComboBox
                 &&newItem->widget != NULL)
         {
             QDomNodeList nodeList = node.childNodes();
             for(int i = 0;i < nodeList.count();i++)
             {
                 ((QComboBox*)(newItem->widget))->addItem(nodeList.at(i).toElement().text());
             }
         }
         else if(nodeInfo.itemRange == domElem->tagName())
         {
              QDomNode *nod = NULL;
              QDomNodeList nodeList = node.childNodes();
              for(int i = 0;i < nodeList.count();i++)
              {   nod =& (nodeList.at(i) );
                  if(nod->toElement().tagName() == "max")
                      newItem->data.maxValue = nod->toElement().text().toUShort();
                  else  if(nod->toElement().tagName() == "min")
                      newItem->data.minValue = nod->toElement().text().toUShort();
              }
         }
         else
         {
            hasAtttribut = false;
            continue;
         }
         node = node.nextSibling();
     }

     return newItem;
}

/*!
 * @brief 解析控件显示类型
 * @param[in] node 待解析类型节点
 * @param[in] element 解析后的控件
 */
void TaskLayoutParse::parseType(QDomNode &node, PubHead *element)
{
    QDomElement *domElem = NULL;
    domElem = &(node.toElement());

    if(domElem->text()== "combox") {
        element->type = ControlType::ComboBox;
        QComboBox * comb = new QComboBox();
        comb->setView(new QListView());
        element->widget = comb;
    }
    else if(domElem->text() == "checkBox") {
        element->type = ControlType::CheckBox;
        element->widget = new QCheckBox();
    }
    else if(domElem->text() == "radioButton") {
        element->type = ControlType::RadioButton;
        element->widget = new QRadioButton();
    }
    else if(domElem->text() == "textEdit") {
        element->type = ControlType::TextEdit;
        element->widget = new QLineEdit();
    }
    else if(domElem->text() == "valueint") {
        element->type = ControlType::ValueIntEdit;
        element->widget = new QSpinBox();
    }
    else if(domElem->text() == "valuefloat") {
        element->type = ControlType::ValueFloatEdit;
        element->widget = new QDoubleSpinBox();
    }
    else if(domElem->text() == "dateEdit") {
        element->type = ControlType::DateEdit;
        element->widget = new QDateEdit();
    }
    else if(domElem->text() == "timeEdit") {
        element->type = ControlType::TimeEdit;
        element->widget = new QTimeEdit();
    }
    else if(domElem->text() == "dialog") {
        element->type = ControlType::Dialog;
        element->widget = new QDialog();
    }
    else if(domElem->text() == "widget") {
        element->type = ControlType::Widget;
        element->widget = new QWidget();
    }
    else if(domElem->text() == "table") {
        element->type = ControlType::Table;
        element->widget = new QTableWidget();
    }
    else if(domElem->text() == "list") {
        element->type = ControlType::List;
        element->widget = new QListWidget();
    }
    else if(domElem->text() == "length")
        element->type = ControlType::Length;
    else if(domElem->text() == "count")
        element->type = ControlType::Count;
}

/*!
 * @brief 解析协议字段
 * @param[in]  node 待解析的item节点
 * @param[in]  element 待保存解析后的容器信息
 */
void TaskLayoutParse::parseItem(QDomNode &node, Container *element)
{
    Field * newItem = parseAttributes(node);
    if(NULL != newItem )
    {
        newItem->data.index = filedIndex++;
        element->fileds.append(newItem);
    }
}

/*!
 * @brief 依照解析后的布局生成控件
 * @note 1.用GridLayout布局来统一处理;
 *       2.采用递归对布局进行生成
 * @param[in] elem  容器
 */
void TaskLayoutParse::generateWidget(Container * container)
{
    QGridLayout * layout = new QGridLayout();
    container->continer.widget->setLayout(layout);
    if(container->childContainer.size() == 0){

        QWidget * filedContainer = NULL;
        for(int i = 0; i < container->fileds.size();i++)
        {
            Field *temp = container->fileds.at(i);
            switch(temp->type)
            {
              case ComboBox:
              case TextEdit:
              case ValueIntEdit:
              case ValueFloatEdit:
              case DateEdit:
              case TimeEdit:
                {
                    QLabel *temp_label = new QLabel(temp->data.name);
                    QHBoxLayout * temp_layout = new QHBoxLayout();
                    temp_layout->addWidget(temp_label);
                    temp_layout->addWidget(temp->widget);
                    filedContainer = new QWidget();
                    filedContainer->setLayout(temp_layout);
                 }
                break;
              default:
                break;
            }

            if(filedContainer){
                switch(container->continer.data.layout){
                    case Grid:
                            layout->addWidget(filedContainer,i/container->continer.data.column,i%container->continer.data.column,1,1);
                        break;
                    case Vertical:
                            layout->addWidget(filedContainer,i,0,1,1);
                        break;
                    case Horizonal:
                            layout->addWidget(filedContainer,0,i,1,1);
                        break;

                    default:
                        break;
                }
            }
        }
    }else{
        for(int i = 0; i < container->childContainer.size();i++)
        {
            generateWidget(container->childContainer.at(i));

            QWidget * child = container->childContainer.at(i)->continer.widget;
            switch(container->continer.data.layout){
                case Grid:
                        layout->addWidget(child,i/container->continer.data.column,i%container->continer.data.column,1,1);
                    break;
                case Vertical:
                        layout->addWidget(child,i,0,1,1);
                    break;
                case Horizonal:
                        layout->addWidget(child,0,i,1,1);
                    break;

                default:
                    break;
            }
        }
    }
}

/*!
 * @brief 解析布局类型
 * @attention 类型为小写英文
 * @param[in] layoutText 布局类型英文描述
 * @return 若存在返回对应类型的枚举值，否则返回None
 */
Layout TaskLayoutParse::parseLayout(QString layoutText)
{
    if(layoutText == "vertical")
        return Vertical;
    else if(layoutText == "horizonal")
        return Horizonal;
    else if(layoutText == "grid")
        return Grid;
    return None;
}

}
