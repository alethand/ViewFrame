/*!
 *  @brief     组合验证接口
 *  @details   可嵌套组合多个验证器，从而实现and、or条件的组合验证。
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.19
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RCOMBINEVALIDATOR_H
#define RCOMBINEVALIDATOR_H

#include "rvalidator.h"
#include "../../base_global.h"

#include <QList>

class BASESHARED_EXPORT RCombineValidator : public RValidator
{
public:
    explicit RCombineValidator();
    ~RCombineValidator();

    typedef QList<RValidator *> ValidatorList;

    void addValidator(RValidator * v);

    virtual RValid::State validate() = 0;

protected:
    ValidatorList vlist;
};

class BASESHARED_EXPORT RAndCombineValidator : public RCombineValidator
{
public:
    explicit RAndCombineValidator();
    RValid::State validate();
};

class BASESHARED_EXPORT ROrCombineValidator : public RCombineValidator
{
public:
    explicit ROrCombineValidator();
    RValid::State validate();
};

#endif // RCOMBINEVALIDATOR_H
