/*!
 *  @brief     UDP信息传输
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef UDPTRANSMIT_H
#define UDPTRANSMIT_H

#include <memory>
#include "basetransmit.h"

namespace Network{

class UDP_ByteDataPacketRule;
class RSocket;

class NETWORKSHARED_EXPORT UdpTransmit : public BaseTransmit
{
public:
    UdpTransmit();
    ~UdpTransmit();

    CommMethod type();
    QString name();

    void setUdpPacketRule(UDP_ByteDataPacketRule * dataPacketRule);

    bool initialize();
    bool bind(const char * ip,ushort port);
    bool startTransmit(SendUnit &unit);
    bool startRecv(char *recvBuff, int recvBuffLen,DataHandler recvDataFunc);
    bool close();

private:
    int sendByteData(const char * buff, const int length, const char *ip, const int port);

private:
    UDP_ByteDataPacketRule * dataPacketRule;
    RSocket* udpSocket;

    UDPByteSender byteSendFunc;

};

} //namespace Network

#endif // UDPTRANSMIT_H
