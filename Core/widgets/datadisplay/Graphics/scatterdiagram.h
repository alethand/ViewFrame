#ifndef SCATTERDIAGRAM_H
#define SCATTERDIAGRAM_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/qchart.h>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/qvalueaxis.h>
#include <QtCharts/qdatetimeaxis.h>
#include <QtCharts/qscatterseries.h>
#include <QtCharts/qbarcategoryaxis.h>
#include <QtCharts/QBarSet>
#include <QString>
#include <QList>
#include <QHBoxLayout>

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


class DiagramSuper:public QWidget{
    Q_OBJECT
  public:
       DiagramSuper(QString name);

      void addAxis(AxisType::type t_Axis,DataType::type t_Data,Qt::Alignment alignment,QString name = QString());
protected:
      virtual void initDefault()=0;     //初始化默认值

public:
    QAbstractAxis *xAxis;        //横轴
    QAbstractAxis *yAxis;        //纵轴

    QChart m_chart;
    QString tableName;
private:
    QChartView  view;
};
/*!
 * \brief The Scatter class   散点图
 */
class Scatter:public DiagramSuper
{
    Q_OBJECT
public:
    Scatter(QString name):DiagramSuper(name){}

    void setXAxis(DataType::type t_Data,QString name,QString format = QString());

    void addSeries(int index = 0);
    void addData(double valX,double valY,int index = 0);
    void addData(const QList<QPointF> &values, int index =0);
protected:
    void initDefault();
private :
    QList<QScatterSeries*> serials;


};

/*!
 * \brief The Histogram class  直方图
 */
class Histogram:public DiagramSuper
{
    Q_OBJECT
public:
     explicit Histogram(QString name);

    void setXAxisLabels(QStringList list,QString name);

    void addBarSet(int index = 0,QString name = QString());
    void addData(double valX,double valY,int index = 0);
    void setData(const QList<double> &values, int index =0);

protected:
     void initDefault();

private:
    QList<QBarSet*> sets;
    QBarSeries  serials;

};
}

#endif // SCATTERDIAGRAM_H
