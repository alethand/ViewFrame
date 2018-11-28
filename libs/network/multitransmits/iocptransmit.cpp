#include "iocptransmit.h"

#include "Base/util/rlog.h"
#include "../base/socket.h"
#include "../win32iocp/iocpcontext.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include <WinSock2.h>
#endif

namespace Network{

IocpTransmit::IocpTransmit():
    BaseTransmit()
{

}

IocpTransmit::~IocpTransmit()
{
    dataPacketRule.reset();
}

CommMethod IocpTransmit::type()
{
    return C_TCP;
}

QString IocpTransmit::name()
{
    return "IOCP";
}

bool IocpTransmit::initialize()
{
    dataPacketRule = std::make_shared<TCP_IocpDataPacketRule>();

    iocpSendFunc = std::bind(&IocpTransmit::sendIocpData,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    return true;
}

/*!
 * @brief 开始传输指定的数据单元
 * @param[in] SendUnit 待发送的数据单元
 * @return 是否传输成功
 * @note 1.根据发送的目的服务器是否与当前服务器一致，若一致则走IOCP链路；若不一致，则走普通链路。
 *       2.若传输失败，则直接关闭socket，并将错误信息交由上层处理。
 */
bool IocpTransmit::startTransmit(SendUnit &unit)
{
    if(!netConnected || unit.method != type())
        return false;

    if(dataPacketRule->wrap(unit,iocpSendFunc))
        return true;

    RLOG_ERROR("Tcp send a data error!");
    return false;
}

/*!
 * @brief 通过IOCP发送数据
 * @param[in] sockId 目标主机连接sock id
 * @param[in] context 重叠I/O，保存待发送数据信息
 * @param[in] sendLength 实际发送数据长度
 * @return 是否发送成功
 */
bool IocpTransmit::sendIocpData(int sockId,IocpContext * context,DWORD & sendLength)
{
    DWORD sendFlags = 0;

    if(WSASend(sockId, &context->getWSABUF(), 1, &sendLength, sendFlags, &context->getOverLapped(), NULL) == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        if(error != ERROR_IO_PENDING){
            //TODO 对错误进行处理
qDebug()<<__FILE__<<__LINE__<<__FUNCTION__<<"writeError!";
            return false;
        }
    }
    return true;
}

bool IocpTransmit::close()
{
    return true;
}

} //namespace Network
