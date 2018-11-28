#include "udptransmit.h"

#include "../base/socket.h"
#include "../wraprule/udp_bytedatapacketrule.h"
#include "Base/util/rlog.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include <WinSock2.h>
#endif

namespace Network{

UdpTransmit::UdpTransmit():dataPacketRule(NULL)
{
    udpSocket = new RSocket();
}

UdpTransmit::~UdpTransmit()
{

}

CommMethod UdpTransmit::type()
{
    return C_UDP;
}

QString UdpTransmit::name()
{
    return "UDP";
}

void UdpTransmit::setUdpPacketRule(UDP_ByteDataPacketRule *dataPacketRule)
{
    this->dataPacketRule = dataPacketRule;
}

bool UdpTransmit::initialize()
{
    if(udpSocket->createSocket(RSocket::R_UDP)){
        byteSendFunc = std::bind(&UdpTransmit::sendByteData,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4);
        return true;
    }
    return false;
}

bool UdpTransmit::startTransmit(SendUnit &unit)
{
    if(!netConnected || unit.method != type() || dataPacketRule == NULL)
        return false;

    if(dataPacketRule->wrap(unit,byteSendFunc))
        return true;

    RLOG_ERROR("Udp send a data error!");

    return true;
}

int UdpTransmit::sendByteData(const char *buff, const int length,const char * ip,const int port)
{
    if(buff == NULL)
        return -1;

    return udpSocket->sendTo(buff,length,ip,port);
}

bool UdpTransmit::startRecv(char *recvBuff, int recvBuffLen, DataHandler recvDataFunc)
{
    if(!udpSocket || this->dataPacketRule)
        return false;

    int recvLen = udpSocket->recvFrom(recvBuff,recvBuffLen);
    if(recvLen > 0)
    {
        if(!dataPacketRule->unwrap(recvBuff,recvLen,recvDataFunc)){
            RLOG_ERROR("Udp socket parse error! %d",udpSocket->getLastError());
        }else{
            return true;
        }
    }
    else
    {
        RLOG_ERROR("Udp socket occour error! %d",udpSocket->getLastError());
    }

    close();

    return false;
}

bool UdpTransmit::close()
{
    if(!udpSocket)
        return false;

    if(netConnected && udpSocket->isValid()){
        if(udpSocket->closeSocket()){
            netConnected = false;
            return true;
        }
    }
    return false;
}

bool UdpTransmit::bind(const char *ip, ushort port)
{
    if(!udpSocket)
        return false;

    bool flag = true;
    udpSocket->setSockopt(SO_REUSEADDR,(char *)&flag,sizeof(flag));

    return udpSocket->bind(ip,port);
}

} //namespace Network
