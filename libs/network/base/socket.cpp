#include "socket.h"

#include <QtGlobal>

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <windows.h>
typedef  int socklen_t;
#pragma  comment(lib,"ws2_32.lib")
#elif defined(Q_OS_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#define closesocket close
#endif

#include <Base/util/rlog.h>

namespace Network {

RSocket::RSocket():socktype(R_NONE)
{
    sockFd = 0;
    socketPort = 0;
    errorCode = 0;
    socketValid = false;
    blockAble = false;
    memset(socketIp,0,sizeof(socketIp));
}

RSocket::RSocket(const RSocket &rsock)
{
    this->sockFd = rsock.sockFd;
    this->socketPort = rsock.socketPort;
    this->errorCode = rsock.errorCode;
    this->socketValid = rsock.socketValid;
    this->blockAble = rsock.blockAble;
    memset(socketIp,0,sizeof(socketIp));
    memcpy(this->socketIp,rsock.socketIp,sizeof(socketIp));
}

bool RSocket::createSocket(SocketType socktype)
{
#ifdef Q_OS_WIN
    static bool isInit = false;
    if(!isInit)
    {
        isInit = true;
        WSADATA ws;
        if(WSAStartup(MAKEWORD(2,2),&ws) != 0)
        {
            RLOG_ERROR("Init windows socket failed!");
            return false;
        }
    }
#endif

    sockFd = socket(AF_INET,socktype,0);
    if(sockFd == INVALID_SOCKET)
    {
        errorCode = getErrorCode();
        RLOG_ERROR("Create socket failed! [ErrorCode:%d]",errorCode);
        return false;
    }

    socketValid = true;
    return true;
}

bool RSocket::bind(const char *ip, unsigned short port)
{
    if(!isValid())
        return false;

    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY)/*inet_addr(ip)*/;

    int ret = ::bind(sockFd,(sockaddr*)&localAddr,sizeof(localAddr));
    if(ret == SOCKET_ERROR)
    {
        closeSocket();
        errorCode = getErrorCode();
        RLOG_ERROR("Bind socket error [%s:%d] [ErrorCode:%d]",ip,port,errorCode);
        return false;
    }

    strcpy(socketIp,ip);
    socketPort = port;

    return true;
}

/*!
 * @brief 开启socket监听
 * @param[in] backlog 同一时刻最大允许连接数量
 * @return 是否监听成功
 */
bool RSocket::listen(int backlog)
{
    if(!isValid())
        return false;

    int ret = ::listen(sockFd,backlog);
    if(ret == SOCKET_ERROR)
    {
        closeSocket();
        errorCode = getErrorCode();
        RLOG_ERROR("Listen socket error! [ErrorCode:%d]",errorCode);
        return false;
    }

    RLOG_INFO("Listen socket success!");

    return true;
}

bool RSocket::closeSocket()
{
    if(isValid())
    {
        if(closesocket(sockFd) == 0)
        {
            socketValid = false;
            sockFd = 0;
            return true;
        }
#ifdef Q_OS_WIN
        RLOG_ERROR("Close socket error [ErrorCode:%d]!",GetLastError());
#endif
    }
    return false;
}

/*!
 * @brief 作为server端接收客户端连接
 * @param[in] 无
 * @return 返回接收的socket描述信息
 */
RSocket RSocket::accept()
{
    RSocket tmpSocket;
    if(!isValid())
        return tmpSocket;

    sockaddr_in clientAddr;
    int len = sizeof(clientAddr);

    int clientSocket = ::accept(sockFd,(sockaddr*)&clientAddr,(socklen_t*)&len);
    if(clientSocket == INVALID_SOCKET)
    {
        RLOG_ERROR("Accept failed [ErrorCode:%d]!",GetLastError());
        return tmpSocket;
    }

    strcpy(tmpSocket.socketIp,inet_ntoa(clientAddr.sin_addr));
    tmpSocket.socketValid = true;
    tmpSocket.socketPort= ntohs(clientAddr.sin_port);
    tmpSocket.sockFd = clientSocket;
    tmpSocket.socktype = R_TCP;

    RLOG_INFO("Recv socket [%s:%d]",tmpSocket.socketIp,tmpSocket.socketPort);

    return tmpSocket;
}

/*!
 * @brief 接收数据，并将结果保存至缓冲区
 * @param[out] buff 保存并返回接收的数据
 * @param[in] length 缓冲区数据长度
 * @return 实际接收数据的长度
 */
int RSocket::recv(char *buff, int length)
{
    if(!isValid() || buff == NULL)
        return -1;

    size_t buffLen = strlen(buff);
    memset(buff,0,buffLen);

    int ret = ::recv(sockFd,buff,length,0);

    return ret;
}

/*!
 * @brief 发送一定长度数据
 * @param[in] buff 待发送数据的缓冲区
 * @param[in] length 待发送的长度
 * @return 是否发送成功
 */
int RSocket::send(const char *buff, const int length)
{
    if(!isValid() || buff == NULL)
        return -1;

    int sendLen = 0;
    while(sendLen != length)
    {
        int ret = ::send(sockFd,buff+sendLen,length-sendLen,0);
        if (ret <= 0)
        {
            sendLen = -1;
            break;
        }
        sendLen += ret;
    }
    return sendLen;
}

/*!
 * @brief 连接socket
 * @param[in] remoteIp 远程IP
 * @param[in] remotePort 远程端口
 * @param[in] teimeouts 超时时间
 * @return 连接是否成功
 */
bool RSocket::connect(const char *remoteIp, const unsigned short remotePort, int timeouts)
{
    if(!isValid())
        return false;

    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(remotePort);
    remoteAddr.sin_addr.s_addr = inet_addr(remoteIp);

    setBlock(false);

    if(::connect(sockFd,(sockaddr*)&remoteAddr,sizeof(remoteAddr)) != 0)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sockFd,&set);
        timeval tm;
        tm.tv_sec = timeouts;
        tm.tv_usec = 0;
        if(select(sockFd+1,0,&set,0,&tm) <= 0)
        {
            RLOG_ERROR("Connect timeout or error [%s:%d] %s ",remoteIp,remotePort,strerror(errno));
            return false;
        }
    }

    setBlock(true);
    RLOG_INFO("connect %s:%d success!\n",remoteIp,remotePort);
    return true;
}

/*!
 * @brief 设置socket是否为阻塞模式
 * @param[in] flag 状态
 * @return 返回设置的是否成功设置
 */
bool RSocket::setBlock(bool flag)
{
    if(!isValid())
        return false;

#if defined(Q_OS_WIN)
    unsigned long ul = 0;
    if(!flag)
    {
        ul = 1;
    }
    ioctlsocket(sockFd,FIONBIO,&ul);
#else defined(Q_OS_LINUX)
    int flags = fcntl(sockFd,F_GETFL,0);
    if(flags<0)
        return false;

    if(flag)
        flags = flags&~O_NONBLOCK;
    else
        flags = flags|O_NONBLOCK;

    if(fcntl(sockFd,F_SETFL,flags)!=0)
        return false;
#endif
    return true;
}

/*!
 * @brief UDP数据接收
 * @param[in] buff  char * 接收的数据缓冲区
 * @param[in] length  int  接收的数据长度
 * @return 实际接收数据的长度
 */
int RSocket::recvFrom(char *buff, int length)
{
    if(!isValid())
        return -1;

    int fromlen = sizeof(sockaddr);
    int recvLen = ::recvfrom(sockFd,buff,length,0,(sockaddr *)&localAddr,&fromlen);
    if(recvLen < 0){
#ifdef Q_OS_WIN
        RLOG_ERROR("Recv socket error [ErrorCode:%d]!",GetLastError());
#endif
        return -1;
    }
    return recvLen;
}

/*!
 * @brief UDP数据发送
 * @param[in] buff char* 待发送数据缓冲区
 * @param[in] length int 待发送数据长度
 * @param[in] dest char* 目的地址
 * @param[in] port int   目的端口号
 * @return 实际发送数据长度
 */
int RSocket::sendTo(const char *buff, const int length, const char *dest, const int port)
{
    if(!isValid())
        return -1;

    sockaddr_in raddr;
    raddr.sin_family = AF_INET;
    raddr.sin_port = htonl(port);
    raddr.sin_addr.s_addr = inet_addr(dest);

    int sendLen = 0;
    while(sendLen != length)
    {
        int ret = ::sendto(sockFd,buff,length,0,(sockaddr*)&raddr,sizeof(sockaddr_in));
        if (ret <= 0)
        {
            sendLen = -1;
            break;
        }
        sendLen += ret;
    }

    return sendLen;
}

int RSocket::setSockopt(int optname, const char *optval, int optlen)
{
    if(!isValid())
        return false;

    return setsockopt(sockFd,SOL_SOCKET,optname,optval,optlen);
}

int RSocket::getLastError()
{
    return errorCode;
}

int RSocket::getErrorCode()
{
#ifdef Q_OS_WIN
    return WSAGetLastError();
#elif defined(Q_OS_LINUX)
    return -1;
#endif
}

}//namespace Network
