/*!
 *  @brief     标题栏中自定义按钮
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.19
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QAbstractButton>

class TitleBarButton  : public QAbstractButton
{
    Q_OBJECT
public:
    explicit TitleBarButton(QWidget *widget);

protected:
    QSize sizeHint() const;
    inline QSize minimumSizeHint() const
    { return sizeHint(); }

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *);
};

#endif // TITLEBARBUTTON_H
