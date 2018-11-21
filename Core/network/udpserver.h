/*!
 *  @brief     UDP服务器
 *  @details   监听端口，接收udp数据
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.21
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "protocol/datastruct.h"
#include "rtask.h"

namespace Network{
class RSocket;
}

namespace Core{

class UdpServer : public RTask
{
public:
    explicit UdpServer();
    ~UdpServer();

    bool init(Datastruct::NetworkInfo & info);
    void registNetworkObserver(QString mid,QStringList protos);

    void startMe();
    void stopMe();

protected:
    void run();

private:
    Network::RSocket * server;
    Datastruct::NetworkInfo netinfo;

    QHash<int,Datastruct::ModuleProtocol> observeredProtocol;
};

} //namespace Core

#endif // UDPSERVER_H
