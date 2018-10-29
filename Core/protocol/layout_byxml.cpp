#include "layout_byxml.h"
#include <QtXml/QDomNodeList>
#include <QtXml/QDomDocument>
#include <QDebug>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

namespace ProtocolHandle
{

/*!
 * \brief ProtocolHandle::XMLLayout_Static::parseProperty_Display
 *              解析显示属性
 * \param node
 */

Element *XMLLayout_Static::parseProperty_Display(QDomNode &node,Element *element)
{
    if(node.toElement().attributeNode("name").value() == "display")
    {
        QDomElement *domElem = NULL;

        QDomNodeList nodeList = node.childNodes();
        for(int i = 0;i < nodeList.count();i++)
        {
            domElem = &(nodeList.at(i).toElement());
            if(domElem->tagName() == "show") {
                 if(domElem->text().toUShort() == 1)
                     element->continer.isShown = true;
                 else
                      element->continer.isShown = false;
            }
            else if(domElem->tagName() == "enabled") {
                 element->continer.isEnabled = domElem->text().toUShort();
            }
            else  if(domElem->tagName() == "column"){
                 element->continer.column = domElem->text().toUShort();
            }
            else if(domElem->tagName() == "size"){
               ;
            }

        }
        return element;
    }
    return NULL;
}

bool XMLLayout_Static::startParse(QDomNode &node)
{
    QDomElement *domElem =&(node.toElement());
    Element *newElem = NULL;
    while(!node.isNull() && domElem != NULL)
    {
       if(domElem->tagName() == "widget")
       {
           newElem = new Element();
           if(domElem->attributeNode("type").value() == "dialog")
               newElem->continer.widget = new QDialog();
           if(domElem->hasAttribute("name"))
           {
               newElem->continer.widget->setWindowTitle(domElem->attributeNode("name").value());
           }
           parseProperty_Display(node.childNodes().at(0),newElem);
           parseProperty_Protocol(node.childNodes().at(1),newElem);

       }
       node = node.nextSibling();
       generateWidget(newElem);
       if(newElem)
          data->append(newElem);
    }
    data->first()->continer.widget->show();
    return true;
}

Element *XMLLayout_Static::parseProperty_Protocol(QDomNode &node,Element *element)
{
    if(node.toElement().attributeNode("name").value() == "protocol")
    {
        QDomNode *domNode =&(  node.firstChild() );
        while(!domNode->isNull() && domNode->toElement().tagName() == "item")
        {
            parseChild_Item(domNode->firstChild(),element);
            domNode = &(domNode->nextSibling());
        }

    }
    return element;
}

/*!
 * \brief XMLLayout_Static::parseAttributes  解析属性
 * \param node                               开始解析的节点
 * \return
 */
Protocol *XMLLayout_Static::parseAttributes(QDomNode &node)
{
     QDomElement *domElem = NULL;
     bool hasAtttribut = true;
     Protocol *newItem = new Protocol();

     while(!node.isNull() && hasAtttribut)
     {
         domElem = &(node.toElement());
         if("name" == domElem->tagName())
         {
             newItem->name = domElem->text().toUtf8();
         }
         else if("bytes" == domElem->tagName())
            newItem->bytes = domElem->text().toUShort();
         else if("signed" == domElem->tagName() )
         {
             if(newItem->bytes !=0)
                 newItem->isSigned = (bool)domElem->text().toUShort();
             else
                 qDebug()<<"error1";
         }
         else if("bits" == domElem->tagName() )
             newItem->bytes = domElem->text().toUShort();
         else if("offset" == domElem->tagName())
         {
             if(newItem->bits !=0)
                newItem->offset = domElem->text().toUShort();
             else
                 qDebug()<<"error2";
         }
         else if("text" == domElem->tagName())
         {
             newItem->displayText = domElem->text();
         }
         else if("weight" == domElem->tagName())
         {
             newItem->weight = domElem->text().toFloat();
         }
         else if("precision" == domElem->tagName())
             newItem->precision = domElem->text().toFloat();
         else if("unit" == domElem->tagName())
             newItem->unit = domElem->text();
         else  if("type" == domElem->tagName()){
               parseType(node,newItem);
         }
         else if("comboxList" == domElem->tagName()&&newItem->type == Type::comboBox
                 &&newItem->widget != NULL)
         {
             QDomNodeList nodeList = node.childNodes();
             for(int i = 0;i < nodeList.count();i++)
             {
                 ((QComboBox*)(newItem->widget))->addItem(nodeList.at(i).toElement().text());
             }
         }
         else if("range" == domElem->tagName())
         {
              QDomNode *nod = NULL;
              QDomNodeList nodeList = node.childNodes();
              for(int i = 0;i < nodeList.count();i++)
              {   nod =& (nodeList.at(i) );
                  if(nod->toElement().tagName() == "max")
                      newItem->maxValue = nod->toElement().text().toUShort();
                  else  if(nod->toElement().tagName() == "min")
                      newItem->minValue = nod->toElement().text().toUShort();
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
 * \brief XMLLayout_Static::parseType   分析类型
 * \param node
 * \param element
 * \return
 */
void XMLLayout_Static::parseType(QDomNode &node, pubHead *element)
{
    QDomElement *domElem = NULL;
    domElem = &(node.toElement());

    if(domElem->text()== "combox") {
        element->type = Type::comboBox;
        element->widget = new QComboBox();
    }
    else if(domElem->text() == "checkBox") {
        element->type = Type::checkBox;
        element->widget = new QCheckBox();
    }
    else if(domElem->text() == "radioButton") {
        element->type = Type::radioButton;
        element->widget = new QRadioButton();
    }
    else if(domElem->text() == "textEdit") {
        element->type = Type::textEdit;
        element->widget = new QLineEdit();
    }
    else if(domElem->text() == "valueint") {
        element->type = Type::valueEdit;
        element->widget = new QSpinBox();
    }
    else if(domElem->text() == "valuefloat") {
        element->type = Type::valueEdit;
        element->widget = new QDoubleSpinBox();
    }
    else if(domElem->text() == "dateEdit") {
        element->type = Type::dateEdit;
        element->widget = new QDateEdit();
    }
    else if(domElem->text() == "timeEdit") {
        element->type = Type::timeEdit;
        element->widget = new QTimeEdit();
    }
    else if(domElem->text() == "dialog") {
        element->type = Type::dialog;
        element->widget = new QDialog();
    }
    else if(domElem->text() == "widget") {
        element->type = Type::widget;
        element->widget = new QWidget();
    }
    else if(domElem->text() == "table") {
        element->type = Type::table;
        element->widget = new QTableWidget();
    }
    else if(domElem->text() == "list") {
        element->type = Type::list;
        element->widget = new QListWidget();
    }
    else if(domElem->text() == "length")
        element->type = Type::length;
    else if(domElem->text() == "count")
        element->type = Type::count;

}


/*!
 * \brief XMLLayout_Static::parseChild_Item  解析子节点Item
 * \param node
 * \param element
 * \return
 */
Element *XMLLayout_Static::parseChild_Item(QDomNode &node, Element *element)
{
    Protocol *newItem;


        newItem = parseAttributes(node);
        if(NULL != newItem )
        {
//            if(!childNode.isNull() && childNode.toElement().tagName() == "item")
//            {
//                Element *childElem = new Element();
//                parseChild_Item(childNode,childElem);
//                if(newItem->type == 0)
//                    newItem->widget = new QGroupBox();
//                element->dataElem.append(childElem);
//            }
//            else
                element->data.append(newItem);



    }
    return element;

}

void XMLLayout_Static::generateWidget(Element *elem)
{
    QWidget *temp_outerWid = NULL;
    QHBoxLayout *temp_layout;
    QLabel *temp_label = NULL;

    QLayout *layout = NULL;
    if(elem->continer.column == 1)
        layout = new QVBoxLayout();
    else
        layout = new QGridLayout();

    for(int i=0;i < elem->data.count();i++)
    {
        Protocol *temp = elem->data.at(i);


        switch(temp->type)
        {
          case comboBox:
          case textEdit:
          case valueEdit:
          case dateEdit:
          case timeEdit:
            temp_label = new QLabel(QString::fromUtf8(temp->name));
            temp_layout = new QHBoxLayout();
            temp_layout->addWidget(temp_label);
            temp_layout->addWidget(temp->widget);
            temp_outerWid = new QWidget();
            temp_outerWid->setLayout(temp_layout);
            layout->addWidget(temp_outerWid);
            break;
        }


    }
    //for(int i = 0;i<)
    elem->continer.widget->setLayout(layout);
   // elem->widget->show();
}











}










