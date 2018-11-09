/*!
 *  @brief     海图绘制面板
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>

class GoogleMap;

class MapView : public QWidget
{
    Q_OBJECT
public:
    explicit MapView(QWidget * parent = 0);
    ~MapView();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QWidget * mainWidget;
    GoogleMap * googleMap;
};

#endif // MAPVIEW_H
