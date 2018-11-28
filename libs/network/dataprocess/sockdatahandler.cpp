#include "sockdatahandler.h"

#include "netglobal.h"

namespace Network{

SockTextDataHandler::SockTextDataHandler()
{

}

/*!
 * @brief 接收数据处理回调方法
 * @details 网络层接收一个完整数据包后，通过设置的回调接口，将数据包传入此方法内。 \p
 *          用户可将传入的数据信息添加至数据队列，将数据处理交由其它线程。
 * @warning 避免直接数据处理，造成数据接收出现问题。
 * @param[in]  recvData 待处理的数据包
 */
void SockTextDataHandler::handle(const RecvUnit &recvData)
{

}

}
