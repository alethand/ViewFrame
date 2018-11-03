#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "superparent.h"
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

namespace Diagram {

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
     void atttachToSeries(){}

private:
    QList<QBarSet*> sets;
    QBarSeries  serials;

};

}
#endif // HISTOGRAM_H
