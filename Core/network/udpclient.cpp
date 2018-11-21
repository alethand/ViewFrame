#include "udpclient.h"

#include "Network/base/socket.h"
#include "global.h"

#include <QDebug>

namespace Core{

UdpClient::UdpClient()
{

}

UdpClient::~UdpClient()
{
    runningFlag = false;
    wait();
}

/*!
 * @brief 初始化udp网络
 * @return
 */
bool UdpClient::init()
{
    client = new Network::RSocket;
    if(client->createSocket(Network::RSocket::R_UDP)){

        return true;
    }
    return false;
}

void UdpClient::registNetworkDestination(Datastruct::NetworkInfo &info)
{
    if(netmap.contains(info.id))
        return ;

    netmap.insert(info.id,info);
}

void UdpClient::addPreSendData(QString networdId, QByteArray &array)
{

}

void UdpClient::startMe()
{
    RTask::startMe();
    runningFlag = true;
    start();
}

void UdpClient::stopMe()
{
    RTask::stopMe();
    runningFlag = false;
}

void UdpClient::run()
{
    while(runningFlag){

    }
}

} //namespace Core
