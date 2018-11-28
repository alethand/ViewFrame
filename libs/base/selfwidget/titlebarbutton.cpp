#include "titlebarbutton.h"

#include <QStyle>
#include <QPainter>
#include <QStyleOptionToolButton>

TitleBarButton::TitleBarButton(QWidget *widget):QAbstractButton(widget)
{
    setFocusPolicy(Qt::NoFocus);
}

QSize TitleBarButton::sizeHint() const
{
    ensurePolished();

    int size = 2*style()->pixelMetric(QStyle::PM_TitleBarHeight, 0, this);
    if (!icon().isNull()) {
        int iconSize = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this);
        QSize sz = icon().actualSize(QSize(iconSize, iconSize));
        size += qMax(sz.width(), sz.height());
    }

    return QSize(size, size);
}

void TitleBarButton::enterEvent(QEvent *event)
{
    if (isEnabled()) update();
    QAbstractButton::enterEvent(event);
}

void TitleBarButton::leaveEvent(QEvent *event)
{
    if (isEnabled()) update();
    QAbstractButton::leaveEvent(event);
}

void TitleBarButton::paintEvent(QPaintEvent *)
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
    int size = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this);
    opt.iconSize = QSize(size, size);
    style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &p, this);
}
