/*!
 *  @brief     标题栏显示区
 *  @details   1.可
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

namespace Core{

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget * parent = 0);
};

}

#endif // TITLEBAR_H
