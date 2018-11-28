#include "superparent.h"

namespace Diagram/*! 图表 */
{


/*!
 * \brief DiagramSuper::addAxis   加入坐标轴
 * \param t_Axis            坐标类型
 * \param t_Data            坐标数据
 * \param alignment         对齐方式
 */
DiagramSuper::DiagramSuper(QString name)
    :QChartView()
{
    m_chart.setTitle(name);
    this->setChart(&m_chart);


    //风格设置
    m_chart.setTheme(QChart::ChartThemeBlueCerulean);
}

void DiagramSuper::setRange(AxisType::type t_Axis, QVariant min, QVariant max,QString format )
{
    AxisData *pAxisData = NULL;
    switch(t_Axis)
    {
    case AxisType::xAxis:
                     pAxisData = &xAxis;
                    break;
    case AxisType::yAxis:
                     pAxisData = &yAxis;
                    break;
     }
    pAxisData->setRange(true,min,max,format);

}

void DiagramSuper::setTickCount(AxisType::type t_Axis, int count)
{
    AxisData *pAxisData = NULL;
    switch(t_Axis)
    {
    case AxisType::xAxis:
                     pAxisData = &xAxis;
                    break;
    case AxisType::yAxis:
                     pAxisData = &yAxis;
                    break;
     }

    switch(pAxisData->dataType)
    {
    case DataType::value:
                     dynamic_cast<QValueAxis*>(pAxisData->axis)->setTickCount(count);
        break;
    case DataType::DateTime:
                    dynamic_cast< QDateTimeAxis*>(pAxisData->axis)->setTickCount(count);
        break;
    }
}

void DiagramSuper::addAxis(AxisType::type t_Axis, DataType::type t_Data,Qt::Alignment alignment,QString name)
{
    AxisData *pAxisData = NULL;
    switch(t_Axis)
    {
    case AxisType::xAxis:
                     pAxisData = &xAxis;
                    break;
    case AxisType::yAxis:
                     pAxisData = &yAxis;
                    break;
     }

    switch(t_Data)
    {
    case DataType::value: (*pAxisData).axis = new QValueAxis();
                         (*pAxisData).dataType = DataType::value;
                          pAxisData->setRange(true,0,50);
        break;
    case DataType::DateTime:(*pAxisData).axis =new QDateTimeAxis();
                          (*pAxisData).dataType = DataType::DateTime;
        break;
    }

    (*pAxisData).axis->setTitleText(name);
    m_chart.addAxis((*pAxisData).axis,alignment);

    if(xAxis.axis != NULL && yAxis.axis != NULL)
        atttachToSeries();
}

void DiagramSuper::AxisData::setRange(bool isMaxRange,QVariant min,QVariant max,QString format)
{
    switch(dataType) {
    case DataType::value:
             dynamic_cast<QValueAxis*>(axis)->setRange(min.toDouble(),max.toDouble());
             break;
    case DataType::DateTime:
         if(QDateTimeAxis *pAxis =  dynamic_cast< QDateTimeAxis*>(axis)) {
               pAxis->setRange(min.toDateTime(),max.toDateTime());
               if(format.isEmpty())
                 pAxis->setFormat("h/mm/ss");
               else
                 pAxis->setFormat(format);
           }
            break;
    }
    valCurRange.first = min;
    valCurRange.second = max;

    if(isMaxRange) {
        valMaxRange.first = min;
        valMaxRange.second = max;
    }

}





}
