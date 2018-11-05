/*额外说明：
 * 波形图：支持100w数据绘制。但功能未提升至父类
 *
 */
#ifndef SUPERPARENT_H
#define SUPERPARENT_H

#include <QWidget>
#include <QVariant>
#include <QHBoxLayout>
#include <QDateTime>

#include <QtCharts/QChartView>
#include <QtCharts/qchart.h>
#include <QtCharts/qvalueaxis.h>
#include <QtCharts/qdatetimeaxis.h>


using namespace QtCharts;
namespace Diagram/*! 图表 */
{
struct AxisType{
    enum type{
      xAxis,
      yAxis
    };
};

/*!
 * \brief The DataType struct  数据类型
 */
struct DataType
{
  enum type{
      value,                //数值
      DateTime,              //日期时间
  }  ;
};


/*!
 * \brief The DiagramSuper class su所有图表的父类
 */
class DiagramSuper:public QChartView{
    Q_OBJECT
  public:
      struct AxisData{
         QAbstractAxis *axis;
         DataType::type dataType;
         QPair<QVariant,QVariant> valMaxRange;
         QPair<QVariant,QVariant> valCurRange;
         AxisData(){axis = NULL;}
         void setRange(bool isMaxRange,QVariant min,QVariant max,QString format = QString());
      };

      DiagramSuper(QString name);
      void setRange(AxisType::type t_Axis,QVariant min,QVariant max,QString format = QString());
      void setTickCount(AxisType::type t_Axis,int count);
      void addAxis(AxisType::type t_Axis,DataType::type t_Data,Qt::Alignment alignment,QString name = QString());
      void setScale(const QChartView::RubberBands &rubberBand)
      {
          this->setRubberBand(rubberBand);
      }
protected:
      virtual void initDefault()=0;     //初始化默认值
      virtual void atttachToSeries() =0;

public:
    AxisData xAxis;        //横轴
    AxisData yAxis;        //纵轴



    QChart m_chart;
    QString tableName;
};

}

#endif // SUPERPARENT_H
