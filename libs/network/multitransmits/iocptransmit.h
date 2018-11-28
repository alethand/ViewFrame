/*!
 *  @brief     TCP数据传输
 *  @details   使用windows iocp对数据单元发送、接收解析
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef IOCPTRANSMIT_H
#define IOCPTRANSMIT_H

#include <memory>

#include "basetransmit.h"
#include "../wraprule/tcp_iocpdatapacketrule.h"
namespace Network{
class RSocket;

class NETWORKSHARED_EXPORT IocpTransmit : public BaseTransmit
{
public:
    IocpTransmit();
    ~IocpTransmit();

    CommMethod type();
    QString name();

    bool initialize();
    bool startTransmit(SendUnit &unit);
    bool close();

private:
    bool sendIocpData(int sockId,IocpContext * context,DWORD & sendLength);

private:
    std::shared_ptr<TCP_IocpDataPacketRule> dataPacketRule;
    IocpContextSender iocpSendFunc;
};

} //namespace Network

#endif // TCPTRANSMIT_H
