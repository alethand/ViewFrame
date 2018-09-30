#include "mfacquistiontable.h"

#include <QVBoxLayout>
#include <QHeaderView>

#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmodelcustom.h"

namespace DataView {

class MFAcquistionTablePrivate
{
    Q_DECLARE_PUBLIC(MFAcquistionTable)
public:
    explicit MFAcquistionTablePrivate(MFAcquistionTable * q):q_ptr(q)
    {
        initView();
    }
    void initView();

    MFAcquistionTable * q_ptr;

    QWidget * mainWidget;
    TableView* dataView;
    TableViewModelCustom* dataViewModel;
};

void MFAcquistionTablePrivate::initView()
{
    mainWidget = new QWidget();

    QVBoxLayout * vlayout = new QVBoxLayout;

    dataView=new TableView(q_ptr);
    dataView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dataViewModel=new TableViewModelCustom(dataView);
    dataView->setModel(dataViewModel);
    vlayout->addWidget(dataView);
    dataViewModel->setTableCustomKind(MF_ACQUISITION_INFO);

    mainWidget->setLayout(vlayout);
}

MFAcquistionTable::MFAcquistionTable(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_MF_ACQUISTION_TABLE,parent),d_ptr(new MFAcquistionTablePrivate(this))
{
    initMFAcquistionTable();
    RSingleton<Base::Subject>::instance()->attach(this);
    retranslateUi();
}

MFAcquistionTable::~MFAcquistionTable()
{

}

bool MFAcquistionTable::initialize()
{
    Q_D(MFAcquistionTable);
    setWidget(d->mainWidget);

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

void MFAcquistionTable::initMFAcquistionTable()
{
    Q_D(MFAcquistionTable);

    QStringList headInfo;

    headInfo<<QStringLiteral("序号")<<QStringLiteral("采集时间")<<QStringLiteral("采集模式")<<QStringLiteral("采集脉冲个数")
                    <<QStringLiteral("采集点数");
    d_ptr->dataViewModel->resetHeadInfo(headInfo);
    d_ptr->dataView->setColumnWidth(0,90);
    for(int i=1;i<headInfo.size();i++)
    {
        d_ptr->dataView->setColumnWidth(i,130);
    }

}

void MFAcquistionTable::retranslateUi()
{
    m_name = tr("Intermediate Frequency Data List");
    setWindowTitle(m_name);
}

/*!
 * @brief 接收中频数据
 * @param[in] buff 缓冲区
 * @param[in] len 缓冲区长度
 */
void MFAcquistionTable::recvMFAcquistionPara(char *buff, int len)
{

}

} //namespace DataView
