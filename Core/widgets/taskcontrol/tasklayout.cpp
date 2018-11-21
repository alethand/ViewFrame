#include "tasklayout.h"
#include <QtXml/QDomNodeList>
#include <QtXml/QDomDocument>
#include <QDebug>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "protocol/commonprotocolparse.h"

namespace TaskControlModel
{

TaskLayoutParse::TaskLayoutParse():Base::RXmlParseMethod(),windowContainer(NULL),filedIndex(0)
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
void TaskLayoutParse::releaseContainer(Datastruct::Container *container)
{
    if(container->childContainer.size() == 0){
        std::for_each(container->fileds.begin(),container->fileds.end(),[&](Datastruct::Field* filed){
            delete filed;
        });
        container->fileds.clear();
    }else{
        std::for_each(container->childContainer.begin(),container->childContainer.end(),[&](Datastruct::Container* cter){
            releaseContainer(cter);
        });
    }
}


bool TaskLayoutParse::startParse(QDomNode &node)
{
    Datastruct::NodeInfo nodeInfo;
    QDomElement *domElem = &(node.toElement());
    if(!node.isNull() && domElem != NULL)
    {
       if(domElem->tagName().toLower() == nodeInfo.window)
       {
           windowContainer = new Datastruct::Container();

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
                    Datastruct::Container * childContainer = new Datastruct::Container();
                    parseContainer(childList.at(i),childContainer);
                    windowContainer->childContainer.append(childContainer);
               }
           }
           generateWidget(windowContainer);
           return true;
       }
    }
    return false;
}

/*!
 * @brief 解析容器及内部节点
 * @param[in] node 中间容器节点
 * @param[in] element 待保存解析后的信息
 */
void TaskLayoutParse::parseContainer(QDomNode &node, Datastruct::Container *element)
{
    QDomNodeList childList = node.childNodes();
    if(childList.size() == 2){
        Datastruct::NodeInfo nodeInfo;

        QDomElement elem = node.toElement();
        if(elem.attribute(nodeInfo.type).toLower() == nodeInfo.widget){
            element->continer.widget = new QWidget();
            element->continer.type = Datastruct::Widget;
        }
        else if(elem.attribute(nodeInfo.type).toLower() == nodeInfo.groupBox){
            QGroupBox * groupbox = new QGroupBox();
            groupbox->setTitle(elem.attribute(nodeInfo.name));
            element->continer.widget = groupbox;
            element->continer.type = Datastruct::Groupbox;
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
void TaskLayoutParse::parseLayout(QDomNode &node,Datastruct::Container *element)
{
    Datastruct::NodeInfo nodeInfo;

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
 */
void TaskLayoutParse::parseItems(QDomNode &node,Datastruct::Container *element)
{
    Datastruct::NodeInfo nodeInfo;
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
Datastruct::Field *TaskLayoutParse::parseAttributes(QDomNode &node)
{
     QDomElement * domElem = NULL;
     bool hasAtttribut = true;
     Datastruct::Field * newItem = new Datastruct::Field();

     Datastruct::NodeInfo nodeInfo;

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
            newItem->data.isSigned = (bool)domElem->text().toUShort();
         }
         else if(nodeInfo.itemBits == domElem->tagName() )
         {
             newItem->data.bitOperator = true;
             Core::CommonProtocolParse::parseBits(node,newItem->data);
         }
         else if(nodeInfo.itemText == domElem->tagName())
         {
             newItem->data.displayText = domElem->text();
         }
         else if(nodeInfo.itemEnable == domElem->tagName())
         {
             newItem->data.enable = domElem->text().toInt();
         }
         else if(nodeInfo.itemVisible == domElem->tagName())
         {
             newItem->data.visible = domElem->text().toInt();
         }
         else if(nodeInfo.itemWeight == domElem->tagName())
         {
             newItem->data.weight = domElem->text().toFloat();
         }
         else if(nodeInfo.itemPrecision == domElem->tagName())
         {
             newItem->data.precision = domElem->text().toFloat();
         }
         else if(nodeInfo.itemUnit == domElem->tagName())
         {
             newItem->data.unit = domElem->text();
         }
         else  if(nodeInfo.itemType == domElem->tagName())
         {
               parseType(node,newItem);
         }
         else if(nodeInfo.itemDefaultValue == domElem->tagName())
         {
             newItem->data.defaultValue = domElem->text();
         }
         else if(nodeInfo.itemComboxList == domElem->tagName()&&newItem->type == Datastruct::ControlType::ComboBox
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
                  if(nod->toElement().tagName() == nodeInfo.itemMax){
                       newItem->data.range.maxValue = nod->toElement().text();
                  }
                  else  if(nod->toElement().tagName() == nodeInfo.itemMin){
                       newItem->data.range.minValue = nod->toElement().text();
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

     return newItem;
}

/*!
 * @brief 解析控件显示类型
 * @param[in] node 待解析类型节点
 * @param[in] element 解析后的控件
 */
void TaskLayoutParse::parseType(QDomNode &node, Datastruct::PubHead *element)
{
    QDomElement *domElem = NULL;
    domElem = &(node.toElement());

    Datastruct::WidgetType wtype;

    int fixedHeight = 24;

    if(domElem->text()== wtype.combox) {
        element->type = Datastruct::ControlType::ComboBox;
        QComboBox * comb = new QComboBox();
        QListView * listView = new QListView();
        listView->setAttribute(Qt::WA_TranslucentBackground);
        listView->setWindowFlags(listView->windowFlags() | Qt::FramelessWindowHint);
        comb->setView(listView);
        element->widget = comb;
        element->widget->setFixedHeight(fixedHeight);
    }
    else if(domElem->text() == wtype.checkBox) {
        element->type = Datastruct::ControlType::CheckBox;
        element->widget = new QCheckBox();
        element->widget->setFixedHeight(fixedHeight);
    }
    else if(domElem->text() == wtype.radioButton) {
        element->type = Datastruct::ControlType::RadioButton;
        element->widget = new QRadioButton();
        element->widget->setFixedHeight(fixedHeight);
    }
    else if(domElem->text() == wtype.textEdit) {
        element->type = Datastruct::ControlType::TextEdit;
        element->widget = new QLineEdit();
    }
    else if(domElem->text() == wtype.valueint) {
        element->type = Datastruct::ControlType::ValueIntEdit;
        element->widget = new QSpinBox();
        element->widget->setFixedHeight(fixedHeight);
    }
    else if(domElem->text() == wtype.valuefloat) {
        element->type = Datastruct::ControlType::ValueFloatEdit;
        element->widget = new QDoubleSpinBox();
        element->widget->setFixedHeight(fixedHeight);
    }
    else if(domElem->text() == wtype.dateEdit) {
        element->type = Datastruct::ControlType::DateEdit;
        element->widget = new QDateEdit();
        element->widget->setFixedHeight(fixedHeight);
    }
    else if(domElem->text() == wtype.timeEdit) {
        element->type = Datastruct::ControlType::TimeEdit;
        element->widget = new QTimeEdit();
        element->widget->setFixedHeight(fixedHeight);
    }
    else if(domElem->text() == wtype.dialog) {
        element->type = Datastruct::ControlType::Dialog;
        element->widget = new QDialog();
    }
    else if(domElem->text() == wtype.widget) {
        element->type = Datastruct::ControlType::Widget;
        element->widget = new QWidget();
    }
    else if(domElem->text() == wtype.table) {
        element->type = Datastruct::ControlType::Table;
        element->widget = new QTableWidget();
    }
    else if(domElem->text() == wtype.list) {
        element->type = Datastruct::ControlType::List;
        element->widget = new QListWidget();
    }

    if(element->widget)
        element->widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

/*!
 * @brief 解析协议字段
 * @param[in]  node 待解析的item节点
 * @param[in]  element 待保存解析后的容器信息
 */
void TaskLayoutParse::parseItem(QDomNode &node, Datastruct::Container *element)
{
    Datastruct::Field * newItem = parseAttributes(node);
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
void TaskLayoutParse::generateWidget(Datastruct::Container * container)
{
    QGridLayout * layout = new QGridLayout();
    container->continer.widget->setLayout(layout);
    if(container->childContainer.size() == 0){

        QWidget * filedContainer = NULL;
        for(int i = 0; i < container->fileds.size();i++)
        {
            Datastruct::Field *temp = container->fileds.at(i);
            switch(temp->type)
            {
              case Datastruct::ComboBox:
              case Datastruct::TextEdit:
              case Datastruct::ValueIntEdit:
              case Datastruct::ValueFloatEdit:
              case Datastruct::DateEdit:
              case Datastruct::TimeEdit:
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
                    case Datastruct::Grid:
                            layout->addWidget(filedContainer,i/container->continer.data.column,i%container->continer.data.column,1,1);
                        break;
                    case Datastruct::Vertical:
                            layout->addWidget(filedContainer,i,0,1,1);
                        break;
                    case Datastruct::Horizonal:
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
                case Datastruct::Grid:
                        layout->addWidget(child,i/container->continer.data.column,i%container->continer.data.column,1,1);
                    break;
                case Datastruct::Vertical:
                        layout->addWidget(child,i,0,1,1);
                    break;
                case Datastruct::Horizonal:
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
Datastruct::Layout TaskLayoutParse::parseLayout(QString layoutText)
{
    if(layoutText == "vertical")
        return Datastruct::Vertical;
    else if(layoutText == "horizonal")
        return Datastruct::Horizonal;
    else if(layoutText == "grid")
        return Datastruct::Grid;
    return Datastruct::None;
}

}
