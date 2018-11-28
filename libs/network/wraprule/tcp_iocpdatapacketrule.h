/*!
 *  @brief     利用IOCP发送、处理数据
 *  @details   1.将待发送的数据通过添加网络层传输控制协议，确保数据不粘包; \p
 *             2.根据接收的数据对数据块按照网络层协议头解析; \p
 *               2.1.将接收到的数据调用回调函数处理
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.26
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef TCP_IOCP_DATAPACKETRULE_H
#define TCP_IOCP_DATAPACKETRULE_H

#include <QHash>

#include "wraprule.h"
#include "../connection/tcpclient.h"

namespace Network{
class IocpContext;
class Handler;

class TCP_IocpDataPacketRule
{
public:
    explicit TCP_IocpDataPacketRule();

    bool wrap(const SendUnit &data, IocpContextSender sendFunc);
    void unwrap(IocpContext * context,unsigned long recvLen);

    //注册接收数据处理
    void registDataHandler(Handler * dataHandler);

private:
    void recvData(const char *recvData, int recvLen);

private:
    int SendPackId;
    char sendBuff[MAX_SEND_BUFF];

    IocpContext * ioContext;              /*!< 当前处理的客户端信息 */
    Handler * handler;                    /*!< 数据处理器 */
    QByteArray lastRecvBuff;              /*!< 断包接收缓冲区 */
    QHash<int,PacketBuff*> packetBuffs;   /*!< 多包缓冲区 */
};

} // namespace ClientNetwork

#endif // TCPDATAPACKETRULE_H
