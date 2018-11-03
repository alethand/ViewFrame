#include "mfacquistiontable.h"



#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmodelcustom.h"
#include "../Util/dataexportandprint.h"

namespace DataView {

class MFAcquistionTablePrivate
{
    Q_DECLARE_PUBLIC(MFAcquistionTable)
public:
    explicit MFAcquistionTablePrivate(MFAcquistionTable * q):q_ptr(q)
      ,timeAreaWaveDiagram(QStringLiteral("中频时域波形图"))
    {
        initData();

        timeAreaWaveDiagram.addAxis(AxisType::xAxis,DataType::value,Qt::AlignBottom,QStringLiteral("时间"));
        timeAreaWaveDiagram.addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft,QStringLiteral("幅度"));
    }
    void initData();

    MFAcquistionTable * q_ptr;
    HugeData_Gram   midFreqData;  //中频采集数据列表
    HugeData_Model* model_MidFreq;
    TableView *viewList;           ///数据列表显示
    WaveForm  timeAreaWaveDiagram;///时域波形图
                                  ///FFT频谱--算法由8511提供
};

void MFAcquistionTablePrivate::initData()
{
    viewList=new TableView(q_ptr);
    viewList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    model_MidFreq=new HugeData_Model(&midFreqData);
    midFreqData.coreData.append(new MidFreqInfo::Core());
    viewList->setModel(model_MidFreq);

    viewList->setSortingEnabled(true);
    QSortFilterProxyModel *dataViewProxy = new QSortFilterProxyModel();
    dataViewProxy->setSourceModel(model_MidFreq);
    viewList->setModel(dataViewProxy);
}


MFAcquistionTable::MFAcquistionTable(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_MF_ACQUISTION_TABLE,parent),d_ptr(new MFAcquistionTablePrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);
    retranslateUi();
}

MFAcquistionTable::~MFAcquistionTable()
{

}

bool MFAcquistionTable::initialize()
{
    mainWidget = new QWidget();

    QWidget * radioWidget = new QWidget();
    btn_load = new QPushButton(radioWidget);
    btn_load->setText(QObject::tr("Output"));
    QObject::connect(btn_load,SIGNAL(clicked()),this,SLOT(on_btn_load_clicked()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(btn_load);
    layout->addStretch(1);
    layout->setContentsMargins(1,1,1,1);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(radioWidget);
    radioWidget->setLayout(layout);
    vlayout->addWidget(d_ptr->viewList);
    mainWidget->setLayout(vlayout);
    setWidget(mainWidget);



    QAction *clearAction = new QAction();
    d_ptr->viewList->addAction(clearAction);
    QObject::connect(clearAction, SIGNAL(triggered(bool)), this, SLOT(clearTable()));
    clearAction->setText(QObject::tr("Clear Table"));
    QAction *timeAreawave = new QAction();
    d_ptr->viewList->addAction(timeAreawave);
    timeAreawave->setText(QStringLiteral("时域频谱图"));
    connect(timeAreawave,SIGNAL(triggered(bool)),this,SLOT(showTimeAreaFreq()));
    QAction *fftDigram = new QAction();
    fftDigram->setText(QStringLiteral("FFT"));
     d_ptr->viewList->addAction(fftDigram);
    connect(fftDigram,SIGNAL(triggered(bool)),this,SLOT(showFFT()));

    return true;
}

void MFAcquistionTable::release()
{

}

QString MFAcquistionTable::pluginName()
{
    return  tr("DataDisplay panel");
}

void MFAcquistionTable::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}



void MFAcquistionTable::retranslateUi()
{
    QStringList headInfo;

    headInfo<<QObject::tr("serial number")<<QObject::tr("acquisition time")<<QObject::tr("acquisition mode")<<QObject::tr("collection pulse number")
                    <<QObject::tr("collection points");
    //d_ptr->model_MidFreq->resetHeadInfo(headInfo);

    m_name = tr("Intermediate Frequency Data List");
    setWindowTitle(m_name);
    d_ptr->viewList->resizeColumnsToContents();
    d_ptr->viewList->horizontalHeader();
    for(int i=0;i< d_ptr->viewList->horizontalHeader()->count();i++)
    {
        d_ptr->viewList->setColumnWidth(i, d_ptr->viewList->columnWidth(i) + 40);
//        d_ptr->dataView->setColumnWidth(i,90);
    }
}

/*!
 * @brief 接收中频数据
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void MFAcquistionTable::recvMidFreqData(char *buff, int len)
{
    MidFreqInfo::Core *mfAcquisitionInfo = new MidFreqInfo::Core();
    short mfFlag;               //中频数据标识

    if(len<20)
	  {
        return;
    }
    memcpy(&mfFlag,buff+2,sizeof(short));
    if(mfFlag!=0xBBBB)
    {
        return;
    }
    memcpy(&(mfAcquisitionInfo->acqMode),buff+6,sizeof(short));
    memcpy(&(mfAcquisitionInfo->iAcqNum),buff+8,sizeof(int));  //个数

    char* pBuff=buff+12;
    int curPackCnt = 0;
    while(mfAcquisitionInfo->iAcqNum>0 && curPackCnt <mfAcquisitionInfo->iAcqNum )
    {
        ulong head;
        memcpy(&head,pBuff,sizeof(head));
        if(head == 0x7E7E9B9B) {
            mfAcquisitionInfo->listInfo.append(MidFreqInfo::Core::OneMsg());
            pBuff += 12;
            ushort value;
            while((*pBuff!=0xCC)&&(*(pBuff+1)!=0xBB)&&(*(pBuff+2)!=0xAA))
            {
               memcpy(&value,pBuff,sizeof(ushort));
               mfAcquisitionInfo->listInfo.back().adData.append(value);
               pBuff +=2;
            }
            pBuff +=3;
        }
        else
            break;
    }

    mfAcquisitionInfo->acqTime=getCurrentDate();
    d_ptr->midFreqData.coreData.append(mfAcquisitionInfo);
    d_ptr->viewList->setModel(d_ptr->model_MidFreq);
   // d_ptr->model_MidFreq->updateMFAcquistionInfoList(d_ptr->midFreqData);
}

void MFAcquistionTable::on_btn_load_clicked()
{
    Q_D(MFAcquistionTable);
    // 指定存储的位置
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                    QString(), tr("EXCEL files (*.xls *.xlsx);;HTML-Files (*.htm *.html);;"));
    if(filepath.isEmpty())
        return;
    DataExportAndPrint::exportToExcel(d->viewList,filepath);
}

void MFAcquistionTable::showTimeAreaFreq()
{
    Q_D(MFAcquistionTable);
    d->timeAreaWaveDiagram.show();
}

void MFAcquistionTable::showFFT()
{
    //Q_D(MFAcquistionTable);

}

/*!
 * @brief 获取当前时间
 */
QDateTime MFAcquistionTable::getCurrentDate()
{
    QString strDate;
    QDateTime date=QDateTime::currentDateTime();
    //strDate=date.toString("yyyy.MM.dd hh:mm:ss.zzz");
    return date;
}

/*!
 * @brief 清空表格
 */
void MFAcquistionTable::clearTable()
{
//    d_ptr->midFreqData.clear();
//    d_ptr->model_MidFreq->updateMFAcquistionInfoList(d_ptr->midFreqData);
}

} //namespace DataView
