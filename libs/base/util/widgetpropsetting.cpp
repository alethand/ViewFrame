#include "widgetpropsetting.h"

#include <QWidget>

namespace Base{

/*!
 * @brief 激活窗口透明显示
 * @param[in]  widget 待设置的窗口
 * @param[in]  flag 设置结果
 */
void WidgetPropSetting::enableWidgetTransparency(QWidget *widget, bool flag)
{
    if(flag)
        widget->setWindowFlags(widget->windowFlags() | Qt::FramelessWindowHint);
    else
        widget->setWindowFlags(widget->windowFlags() & ~Qt::FramelessWindowHint);

    widget->setAttribute(Qt::WA_TranslucentBackground,flag);
}

}
