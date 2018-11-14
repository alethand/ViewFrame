#include "mfacquisitiongraphics.h"

#include <QVBoxLayout>
#include <QValueAxis>
#include <QPointF>
#include <QtCharts/QChartView>
#include <QtCharts/qchart.h>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

#include "Base/constants.h"
#include "Base/util/rsingleton.h"

namespace DataView {

class MFAcquisitionGraphicsPrivate
{
    Q_DECLARE_PUBLIC(MFAcquisitionGraphics)
public:
    explicit MFAcquisitionGraphicsPrivate(MFAcquisitionGraphics * q):q_ptr(q)
    {
        initTimeFiledGraphis();
        initView();
    }
    void initView();
    void initTimeFiledGraphis();

    QWidget * mainWidget;
    MFAcquisitionGraphics * q_ptr;

    QChartView* chartTimeFiledView;  //时域图
    QChart* m_chart;
    QLineSeries *m_series;
    int timerId;
};

void MFAcquisitionGraphicsPrivate::initView()
{
    mainWidget = new QWidget();

    QVBoxLayout * vlayout = new QVBoxLayout;
    vlayout->addWidget(chartTimeFiledView);

    mainWidget->setLayout(vlayout);
}

MFAcquisitionGraphics::MFAcquisitionGraphics(QWidget *parent) :
    Core::RComponent(Constant::PLUGIN_MF_QUISITION_GRAPHICS,parent),d_ptr(new MFAcquisitionGraphicsPrivate(this))
{
    RSingleton<Core::Subject>::instance()->attach(this);
    retranslateUi();
}

MFAcquisitionGraphics::~MFAcquisitionGraphics()
{

}

QWidget *MFAcquisitionGraphics::initialize(QWidget *parent)
{
    Q_D(MFAcquisitionGraphics);
//    setWidget(d->mainWidget);
    return d->mainWidget;
}

void MFAcquisitionGraphics::release()
{

}

QString MFAcquisitionGraphics::pluginName()
{
    return tr("DataDisplay panel");
}

Core::RComponent *MFAcquisitionGraphics::clone()
{
    return new MFAcquisitionGraphics;
}

void MFAcquisitionGraphics::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void MFAcquisitionGraphics::onNetwork(int protocolType, Datastruct::FieldValues &data)
{

}

/*!
 * @brief 初始化时域图
 */
void MFAcquisitionGraphicsPrivate::initTimeFiledGraphis()
{
    int maxSize=10000;

    m_chart=new QChart();
    chartTimeFiledView = new QChartView(m_chart);
    chartTimeFiledView->setRubberBand(QChartView::RectangleRubberBand);

    m_series=new QLineSeries();
    m_chart->addSeries(m_series);

    //导致窗口全屏后无法菜单栏无法弹出
//    m_series->setUseOpenGL(true);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0,maxSize);
    axisX->setLabelFormat("%g");
    axisX->setTitleText(QStringLiteral("点数"));
    axisX->setTickCount(11);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,10);
    axisY->setTitleText(QStringLiteral("幅度"));
    axisY->setTickCount(11);

    m_chart->setAxisX(axisX,m_series);
    m_chart->setAxisY(axisY,m_series);
    m_chart->legend()->hide();
    m_chart->setTitle(QStringLiteral("中频采集数据时域波形图"));

    int them=2;
    QChart::ChartTheme theme = (QChart::ChartTheme)them ;
    chartTimeFiledView->chart()->setTheme(theme);
}

void MFAcquisitionGraphics::retranslateUi()
{
    m_name = tr("Intermediate Frequency Data Graphics");
    setObjectName("mfGraphics");
//    setWindowTitle(m_name);
}

} //namespace DataView
