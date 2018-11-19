#include "tcpserver.h"

#include "../global.h"
#include "Base/util/rlog.h"
#include "Network/base/socket.h"
#include "Base/util/rsingleton.h"
#include "protocol/protocolmanager.h"

#include <QList>
#include <QDebug>

#include <climits>


#ifdef Q_OS_WIN
#include <winsock2.h>
#include <windows.h>
#pragma  comment(lib,"ws2_32.lib")
#endif

#define RECV_BUFF 1024*64

namespace Core{

TcpServer::TcpServer():RTask()
{

}

TcpServer::~TcpServer()
{
    runningFlag = false;
    wait();
}

bool TcpServer::init(Datastruct::NetworkInfo &info)
{
    if(info.baseInfo.port <= 0 || info.baseInfo.port > USHRT_MAX){
        return false;
    }
    this->netinfo = info;
    server = new Network::RSocket;
    if(server->createSocket(Network::RSocket::R_TCP)){
        if(!server->setBlock(false)){
            RLOG_ERROR("set block model error!");
            return false;
        }

        if(!server->bind(info.baseInfo.ip.toLocal8Bit().data(),info.baseInfo.port)){
            RLOG_ERROR("bind socket error!");
            return false;
        }

        if(!server->listen()){
            RLOG_ERROR("listen socket error!");
            return false;
        }

        return true;
    }
    return false;
}

/*!
 * @brief 组件注册网络感兴趣的协议
 * @param[in] mid 组件id
 * @param[in] protos 需订阅的协议id集合
 */
//TODO 20181113将数据解析从此文件中提取至单独文件
void TcpServer::registNetworkObserver(QString mid, QStringList protos)
{
    std::for_each(protos.begin(),protos.end(),[&](QString protocol){
        bool existed = false;
        const Datastruct::BaseProtocol *prot =RSingleton<Core::ProtocolManager>::instance()->getProtocol(protocol.toInt(),&existed);
        if(existed && prot != NULL){
            ModuleProtocol mprotocol;
            mprotocol.startCode = prot->startCode;
            mprotocol.endCode = prot->endCode;
            mprotocol.type = prot->type;
            mprotocol.moduleId = mid;
            observeredProtocol.insert(protocol.toInt(),mprotocol);
        }
    });
}

void TcpServer::startMe()
{
    RTask::startMe();
    runningFlag = true;
    start();
}

void TcpServer::stopMe()
{

}

void TcpServer::run()
{
    while(runningFlag){

        fd_set f_sock;
        FD_ZERO(&f_sock);

        FD_SET(server->getSocket(),&f_sock);
        int maxSockId = server->getSocket();

        std::for_each(clients.begin(),clients.end(),[&](Network::RSocket * client){
            FD_SET(client->getSocket(),&f_sock);
            if(maxSockId < client->getSocket())
                maxSockId = client->getSocket();
        });
        struct timeval tv;
        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int resultSize = select(maxSockId + 1,&f_sock,NULL,NULL,&tv);
        if(resultSize == 0){
            //超时
        }
        else if(resultSize > 0)
        {
            if(FD_ISSET(server->getSocket(),&f_sock)){
                Network::RSocket cli = server->accept();
                if(!cli.setBlock(false)){
                    RLOG_ERROR("set client block model error!");
                    break;
                }
                Network::RSocket * client = new Network::RSocket(cli);
                clients.append(client);
                resultSize--;
            }

            if(resultSize > 0){
                QList<Network::RSocket *>::iterator iter = clients.begin();
                while(iter != clients.end() && resultSize > 0){
                    Network::RSocket * client = *iter;
                    if(FD_ISSET(client->getSocket(),&f_sock)){
                        char buff[RECV_BUFF]={0};
                        int recvLen = client->recv(buff,RECV_BUFF);

                        bool closed = false;
                        if(recvLen > 0)
                        {
                            RGlobal::G_ProtocolQueueMutex.lock();
                            qDebug()<<buff;
                            ProtocolArray array;
                            array.protocolType = 1;
                            array.data.append(buff);
                            RGlobal::G_ProtoQueue.push_back(array);
                            //TODO 20181113将数据进行处理,创建tcpclient,保存每个单独的连接,具有接收缓存等功能
                            RGlobal::G_ProtocolQueueMutex.unlock();

                            RGlobal::G_ProtocolQueueCondition.notify_one();
                        }
                        else if(recvLen == 0)
                        {
                            RLOG_ERROR("Tcp socket closed! %d",client->getLastError());
                            closed = true;
                        }else{
                            int error = client->getLastError();
                            if(errno == EAGAIN || errno == EWOULDBLOCK || error == (int)WSAETIMEDOUT){
                                continue;
                            }
                            RLOG_ERROR("Tcp socket occour error! %d",client->getLastError());
                        }

                        resultSize--;

                        if(closed){
                            delete client;
                            iter = clients.erase(iter);
                            continue;
                        }
                    }
                    iter++;
                }
            }
        }
    }
}

} //namespace Core
