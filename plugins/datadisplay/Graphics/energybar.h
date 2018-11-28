#ifndef ENERGYBAR_H
#define ENERGYBAR_H

#include <QtWidgets/qprogressbar.h>
#include <QWidget>
#include <QStringList>
#include <QString>
#include <QPair>
#include <QtCharts>

namespace  CustomWidget{

class EnergyBar:public QWidget
{
    Q_OBJECT
public:
    enum Direction{
        Left_toRigth,
        Right_ToLeft,
        Up_ToDown,
        Down_ToUp
    };

    explicit EnergyBar(Direction dir,QWidget *parent = NULL);

    void setProperties(const QStringList list);
    void setVlaueRange(double min,double max);

    void setValueList(const QStringList list);
    void setValue(double val,int pos = -1);




private:
    Direction    direction;             //朝向
    QPair<double,double>   range;       //方向

    QStringList  properties;            //属性名
    QList<QProgressBar*> bars;          //能量条
    QList<QLabel*>       barsMark;      //能量条所属标记


private:
    void initVew();             //初始化视图







};






}


#endif // ENERGYBAR_H
