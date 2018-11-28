#include "healthstate_display.h"

#include <QString>
#include <QResizeEvent>
#include <QDebug>
#include <QVariant>
#include <QHBoxLayout>

#include "dynamiclayoutbynet.h"
#include "staticlayoutbyxml.h"

QVariant transform(char *startPos,int size)
{
    switch(size)
    {
        case 1:
             return *startPos;
        case 2:/*to  short*/
             return ((*(startPos+1))<<8 )&(*startPos);

        case 4:/*to long*/
            return ((*(startPos+1))<<24 )&((*(startPos+2))<<16)&((*(startPos+1))<<8 )&(*startPos);
        default:
             qDebug()<<QString("function transform don't support %1 byte transform").arg(size);
             return -1;
    }
}

QString  transformToString(char *startPos,int size)
{
    QString s /*= QTextCodec::toUnicode(startPos,size)*/;
    return s;
}

HealthState_Display::HealthState_Display(QWidget *parent) :
    QWidget(parent)
{
    ifDynamicLayout = false;
    mDisplayElem = NULL;
    xlsPrinter = NULL;
}

void HealthState_Display::handleNetData(HealthData &result)
{
    HealthData *data = new HealthData;
    *data = result;
    mDisplayElem->startLayout(*data,true);
}

void HealthState_Display::resizeEvent(QResizeEvent *event)
{
    if(NULL != mDisplayElem)
        mDisplayElem->reLayout_BySize();
    event->accept();
}

HealthState_Display::~HealthState_Display()
{
    delete mDisplayElem;
    if(xlsPrinter != NULL)
    {
        xlsPrinter->save();
        xlsPrinter->close();
    }
    delete xlsPrinter;
}

/*!
 * \brief 开启动态布局
 * \param ifOpen =true   使用网络数据进行布局
 *        ifOpen = false 使用xls文件进行布局
 * \attention 使用了goto语句
 */
void HealthState_Display::openDynamicLayout(bool ifOpen)
{
    ifDynamicLayout = ifOpen;

    if(ifOpen)
    {
layoutByNet:
        mDisplayElem = new  DynamicLayoutByNet(this);
    }
    else
    {
        mDisplayElem = new  StaticLayoutByXml(this);

        if(!( mDisplayElem->startLayout(mNetData,false) ))
        {
            qDebug()<<"SoftWare：HealthState_Display.h:layout by xml file failed ,swithc to net";
            delete mDisplayElem;
            goto layoutByNet;
        }
    }

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(mDisplayElem);
    this->setLayout(layout);

    connect(this,SIGNAL(sendForHealthPanelResize()),mDisplayElem,SLOT(reLayout_BySize()));
}

/*!
 * \brief 开启记录数据
 * \param ifRecord = true 在每次接收到网络数据之后，将会自动写入xls文件
 */
void HealthState_Display::openRecordData(bool ifRecord)
{
    static bool hasSet = false;
    if(hasSet)
    {
        hasSet = true;
        ifRecordData = ifRecord;
        if(ifRecord)
        {
            xlsPrinter = new HealthState::XlsHealthState();
        }
    }
}

/*!
 * \brief 处理网络数据，如果是动态布局（网络）,根据信息进行布局
 * \param data 数据报文首地址
 * \param size 数据报文长度
 */
void HealthState_Display::handleNetData(char *data, int size)
{
    static const int netDataFixedLen = size;
    if(size != netDataFixedLen)
    {
        qDebug()<<"HealthState_Display:netData--HealthStaate ,dataLen lentgth change by time";
        return;
    }

    analysisNetData(data,size,mNetData);
    if(ifDynamicLayout)
    {
        if(NULL == mDisplayElem)
            qDebug()<<"HealthState_Display:hasn't select layout's generation method";
        else
        {
            bool result = false;
            result = mDisplayElem->startLayout(mNetData,true);
            if(!result)
            {

            }
        }
    }
    mDisplayElem->flushScreen(mNetData);

    if(ifRecordData)
    {
        xlsPrinter->addData(mNetData);
    }
}
/**
 * @brief 健康管理界面布局重置修复
 */
void HealthState_Display::recForHealthPanelResize(){
    emit sendForHealthPanelResize();
}

/*!
 * \brief 解析网络数据，从字符串将数据解析成对应的结构体
 * \param data    网络数据的首地址
 * \param size    网络数的大小
 * \param result  最后解析的数据的存储位置
 */
void HealthState_Display::analysisNetData(char *data, int size,HealthData &result)
{
    unsigned int index=0;
    unsigned short head,dataType,tail;
    unsigned long length;
    head =(unsigned short) transform(data+index,2).toUInt();index +=2;
    if(head != 0x1ACF)
    {
        qDebug()<<QString("healthState DataGram's headMark error:%1").arg(head);
        return;
    }

    dataType =(unsigned short)  transform(data+index,2).toUInt();index +=2;
    if(dataType != 0x2345)
    {
        qDebug()<<QString("healthState DataGram's dataType error:%1").arg(dataType);
        return;
    }


    length =(unsigned long)transform(data+index,4).toUInt();index +=4;
    if(length != size)
    {
        qDebug()<<QString("healthState DataGram's length error:%1").arg(length);
        return;
    }

    result.workStateNums = (unsigned long)transform(data+index,4).toUInt();index +=4;
    for(unsigned long i=0;i < result.workStateNums;i++)
    {
        WorkStruct temp;
        temp.name = transformToString(data+index,64);index +=64;
        temp.content = transformToString(data+index,64);index +=64;
        temp.contentColor = transform(data+index,2).toInt();index +=2;
        if(result.wsInfo.count() < result.workStateNums)
        {
            result.wsInfo.append(temp);
        }
        else
        {
            result.wsInfo.operator [](i).name = temp.name;
            result.wsInfo.operator [](i).content = temp.content;
            result.wsInfo.operator [](i).contentColor = temp.contentColor;
        }
    }


    result.subMachineNums = (unsigned long)transform(data+index,4).toUInt();index +=4;
    for(unsigned long i=0;i < result.subMachineNums;i++)
    {
        SubMachine temp;
        temp.name = transformToString(data+index,64);index +=64;
        temp.componetNum = (unsigned long)transform(data+index,4).toUInt();index +=4;
        for(unsigned long j=0;j<temp.componetNum;j++)
        {
            Componet  comp;
            comp.name = transformToString(data+index,64);index +=64;
            comp.state = (unsigned short) transform(data+index,2).toUInt();index +=2;


            if(result.smInfo.at(i).compInfo.count() < temp.componetNum )
            {
                temp.compInfo.append(comp);
            }
            else
            {
               result.smInfo.operator [](i).compInfo.operator[](j).name = comp.name;
               result.smInfo.operator [](i).compInfo.operator[](j).state = comp.state;
            }
        }

        if(result.smInfo.count() < result.subMachineNums)
        {
            result.smInfo.append(temp);
        }
        else
        {
            result.smInfo.operator [](i).name= temp.name;
            result.smInfo.operator [](i).componetNum = temp.componetNum;
        }
    }

    tail = (unsigned long)transform(data+index,4).toUInt();index +=4;
    if(tail != 0x0000FC1D)
    {
        qDebug()<<QString("healthState DataGram's tail error:%1").arg(tail);
        return;
    }
}
