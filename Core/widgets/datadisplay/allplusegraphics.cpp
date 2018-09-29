#include "allplusegraphics.h"

#include <QVBoxLayout>
#include <QValueAxis>
#include <QPointF>
#include <QBrush>
#include <QColor>
#include <QtCharts/QChartView>
#include <QtCharts/qchart.h>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

#include "Base/util/rsingleton.h"
#include "Base/constants.h"

namespace DataView {

class AllPluseGraphicsPrivate
{
    Q_DECLARE_PUBLIC(AllPluseGraphics)
public:
    explicit AllPluseGraphicsPrivate(AllPluseGraphics * q):q_ptr(q)
    {
//        initTimeParaGraphis();
        initBarGraphis();
        initView();
    }
    void initView();
    void initTimeParaGraphis();
    void initBarGraphis();

    QWidget * mainWidget;
    AllPluseGraphics * q_ptr;

    QChartView* chartTimeView;          //时间特性图
    QChartView* chartBarView;           //直方图
    QChart* m_chart;
    QLineSeries *m_series;
    int timerId;
};

void AllPluseGraphicsPrivate::initView()
{
    mainWidget = new QWidget();

    QVBoxLayout * vlayout = new QVBoxLayout;
//    vlayout->addWidget(chartTimeView);
    vlayout->addWidget(chartBarView);

    mainWidget->setLayout(vlayout);
}

AllPluseGraphics::AllPluseGraphics(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_ALL_PLUS_GRAPHICS,parent),d_ptr(new AllPluseGraphicsPrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);
    retranslateUi();
}

AllPluseGraphics::~AllPluseGraphics()
{

}

bool AllPluseGraphics::initialize()
{
    Q_D(AllPluseGraphics);
    setWidget(d->mainWidget);
    return true;
}

void AllPluseGraphics::release()
{

}

void AllPluseGraphics::onMessage(MessageType::MessType type)
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
 * @brief 初始化时间特性图
 */
void AllPluseGraphicsPrivate::initTimeParaGraphis()
{
    int maxSize=500;

    m_chart=new QChart();
    chartTimeView = new QChartView(m_chart);
    chartTimeView->setRubberBand(QChartView::RectangleRubberBand);

    m_series=new QLineSeries();
    m_chart->addSeries(m_series);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0,maxSize);
    axisX->setLabelFormat("%g");
    axisX->setTitleText(QStringLiteral("到达时间"));
    axisX->setTickCount(11);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(-5,5);
    axisY->setTitleText(QStringLiteral("脉冲特性"));
    axisY->setTickCount(11);

    m_chart->setAxisX(axisX,m_series);
    m_chart->setAxisY(axisY,m_series);
    m_chart->legend()->hide();
    m_chart->setTitle(QStringLiteral("全脉冲数据时间特性图"));

    int them=2;
    QChart::ChartTheme theme = (QChart::ChartTheme)them ;
    chartTimeView->chart()->setTheme(theme);

    *m_series << QPointF(1, 4.5) << QPointF(189, -2) << QPointF(398, 1.5) << QPointF(500, -2.5);
}


/*!
 * @brief 初始化频度直方图
 */
void AllPluseGraphicsPrivate::initBarGraphis()
{
    //![1]
        QBarSet *set0 = new QBarSet("PARA1");
        QBarSet *set1 = new QBarSet("PARA2");
        QBarSet *set2 = new QBarSet("PARA3");
        QBarSet *set3 = new QBarSet("PARA4");
        QBarSet *set4 = new QBarSet("PARA5");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6;
        *set1 << 5 << 0 << 0 << 4 << 0 << 7;
        *set2 << 3 << 5 << 8 << 13 << 8 << 5;
        *set3 << 5 << 6 << 7 << 3 << 4 << 5;
        *set4 << 9 << 7 << 5 << 3 << 1 << 2;
    //![1]

    //![2]
        QBarSeries *series = new QBarSeries();
        series->append(set0);
        series->append(set1);
        series->append(set2);
        series->append(set3);
        series->append(set4);

    //![2]

    //![3]
        QChart *chart = new QChart();
        chart->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        chart->addSeries(series);
//        chart->setTitle(QStringLiteral("全脉冲参数频度直方图"));
        chart->setAnimationOptions(QChart::SeriesAnimations);
    //![3]

    //![4]
        QStringList categories;
        categories << "W1" << "W2" << "W3" << "W4" << "W5" << "W6";
        //QString str=QStringLiteral("参数分布");
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis->append(categories);
//        axis->setTitleText(QStringLiteral("参数分布"));
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);
    //![4]

    //![5]
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
    //![5]

    //![6]
        chartBarView = new QChartView(chart);
        chartBarView->setRenderHint(QPainter::Antialiasing);
    //![6]

    //![7]
        int them=2;
        QChart::ChartTheme theme = (QChart::ChartTheme)them ;
        chartBarView->chart()->setTheme(theme);
        //![7]
}

void AllPluseGraphics::retranslateUi()
{
    m_name = tr("DataDisplay panel");
    setWindowTitle(m_name);
}

} //namespace DataView
