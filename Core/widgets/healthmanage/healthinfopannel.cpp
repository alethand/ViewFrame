#include "healthinfopannel.h"

#include "base/constants.h"
#include "Base/actionmanager/action.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/util/rsingleton.h"
//
#include <iostream>
using namespace std;

#include "file/programfilepath.h"
#include "protocol/protocolmanager.h"

#include <QHostAddress>
#include <QTimer>


OriginDataStack G_RecvDataStack;    /*!< 接收未解析的数据信息 */

QWaitCondition G_NetCondtion;
QMutex G_NetMutex;

TcpConnection::TcpConnection(QObject *parent):QThread(parent),tcpSocket(NULL){

}

bool TcpConnection::init()
{

    return true;
}

bool TcpConnection::connectToServer()
{
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"),8023);
    if(tcpSocket->waitForConnected(3000))
        return true;

    return false;
}

void TcpConnection::run()
{
    tcpSocket = new QTcpSocket();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(processData()),Qt::DirectConnection);
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"),8023);

    exec();
}

void TcpConnection::processData()
{
    QByteArray buff /*= tcpSocket->readAll()*/;
    //将ProtocolManager中的已存在的协议注册到网络接收中，通过帧有效标记、数据类型以及包尾来判断截取数据
    //TODO 【20181108将数据按照一定长度压入待解析栈内】
    qDebug()<<buff;

    NetOrginData tmpData;
    tmpData.startCode = 1;

    tmpData.data = buff;

    G_NetMutex.lock();
    G_RecvDataStack.push(tmpData);
    G_NetMutex.unlock();

    qDebug()<<"NetWork:::"<<G_RecvDataStack.size();

    G_NetCondtion.wakeOne();
}

ProtocolParseThread::ProtocolParseThread(QObject *parent):QThread(parent)
{

}

void ProtocolParseThread::run()
{
    while(true){
        qDebug()<<G_RecvDataStack.size()<<"++++++";
        while(G_RecvDataStack.size() == 0){
            G_NetMutex.lock();
            G_NetCondtion.wait(&G_NetMutex);
        }

        G_NetMutex.lock();
        qDebug()<<"+====222==";
        NetOrginData data = G_RecvDataStack.pop();
        G_NetMutex.unlock();
        parseNetData(data);
    }
}

void ProtocolParseThread::parseNetData(NetOrginData &data)
{
    qDebug()<<data.startCode<<"__"<<data.data;
}

HealthInfoDockPanel::HealthInfoDockPanel(QWidget *parent)
    :Core::RComponent(Constant::PLUGIN_HEALTH_MANAGER,parent)
{
    retranslateUi();
    RSingleton<Core::Subject>::instance()->attach(this);
}

bool HealthInfoDockPanel::initialize()
{
    //TODO 【20181108将字段设置至表格上】
    ProgramFilePath filePath;
    RSingleton<Core::ProtocolManager>::instance()->parseLocalDir(filePath.healthManagePath);

    bool existed = false;
    Datastruct::BaseProtocol bprotocol = RSingleton<Core::ProtocolManager>::instance()->getProtocol(QStringLiteral("健康管理"),existed);

//    if(existed){
//        Datastruct::SignalProtocol sprotocol = bprotocol.protocols.at(0);
//        std::for_each(sprotocol.fields.begin(),sprotocol.fields.end(),[&](Datastruct::FieldData fdata){
//        });
//    }

//    //TODO 【20181108将网络数据解析】
//    TcpConnection * connection = new TcpConnection;
//    connection->init();
//    connection->start();

//    ProtocolParseThread * thread = new ProtocolParseThread;
//    thread->start();


    infoWidget = new HealthState_Display();
    setWidget(infoWidget);

    //todo 以下需要删除
    infoWidget->openDynamicLayout(true);
    HealthData  netdata;
    netdata.workStateNums = 100;
    WorkStruct temp;
    for(int i=0;i< netdata.workStateNums;i++)
    {   temp.name = QString::number(i);
        netdata.wsInfo.append(temp);
    }
    netdata.subMachineNums = 10;

    for(int i=0;i<netdata.subMachineNums;i++)
    {
        SubMachine sm;
        Componet cp;
        sm.name = QStringLiteral("分机")+QString::number(i);
//        sm.name = MachineName()+QString::number(i).toStdString().data();
        sm.componetNum = 10;
        for(int j = 0;j<sm.componetNum;j++)
        {
            cp.name = "CP"+QString::number(i)+"--"+QString::number(j);
            sm.compInfo.append(cp);
        }
        netdata.smInfo.append(sm);
    }

    infoWidget->handleNetData(netdata);//注意这里调用的是测试接口

    connect(this,SIGNAL(sendForHealthPanelResize()),infoWidget,SLOT(recForHealthPanelResize()));

    return true;
}

QString HealthInfoDockPanel::pluginName()
{
    return tr("HealthInfo panel");
}

QString HealthInfoDockPanel::MachineName()
{
    return tr("Machine");
}

Core::RComponent *HealthInfoDockPanel::clone()
{
    return new HealthInfoDockPanel;
}

void HealthInfoDockPanel::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void HealthInfoDockPanel::onNetwork(int protocolType, Datastruct::FieldValues &data)
{

}

QSize HealthInfoDockPanel::sizeHint() const
{
    return QSize(300,200);
}

void HealthInfoDockPanel::retranslateUi()
{
    setObjectName("healthControl");
    m_name = tr("HealthInfo panel");
    pluginId = "0x0001";
    setWindowTitle(m_name);
}

/**
 * @brief 健康管理界面布局重置
 */
void HealthInfoDockPanel::recForHealthPanelResize(){
    emit sendForHealthPanelResize();
}
