#include "radiationsourcetablerenovatedialog.h"
#include "ui_radiationsourcetablerenovatedialog.h"
#include "modelview/tableviewdata.h"
#include "modelview/tableviewmoderradiationsource.h"

#include <QVBoxLayout>
#include <QString>

namespace DataView {

RadiationSourceTableRenovateDialog::RadiationSourceTableRenovateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RadiationSourceTableRenovateDialog)
{
    ui->setupUi(this);

    QString m_name = tr("Data source Renovate");
    setWindowTitle(m_name);

    QVBoxLayout * vlayout = new QVBoxLayout;

    dataView = new TableView(this);
    dataView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    dataViewModel = new TableViewModelRadiationSource(dataView);
    dataView->setModel(dataViewModel);
    vlayout->addWidget(dataView);
    setLayout(vlayout);

    initTableHeaderInfo();
}

RadiationSourceTableRenovateDialog::~RadiationSourceTableRenovateDialog()
{
    rsDataList.clear();
    delete dataViewModel;
    delete dataView;
    delete ui;
}

/*!
 * @brief 初始化表格头部信息以及列数
 */
void RadiationSourceTableRenovateDialog::initTableHeaderInfo()
{
    QStringList headInfo;
    headInfo<<QStringLiteral("序号")<<QStringLiteral("辐射源批号")<<QStringLiteral("脉间类型")<<QStringLiteral("脉内类型")<<QStringLiteral("载频个数")
               <<QStringLiteral("连续波标记")<<QStringLiteral("载频脉组内脉冲数")<<QStringLiteral("载频频段码")<<QStringLiteral("RF1")<<QStringLiteral("RF2")
                <<QStringLiteral("RF3")<<QStringLiteral("RF4")<<QStringLiteral("RF5")<<QStringLiteral("RF6")<<QStringLiteral("RF7")
                  <<QStringLiteral("RF8")<<QStringLiteral("重频类型")<<QStringLiteral("重频个数")<<QStringLiteral("重频脉组内脉冲数")<<QStringLiteral("PRI1")
                    <<QStringLiteral("PRI2")<<QStringLiteral("PRI3")<<QStringLiteral("PRI4")<<QStringLiteral("PRI5")<<QStringLiteral("PRI6")
                      <<QStringLiteral("PRI7")<<QStringLiteral("PRI8")<<QStringLiteral("脉宽类型")<<QStringLiteral("脉宽个数")<<QStringLiteral("脉宽脉组内脉冲数")
                        <<QStringLiteral("PW1")<<QStringLiteral("PW2")<<QStringLiteral("PW3")<<QStringLiteral("PW4")<<QStringLiteral("PW5")
                          <<QStringLiteral("PW6")<<QStringLiteral("PW7")<<QStringLiteral("PW8")<<QStringLiteral("数字幅度")<<QStringLiteral("模拟幅度")
                            <<QStringLiteral("数字功率")<<QStringLiteral("模拟功率")<<QStringLiteral("方位角")<<QStringLiteral("俯仰角")<<QStringLiteral("经度")
                              <<QStringLiteral("纬度")<<QStringLiteral("高度")<<QStringLiteral("脉内有效标识")<<QStringLiteral("脉内特征信息")<<QStringLiteral("CRC校验")
                                <<QStringLiteral("截获次数")<<QStringLiteral("截获时间");

    for(int i=0;i<headInfo.size();i++)
    {
        dataView->setColumnWidth(i,90);
    }

    dataViewModel->resetHeadInfo(headInfo);
}

/*!
 * @brief 设置数据源列表
 * @param[in] rsId 数据源批号
 * @param[in] rsList 数据源列表
 */
void RadiationSourceTableRenovateDialog::setRadiationSourceList(const unsigned short rsId,const RSDataList& rsList)
{
    usSourceNo=rsId;
    rsDataList=rsList;
    showTablePara();
}

/*!
 * @brief 设置数据源参数
 */
void RadiationSourceTableRenovateDialog::showTablePara()
{
    RadiationSourceRenovate rsRm;
    RSDataList rsList;
    for(int i=0;i<rsDataList.size();i++) //过滤
    {
        rsRm=rsDataList.at(i);
        if(rsRm.rsInfo.usSourceNo==usSourceNo)      //过滤非目标批号信息
        {
            rsList.append(rsRm);
        }
    }
    dataViewModel->updateTableData(rsList,true);
}

void RadiationSourceTableRenovateDialog::recvRSDataList(RSDataList* rsList)
{
    rsDataList=*rsList;
    showTablePara();
}


}//namespace DataView
