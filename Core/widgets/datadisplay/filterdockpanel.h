/*!
 *  @details   过滤器,用于数据筛选显示
 *  @author    hca
 *  @version   1.0
 *  @date      2018.11.2
 *  @copyright NanJing RenGu.
 */
#ifndef FILTERDOCKPANEL_H
#define FILTERDOCKPANEL_H
#include<QList>
#include <QDockWidget>

namespace Ui {
class filterDockPanel;
}

class filterDockPanel : public QDockWidget
{
    Q_OBJECT

public:
    explicit filterDockPanel(QWidget *parent = 0);
    ~filterDockPanel();
signals:
    void sendFilterMessage(QList<double> filterMessage);

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::filterDockPanel *ui;
    QList<double> filterMessages;
};

#endif // FILTERDOCKPANEL_H
