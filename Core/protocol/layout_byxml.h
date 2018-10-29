#ifndef LAYOUT_BYXML_H
#define LAYOUT_BYXML_H

#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <QListWidget>

#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLabel>

#include <QString>
#include <QList>
#include <QByteArray>

#include "Base/util/fileutils.h"

namespace ProtocolHandle
{


enum Type{

       Empty = 0,
       /*!   输入框     */
       comboBox=1,            //下拉框
       checkBox,            //复选框
       radioButton,         //单选框

       textEdit,            //文本输入框
       valueEdit,           //数值输入框
       dateEdit,            //日期输入框
       timeEdit,            //时间输入框

       /*!    容器框        */
       dialog,              //对话框
       widget,              //普通框
       table,               //表格
       list,                //列表

       /*! 其它协议类型      */
       length,              //长度
       count                //计数个数
};


  struct pubHead{
      QByteArray name;         //名称
      Type     type;
      QWidget  *widget;       //实际生成的控件

      pubHead():name()
      {
          type = Empty;
          widget = NULL;
      }

  };
  struct Protocol:public pubHead {

      Protocol():pubHead()
        ,unit()
      {
          bytes =0;isSigned = false;
          bits = 0; offset = 0;
          weight = 1;
          precision = 1;
          maxValue = USHRT_MAX;
          minValue = 0;
      }

      /*! 以下内容只有1组有效：字节数-符号位，比特数-偏移量 */
      ushort bytes;         //字节数
      bool   isSigned;      //有无符号位
      ushort bits;          //比特位数
      ushort offset;        //偏移量
      float weight;         //权值
      float precision;      //精度
      QString unit;         //单位
      ushort maxValue;      //最大值
      ushort minValue;     //最小值

      QString displayText;  //显示文本
  };

  struct Display:public pubHead{
      bool isShown;         //可进行显示
      bool isEnabled;       //可编辑
      ushort column;        //进行显示的列数
      ushort width;         //宽度
      ushort height;        //高度

      Display():pubHead()
      {
          isShown = true;
          isEnabled = true;
          column = 1;
      }
  };






struct Element{

    Display     continer;      //容器
    QList<Protocol*> data;
    QList<Element*>  dataElem;

    //QByteArray getByteArray();

};


class  XMLLayout_Static : public RXmlParseMethod
{
   public:
       XMLLayout_Static(){data = new QList<Element*>();}
        //virtual ~TaskCntlDisplay_ParseXML();

    virtual bool  startParse(QDomNode & node);

public:
       QList<Element*> *data;


private:
        Element* parseProperty_Protocol(QDomNode &node,Element *element);            //  解析协议-属性
        Element* parseProperty_Display(QDomNode &node,Element *element);                //  解析显示-属性


       Protocol* parseAttributes(QDomNode &node);              //  解析特性
        void parseType(QDomNode &node,pubHead *element);       //解析类型
        Element* parseChild_Item(QDomNode &node,Element *element);

        void generateWidget(Element *elem);
};


}
#endif
