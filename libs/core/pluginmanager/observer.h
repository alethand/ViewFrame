﻿/*!
 *  @brief     订阅端
 *  @details   订阅消息，根据消息类型，及时刷新显示
 *  @file
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.XX
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef OBSERVER_H
#define OBSERVER_H

#include "subject.h"
#include <Base/messagetype.h>
#include <core/core_global.h>

namespace Core{

class CORESHARED_EXPORT Observer
{
public:
    Observer();
    virtual ~Observer();

    virtual void onMessage(MessageType::MessType type) = 0;
};

} //namespace Base

#endif // OBSERVER_H
