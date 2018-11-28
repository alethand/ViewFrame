/*!
 *  @brief     自定义dock中标题按钮
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.13
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MYDOCKWIDGETTITLEBUTTON_H
#define MYDOCKWIDGETTITLEBUTTON_H

#include <QAbstractButton>

namespace Core{

class MyDockWidget;

class MyDockWidgetTitleButton : public QAbstractButton
{
    Q_OBJECT
public:
    MyDockWidgetTitleButton(QWidget *dockWidget);

    QSize sizeHint() const;
    inline QSize minimumSizeHint() const
    { return sizeHint(); }

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

};

}

#endif // MYDOCKWIDGETTITLEBUTTON_H
