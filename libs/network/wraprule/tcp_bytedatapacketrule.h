/*!
 *  @brief     通用TCP数据包封装
 *  @details   用于对TCP数据进行解包，确保数据接收正确。
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.26
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef TCP_BYTEDATAPACKETRULE_H
#define TCP_BYTEDATAPACKETRULE_H

#include <QHash>

#include "wraprule.h"

namespace Network{

class TCP_ByteDataPacketRule
{
public:
    explicit TCP_ByteDataPacketRule();

    bool wrap(const SendUnit &sunit, ByteSender sendDataFunc);
    bool unwrap(const char * data,const int length,DataHandler handler);

private:
    bool recvData(const char *recvData, int recvLen);

private:
    int SendPackId;
    char sendBuff[MAX_SEND_BUFF];

    QByteArray lastRecvBuff;              //断包接收缓冲区
    QHash<int,PacketBuff*> packetBuffs;   //多包缓冲区

    DataHandler dHandler;
};

} // namespace ClientNetwork

#endif // TCP_BYTEDATAPACKETRULE_H
