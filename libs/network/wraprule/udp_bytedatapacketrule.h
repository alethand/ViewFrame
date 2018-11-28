/*!
 *  @brief     UDP字节流数据处理
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef UDP_BYTEDATAPACKETRULE_H
#define UDP_BYTEDATAPACKETRULE_H

#include "wraprule.h"

namespace Network{

class Handler;

class UDP_ByteDataPacketRule
{
public:
    UDP_ByteDataPacketRule();
    ~UDP_ByteDataPacketRule();

    virtual bool wrap(const SendUnit &sunit, UDPByteSender sendDataFunc);
    virtual bool unwrap(const char * data,const int length,DataHandler handler);

    //注册接收数据处理
    void registDataHandler(Handler * dataHandler);

};

} //namespace Network

#endif // UDP_BYTEDATAPACKETRULE_H
