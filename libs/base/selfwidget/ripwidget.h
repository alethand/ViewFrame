/*!
 *  @brief     IP地址输入框
 *  @details   1.可设置、获取Ip地址信息；
 *             2.在输入时对ip地址验证
 *  @author    SC
 *  @version   1.0
 *  @date      2018.11.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RIPWIDGET_H
#define RIPWIDGET_H

#include <QLineEdit>

struct RIPStruct{
    unsigned char ip1;
    unsigned char ip2;
    unsigned char ip3;
    unsigned char ip4;
};

#include "../base_global.h"

class BASESHARED_EXPORT  RIPWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RIPWidget(QWidget *parent = 0);
    ~RIPWidget();
    bool setIP(QString IP);
    bool setIP(const char* IP,int size=4);
    bool setIP(unsigned long IP);
    bool setIP(RIPStruct IP);

    QString getIPString();
    RIPStruct getIPStruct();
    unsigned long getIPULong();

    void clear();

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    QList<QLineEdit *> m_lineEidt;
    int getIndex(QLineEdit *pEdit);
};

#endif // RIPWIDGET_H
