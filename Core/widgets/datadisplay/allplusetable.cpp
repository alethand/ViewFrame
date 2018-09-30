#include "allplusetable.h"

#include <QString>
#include <QHBoxLayout>
#include <QRadioButton>

//#include "table.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmodelcustom.h"

namespace DataView {

class AllPluseTablePrivate
{
    Q_DECLARE_PUBLIC(AllPluseTable)
public:
    explicit AllPluseTablePrivate(AllPluseTable * q):q_ptr(q),showTableMode(AllPluseTable::RealTimeDisplay)
    {
        initView();
    }
    void initView();

    //Table *allPluseTable;
    AllPluseTable::WorkModel showTableMode;

    QWidget * mainWidget;
    AllPluseTable * q_ptr;

    QRadioButton * radioButton_RealityShow;
    QRadioButton * radioButton_HistoryShow;

    TableView* dataViewStatictis;                   //统计信息表格
    TableViewModelCustom* dataViewModelStatictis;
    TableView* dataViewOriginal;                    //原始信息表格
    TableViewModelCustom* dataViewModelOriginal;
};

void AllPluseTablePrivate::initView()
{
    mainWidget = new QWidget();

    QWidget * radioWidget = new QWidget();
    radioButton_RealityShow = new QRadioButton(radioWidget);
    radioButton_HistoryShow = new QRadioButton(radioWidget);

    QObject::connect(radioButton_RealityShow,SIGNAL(clicked()),q_ptr,SLOT(on_radioButton_RealityShow_clicked()));
    QObject::connect(radioButton_RealityShow,SIGNAL(clicked()),q_ptr,SLOT(on_radioButton_HistoryShow_clicked()));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addStretch(1);
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(radioButton_RealityShow);
    layout->addWidget(radioButton_HistoryShow);
    radioWidget->setLayout(layout);

    dataViewStatictis=new TableView(q_ptr);
    dataViewStatictis->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dataViewModelStatictis=new TableViewModelCustom(dataViewStatictis);
    dataViewStatictis->setModel(dataViewModelStatictis);
    dataViewModelStatictis->setTableCustomKind(STATISTICAL_INFO);

    dataViewOriginal=new TableView(q_ptr);
    dataViewOriginal->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dataViewModelOriginal=new TableViewModelCustom(dataViewOriginal);
    dataViewOriginal->setModel(dataViewModelOriginal);
    dataViewModelOriginal->setTableCustomKind(ORIGINAL_INFO);

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(radioWidget);

    vlayout->addWidget(dataViewStatictis);
    vlayout->addWidget(dataViewOriginal);

    mainWidget->setLayout(vlayout);
}

AllPluseTable::AllPluseTable(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_ALL_PLUS_TABLE,parent),d_ptr(new AllPluseTablePrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);
    initAllPulseTable();
    retranslateUi();
}

AllPluseTable::~AllPluseTable()
{
}

bool AllPluseTable::initialize()
{
    Q_D(AllPluseTable);
    setWidget(d->mainWidget);
    return true;
}

void AllPluseTable::release()
{

}

QString AllPluseTable::pluginName()
{
    return tr("DataDisplay panel");
}

void AllPluseTable::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void AllPluseTable::initAllPulseTable()
{
    Q_D(AllPluseTable);

    //初始化统计信息列表
    QStringList headInfo;
    headInfo<<QStringLiteral("序号")<<QStringLiteral("统计参数名称")<<QStringLiteral("最小值")<<QStringLiteral("最大值")<<QStringLiteral("均值")
               <<QStringLiteral("均方差");

    d_ptr->dataViewModelStatictis->resetHeadInfo(headInfo);
    d_ptr->dataViewStatictis->setColumnWidth(0,70);
    for(int i=1;i<headInfo.size();i++)
    {
        d_ptr->dataViewStatictis->setColumnWidth(i,110);
    }

    headInfo.clear();
    int N=5;
    int M=3;
    QString strValue;
    headInfo<<QStringLiteral("序号");
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++)
        {
            strValue=QStringLiteral("原始数据%1属性%2名称").arg(i+1).arg(j+1);
            headInfo<<strValue;

            strValue=QStringLiteral("原始数据%1属性%2是否绘图").arg(i+1).arg(j+1);
            headInfo<<strValue;

            strValue=QStringLiteral("原始数据%1属性%2值").arg(i+1).arg(j+1);
            headInfo<<strValue;
        }
    }

    d_ptr->dataViewModelOriginal->resetHeadInfo(headInfo);
    d_ptr->dataViewOriginal->setColumnWidth(0,70);
    for(int i=1;i<headInfo.size();i++)
    {
        d_ptr->dataViewOriginal->setColumnWidth(i,110);
    }

}

/*!
 * @brief 改变表格头部信息以及列数
 * @param[in] blRealityTypeFlag 实时模式
 */
void AllPluseTable::changeTableHeaderInfo(bool blRealityTypeFlag)
{
    Q_D(AllPluseTable);
//    for(int i=0;i<d->allPluseTable->rowCount();i++)
//    {
//        QString strValue=QString("%1").arg(i+1);           //行号
//        d->allPluseTable->addRowValue(i,0,strValue);
//    }

    QStringList headInfo;
    int colCount=50;    //列数
    QString strValue;
    if(blRealityTypeFlag) //实时模式
    {
         headInfo<<QStringLiteral("序号")<<QStringLiteral("统计参数名称")<<QStringLiteral("最小值")<<QStringLiteral("最大值")<<QStringLiteral("均值")
                    <<QStringLiteral("均方差")<<QStringLiteral("原始数据是否有效")<<QStringLiteral("原始数据个数");

         int N=5;
         int M=3;
         for(int i=0;i<N;i++)
         {
             for(int j=0;j<M;j++)
             {
                 strValue=QStringLiteral("原始数据%1属性%2名称").arg(i+1).arg(j+1);
                 headInfo<<strValue;

                 strValue=QStringLiteral("原始数据%1属性%2是否绘图").arg(i+1).arg(j+1);
                 headInfo<<strValue;

                 strValue=QStringLiteral("原始数据%1属性%2值").arg(i+1).arg(j+1);
                 headInfo<<strValue;
             }
         }
    }
    else  //历史模式
    {
        int N=10;
        headInfo<<QStringLiteral("序号")<<QStringLiteral("统计参数是否有效")<<QStringLiteral("统计参数个数");
        for(int i=0;i<N;i++)
        {
            strValue=QStringLiteral("统计参数%1名称").arg(i+1);
            headInfo<<strValue;

            strValue=QStringLiteral("统计参数%1最小值").arg(i+1);
            headInfo<<strValue;

            strValue=QStringLiteral("统计参数%1最大值").arg(i+1);
            headInfo<<strValue;

            strValue=QStringLiteral("统计参数%1均值").arg(i+1);
            headInfo<<strValue;

            strValue=QStringLiteral("统计参数%1均方差").arg(i+1);
            headInfo<<strValue;
        }
    }

    //d->allPluseTable->setColumnValue(colCount, headInfo);
}

void AllPluseTable::retranslateUi()
{
    Q_D(AllPluseTable);
    m_name = tr("Full Pulse List");
    setWindowTitle(m_name);

    d->radioButton_RealityShow->setText(tr("Real-time display"));
    d->radioButton_HistoryShow->setText(tr("Historical display"));
}

/*!
 * @brief 实时模式单选框槽
 */
void AllPluseTable::on_radioButton_RealityShow_clicked()
{
    Q_D(AllPluseTable);
    if(d->showTableMode != RealTimeDisplay)
    {
        d->showTableMode = RealTimeDisplay;
        changeTableHeaderInfo(true);
    }
}

/*!
 * @brief 历史模式单选框槽
 */
void AllPluseTable::on_radioButton_HistoryShow_clicked()
{
    Q_D(AllPluseTable);
    if(d->showTableMode!=HistoricalDisplay)
    {
        d->showTableMode = HistoricalDisplay;
        changeTableHeaderInfo(false);
    }
}

/*!
 * @brief 接收全脉冲数据
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void AllPluseTable::recvAllPlusePara(char *buff, int len)
{

}

} //namespace DataView
