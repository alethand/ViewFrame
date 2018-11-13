/*!
 *  @brief     TCP服务器
 *  @details   v0.1:实现基础功能,待以后再做调整
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

namespace Network{
class RSocket;
}

namespace Core{

class TcpServer : public RTask
{
public:
    explicit TcpServer();
    ~TcpServer();

    struct ModuleProtocol{
        int startCode;      /*!< 开始标志码 */
        int type;           /*!< 协议类型 */
        int length;         /*!< 整包数据长度 */
        int endCode;        /*!< 结束标志码 */
        QString moduleId;   /*!< 模块Id */
    };

    bool init(Datastruct::NetworkInfo & info);

    void registNetworkObserver(QString mid,QStringList protos);

    void startMe();
    void stopMe();

protected:
    void run();

private:
    Network::RSocket * server;
    QList<Network::RSocket *> clients;
    Datastruct::NetworkInfo netinfo;

    QHash<int,ModuleProtocol> observeredProtocol;
};

}

#endif // TCPSERVER_H
