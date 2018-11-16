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
#include <QMap>

namespace Core{

class WidgetPrivate;

class Widget : public QWidget
{
    Q_OBJECT
    Q_FLAGS(WidgetFeatures)
    Q_DECLARE_PRIVATE(Widget)
public:
    explicit Widget(QWidget * parent);
    ~Widget();

    typedef QMap<QString , Widget * > WidgetMap;

    enum WidgetFeature{
        WidgetClosable    = 0x0001,
        WidgetMovable     = 0x0002,
        WidgetFloatable   = 0x0004,
        WidgetVerticalTitleBar = 0x0008,
        WidgetExpanable = 0x000A,
        WidgetResizeable = 0x000C,
        WidgetVisible = 0x0010,

        WidgetFeatureMask = 0x000f,
        AllWidgetFeatures = WidgetClosable|WidgetMovable|WidgetFloatable|WidgetExpanable|WidgetResizeable|WidgetVisible,
        NoWidgetFeatures  = 0x0000,

        Reserved              = 0x00ff
    };
    Q_DECLARE_FLAGS(WidgetFeatures,WidgetFeature)

    void setWidgetFeatures(WidgetFeatures feature);
    Widget::WidgetFeatures getWidgetFeatures();
    bool testFeatures(WidgetFeatures feature);

    void setObjectName(const QString & name);

    static WidgetMap getAllWidgets(){return widgets;}

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

protected:
    virtual void updateFeatures(){}

    WidgetFeatures currentFeatures;

private:
    int countMouseArea(QPoint p, int row);
    int countHorizonalArea(QPoint p);
    void setCursorType(int areaCode);

private:
    bool leftButtonPressed;
    int mousePressArea;
    QPoint globalMouseStartPoint;

    static WidgetMap widgets;
};

}

#endif // ABSTRACTWIDGET_H
