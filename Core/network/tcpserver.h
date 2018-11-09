/*!
 *  @brief     TCP服务器
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "protocol/datastruct.h"
#include "rtask.h"

namespace Network {
class RSocket;
}

namespace Core{

class TcpServer : public RTask
{
public:
    explicit TcpServer();
    ~TcpServer();

    void setNetworkInfo();

    void startMe();
    void stopMe();

protected:
    void run();

private:
    Network::RSocket * rsocket;
};

}

#endif // TCPSERVER_H
