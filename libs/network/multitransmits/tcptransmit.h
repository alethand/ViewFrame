/*!
 *  @brief     TCP数据传输
 *  @details   使用原始socket对数据单元发送、接收解析
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef TCPTRANSMIT_H
#define TCPTRANSMIT_H

#include <memory>

#include "basetransmit.h"
#include "../wraprule/tcp_bytedatapacketrule.h"

namespace Network{

class RSocket;

class NETWORKSHARED_EXPORT TcpTransmit : public BaseTransmit
{
public:
    TcpTransmit();
    ~TcpTransmit();

    CommMethod type();
    QString name();

    bool initialize();
    bool startTransmit(SendUnit &unit);
    bool startRecv(char *recvBuff, int recvBuffLen,DataHandler recvDataFunc);
    bool close();

    bool connect(const char *remoteIp, const unsigned short remotePort, int timeouts);

private:
    int sendByteData(int sockId,const char * buff,const int length);

private:
    std::shared_ptr<TCP_ByteDataPacketRule> dataPacketRule;
    RSocket* tcpSocket;

    ByteSender byteSendFunc;
};

} //namespace Network

#endif // TCPTRANSMIT_H
