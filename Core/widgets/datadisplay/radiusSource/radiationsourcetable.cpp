#include "radiationsourcetable.h"
#include "ui_radiationsourcetable.h"

#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QHBoxLayout>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QDesktopServices>
#include <QPushButton>
//#include <QMenu>
//#include "table.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "widgets/datadisplay/modelview/tableviewdata.h"
#include "widgets/datadisplay/modelview/tableviewmoderradiationsource.h"
#include "radiationsourcetablerenovatedialog.h"
#include "widgets/Util/dataexportandprint.h"

#include <iostream>
using namespace std;

namespace DataView {

const int INIT_TABLE_COL_NUM =50;                                   //表格初始化为50列
const int INIT_TABLE_ROW_NUM=40;                                    //表格初始化为20行
const int RADIASOURCE_START_POINT=140;                              //数据源基信息起始位置
const char ACTION_TYPE[] = "ActionType";

class RadiationSourceTablePrivate
{
    Q_DECLARE_PUBLIC(RadiationSourceTable)
public:
    explicit RadiationSourceTablePrivate(RadiationSourceTable * q):q_ptr(q),enRSReKind(SCROLL_RENOVATE),
        m_currentRowToInsert(-1)
    {
        initView();
        initTableViewMenu();
    }
    void initView();
    void initTableViewMenu();
    void intRSTable();

    RadiationSourceTable * q_ptr;

    QWidget * mainWidget;

    QPushButton * btn_load;

    QRadioButton * radioButtonCoverReno;
    QRadioButton * radioButtonScrollReno;

    DataRefreshModel enRSReKind;                //刷新方式
    //Table * rsTable;
    RSDataMap rsReMap;                          //覆盖刷新下的map,key：数据源批号 value:数据源的具体信息
    int m_currentRowToInsert;
    RSDataList rsDataList;
    TableView* dataView;
    TableViewModelRadiationSource * dataViewModel;
    bool blDoubleClickedFlag;                   //覆盖刷新模式下的双击标识

    QAction *clearAction;
    QLabel *rowCounterLabel;                    //行数标签
};

void RadiationSourceTablePrivate::initView()
{
    blDoubleClickedFlag=false;
    mainWidget = new QWidget();

    QWidget * radioWidget = new QWidget();
    radioButtonCoverReno = new QRadioButton(radioWidget);
    radioButtonScrollReno = new QRadioButton(radioWidget);

    btn_load = new QPushButton(radioWidget);
    btn_load->setText(QObject::tr("Output"));

    QObject::connect(btn_load,SIGNAL(clicked()),q_ptr,SLOT(on_btn_load_clicked()));
    QObject::connect(radioButtonCoverReno,SIGNAL(clicked()),q_ptr,SLOT(on_radioButtonCoverReno_clicked()));
    QObject::connect(radioButtonScrollReno,SIGNAL(clicked()),q_ptr,SLOT(on_radioButtonScrollReno_clicked()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(btn_load);
    layout->addStretch(1);
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(radioButtonCoverReno);
    layout->addWidget(radioButtonScrollReno);

    radioWidget->setLayout(layout);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(radioWidget);

    dataView = new TableView(q_ptr);
    dataView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    m_tableView->resizeColumnsToContents();

    dataViewModel = new TableViewModelRadiationSource(dataView);

    dataView->setSortingEnabled(true);
    QSortFilterProxyModel *dataViewProxy = new QSortFilterProxyModel();
    dataViewProxy->setSourceModel(dataViewModel);
    dataView->setModel(dataViewProxy);

//    dataView->resizeColumnsToContents();


    vlayout->addWidget(dataView);

    QWidget *rowLabelWidget=new QWidget();
    QLabel *rowLabel=new QLabel(QStringLiteral("row:"));
    rowCounterLabel=new QLabel(QStringLiteral("0"));
    QHBoxLayout *hLayout=new QHBoxLayout();
    hLayout->addStretch(1);
    hLayout->setContentsMargins(1,1,1,1);
    hLayout->addWidget(rowLabel);
    hLayout->addWidget(rowCounterLabel);
    rowLabelWidget->setLayout(hLayout);
    vlayout->addWidget(rowLabelWidget);

    mainWidget->setLayout(vlayout);

    QObject::connect(dataView, SIGNAL(tableCheckDoubleSignal(QModelIndex)), q_ptr, SLOT(viewRSData(QModelIndex)));
}

void RadiationSourceTablePrivate::initTableViewMenu()
{
    clearAction = new QAction();
    dataView->addAction(clearAction);
//    dataView->resizeColumnsToContents();
    QObject::connect(clearAction, SIGNAL(triggered(bool)), q_ptr, SLOT(clearTable()));
}

RadiationSourceTable::RadiationSourceTable(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_RADIATION_SOURCE_TABLE,parent),d_ptr(new RadiationSourceTablePrivate(this))
{
    initRSTable();
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

RadiationSourceTable::~RadiationSourceTable()
{

}

bool RadiationSourceTable::initialize()
{
    Q_D(RadiationSourceTable);
    setWidget(d->mainWidget);
    return true;
}

void RadiationSourceTable::release()
{

}

QString RadiationSourceTable::pluginName()
{
    return tr("DataDisplay panel");
}

void RadiationSourceTable::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

/*!
 * @brief 初始化数据源表格
 */
void RadiationSourceTable::initRSTable()
{
    Q_D(RadiationSourceTable);
    d->clearAction->setText(QObject::tr("Clear Table"));
    changeTableHeaderInfo(false);
}

/*!
 * @brief 滚动刷新单选框
 */
void RadiationSourceTable::on_radioButtonScrollReno_clicked()
{
    Q_D(RadiationSourceTable);
    if(d->enRSReKind!=SCROLL_RENOVATE)
    {
        d->enRSReKind=SCROLL_RENOVATE;
        changeTableHeaderInfo(false);
        //d->m_currentRowToInsert=0;
        d_ptr->dataViewModel->resetRefreshModel(d->enRSReKind);
        //d->rsDataList.clear();
        //d->rsReMap.clear();
    }
}

/*!
 * @brief 覆盖刷新单选框
 */
void RadiationSourceTable::on_radioButtonCoverReno_clicked()
{
    Q_D(RadiationSourceTable);
    if(d->enRSReKind!=COVER_RENOVATE)
    {
        d->enRSReKind=COVER_RENOVATE;
        changeTableHeaderInfo(true);
        d_ptr->dataViewModel->resetRefreshModel(d->enRSReKind);
        //d->rsDataList.clear();
        //d->rsReMap.clear();
    }
}

/*!
 * @brief 改变表格头部信息以及列数
 * @param[in] blAddColFlag 是否新增列
 */
void RadiationSourceTable::changeTableHeaderInfo(bool blAddColFlag)
{
    theblAddColFlag = blAddColFlag;
    QStringList headInfo;
    //int colCount=INIT_TABLE_COL_NUM;    //列数
//    headInfo<<QStringLiteral("序号")<<QStringLiteral("辐射源批号")<<QStringLiteral("脉间类型")<<QStringLiteral("脉内类型")<<QStringLiteral("载频个数")
//               <<QStringLiteral("连续波标记")<<QStringLiteral("载频脉组内脉冲数")<<QStringLiteral("载频频段码")<<QStringLiteral("RF1")<<QStringLiteral("RF2")
//                <<QStringLiteral("RF3")<<QStringLiteral("RF4")<<QStringLiteral("RF5")<<QStringLiteral("RF6")<<QStringLiteral("RF7")
//                  <<QStringLiteral("RF8")<<QStringLiteral("重频类型")<<QStringLiteral("重频个数")<<QStringLiteral("重频脉组内脉冲数")<<QStringLiteral("PRI1")
//                    <<QStringLiteral("PRI2")<<QStringLiteral("PRI3")<<QStringLiteral("PRI4")<<QStringLiteral("PRI5")<<QStringLiteral("PRI6")
//                      <<QStringLiteral("PRI7")<<QStringLiteral("PRI8")<<QStringLiteral("脉宽类型")<<QStringLiteral("脉宽个数")<<QStringLiteral("脉宽脉组内脉冲数")
//                        <<QStringLiteral("PW1")<<QStringLiteral("PW2")<<QStringLiteral("PW3")<<QStringLiteral("PW4")<<QStringLiteral("PW5")
//                          <<QStringLiteral("PW6")<<QStringLiteral("PW7")<<QStringLiteral("PW8")<<QStringLiteral("数字幅度")<<QStringLiteral("模拟幅度")
//                            <<QStringLiteral("数字功率")<<QStringLiteral("模拟功率")<<QStringLiteral("方位角")<<QStringLiteral("俯仰角")<<QStringLiteral("经度")
//                              <<QStringLiteral("纬度")<<QStringLiteral("高度")<<QStringLiteral("脉内有效标识")<<QStringLiteral("脉内特征信息")<<QStringLiteral("CRC校验");
//    headInfo<<QObject::tr("Index")<<QObject::tr("Type")<<QObject::tr("Parameter")
//                  <<QObject::tr("Dispatch Time")<<QObject::tr("Execute Time")<<QObject::tr("Issued status");
    headInfo<<QObject::tr("Serial number")<<QObject::tr("Radiation source batch number")<<QObject::tr("Interpulse type")<<QObject::tr("Intrapulse type")<<QObject::tr("Number of carrier frequencies")
               <<QObject::tr("Continuous wave mark")<<QObject::tr("Number of pulses in carrier frequency group")<<QObject::tr("Carrier frequency band code")<<QObject::tr("RF1")<<QObject::tr("RF2")
                <<QObject::tr("RF3")<<QObject::tr("RF4")<<QObject::tr("RF5")<<QObject::tr("RF6")<<QObject::tr("RF7")
                  <<QObject::tr("RF8")<<QObject::tr("Re-frequency type")<<QObject::tr("Re-frequency number")<<QObject::tr("Number of pulses in the re-frequency group")<<QObject::tr("PRI1")
                    <<QObject::tr("PRI2")<<QObject::tr("PRI3")<<QObject::tr("PRI4")<<QObject::tr("PRI5")<<QObject::tr("PRI6")
                      <<QObject::tr("PRI7")<<QObject::tr("PRI8")<<QObject::tr("Pulse width type")<<QObject::tr("Pulse width number")<<QObject::tr("Pulse width pulse number")
                        <<QObject::tr("PW1")<<QObject::tr("PW2")<<QObject::tr("PW3")<<QObject::tr("PW4")<<QObject::tr("PW5")
                          <<QObject::tr("PW6")<<QObject::tr("PW7")<<QObject::tr("PW8")<<QObject::tr("Digital amplitude")<<QObject::tr("Analog amplitude")
                            <<QObject::tr("Digital power")<<QObject::tr("Analog power")<<QObject::tr("Azimuth")<<QObject::tr("Pitch angle")<<QObject::tr("Longitude")
                              <<QObject::tr("Latitude")<<QObject::tr("Height")<<QObject::tr("Intra-pulse effective identification")<<QObject::tr("Intra-pulse characteristic information")<<QObject::tr("CRC check");


    if(blAddColFlag) //新增两列
    {
        headInfo<<QObject::tr("Interceptions")<<QObject::tr("Interception time");
    }

    d_ptr->dataView->resizeColumnsToContents();
    d_ptr->dataView->horizontalHeader();
    for(int i=0;i<headInfo.size();i++)
    {
        d_ptr->dataView->setColumnWidth(i, d_ptr->dataView->columnWidth(i) + 40);
//        d_ptr->dataView->setColumnWidth(i,90);
    }

    d_ptr->dataViewModel->resetHeadInfo(headInfo);
}

/*!
 * @brief 接收辐射源
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void RadiationSourceTable::recvRSPara(char *buff, int len)
{
    Q_D(RadiationSourceTable);
    if((NULL==buff)||(len<=RADIASOURCE_START_POINT)) //小于辐射源首部字段固定长度
    {
        return;
    }

    int rsNum;
    RadiationSource::RadiationSourceBase rsData;
    RadiationSourceRenovate rsReData;
    memcpy(&rsNum,buff+RADIASOURCE_START_POINT-4,4);
    for(int i=0;i<rsNum;i++)
    {
        bool blExist=false;
        memcpy(&rsData,buff+RADIASOURCE_START_POINT,sizeof(RadiationSource::RadiationSourceBase));
        if(d->enRSReKind==COVER_RENOVATE) //覆盖刷新模式
        {
            int num=rsData.usSourceNo; //数据源批号
            if(!d->rsReMap.isEmpty())
            {
                QMap<int,RadiationSourceRenovate>::Iterator iter;
                iter=d->rsReMap.find(num);
                if(iter!=d->rsReMap.end())
                {
                    blExist=true;
                    rsReData=iter.value();
                    rsReData.iHoldCount++;
                    rsReData.strHoldTime=getCurrentDate();
                    rsReData.rsInfo=rsData;
                    d->rsReMap[num]=rsReData;  //重新赋值
                    d->m_currentRowToInsert=rsReData.iInsertRow;
                }
            }

            if(!blExist)        //新增key
            {
                rsReData.iHoldCount=1;
                rsReData.strHoldTime=getCurrentDate();
                rsReData.rsInfo=rsData;
                rsReData.iInsertRow=d->rsReMap.size();
                d->m_currentRowToInsert=rsReData.iInsertRow;
                d->rsReMap.insert(num,rsReData);
            }
        }

        rsReData.rsInfo=rsData;
        if(!blExist)
        {
            rsReData.strHoldTime=getCurrentDate();
        }

        d->rsDataList.append(rsReData);
        showTSPara(rsData);

    }
}

/*!
 * @brief 显示辐射源信息
 * @param[in] rsData 辐射源信息
 */
void RadiationSourceTable::showTSPara(const RadiationSource::RadiationSourceBase& rsData)
{
    Q_D(RadiationSourceTable);

    if((rsData.usStartCode!=0x55AA)||(rsData.usEndCode!=0x55AA))
    {
        return;
    }

    d_ptr->dataViewModel->updateTableData(d->rsReMap);
    d_ptr->dataViewModel->updateTableData(d->rsDataList,false);
    if(d->enRSReKind==COVER_RENOVATE) //覆盖刷新模式,设置高亮行
    {
        d_ptr->dataViewModel->resetHigetLightRow(d->m_currentRowToInsert);
    }
    if(d_ptr->blDoubleClickedFlag)
    {
        emit sendRSDataList(&(d->rsDataList));
    }
    showRowCounter();
}

/*!
 * @brief 显示当前表格数据总行数
 */
void RadiationSourceTable::showRowCounter()
{
    QString strNum;
    if(d_ptr->enRSReKind==COVER_RENOVATE) //覆盖刷新模式
    {
        strNum=QString("%1").arg(d_ptr->rsReMap.size());
    }
    else if(d_ptr->enRSReKind==SCROLL_RENOVATE) //滚动刷新模式
    {
        strNum=QString("%1").arg(d_ptr->rsDataList.size());
    }
    else
    {
        return;
    }
    d_ptr->rowCounterLabel->setText(strNum);
}

/*!
 * @brief 清空表格
 */
void RadiationSourceTable::clearTable()
{
    d_ptr->rsReMap.clear();
    d_ptr->rsDataList.clear();
    d_ptr->dataViewModel->updateTableData(d_ptr->rsReMap);
    d_ptr->dataViewModel->updateTableData(d_ptr->rsDataList,false);
    if(d_ptr->enRSReKind==COVER_RENOVATE) //覆盖刷新模式,设置高亮行
    {
        d_ptr->dataViewModel->resetHigetLightRow(d_ptr->m_currentRowToInsert);
    }
    if(d_ptr->blDoubleClickedFlag)
    {
        emit sendRSDataList(&(d_ptr->rsDataList));
    }
}

/*!
 * @brief 获取当前时间
 */
QString RadiationSourceTable::getCurrentDate()
{
    QString strDate;
    QDateTime date=QDateTime::currentDateTime();
    strDate=date.toString("yyyy.MM.dd hh:mm:ss.zzz");
    return strDate;
}

void RadiationSourceTable::retranslateUi()
{
//    cout<<"the1"<<endl;
    Q_D(RadiationSourceTable);
//    QStringList headInfo;
    m_name = tr("Radiation source data");
    setWindowTitle(m_name);


    d->radioButtonCoverReno->setText(tr("Overwrite Refresh"));
    d->radioButtonScrollReno->setText(tr("Scrolling Refresh"));

    changeTableHeaderInfo(theblAddColFlag);

    d_ptr->dataView->resizeColumnsToContents();
    d_ptr->dataView->horizontalHeader();
    for(int i=0;i< d_ptr->dataView->horizontalHeader()->count();i++)
    {
        d_ptr->dataView->setColumnWidth(i, d_ptr->dataView->columnWidth(i) + 40);
//        d_ptr->dataView->setColumnWidth(i,90);
    }

}

void RadiationSourceTable::viewRSData(QModelIndex index)
{
    showRSDialog(index);
}

/*!
 * @brief 显示数据源覆盖刷新模式下的详细信息
 */
void RadiationSourceTable::showRSDialog(QModelIndex index)
{
    if(d_ptr->enRSReKind==COVER_RENOVATE)
    {
        RadiationSourceTableRenovateDialog dialog(this);
        dialog.resize(800,600);
        d_ptr->blDoubleClickedFlag=true;
        unsigned short usSourceNo=0;
        int row=index.row();
        RadiationSourceRenovate rsRe;
        QMap<int,RadiationSourceRenovate>::const_iterator iter;
        for(iter=d_ptr->rsReMap.begin();iter!=d_ptr->rsReMap.end();iter++)  //根据所在行找到数据源批号
        {
            rsRe=iter.value();
            if(rsRe.iInsertRow==row)
            {
                usSourceNo=rsRe.rsInfo.usSourceNo;
                break;
            }
        }

        QObject::connect(this,SIGNAL(sendRSDataList(RSDataList*)),&dialog,SLOT(recvRSDataList(RSDataList*)));
        dialog.setRadiationSourceList(usSourceNo,d_ptr->rsDataList);
        dialog.exec();
        d_ptr->blDoubleClickedFlag=false;
    }
}

void RadiationSourceTable::on_btn_load_clicked()
{
    Q_D(RadiationSourceTable);
    // 指定存储的位置
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                    QString(), tr("EXCEL files (*.xls *.xlsx);;HTML-Files (*.htm *.html);;"));
    if(filepath.isEmpty())
        return;
    DataExportAndPrint::exportToExcel(d->dataView,filepath);
}
} //namespace DataView
