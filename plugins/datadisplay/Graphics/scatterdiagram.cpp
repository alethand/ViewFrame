#include "scatterdiagram.h"
#include <QDebug>

namespace Diagram/*! 图表 */
{




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
    if(xAxis.axis == NULL || yAxis.axis == NULL)
    {

    }
    m_chart.addSeries(pScatter);
    serials.insert(index,pScatter);

    serials.at(index)->attachAxis(xAxis.axis);
    serials.at(index)->attachAxis(yAxis.axis);


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

/*!
 * \brief Scatter::addData  用来处理只有x轴的取值范围与纵轴
 * \param minX
 * \param maxX
 * \param values
 */
void Scatter::addData(double minX, double maxX, const QList<double> &values)
{
    QList <QPointF> tempValue;
    double interval = (maxX - minX)/values.count();
    for(int i=0;i< values.count();i++) {
        tempValue.append(QPointF(interval*i+minX,values.at(i)));
    }
    addData(tempValue);
}

void Scatter::initDefault()
{

    if(NULL == yAxis.axis)
    {
        addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft);
    }

    if(serials.isEmpty())
        addSeries();
}


}


