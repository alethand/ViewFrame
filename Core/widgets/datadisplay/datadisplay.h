/*!
 *  @details   数据显示模块接口
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.10
 *  @copyright NanJing RenGu.
 */
#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <QWidget>
#include <QGridLayout>
#include "protocol/datastruct.h"
#include "Base/pluginmanager/observer.h"

class DataDisplayPrivate;

class DataDisplay : public QWidget,public Base::Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DataDisplay)
public:
    explicit DataDisplay(QWidget *parent = 0);
    ~DataDisplay();

    void onMessage(MessageType::MessType type);
    void retranslateUi();

private:
    void hideAllTable();

signals:
    void radiaSourceSignal(char *buff, int len);

private slots:
    void switchDockVisible(bool flag);

private:
    DataDisplayPrivate * d_ptr;
};

#endif // DATADISPLAY_H
