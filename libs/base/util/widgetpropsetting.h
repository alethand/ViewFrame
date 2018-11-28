/*!
 *  @brief     窗口属性设置
 *  @details   用于设置widget的各种属性，提供静态访问方式。
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.19
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef WIDGETPROPSETTING_H
#define WIDGETPROPSETTING_H

class QWidget;

#include "../base_global.h"

namespace Base{

class BASESHARED_EXPORT WidgetPropSetting
{

public:
    static void enableWidgetTransparency(QWidget * widget,bool flag);

};

}

#endif // WIDGETPROPSETTING_H
