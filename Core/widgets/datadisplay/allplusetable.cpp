#include "allplusetable.h"
#include <QSet>
#include <QMap>

//#include "table.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmodelcustom.h"
#include "../Util/dataexportandprint.h"
#include <iostream>
#include <QHeaderView>

#include "widgets/datadisplay/Graphics/scatterdiagram.h"
#include "widgets/datadisplay/Graphics/histogram.h"
using namespace std;
using namespace Diagram;

namespace DataView {
const int PACKAGE_START=0x1ACF1ACF; //包头
const int PACKAGE_END=0xFC1DFC1D;   //包尾
const int MIN_PACKAGE_LEN=36;       //数据包最小长度


AllPluseDock::AllPluseDock(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_ALL_PLUS_TABLE,parent)
{
    RSingleton<Base::Subject>::instance()->attach(this);

    ininData();
    initView();
    switchModeTo(RealTime);
    this->setWindowTitle(QStringLiteral("全脉冲数据"));
    retranslateUi();

    QObject::connect(btn_load,SIGNAL(clicked()),this,SLOT(on_btn_load_clicked()));
    QObject::connect(&view_Statistic,SIGNAL(clicked(QModelIndex)),this,SLOT(statistic_ShowOrginInfo(QModelIndex)));
    QObject::connect(radioButton_RealityShow,SIGNAL(clicked()),this,SLOT(on_radioButton_RealityShow_clicked()));
    QObject::connect(radioButton_HistoryShow,SIGNAL(clicked()),this,SLOT(on_radioButton_HistoryShow_clicked()));
}

AllPluseDock::~AllPluseDock()
{
}

bool AllPluseDock::initialize()
{
//    Q_D(AllPluseDock);
//    setWidget(d->mainWidget);
    return true;
}

void AllPluseDock::release()
{

}

QString AllPluseDock::pluginName()
{
    return tr("DataDisplay panel");
}

void AllPluseDock::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}



void AllPluseDock::switchModeTo(AllPluseDock::WorkMode mode)
{
    switch(mode)
    {
     case RealTime/*实时模式*/:

                    viewRT_Origin.setModel(&modelRT_originData);
                    mWidget->setCurrentIndex(0);
                    break;

     case Historical/*历史模式*/:

                    viewHS_Origin.setModel(&modelHS_orginData);
                    view_Statistic.setModel(&model_StatisticData);
                     mWidget->setCurrentIndex(1);
                    break;
    }
}

void AllPluseDock::initView()
{

    viewRT_Origin.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    /*! 能量条*/
    energvGraphics = new EnergyBar(EnergyBar::Left_toRigth);
    QStringList properList;
    properList<<QStringLiteral("最大值")<<QStringLiteral("最小值")<<QStringLiteral("均值")<<QStringLiteral("方差");
    energvGraphics->setProperties(properList);
    energvGraphics->setVlaueRange(0,100);
    energvGraphics->setValue(50);
    energvGraphics->setFixedHeight(300);

    /*! 按钮 */
    QWidget *radioWidget = new QWidget();
    radioButton_RealityShow = new QRadioButton(radioWidget);
    radioButton_HistoryShow = new QRadioButton(radioWidget);
    btn_load = new QPushButton(radioWidget);
    radioWidget->setFixedHeight(50);

    btn_load->setText(QObject::tr("Output"));

    QHBoxLayout * layout = new QHBoxLayout;

    layout->addWidget(btn_load);
    layout->addStretch(1);
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(radioButton_RealityShow);
    layout->addWidget(radioButton_HistoryShow);
    radioWidget->setLayout(layout);

    QWidget *widget =new QWidget();
    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(radioWidget);

    mWidget = new QStackedWidget();
    QWidget *temp = new QWidget();
    temp->setLayout(new QVBoxLayout);
    temp->layout()->addWidget(&viewRT_Origin);
    temp->layout()->addWidget(energvGraphics);
    mWidget->insertWidget(0,temp);


    temp = new QWidget();
    QHBoxLayout *hlayout = new QHBoxLayout();
    temp->setLayout(hlayout);
    hlayout->addWidget(&view_Statistic,4);
    hlayout->addWidget(&viewHS_Origin,1);
    mWidget->insertWidget(1,temp);


    vlayout->addWidget(mWidget);
    widget->setLayout(vlayout);

    this->setWidget(widget);

}

void AllPluseDock::ininData()
{
    // model_StatisticData->mdata->isReverse = true;
    originData = new HugeData_Gram();
    originData->coreData.append(new AllPluseInfo::OriginData::Core());
    modelRT_originData.setDataSrc(originData);
    modelRT_originData.openShowLatestMsg(50);

    modelHS_orginData.setDataSrc(originData);
    modelHS_orginData.openPointedColShow(0);
    staticsData = new HugeData_Gram();
    staticsData->coreData.append(new AllPluseInfo::Statistic::Core());
    model_StatisticData.setDataSrc(staticsData);

    updateView();
}



void AllPluseDock::retranslateUi()
{
//    QStringList headInfo;
////    headInfo<<QObject::tr("Serial number")
//    initTable();


//    Q_D(AllPluseDock);
//    m_name = tr("Full Pulse List");
//    setWindowTitle(m_name);

//    d->radioButton_RealityShow->setText(tr("Real-time display"));
//    d->radioButton_HistoryShow->setText(tr("Historical display"));

//    d_ptr->dataViewStatictis->resizeColumnsToContents();
//    d_ptr->dataViewStatictis->horizontalHeader();
//    for(int i=0;i< d_ptr->dataViewStatictis->horizontalHeader()->count();i++)
//    {
//        d_ptr->dataViewStatictis->setColumnWidth(i, d_ptr->dataViewStatictis->columnWidth(i) + 40);
////        d_ptr->dataView->setColumnWidth(i,90);
//    }
//    d_ptr->dataViewOriginal->resizeColumnsToContents();
//    d_ptr->dataViewOriginal->horizontalHeader();
//    for(int i=0;i< d_ptr->dataViewOriginal->horizontalHeader()->count();i++)
//    {
//        d_ptr->dataViewOriginal->setColumnWidth(i, d_ptr->dataViewOriginal->columnWidth(i) + 40);
////        d_ptr->dataView->setColumnWidth(i,90);
    //    }
}



/*!
 * @brief 实时模式单选框槽
 */
void AllPluseDock::on_radioButton_RealityShow_clicked()
{
    switchModeTo(RealTime);
}

/*!
 * @brief 历史模式单选框槽
 */
void AllPluseDock::on_radioButton_HistoryShow_clicked()
{
    switchModeTo(Historical);
}

/*!
 * @brief 接收全脉冲数据
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void AllPluseDock::recvAllPlusePara(char *buff, int len)
{
if((buff==NULL)||(len<MIN_PACKAGE_LEN))
    {
        return;
    }
    int startCode;          //包头
    int endCode;            //包尾
    int bufMemcpyPos=0;     //缓冲拷贝位置
    int iPackageLen;        //缓冲区总长度(从包头至包尾)
    AllPluseInfo::Statistic::Core statisticInfoBase;
//    AllPluseStatisticInfo statisticInfo;
//    AllPluseOriginalInfoAttribute originalInfo;
//    AllPluseStatisticInfoList statisticInfoTempList;    //统计信息临时链表,绘图使用
//    AllPulseOriginalInfoList originalInfoTempList;      //全脉冲原始信息临时列表
    int statisticInfoEff;       //统计信息有效标识 1:有效
    int originalInfoEff;        //原始数据有效标识
    int statisticInfoNumN=0;    //统计参数个数N
    int originalInfoNumM=0;     //原始数据个数M
    int attributeInfoNumL=0;    //原始数据属性个数L

    memcpy(&startCode,buff,sizeof(int));
    bufMemcpyPos=4+2;
    memcpy(&iPackageLen,buff+bufMemcpyPos,sizeof(int));
    if(iPackageLen<MIN_PACKAGE_LEN)
    {
        return;
    }

    memcpy(&endCode,buff+iPackageLen-4,sizeof(int));
    if((startCode!=PACKAGE_START)||(endCode!=PACKAGE_END))
    {
        return;
    }

    bufMemcpyPos+=sizeof(int);
    memcpy(&statisticInfoEff,buff+bufMemcpyPos,sizeof(int));

    bufMemcpyPos+=sizeof(int);
    memcpy(&statisticInfoNumN,buff+bufMemcpyPos,sizeof(int));

    if(statisticInfoEff==1) //统计信息有效
    {
        bufMemcpyPos+=sizeof(int);
        if((bufMemcpyPos+sizeof(AllPluseInfo::Statistic)*statisticInfoNumN)>=iPackageLen)    //判断长度,防止缓冲越界
        {
            return;
        }
        AllPluseInfo::Statistic::Core *pStatisticData;
        for(int i=0;i<statisticInfoNumN;i++)
        {
            pStatisticData = NULL;
            memset(&statisticInfoBase,0x00,sizeof(AllPluseInfo::Statistic::Core));
            memcpy(&statisticInfoBase,buff+bufMemcpyPos,sizeof(AllPluseInfo::Statistic::Core));
            if(i > staticsData->coreData.count())
                staticsData->coreData.append(new AllPluseInfo::Statistic::Core());
            /* 更新统计值 */
            pStatisticData = dynamic_cast <AllPluseInfo::Statistic::Core*>(staticsData->coreData.at(i));
            pStatisticData->name = statisticInfoBase.name;
            pStatisticData->maxVal = statisticInfoBase.maxVal;
            pStatisticData->minVal = statisticInfoBase.minVal;
            pStatisticData->meanVal = statisticInfoBase.meanVal;
            pStatisticData->variance = statisticInfoBase.variance;
        }
    }

    memcpy(&originalInfoEff,buff+bufMemcpyPos,sizeof(int));
    if(originalInfoEff==1) //原始信息有效
    {
        bufMemcpyPos+=sizeof(int);
        memcpy(&originalInfoNumM,buff+bufMemcpyPos,sizeof(int));

        bufMemcpyPos+=sizeof(int);
        memcpy(&attributeInfoNumL,buff+bufMemcpyPos,sizeof(int));

        if((bufMemcpyPos+68*attributeInfoNumL+8*attributeInfoNumL*originalInfoNumM)>=iPackageLen)    //判断长度,防止缓冲越界
        {
            return;
        }

        bufMemcpyPos+=sizeof(int);
        for(int l=0;l<attributeInfoNumL;l++)
        {
            AllPluseInfo::OriginData::Core *originD = new AllPluseInfo::OriginData::Core();
            int drawFlag; //是否绘图
            memcpy(&originD->name,(char*)buff+bufMemcpyPos,64);
            bufMemcpyPos+=64;
            memcpy(&drawFlag,buff+bufMemcpyPos,sizeof(int));
            originD->ifDrawPic=drawFlag;
            bufMemcpyPos+=sizeof(int);

            if(l > originData->coreData.count()) {
                originData->coreData.append(originD);
            }
            else
                delete originD;
            ////!!!!!!!!!不支持在网络传输过程中更改属性数值
        }
        /* 添加实际数值 */
        double valueTemp;
        for(int m=0;m<originalInfoNumM;m++) //原始数据M
        {
            for(int k=0;k<attributeInfoNumL;k++) {//属性K
              valueTemp =0;
              memcpy(&valueTemp,buff+bufMemcpyPos,sizeof(valueTemp));
              bufMemcpyPos += sizeof(valueTemp);
              dynamic_cast<AllPluseInfo::OriginData::Core*>(originData->coreData.at(k))
                      ->valuelist.append(valueTemp);
            }
        }
    }
    updateView();
}




void AllPluseDock::on_btn_load_clicked()
{
  //  Q_D(AllPluseDock);
    // 指定存储的位置
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                    QString(), tr("EXCEL files (*.xls *.xlsx);;HTML-Files (*.htm *.html);;"));
    if(filepath.isEmpty())
        return;
}



void AllPluseDock::statistic_ShowOrginInfo(QModelIndex index)
{
    int row=index.column();
    if(mWidget->currentIndex() == (int)Historical) //历史显示模式
    {
        model_StatisticData.openPointedColShow(row);
        viewHS_Origin.setModel(&modelHS_orginData);
    }
}

/*!
 * \brief AllPluseDock::show_ParamTimeProperties        显示时间特性图
 * \param cycleStart        周期开始
 * \param cycleEnd          周期结束
 * \param properties        特性序列号-即列号
 */
void AllPluseDock::showTimeProperties(double cycleStart, double cycleEnd, int properties)
{
      QString tittle(QStringLiteral("全脉冲时间特性图"));
      tittle += originData->getHeadName(properties);
      Diagram::Scatter  timeScatter(tittle);
      timeScatter.addAxis(AxisType::xAxis,DataType::value,Qt::AlignBottom,QStringLiteral("到达时间"));
      timeScatter.addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft,QStringLiteral("脉冲参数"));
      timeScatter.addData(cycleStart,cycleEnd, dynamic_cast<AllPluseInfo::OriginData::Core*>
                          (originData->coreData.at(properties) )->valuelist);
      timeScatter.show();
}

/*!
 * \brief AllPluseDock::showFreqHistogram  显示对应的参数频度直方图
 * \param properties        特性序列号-即列号
 */
void AllPluseDock::showFreqHistogram(int properties)
{
    QString tittle(QStringLiteral("全脉冲频度直方图"));
    tittle += originData->getHeadName(properties);
    Diagram::Histogram freqHistogram(tittle);

    QMap<int,int> freqMap;
    QMap<int,int>::iterator ite;
    AllPluseInfo::OriginData::Core *data =  dynamic_cast<AllPluseInfo::OriginData::Core*>
            (originData->coreData.at(properties) );
    for(int i=0;i< data->valuelist.count();i++){
        ite = freqMap.find(data->valuelist.at(i));
        if( ite == freqMap.end())
            freqMap.insert(data->valuelist.at(i),1);
        else
            ite.value()++;
    }

    QStringList freqList;
    ite = freqMap.begin();
    while(ite != freqMap.end()) {
        freqList.append(QString::number(ite.key()));
        ite++;
    }
    freqHistogram.setXAxisLabels(freqList,QStringLiteral("参数分布"));
    freqHistogram.addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft,QStringLiteral("统计次数"));
    ite = freqMap.begin();
    while(ite != freqMap.end()) {
        freqHistogram.addData(ite.key(),ite.value());
        ite++;
    }
    freqHistogram.show();
}

void AllPluseDock::updateView()
{
    viewRT_Origin.setModel(&modelRT_originData);
    viewHS_Origin.setModel(&modelHS_orginData);
    view_Statistic.setModel(&model_StatisticData);
}

/*!
 * @brief 获取当前时间
 */
QString AllPluseDock::getCurrentDate()
{
    QString strDate;
    QDateTime date=QDateTime::currentDateTime();
    strDate=date.toString("yyyy.MM.dd hh:mm:ss.zzz");
    return strDate;
}

} //namespace DataView
