#include "udpserver.h"

#include "../global.h"
#include "Base/util/rlog.h"
#include "Network/base/socket.h"
#include "Base/util/rsingleton.h"
#include "protocol/protocolmanager.h"

namespace Core{

UdpServer::UdpServer():RTask()
{

}

UdpServer::~UdpServer()
{
    runningFlag = false;
    wait();
}

bool UdpServer::init(Datastruct::NetworkInfo &info)
{
    if(info.baseInfo.port <= 0 || info.baseInfo.port > USHRT_MAX){
        return false;
    }

    this->netinfo = info;
    server = new Network::RSocket;
    if(server->createSocket(Network::RSocket::R_UDP)){
        if(!server->bind(info.baseInfo.ip.toLocal8Bit().data(),info.baseInfo.port)){
            RLOG_ERROR("bind socket error!");
            return false;
        }
        return true;
    }
    return false;
}

void UdpServer::registNetworkObserver(QString mid, QStringList protos)
{
    std::for_each(protos.begin(),protos.end(),[&](QString protocol){
        bool existed = false;
        const Datastruct::BaseProtocol *prot =RSingleton<Core::ProtocolManager>::instance()->getProtocol(protocol.toInt(),&existed);
        if(existed && prot != NULL){
            Datastruct::ModuleProtocol mprotocol;
            mprotocol.startCode = prot->startCode;
            mprotocol.endCode = prot->endCode;
            mprotocol.type = prot->type;
            mprotocol.moduleId = mid;
            observeredProtocol.insert(protocol.toInt(),mprotocol);
        }
    });
}

void UdpServer::startMe()
{
    RTask::startMe();
    runningFlag = true;
    start();
}

void UdpServer::stopMe()
{
    RTask::stopMe();
    runningFlag = false;
}

void UdpServer::run()
{
    char buff[RECV_BUFF] = {0};
    while(runningFlag){
        int recvLen = server->recvFrom(buff,RECV_BUFF);
        //TODO 20181121对接收数据处理
    }
}

} //namespace Core
