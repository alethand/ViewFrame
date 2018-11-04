#include "allplusegraphics.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QGridLayout>
#include <QScatterSeries>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QPointF>
#include <QFont>
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
        initTimeParaGraphis();
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

/*
QChart* m_chart;
    QLineSeries *m_series;
    int timerId;*/
    //QLineSeries *m_series;
    QScatterSeries *m_series_Time;
    QValueAxis *axisY_Time;             //时间特性图Y轴
    //QBarCategoryAxis *axisX_Time;       //时间特性图X轴
    QDateTimeAxis *axisX_Time;
};

void AllPluseGraphicsPrivate::initView()
{
    mainWidget = new QWidget();

//    QVBoxLayout * vlayout = new QVBoxLayout;
//    vlayout->addWidget(chartTimeView);
    QGridLayout *layout=new QGridLayout;
    layout->setContentsMargins(0,0,0,0);

    QWidget *wightTimeView=new QWidget();
    QVBoxLayout * layoutTime = new QVBoxLayout;

    layoutTime->addWidget(chartTimeView);// chartTimeView

    wightTimeView->setLayout(layoutTime);

    QWidget *wightTimeBar=new QWidget();
    QVBoxLayout * layoutBar = new QVBoxLayout;

    layoutBar->addWidget(chartBarView);//chartBarView
    wightTimeBar->setLayout(layoutBar);

    layout->addWidget(wightTimeView,0,0);
    layout->addWidget(wightTimeBar,0,1);
    mainWidget->setLayout(layout);
}

AllPluseGraphics::AllPluseGraphics(QWidget *parent) :timeScatter(QStringLiteral("时间特性图")),freqHistogram(QStringLiteral("频度直方图")),
    timeAreaWaveForm(QStringLiteral("时域波形图")),
    Base::RComponent(Constant::PLUGIN_ALL_PLUS_GRAPHICS,parent),d_ptr(new AllPluseGraphicsPrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);
    retranslateUi();

//    timeScatter.setXAxis(DataType::DateTime,QStringLiteral("到达时间"));
//    timeScatter.addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft,QStringLiteral("脉冲特性"));
//    timeScatter.addSeries(0);timeScatter.addSeries(1);
//    timeScatter.addData(0,20); timeScatter.addData(0,20,1);
//    timeScatter.show();

//    QStringList categories;
//    categories << "W1" << "W2" << "W3" << "W4" << "W5" << "W6";
//    freqHistogram.setXAxisLabels(categories,QStringLiteral("特性分布"));
//    freqHistogram.addBarSet(0);
//    freqHistogram.addData(0,1);
//     freqHistogram.addData(1,2);
//      freqHistogram.show();

    //HugeData_Model model =new HugeData_Model(new HugeData_Gram());


//    QList<QDateTime> times;
//    times.append(QDateTime(QDate(2008,1,1)));
//    times.append(QDateTime(QDate(2009,1,1)));
////    timeAreaWaveForm.addAxis(AxisType::xAxis,DataType::DateTime,Qt::AlignBottom,QStringLiteral("时间"));
////    timeAreaWaveForm.setRange(AxisType::xAxis,times.at(0),times.at(1),"M/dd");
////    timeAreaWaveForm.setTickCount(AxisType::xAxis,10);

//    timeAreaWaveForm.addAxis(AxisType::xAxis,DataType::value,Qt::AlignBottom,QStringLiteral("时间"));
//    timeAreaWaveForm.setRange(AxisType::xAxis,0,1500);
//     timeAreaWaveForm.setTickCount(AxisType::xAxis,20);

//    timeAreaWaveForm.addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft,QStringLiteral("幅度"));
//    timeAreaWaveForm.setRange(AxisType::yAxis,0,500);
//    //timeAreaWaveForm.useDataSrc(NULL);
//   // timeAreaWaveForm.setTimeLables(times);

//   // timeAreaWaveForm.setYRange(0,100);
//    timeAreaWaveForm.show();

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

QString AllPluseGraphics::pluginName()
{
    return tr("DataDisplay panel");
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

    QChart* m_chart;
    m_chart=new QChart();

    m_series_Time=new QScatterSeries(m_chart);// QLineSeries
    m_chart->addSeries(m_series_Time);

    //m_series->setUseOpenGL(true);

    axisX_Time=new QDateTimeAxis;

    axisX_Time->setFormat(QString("hh:mm:ss.zzz"));
    QFont font;
    font.setWeight(10);
    axisX_Time->setLabelsFont(font);
    axisX_Time->setTitleText(QStringLiteral("到达时间"));
    axisX_Time->setTickCount(6);

//    QValueAxis *axisX = new QValueAxis();
//    axisX->setRange(0,maxSize);
//    axisX->setLabelFormat("%g");
//    axisX->setTitleText(QStringLiteral("到达时间"));
//    axisX->setTickCount(11);

    axisY_Time=new QValueAxis();
    axisY_Time->setRange(-5,5);
    axisY_Time->setLabelFormat("%0.2f");
    axisY_Time->setTitleText(QStringLiteral("脉冲特性"));
    axisY_Time->setTickCount(11);


    m_chart->addAxis(axisX_Time, Qt::AlignBottom);
    m_chart->addAxis(axisY_Time, Qt::AlignLeft);
    m_series_Time->attachAxis(axisX_Time);
    m_series_Time->attachAxis(axisY_Time);

    //m_chart->setAxisX(axisX_Time,m_series_Time);
    //m_chart->setAxisY(axisY_Time,m_series_Time);

    m_chart->legend()->hide();
    m_chart->setTitle(QStringLiteral("全脉冲数据时间特性图"));

//    m_series_Time->append(0.5,2);
//    m_series_Time->append(0,3);

    chartTimeView=new QChartView(m_chart);

    int them=1;
    QChart::ChartTheme theme = (QChart::ChartTheme)them ;
    chartTimeView->chart()->setTheme(theme);

    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chart->setWindowFrameMargins(0,0,0,0);

/*  int maxSize=500;

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
*/
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
        chart->setTitle(QStringLiteral("全脉冲参数频度直方图"));
        chart->setAnimationOptions(QChart::SeriesAnimations);
    //![3]

    //![4]
        QStringList categories;
        categories << "W1" << "W2" << "W3" << "W4" << "W5" << "W6";
        //QString str=QStringLiteral("参数分布");
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis->append(categories);
        axis->setTitleText(QStringLiteral("参数分布"));

        chart->createDefaultAxes();
        chart->setAxisX(axis, series);
    //![4]

        chart->axisX()->setReverse(true);

    //![5]
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
    //![5]

    //![6]
        chartBarView = new QChartView(chart);
        chartBarView->setRenderHint(QPainter::Antialiasing);
    //![6]

    //![7]
        int them=1;
        QChart::ChartTheme theme = (QChart::ChartTheme)them ;
        chartBarView->chart()->setTheme(theme);
        //![7]
}

void AllPluseGraphics::retranslateUi()
{
    m_name = tr("DataDisplay panel");
    setWindowTitle(m_name);
}

/*!
 * \brief  接收全脉冲信息列表信号
 * \param statisticInfoList 统计信息
 * \param originalInfoList 原始信息
 */
//void AllPluseGraphics::recvAllPulseInfoList(AllPluseStatisticInfoList* statisticInfoList,AllPulseOriginalInfoList* originalInfoList)
//{
//    allPluseStatisticInfoList=*statisticInfoList;
//    allPluseOriginalInfoList=*originalInfoList;

//    drawTimeParaGraphis();
//    drawBarGraphis();
//}


/*!
 * \brief 绘制频度直方图
 */
void AllPluseGraphics::drawBarGraphis()
{

}

} //namespace DataView
