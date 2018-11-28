/*!
 *  @brief     处理网络层解包好的数据
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.X27
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef SOCKDATAHANDLER_H
#define SOCKDATAHANDLER_H

#include "handler.h"

namespace Network{

class SockTextDataHandler : public Handler
{
public:
    SockTextDataHandler();

    void handle(const RecvUnit &recvData);
};

}

#endif // SOCKDATAHANDLER_H
