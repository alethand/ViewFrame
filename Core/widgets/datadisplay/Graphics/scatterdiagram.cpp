#include "scatterdiagram.h"
#include <QDebug>

namespace Diagram/*! 图表 */
{


/*!
 * \brief DiagramSuper::addAxis   加入坐标轴
 * \param t_Axis            坐标类型
 * \param t_Data            坐标数据
 * \param alignment         对齐方式
 */
DiagramSuper::DiagramSuper(QString name)
    :QWidget()
{
    m_chart.setTitle(name);
    xAxis = NULL;
    yAxis = NULL;
    view.setChart(&m_chart);
    QHBoxLayout *layout  = new QHBoxLayout();
    layout->addWidget(&view);
    layout->setMargin(0);
    this->setLayout(layout);

    //风格设置
    m_chart.setTheme(QChart::ChartThemeBlueCerulean);
}

void DiagramSuper::addAxis(AxisType::type t_Axis, DataType::type t_Data,Qt::Alignment alignment,QString name)
{
    QAbstractAxis **pAxis = NULL;

    switch(t_Axis)
    {
    case AxisType::xAxis:
                     pAxis = &xAxis ;

                    break;
         case AxisType::yAxis:
                     pAxis = &yAxis;
                    break;
     }

    switch(t_Data)
    {
    case DataType::value: *pAxis = new QValueAxis();
                          (*pAxis)->setRange(0,50);
        break;
    case DataType::DateTime:*pAxis =new QDateTimeAxis();

        break;
    }

    (*pAxis)->setTitleText(name);
    m_chart.addAxis(*pAxis,alignment);
}

void Scatter::setXAxis(DataType::type t_Data, QString name, QString format)
{
    addAxis(AxisType::xAxis,t_Data,Qt::AlignBottom,name);
    if(t_Data == DataType::DateTime)
    {
        if(format.isEmpty())
            format = "mm:ss";
        dynamic_cast<QDateTimeAxis*>(xAxis)->setFormat(format);
       // dynamic_cast<QDateTimeAxis*>(xAxis)->setTickCount(6);
    }

}


/*!
 * \brief Scatter::addSeries 添加散点图系列
 * \param index   为散点图系列号
 *              推荐从0顺序增加
 */
void Scatter::addSeries(int index)
{
    if(index != serials.count())
        return;

    QScatterSeries *pScatter = new QScatterSeries();
    if(xAxis == NULL || yAxis == NULL)
    {

    }
    m_chart.addSeries(pScatter);
    serials.insert(index,pScatter);

    serials.at(index)->attachAxis(xAxis);
    serials.at(index)->attachAxis(yAxis);


    m_chart.legend()->setVisible(serials.count()!=1);
}

void Scatter::addData(double valX,double valY, int index)
{
    initDefault();
    if(index >= serials.count() ||index < 0)
        return;
    serials.at(index)->append(valX,valY);
}

void Scatter::addData(const QList<QPointF> &values, int index)
{
    initDefault();
    if(index >= serials.count() ||index < 0)
        return;
    serials.at(index)->append(values);
}

void Scatter::initDefault()
{

    if(NULL == yAxis)
    {
        addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft);
    }

    if(serials.isEmpty())
        addSeries();
}

Histogram::Histogram(QString name):DiagramSuper(name){

    m_chart.addSeries(&serials);
    serials.setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    serials.setLabelsVisible(true);
}

void Histogram::addBarSet(int index,QString name)
{
    QBarSet *set = new QBarSet(name);
    sets.insert(index,set);
    serials.append(set);
    for(int i=0;i < ((QBarCategoryAxis*)xAxis)->categories().count();i++)
        set->insert(i,0);

    m_chart.legend()->setVisible(sets.count() !=1);
}

void Histogram::addData(double valX, double valY, int index)
{
    initDefault();
    double temp = sets.at(index)->at(valX);
    sets.at(index)->replace(valX,temp+valY);
}

void Histogram::setData(const QList<double> &values, int index)
{
    initDefault();
    if(sets.at(index)->count() != 0)
    {
        //todo 移除所有内容
    }
    sets.at(index)->append(values);
}

void Histogram::initDefault()
{
    if(sets.isEmpty())
    {
        /*! 自动加入空barSet*/
        addBarSet();
    }


    if(NULL == yAxis)
    {
        addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft);
        serials.attachAxis(yAxis);
    }
}


void Histogram::setXAxisLabels(QStringList list,QString name)
{
    if(xAxis == NULL)
    {
        xAxis = new QBarCategoryAxis();
        xAxis->setTitleText(name);
        ((QBarCategoryAxis*)xAxis)->append(list);
         m_chart.setAxisX(xAxis,&serials);
    }
}















}
