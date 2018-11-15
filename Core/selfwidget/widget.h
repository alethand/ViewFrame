/*!
 *  @brief     无边框窗口基类
 *  @details   用于实现通用功能，如边框阴影
 *  @file      widget.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note      此窗口设置了一定的边距，用于绘制渐变的背景。
 *      20180222:LYS:添加获取窗口中阴影边框宽度方法shadowWidth
 *      20180425:wey:添加设置工具栏移动，避免全面板可移动
 *      20180720:LYS:修改enterEvent方法，将阴影边框纳入考虑范围
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Core{

class WidgetPrivate;

class Widget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Widget)
public:
    explicit Widget(QWidget * parent);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    int countMouseArea(QPoint p, int row);
    int countHorizonalArea(QPoint p);
    void setCursorType(int areaCode);

private:
    bool leftButtonPressed;
    int mousePressArea;
    QPoint globalMouseStartPoint;

};

}

#endif // ABSTRACTWIDGET_H
