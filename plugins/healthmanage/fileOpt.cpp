#include "fileOpt.h"

#include <QtXml/QDomNodeList>
#include <QtXml/QDomDocument>
#include <QRgb>
#include <QDebug>

namespace HealthState {


/*!
 * \brief transformFrHexToRGB   转换16进制颜色信息到QColor 例如#000000转换成白色
 * \param strColor  颜色字符串信息，e.g. #ff0000
 * \return  相应的颜色信息
 */
QColor transformFrHexToRGB(QString strColor)
{

   if(strColor.startsWith("0#"))
   {
      strColor = strColor.mid(2);
      unsigned short r,g,b;
      r = HexToOct( strColor.mid(0,2) );
      g = HexToOct( strColor.mid(2,2) );
      b = HexToOct( strColor.mid(4,2) );
      return QColor(r,g,b);
   }

   return QColor();
}

ParseXML_HealthStateLayout::ParseXML_HealthStateLayout()
    :Base::RXmlParseMethod()
{
    mHealthData = new HealthData();
}

ParseXML_HealthStateLayout::~ParseXML_HealthStateLayout()
{
    delete mHealthData;
}

/*!
 * @brief   根据不同的xml文件，统一设置解析的方式
 * @param[in]   node    xml文件的根节点
 * @attention  注意使用格式，先调用父节点的解析方式
 *              然后使用循环解析自己的方式，并把节点更新为下一个节点，有利于下一个解析方式的使用
 * @todo @1将解析的xml数据关于工作状态的颜色信息设置保存起来
 */
bool ParseXML_HealthStateLayout::startParse(QDomNode &node)
{
    LayoutAndDisplay::colorMap.clear();
    StateLamp::colorMap.clear();

    QDomElement root = node.toElement();
    if(root.tagName() != "HealthState")
    {
        qDebug()<<QString("ParseXML_HealthStateLayout:param  node isn't healState's tag,standardName %1 ").arg(root.tagName());
        return false;
    }

    node = root.firstChild();
    if(!node.isNull())
    {

        QDomElement wkColor = node.toElement();
        if(wkColor.tagName() == "WorkStateColor")
        {
            QDomNodeList colors = wkColor.childNodes() ;
            for(int i=0;i < colors.count();i++)
            {
                QDomElement colors_elem = colors.item(i).toElement();
                if(colors_elem.tagName()== "Mark")
                {
                    if(colors_elem.hasAttribute("num") && colors_elem.hasAttribute("color"))
                    {
                        if(colors_elem.attributeNode("num").value() != "others")
                        {
                            LayoutAndDisplay::colorMap.insert(colors_elem.attributeNode("num").value().toInt(),
                                                              QColor(transformFrHexToRGB(colors_elem.attributeNode("color").value())));
                        }
                        else
                            LayoutAndDisplay::othersColor = QColor(transformFrHexToRGB(colors_elem.attributeNode("color").value()));

                    }
                    else
                    {
                        qDebug()<<QString("ParseXML_HealthStateLayout:WorkStateColor failed,no attributes  num  or color");
                       return false;
                    }
                }
                else
                {
                    qDebug()<<QString("ParseXML_HealthStateLayout:WorkStateColor failed,no TagName 'Mark'")<<i;
                    return false;
                }
            }
        }
        else
        {
            qDebug()<<QString("ParseXML_HealthStateLayout:WorkStateColor failed,standardName %1 ").arg(wkColor.tagName());
            return false;
        }

        node = node.nextSibling();
        QDomElement subMColor = node.toElement();
        if(subMColor.tagName() == "SubMachineColor")
        {
            QDomNodeList colors = wkColor.childNodes() ;
            for(int i=0;i < colors.count();i++)
            {
                QDomElement colors_elem = colors.item(i).toElement();
                if(colors_elem.tagName()== "Mark")
                {
                    if(colors_elem.hasAttribute("num") && colors_elem.hasAttribute("color"))
                    {
                        if(colors_elem.attributeNode("num").value() != "others")
                        {
                            StateLamp::colorMap.insert(colors_elem.attributeNode("num").value().toInt(),
                                                       QColor(transformFrHexToRGB(colors_elem.attributeNode("color").value())));
                        }
                        else
                            StateLamp::othersColor = QColor(transformFrHexToRGB(colors_elem.attributeNode("color").value()));

                    }
                    else
                    {
                        qDebug()<<QString("ParseXML_HealthStateLayout:SubMachineColor failed,no attributes  num  or color");
                       return false;
                    }
                }
                else
                {
                    qDebug()<<QString("ParseXML_HealthStateLayout:WorkStateColor failed,no TagName \"Mark\"");
                   return false;
                }
            }
        }
        else
        {
            qDebug()<<QString("ParseXML_HealthStateLayout:SubMachineColor failed,standardName %1 ").arg(subMColor.tagName());
            return false;
        }

        node = node.nextSibling();
        QDomElement wkInfo = node.toElement();
        if(!handleWorkStateInfo(wkInfo))
        {
           return false;
        }

        node = node.nextSibling();
        QDomElement subMInfo = node.toElement();
        if(!handleSubMachineInfo(subMInfo))
        {
           return false;
        }
        node = node.nextSibling();
    }
    return true;
}

bool ParseXML_HealthStateLayout::handleWorkStateInfo(QDomElement &node)
{
    if(node.tagName() == "WorkState")
    {
        QDomNodeList infos = node.childNodes() ;
        mHealthData->workStateNums = infos.count();
        for(int i=0;i < infos.count();i++)
        {
            QDomElement infos_elem = infos.item(i).toElement();
            if(infos_elem.hasAttribute("id") )
            {
                struct WorkStruct  temp;
                temp.name = infos_elem.attributeNode("id").value();
                mHealthData->wsInfo.insert(i,temp);
            }
            else
            {
                qDebug()<<QString("ParseXML_HealthStateLayout:workState info no attributes  id");
                return false;
            }
        }
        return true;

    }
    else
    {
        qDebug()<<QString("ParseXML_HealthStateLayout:WorkState   failed,standardName %1 ").arg(node.tagName());
        return false;
    }
}

bool ParseXML_HealthStateLayout::handleSubMachineInfo(QDomElement &node)
{
    if(node.tagName() == "SubMachine")
    {
        QDomNodeList infos = node.childNodes() ;
        mHealthData->subMachineNums = infos.count();
        for(int i=0;i < infos.count();i++)
        {
            QDomElement infos_elem = infos.item(i).toElement();
            if(infos_elem.tagName() == "SubMachineItem")
            {
                if(infos_elem.hasAttribute("id"))
                {

                    QDomNodeList compInfos = infos_elem.childNodes() ;

                    SubMachine  tempSubMachine;
                    tempSubMachine.name = infos_elem.attributeNode("id").value();
                    tempSubMachine.componetNum =compInfos.count();

                    for(int j=0;j < compInfos.count();j++)
                    {
                        QDomElement info = compInfos.item(i).toElement();
                        if(info.tagName() == "ComponentItem" && info.hasAttribute("id") )
                        {
                            struct Componet tempComPonent;
                            tempComPonent.name = info.attributeNode("id").value();
                            tempSubMachine.compInfo.insert(j,tempComPonent);

                        }
                        else
                        {
                            qDebug()<<QString("ParseXML_HealthStateLayout:ComponentItem errors");
                            return false;
                        }
                    }
                    mHealthData->smInfo.insert(i,tempSubMachine);

                }
                else
                {
                    qDebug()<<QString("ParseXML_HealthStateLayout:workState info no attributes  id");
                    return false;
                }
            }
            else
            {
                qDebug()<<QString("ParseXML_HealthStateLayout:SubMachineItem info no tagName is Error:standardName SubMachineItem");
                return false;
            }
        }
        return true;
    }
    else
    {
        qDebug()<<QString("ParseXML_HealthStateLayout:SubMachine   failed,standardName %1 ").arg(node.tagName());
        return false;
    }
}


/*!
 * \brief HexToOct  16进制的颜色信息转换为rgb值
 * \param s 颜色的16进制信息，例如0xFF0000,仅传入FF,00,00
 * \return  相应的16进制对应的rgb值
 */
int HexToOct(QString s)
{
    if(s.length() ==2)
    {
        int total=0;
        for(int i=0;i<2;i++)
        {
            int temp =0;
            char c = s.at(i).toLower().toLatin1();
            switch(c)
            {
                case '0': temp =0;break;
                case '1': temp =1;break;
                case '2': temp =2;break;
                case '3': temp =3;break;
                case '4': temp =4;break;
                case '5': temp =5;break;
                case '6': temp =6;break;
                case '7': temp =7;break;
                case '8': temp =8;break;
                case '9': temp =9;break;
                case 'a': temp =10;break;
                case 'b': temp =11;break;
                case 'c': temp =12;break;
                case 'd': temp =13;break;
                case 'e': temp =14;break;
                case 'f': temp =15;break;

                default:
                    qDebug()<<"hex to rgb param string is error";
                    break;
            }
            total += temp*((i==0)?16:1 );
        }
        return total;
    }
    return 0;
}


XlsHealthState::XlsHealthState(bool isVisible)
    :TableXLSProxery(isVisible)
{
      mData = NULL;
      bisFirstReceive = false;
}

/*!
 * @brief 对接收到的健康信息，开启线程进行处理
 * @param[in] data 待保存数据
 */
void XlsHealthState::addData(HealthData &data)
{
    mData = &data;
    this->start();
}

/*!
 * @brief 初始化xls文件的第一行，即表头
 */
void XlsHealthState::initXlsFirstLine()
{
    bool isExist = false;
    this->openWorkSheet(1);
    for(unsigned long  i = 0;i<mData->workStateNums;i++)
    {
        this->setCellData(1,i+1,mData->wsInfo.at(i).name);
    }

    for(unsigned long i =0;i < mData->subMachineNums;i++)
    {
        isExist = this->openWorkSheet(i+2);
        if(!isExist)
            this->addWorkSheet(mData->smInfo.at(i).name);
        for(unsigned long j = 0;j < mData->smInfo.at(i).componetNum;j++)
        {
            this->setCellData(1,j+1,mData->smInfo.at(i).compInfo.at(j).name);
        }
    }
}

void XlsHealthState::writeData()
{
    QString value;
    this->openWorkSheet(1);
    int rowCount = this->getRowCount();
    for(unsigned long i = 0;i<mData->workStateNums;i++)
    {
        value  = mData->wsInfo.at(i).content +" ["+QString::number(mData->wsInfo.at(i).contentColor) + "]";
        this->setCellData(rowCount+1,i+1,value);
    }


    for(unsigned long i =0;i < mData->subMachineNums;i++)
    {
        this->openWorkSheet(i+2);
        rowCount = this->getRowCount();
        for(unsigned long j = 0;j < mData->smInfo.at(i).componetNum;j++)
        {
            this->setCellData(rowCount+1,j,mData->smInfo.at(i).compInfo.at(j).state);
        }
    }
}

void XlsHealthState::run()
{
    TableXLSProxery::run();

    if(NULL == mData)
        return;

    if(!bisFirstReceive)
    {
        initXlsFirstLine();
        bisFirstReceive = true;
    }
    writeData();

    save();
    bIsSaveAlready = false;     //传来一次数据保存一次
}

}
