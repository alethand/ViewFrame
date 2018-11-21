/*!
 *  @brief     自定义dock容器
 *  @details   此dock与QDockWidget的区别;
 *             1.不需于QMainWindow结合使用,可与widget结合使用; @n
 *             2.只用于悬浮显示,不与mainWidget做layout结合; @n
 *
 *             具有的功能:
 *             1.标题栏具有拖动,关闭,最小化,最大化按钮,可配; @n
 *             2.双击标题栏可折叠显示区; @n
 *             3.标题栏可设置图标显示; @n
 *             4.标题栏可显示缩略信息,即只显示图标,鼠标点击后可显示全部内容区 @n
 *             5.之后考虑实现相互叠加 @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.13
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MYDOCKWIDGET_H
#define MYDOCKWIDGET_H

#include <QWidget>
#include <QWidgetItem>
#include <QVector>
#include <QLayout>

#include "widget.h"

class QStyleOptionDockWidget;
class QAction;

namespace Core{

class MyDockWidgetPrivate;
class DockLayout;

class MyDockWidget : public Widget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MyDockWidget)
public:
    explicit MyDockWidget(QWidget * parent = 0);
    ~MyDockWidget();

    enum ButtonRole{
        Content, CloseButton, MoveButton, TitleBar, TitleLabel, RoleCount
    };
    void setTitle(const QString title);

    void setWidget(QWidget * widget);
    QWidget * widget();

    void setFloating(bool floating);
    inline bool isFloating() const { return isWindow(); }

    QAction * toggleViewAction() const;

    void setGeometry(const QRect &rect);
    QRect getGeometry() const;

protected:
    bool event(QEvent *event);
    void updateFeatures();

private:
    void initStyleOption(QStyleOptionDockWidget *option);

private slots:
    void toggleTopLevel();
    void toggleView(bool visible);
    void triggerMoveable(bool moveable);
    void triggerResizeable(bool resizeable);
    void triggerExpanable(bool expanded);
    void hideWidget();

private:
    MyDockWidgetPrivate * d_ptr;

    friend class DockLayout;
};

/*!
 *  @brief 控件管理器
 *  @details 暂时用于管理容器,后期将其实例化位QLayout的子类
 */
class DockLayout : public QLayout
{
    Q_OBJECT
public:
    explicit DockLayout(MyDockWidget * parent);

    void addWidget(MyDockWidget::ButtonRole role,QWidget * w);
    QWidget * getWidget(MyDockWidget::ButtonRole role) const;

    void addItem(QLayoutItem * item);
    void setGeometry(const QRect &geometry);
    QLayoutItem *itemAt(int index)const;
    QLayoutItem *takeAt(int index);
    int count() const;
    QSize minimumSize();
    QSize maximumSize();
    QSize sizeHint() const;

    QRect titleArea() const { return _titleArea; }

private:
    void removeWidget(QWidget *widget);
    int titleHeight() const;
    int minimumTitleWidth() const;
    bool nativeWindowDeco() const;
    bool nativeWindowDeco(bool floating) const;

    QSize sizeFromContent(const QSize &content, bool floating) const;

private:
    QVector<QWidgetItem *> items;
    bool verticalTitleBar;
    QRect _titleArea;

    friend class MyDockWidgetPrivate;
};

}

#endif // MYDOCKWIDGET_H
