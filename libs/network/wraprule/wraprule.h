/*!
 *  @brief     协议封装规则
 *  @details   用于多种协议的封装，每种协议内部控制封装和解封装的过程 \n
 *             1.若在传输的数据正文前，需要包裹多层数据协议，通过继承此类，将每种协议对数据的处理隔离开；
 *               传输数据体【DATA】时，需要在DATA前分别包裹协议A、协议B，那么可分别创建A_WrapRule、B_WrapRule，在对应的wrap、unwrap方法 \p
 *               内部分别利用A、B两种协议对数据进行封装处理，从而实现： \p
 *               a):在【DATA】分别结果A_WrapRule、B_WrapRule的wrap处理后，数据【[B][A][DATA]】 \p
 *               b):在接收到【[B][A][DATA]】后，分别经过A_WrapRule、B_WrapRule的unwrap处理，提炼出数据【[DATA]】 \p
 *  @author    wey
 *  @version   1.0
 *  @date      2018.06.07
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef WRAPRULE_H
#define WRAPRULE_H

#include <QByteArray>

#include "../network_global.h"
#include "../head.h"

namespace Network{

class NETWORKSHARED_EXPORT WrapRule
{
public:
    WrapRule();

    /*!
     * @brief 对data内的正文部分进行协议包装处理
     * @param[in/out] data 处理后的数据信息
     */
    virtual void wrap(ProtocolPackage & /*data*/){}

    /*!
     * @brief 对data进行反向的协议解析，逐层剥掉协议，返回数据体
     * @param[in/out] data 接收到的原始信息
     * @param[in/out] result 逐层剥离协议的数据
     */
    virtual bool unwrap(const QByteArray & /*data*/,ProtocolPackage & /*result*/){return true;}
};

} // namespace ClientNetwork

#endif // WRAPRULE_H
