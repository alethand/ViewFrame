/*!
 *  @brief     UDP传输客户端
 *  @details   1.暂存待发送的数据信息 @n
 *             2.根据不同的网卡，将信息从指定的网卡发出 @n
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.21
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "protocol/datastruct.h"
#include "rtask.h"

namespace Network{
class RSocket;
}

namespace Core{

class UdpClient : public RTask
{
public:
    explicit UdpClient();
    ~UdpClient();

    bool init();
    void registNetworkDestination(Datastruct::NetworkInfo & info);

    void addPreSendData(QString networdId,QByteArray & array);

    void startMe();
    void stopMe();

protected:
    void run();

private:
    typedef QMap<QString,Datastruct::NetworkInfo> NetworkMap;
    NetworkMap netmap;

    Network::RSocket * client;
};

} //namespace Core

#endif // UDPCLIENT_H
