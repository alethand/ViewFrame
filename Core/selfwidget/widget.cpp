#include "widget.h"

#include <QMouseEvent>
#include <QDebug>

namespace Core{

#define MOUSE_TRIGGER_SPACE 5        //窗口各边对鼠标事件触发间距

Widget::WidgetMap Widget::widgets;

Widget::Widget(QWidget *parent):
    QWidget(parent),currentFeatures(AllWidgetFeatures),widgetExpanded(true)
{
    setMouseTracking(true);
}

Widget::~Widget()
{

}

void Widget::setWidgetFeatures(WidgetFeatures feature)
{
    currentFeatures = feature;
    updateFeatures();
}

Widget::WidgetFeatures Widget::getWidgetFeatures()
{
    return currentFeatures;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && testFeatures(WidgetResizeable)){
        globalMouseStartPoint = event->globalPos();
        leftButtonPressed = true;
        mousePressArea = countMouseArea(event->pos(), countHorizonalArea(event->pos()));
    }
}

/*!
 * @details 鼠标移动需要对窗口的属性进行检测,需检测的包括: @a
 *          1.WidgetResizeable
 *          2.WidgetRangeLimit
 */
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(leftButtonPressed && testFeatures(WidgetResizeable))
    {
        int poss = countMouseArea(event->pos(), countHorizonalArea(event->pos()));
        QPoint ptemp = event->globalPos() - globalMouseStartPoint;
        setCursorType(poss);
        if (mousePressArea != 22)
        {
            QRect newGem = geometry();
            switch (mousePressArea)
            {
                case 11:newGem.setTopLeft(newGem.topLeft() + ptemp); break;
                case 12:newGem.setTop(newGem.top() + ptemp.y()); break;
                case 13:newGem.setTopRight(newGem.topRight() + ptemp); break;

                case 21:newGem.setLeft(newGem.left() + ptemp.x()); break;
                case 23:newGem.setRight(newGem.right() + ptemp.x()); break;

                case 32:newGem.setBottom(newGem.bottom() + ptemp.y()); break;
                case 33:newGem.setBottomRight(newGem.bottomRight() + ptemp); break;
                case 31:newGem.setBottomLeft(newGem.bottomLeft() + ptemp); break;
            }

            if(newGem.width()<= minimumSize().width() || newGem.height() <=minimumSize().height())
                return;
            setGeometry(newGem);
        }
         globalMouseStartPoint = event->globalPos();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent */*event*/)
{
    leftButtonPressed = false;
    setCursor(Qt::ArrowCursor);
}

int Widget::countMouseArea(QPoint p, int row)
{
  if (p.y() < MOUSE_TRIGGER_SPACE)
      return 10 + row;
  else if (p.y()>height() - MOUSE_TRIGGER_SPACE)
      return 30 + row;
  else
      return 20 + row;
}

/*!
 * @brief 根据鼠标所属的区域设置鼠标样式
 * @param[in] areaCode 区域代码
 */
void Widget::setCursorType(int areaCode)
{
    Qt::CursorShape cursor;
    switch (areaCode)
    {
        case 11:
        case 33:
            cursor = Qt::SizeFDiagCursor; break;
        case 13:
        case 31:
            cursor = Qt::SizeBDiagCursor; break;
        case 21:
        case 23:
            cursor = Qt::SizeHorCursor; break;
        case 12:
        case 32:
            cursor = Qt::SizeVerCursor; break;
        case 22:
            cursor = Qt::ArrowCursor; break;
        default:
            cursor = Qt::ArrowCursor; break;
            break;
    }
    setCursor(cursor);
}

bool Widget::testFeatures(WidgetFeatures feature)
{
    return this->currentFeatures & feature;
}

void Widget::setObjectName(const QString &name)
{
    QWidget::setObjectName(name);
    if(!widgets.contains(name)){
        widgets.insert(name,this);
    }
}

void Widget::setGeometry(int ax, int ay, int aw, int ah)
{
    QWidget::setGeometry(ax,ay,aw,ah);
}

void Widget::setGeometry(const QRect &rect)
{
    QWidget::setGeometry(rect);
}

QRect Widget::getGeometry()const
{
    return geometry();
}

void Widget::setExpanded(bool expandable)
{
    widgetExpanded = expandable;
}

bool Widget::getExpanded()
{
    return widgetExpanded;
}

/*!
 * @brief 计算鼠标水平方向所处位置
 * @param[in]  p 鼠标当前的位置
 * @return 区域代码
 */
int Widget::countHorizonalArea(QPoint p)
{
    return (p.x() < MOUSE_TRIGGER_SPACE) ? 1 : ((width() - p.x() < MOUSE_TRIGGER_SPACE) ? 3 : 2);
}

}
