#include "energybar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>


namespace  CustomWidget {

EnergyBar::EnergyBar(Direction dir,QWidget *parent)
    :QWidget(parent)
{
    direction = dir;
}

void EnergyBar::setProperties(const QStringList list)
{
    properties = list;
    QProgressBar *bar = NULL;
    QLabel *barMark = NULL;

    bars.clear();
    barsMark.clear();
    for(int i = 0;i < list.count();i++)
    {
        bar = new QProgressBar();
        bars.append(bar);
        barMark = new QLabel();
        barsMark.append(barMark);
    }

    initVew();
}

void EnergyBar::setVlaueRange(double min, double max)
{
    range.first = min;
    range.second = max;
    for(int i = 0;i < bars.count();i++)
    {
        bars.at(i)->setRange(min,max);
    }
}

void EnergyBar::setValueList(const QStringList list)
{
    for(int i = 0;i < bars.count();i++)
    {
        bars.at(i)->setValue(list.at(i).toDouble());
    }
}

void EnergyBar::setValue(double val, int pos)
{
    if(-1 == pos)
    {
        for(int i = 0;i < bars.count();i++)
        {
            bars.at(i)->setValue(val);
        }
    }
    else
        if(pos < bars.count())
        {
            bars.at(pos)->setValue(val);
        }
}

void EnergyBar::initVew()
{
    QLayout *barLlayout = NULL,*layout = NULL;
    QWidget *widget = NULL;
    QLabel *name = NULL;

    /*       设置总体布局           */
    switch(direction)
    {
     case Left_toRigth:
          layout = new QVBoxLayout();
          break;
     case Right_ToLeft:
          break;
     case Up_ToDown:
          break;
     case Down_ToUp:
          break;
    }


    /*      设置每一个能量条的布局         */
    for(int i=0;i < properties.count();i++)
    {
        name = new QLabel();
        name->setFixedWidth(50);
        name->setText(properties.at(i));

        widget = new QWidget();
        switch(direction)
        {
         case Left_toRigth:
              barLlayout = new QHBoxLayout();
              barLlayout->addWidget(name);
              barLlayout->addWidget(bars.at(i));
              barLlayout->addWidget(barsMark.at(i));

              widget->setLayout(barLlayout);
              layout->addWidget(widget);
              break;
         case Right_ToLeft:
              break;
         case Up_ToDown:
              break;
         case Down_ToUp:
              break;
        }
    }

    this->setLayout(layout);

}






}
