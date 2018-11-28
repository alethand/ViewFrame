#include "mapview.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

#include "others/googlemap.h"

namespace Core{

MapView::MapView(QWidget *parent):QWidget(parent)
{
    mainWidget = new QWidget(this);
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(mainWidget);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    mainWidget->installEventFilter(this);

    googleMap = new GoogleMap(mainWidget);
}

MapView::~MapView()
{

}

bool MapView::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == mainWidget){
        if(event->type() == QEvent::Paint){
            googleMap->Invalidate(true);
        }else if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
            googleMap->SetLButtonPoint(eve->pos());
        }else if(event->type() == QEvent::MouseMove){
            QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
            googleMap->addoffset(eve->pos());
            update();
        }else if(event->type() == QEvent::MouseButtonRelease){
            googleMap->UpdateNewTopPoint();
            update();
        }else if(event->type() == QEvent::Wheel){
            QWheelEvent * eve = dynamic_cast<QWheelEvent *>(event);
            float fModulus;
            if(eve->delta() > 0)
                fModulus = 1.5;
            else
                fModulus = 0.5;
            googleMap->zoompoint(eve->pos(),fModulus);
            update();
        }
    }
    return QWidget::eventFilter(watched,event);
}

} //namespace Core
