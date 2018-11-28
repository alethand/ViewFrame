#ifndef SCATTERDIAGRAM_H
#define SCATTERDIAGRAM_H


#include <QtCharts/QLineSeries>


#include <QtCharts/qscatterseries.h>
#include <QtCharts/qbarcategoryaxis.h>
#include <QString>
#include <QList>
#include <QPair>

#include "superparent.h"

using namespace QtCharts;
namespace Diagram/*! 图表 */
{

/*!
 * \brief The Scatter class   散点图
 */
class Scatter:public DiagramSuper
{
    Q_OBJECT
public:
    Scatter(QString name):DiagramSuper(name){}

    void addSeries(int index = 0);
    void addData(double valX,double valY,int index = 0);
    void addData(const QList<QPointF> &values, int index =0);
    void addData(double minX,double maxX,const QList<double> &values);
protected:
    void initDefault();
    void atttachToSeries(){}
private :
    QList<QScatterSeries*> serials;


};

}

#endif // SCATTERDIAGRAM_H
