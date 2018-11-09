#include "tcpserver.h"

#include "Network/base/socket.h"

namespace Core{

TcpServer::TcpServer():RTask()
{

}

TcpServer::~TcpServer()
{

}

void TcpServer::setNetworkInfo()
{

}

void TcpServer::startMe()
{

}

void TcpServer::stopMe()
{

}

//bool TcpServer::startMe(const char *ip, unsigned short port)
//{
//    if(rsocket.createSocket(Network::RSocket::R_TCP))
//        if(rsocket.bind(ip,port))
//            if(rsocket.listen())
//                return true;

//    return false;
//}

void TcpServer::run()
{

}

} //namespace Core
