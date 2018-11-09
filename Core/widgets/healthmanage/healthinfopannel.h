/*!
 *  @brief     健康管理显示面板
 *  @details
 *  @author    yww
 *  @version   1.0
 *  @date      2018.09.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef HEALTHINFOPANNEL_H
#define HEALTHINFOPANNEL_H

#include "pluginmanager/rcomponent.h"
#include "healthstate_display.h"

#include <QTcpSocket>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QStack>

struct NetOrginData{
    int startCode;      /*!< 开始标志码 */
    QByteArray data;    /*!< 接收数据体 */
};

typedef QStack<NetOrginData> OriginDataStack;

extern OriginDataStack G_RecvDataStack;    /*!< 接收未解析的数据信息 */

extern QWaitCondition G_NetCondtion;
extern QMutex G_NetMutex;

class TcpConnection : public QThread
{
    Q_OBJECT
public:
    explicit TcpConnection(QObject * parent = 0);

    bool init();
    bool connectToServer();

protected:
    void run();

private slots:
    void processData();

private:
    QTcpSocket * tcpSocket;

    QByteArray lastBuff;
};

class ProtocolParseThread : public QThread
{
    Q_OBJECT
public:
    explicit ProtocolParseThread(QObject * parent = 0);

protected:
    void run();

private:
    void parseNetData(NetOrginData & data);
};



class  HealthInfoDockPanel : public Core::RComponent
{
  Q_OBJECT
public:
     HealthInfoDockPanel(QWidget *parent = 0);
     virtual ~HealthInfoDockPanel(){}

     bool initialize();
     void release(){}
     QString pluginName();
     QString MachineName();
     Core::RComponent * clone();
     QSize sizeHint()const;
     void onMessage(MessageType::MessType type);
     void onNetwork(int protocolType,Datastruct::FieldValues & data);

private slots:
     void retranslateUi();
signals:
     void sendForHealthPanelResize();
public slots:
     void recForHealthPanelResize();

private:
     HealthState_Display *infoWidget;
};

#endif // HEALTHINFOPANNEL_H
