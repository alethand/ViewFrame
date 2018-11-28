#include "mydockwidgettitlebutton.h"

#include <QStyle>
#include <QPainter>
#include <QDebug>
#include <QStyleOptionToolButton>

namespace Core{

MyDockWidgetTitleButton::MyDockWidgetTitleButton(QWidget *dockWidget)
    : QAbstractButton(dockWidget)
{
    setFocusPolicy(Qt::NoFocus);
}

QSize MyDockWidgetTitleButton::sizeHint() const
{
    ensurePolished();

    return QSize(22,22);
}

void MyDockWidgetTitleButton::enterEvent(QEvent *event)
{
    if (isEnabled()) update();
    QAbstractButton::enterEvent(event);
}

void MyDockWidgetTitleButton::leaveEvent(QEvent *event)
{
    if (isEnabled()) update();
    QAbstractButton::leaveEvent(event);
}

void MyDockWidgetTitleButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QStyleOptionToolButton opt;
    opt.init(this);
    opt.state |= QStyle::State_AutoRaise;

    if (style()->styleHint(QStyle::SH_DockWidget_ButtonsHaveFrame, 0, this))
    {
        if (isEnabled() && underMouse() && !isChecked() && !isDown())
            opt.state |= QStyle::State_Raised;
        if (isChecked())
            opt.state |= QStyle::State_On;
        if (isDown())
            opt.state |= QStyle::State_Sunken;
        style()->drawPrimitive(QStyle::PE_PanelButtonTool, &opt, &p, this);
    }

    opt.icon = icon();
    opt.subControls = 0;
    opt.activeSubControls = 0;
    opt.features = QStyleOptionToolButton::None;
    opt.arrowType = Qt::NoArrow;
    int size = style()->pixelMetric(QStyle::PM_IconViewIconSize, 0, this);
    opt.iconSize = QSize(size, size);
    style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &p, this);
}

}
