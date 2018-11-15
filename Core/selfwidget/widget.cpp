#include "widget.h"

#include <QMouseEvent>
#include <QDebug>

namespace Core{

#define MOUSE_TRIGGER_SPACE 5        //窗口各边对鼠标事件触发间距

Widget::Widget(QWidget *parent):
    QWidget(parent)
{
    setMouseTracking(true);
}

Widget::~Widget()
{

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        globalMouseStartPoint = event->globalPos();
        leftButtonPressed = true;
        mousePressArea = countMouseArea(event->pos(), countHorizonalArea(event->pos()));
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int poss = countMouseArea(event->pos(), countHorizonalArea(event->pos()));

    if(leftButtonPressed)
    {
        QPoint ptemp = event->globalPos() - globalMouseStartPoint;
        setCursorType(poss);
        if (mousePressArea != 22)
        {
            QRect wid = geometry();
            switch (mousePressArea)
            {
                case 11:wid.setTopLeft(wid.topLeft() + ptemp); break;
                case 12:wid.setTop(wid.top() + ptemp.y()); break;
                case 13:wid.setTopRight(wid.topRight() + ptemp); break;

                case 21:wid.setLeft(wid.left() + ptemp.x()); break;
                case 23:wid.setRight(wid.right() + ptemp.x()); break;

                case 32:wid.setBottom(wid.bottom() + ptemp.y()); break;
                case 33:wid.setBottomRight(wid.bottomRight() + ptemp); break;
                case 31:wid.setBottomLeft(wid.bottomLeft() + ptemp); break;
            }
            setGeometry(wid);
        }
         globalMouseStartPoint = event->globalPos();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
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
