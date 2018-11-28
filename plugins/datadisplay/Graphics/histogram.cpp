#include "histogram.h"

namespace Diagram {

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
    for(int i=0;i < ((QBarCategoryAxis*)xAxis.axis)->categories().count();i++)
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


    if(NULL == yAxis.axis)
    {
        addAxis(AxisType::yAxis,DataType::value,Qt::AlignLeft);
        serials.attachAxis(yAxis.axis);
    }
}




void Histogram::setXAxisLabels(QStringList list,QString name)
{
    if(xAxis.axis == NULL)
    {
        xAxis.axis = new QBarCategoryAxis();
        xAxis.axis->setTitleText(name);
        ((QBarCategoryAxis*)xAxis.axis)->append(list);
         m_chart.setAxisX(xAxis.axis,&serials);
    }
}

}
