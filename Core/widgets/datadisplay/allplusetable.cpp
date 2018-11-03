#include "allplusetable.h"

#include <QSortFilterProxyModel>
//#include "table.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmodelcustom.h"
#include "../Util/dataexportandprint.h"
#include "filterdockpanel.h"
#include <iostream>
#include <QHeaderView>
using namespace std;

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
    //    QObject::connect(btn_search,SIGNAL(clicked()),q_ptr,SLOT(on_btn_search_clicked()));
    QObject::connect(radioButton_RealityShow,SIGNAL(clicked()),this,SLOT(on_radioButton_RealityShow_clicked()));
    QObject::connect(radioButton_HistoryShow,SIGNAL(clicked()),this,SLOT(on_radioButton_HistoryShow_clicked()));
    //过滤器
    QObject::connect(&viewRT_Origin, SIGNAL(tableCheckDoubleSignal(QModelIndex)), this, SLOT(filterRT(QModelIndex)));
    QObject::connect(&viewHS_Origin, SIGNAL(tableCheckDoubleSignal(QModelIndex)), this, SLOT(filterHS(QModelIndex)));
    QObject::connect(&view_Statistic, SIGNAL(tableCheckDoubleSignal(QModelIndex)), this, SLOT(filterStatistic(QModelIndex)));
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
    temp->setLayout(new QHBoxLayout);
    temp->layout()->addWidget(&view_Statistic);
    temp->layout()->addWidget(&viewHS_Origin);
    mWidget->insertWidget(1,temp);
    viewHS_Origin.hide();


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

    staticsData = new HugeData_Gram();
    staticsData->coreData.append(new AllPluseInfo::Statistic::Core());
    model_StatisticData.setDataSrc(staticsData);

    viewRT_Origin.setModel(&modelRT_originData);
    viewHS_Origin.setModel(&modelHS_orginData);
    view_Statistic.setModel(&model_StatisticData);

    //排序功能
    QSortFilterProxyModel *dataViewProxyRT = new QSortFilterProxyModel();
    dataViewProxyRT->setSourceModel(&modelRT_originData);
    viewRT_Origin.setModel(dataViewProxyRT);

    QSortFilterProxyModel *dataViewProxyHS = new QSortFilterProxyModel();
    dataViewProxyHS->setSourceModel(&modelHS_orginData);
    viewHS_Origin.setModel(dataViewProxyHS);

    QSortFilterProxyModel *dataViewProxyStatistic = new QSortFilterProxyModel();
    dataViewProxyStatistic->setSourceModel(&model_StatisticData);
    view_Statistic.setModel(dataViewProxyStatistic);

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
    AllPluseStatisticInfoBase statisticInfoBase;
    AllPluseOriginalInfoAttributeBase originalInfoBase;
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
        if((bufMemcpyPos+sizeof(AllPluseStatisticInfoBase)*statisticInfoNumN)>=iPackageLen)    //判断长度,防止缓冲越界
        {
            return;
        }

        for(int i=0;i<statisticInfoNumN;i++)
        {
            memset(&statisticInfoBase,0x00,sizeof(AllPluseStatisticInfoBase));
            memcpy(&statisticInfoBase,buff+bufMemcpyPos,sizeof(AllPluseStatisticInfoBase));

//            statisticInfo.iDataOutsideNo=d_ptr->dataOutsideNo;
//            statisticInfo.iDataInsideNo=i;
//            statisticInfo.allPluseStatisticInfoBase=statisticInfoBase;
//            d_ptr->allPluseStatisticInfoList.append(statisticInfo);
//            statisticInfoTempList.append(statisticInfo);
//            bufMemcpyPos+=sizeof(AllPluseStatisticInfoBase);
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
            int drawFlag; //是否绘图
            memset(&originalInfoBase,0x00,sizeof(AllPluseOriginalInfoAttributeBase));
            memcpy(originalInfoBase.arrOrgInfoAttributeName,(char*)buff+bufMemcpyPos,64);
            bufMemcpyPos+=64;
            memcpy(&drawFlag,buff+bufMemcpyPos,sizeof(int));
            originalInfoBase.iDrawFlag=drawFlag;
            bufMemcpyPos+=sizeof(int);
            for(int m=0;m<originalInfoNumM;m++)
            {
//                originalInfo.allPluseOriginalInfoBase=originalInfoBase;
//                originalInfo.iDataOutsideNo=d_ptr->dataOutsideNo;
//                originalInfo.iDataInsideNo=l;
//                originalInfo.dtPulseArriveColck=QDateTime::currentDateTime();
//                //originalInfo.strPulseArriveColck=getCurrentDate();
//                originalInfoTempList.append(originalInfo);
            }
        }

//        for(int m=0;m<originalInfoNumM;m++)
//        {
//            int move;   //偏移量,从包头到当前值
//            double value;
//            for(int ll=0;ll<attributeInfoNumL;ll++)
//            {
//               move=18+sizeof(AllPluseStatisticInfoBase)*statisticInfoNumN+12+68*attributeInfoNumL+sizeof(double)*m+ll;
//               memcpy(&value,buff+move,sizeof(double));
//               originalInfo=originalInfoTempList.at(m*attributeInfoNumL+ll);
//               originalInfo.allPluseOriginalInfoBase.dValue=value;
//               originalInfoTempList.replace(m*attributeInfoNumL+ll,originalInfo);
//            }
//        }

//        for(int i=0;i<originalInfoTempList.size();i++)  //将原始数据信息存放到全局列表
//        {
//            originalInfo=originalInfoTempList.at(i);
//            d_ptr->allPulseOriginalInfoList.append(originalInfo);
//        }
    }

    setTableData();
   // sendAllPulseInfoList(&statisticInfoTempList,&originalInfoTempList);
   // d_ptr->dataOutsideNo++;
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
void AllPluseDock::setTableData()
{
//    if(d_ptr->showTableMode == RealTimeDisplay) //实时显示模式
//    {
//        d_ptr->dataViewModelStatictis->updateAllPluseStatisticInfoList(d_ptr->allPluseStatisticInfoList);
//        int size=d_ptr->allPluseStatisticInfoList.size();
//        if(size>0)
//        {
//            int outsideNo=d_ptr->allPluseStatisticInfoList.at(size-1).iDataOutsideNo;
//            int insideNo=d_ptr->allPluseStatisticInfoList.at(size-1).iDataInsideNo;
//            AllPulseOriginalInfoList originalList;
//            AllPluseOriginalInfoAttribute originalInfo;
//            for(int i=0;i<d_ptr->allPulseOriginalInfoList.size();i++)
//            {
//                if((d_ptr->allPulseOriginalInfoList.at(i).iDataOutsideNo==outsideNo)
//                   &&(d_ptr->allPulseOriginalInfoList.at(i).iDataInsideNo==insideNo))
//                {
//                    originalInfo=d_ptr->allPulseOriginalInfoList.at(i);
//                    originalList.append(originalInfo);
//                }
//            }
//            d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(originalList);
//        }
//    }
}

/*!
 * \brief 双击统计信息表格
 * \param index 信息
 */
void AllPluseDock::doubleClickedTable(QModelIndex index)
{
//    int row=index.column();
//    if(d_ptr->showTableMode == HistoricalDisplay) //历史显示模式
//    {
//        int size=d_ptr->allPluseStatisticInfoList.size();
//        if(size>row)
//        {
//            int outsideNo=d_ptr->allPluseStatisticInfoList.at(row).iDataOutsideNo;
//            int insideNo=d_ptr->allPluseStatisticInfoList.at(row).iDataInsideNo;
//            AllPulseOriginalInfoList originalList;
//            AllPluseOriginalInfoAttribute originalInfo;
//            for(int i=0;i<d_ptr->allPulseOriginalInfoList.size();i++)
//            {
//                if((d_ptr->allPulseOriginalInfoList.at(i).iDataOutsideNo==outsideNo)
//                   &&(d_ptr->allPulseOriginalInfoList.at(i).iDataInsideNo==insideNo))
//                {
//                    originalInfo=d_ptr->allPulseOriginalInfoList.at(i);
//                    originalList.append(originalInfo);
//                }
//            }
//            d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(originalList);
//        }
//    }
}

/*!
 * @brief 清空表格
 */
void AllPluseDock::clearTable()
{
//    d_ptr->allPluseStatisticInfoList.clear();
//    d_ptr->allPulseOriginalInfoList.clear();

//    d_ptr->dataViewModelStatictis->updateAllPluseStatisticInfoList(d_ptr->allPluseStatisticInfoList);
//    d_ptr->dataViewModelOriginal->updateAllPulseOriginalInfoList(d_ptr->allPulseOriginalInfoList);
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


/*!
 * \brief 过滤页面，实时源数据
 * \param 页面
 */
void AllPluseDock::filterRT(QModelIndex index){
    filterDockPanel * dock = new filterDockPanel;
    connect(dock,SIGNAL(sendFilterMessage(QList<double>)),this,SLOT(recFilterMessageRT(QList<double>)));
    filterIndexRT = index;
    dock->show();
}
/*!
 * \brief 过滤功能实现
 * \param filterMessage存储筛选功能上下界限
 */
void AllPluseDock::recFilterMessageRT(QList<double> filterMessage){
    double filterLower = filterMessage.at(0);
    double filterUpper = filterMessage.at(1);
    double theData;
//    qDebug()<<filterLower<<"and"<<filterUpper<<endl;
    try{
        for(int i =0;i<modelRT_originData.rowCount();i++){viewRT_Origin.showRow(i);}
        for(int i =0;i<modelRT_originData.rowCount();i++){
            theData=(modelRT_originData.index(i,filterIndexRT.column(),QModelIndex())).data().toDouble();
            if(theData>filterUpper||theData<filterLower){viewRT_Origin.hideRow(i);}
        }
    }catch(const char* msg){qDebug()<<msg<<endl;}
}

/*!
 * \brief 过滤页面，历史源数据
 * \param 页面
 */
void AllPluseDock::filterHS(QModelIndex index){
    filterDockPanel * dock = new filterDockPanel;
    connect(dock,SIGNAL(sendFilterMessage(QList<double>)),this,SLOT(recFilterMessageHS(QList<double>)));
    filterIndexHS = index;
    dock->show();
}

/*!
 * \brief 过滤功能实现
 * \param filterMessage存储筛选功能上下界限
 */
void AllPluseDock::recFilterMessageHS(QList<double> filterMessage){
    double filterLower = filterMessage.at(0);
    double filterUpper = filterMessage.at(1);
    double theData;
//    qDebug()<<filterLower<<"and"<<filterUpper<<endl;
    try{
        for(int i =0;i<modelHS_orginData.rowCount();i++){viewHS_Origin.showRow(i);}
        for(int i =0;i<modelHS_orginData.rowCount();i++){
            theData=(modelHS_orginData.index(i,filterIndexHS.column(),QModelIndex())).data().toDouble();
            if(theData>filterUpper||theData<filterLower){viewHS_Origin.hideRow(i);}
        }
    }catch(const char* msg){qDebug()<<msg<<endl;}
}

/*!
 * \brief 过滤页面，统计数据
 * \param 页面
 */
void AllPluseDock::filterStatistic(QModelIndex index){
    filterDockPanel * dock = new filterDockPanel;
    connect(dock,SIGNAL(sendFilterMessage(QList<double>)),this,SLOT(recFilterMessageStatistic(QList<double>)));
    filterIndexStatistic = index;
    dock->show();
}

/*!
 * \brief 过滤功能实现
 * \param filterMessage存储筛选功能上下界限
 */
void AllPluseDock::recFilterMessageStatistic(QList<double> filterMessage){
    double filterLower = filterMessage.at(0);
    double filterUpper = filterMessage.at(1);
    double theData;
//    qDebug()<<filterLower<<"and"<<filterUpper<<endl;
    try{
        for(int i =0;i<model_StatisticData.rowCount();i++){view_Statistic.showRow(i);}
        for(int i =0;i<model_StatisticData.rowCount();i++){
            theData=(model_StatisticData.index(i,filterIndexStatistic.column(),QModelIndex())).data().toDouble();
            if(theData>filterUpper||theData<filterLower){view_Statistic.hideRow(i);}
        }
    }catch(const char* msg){qDebug()<<msg<<endl;}
}

} //namespace DataView
