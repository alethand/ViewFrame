/*!
 *  @brief     基础验证器
 *  @details   提供了对不同验证类型的统一接口 \p
 *             [1]:RNumericValidator，数字类型验证器，支持对基本数据类型的范围验证； \p
 *             [2]:RStringValidator，字符串验证器，支持对字符串采用正则表达式匹配，内嵌几种基本类型，支持自定义设置匹配表达式 \p
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.19
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RVALIDATOR_H
#define RVALIDATOR_H

#include <QString>
#include <climits>
#include <QDebug>

#include "../../base_global.h"

namespace RValid {

enum State {
    Invalid,
    Acceptable
};

/*!
 *  @brief 验证条件类型
 *  @details
 */
enum Condition{
    NoC,Lt,Le,Eq,Gt,Ge,Ne
};

/*!
 *  @brief 字符串验证器内置默认的正则表达式
 *  @details 用户可设置自定义的正则表达式
 */
enum RegularExpress{
    R_FULL_SPACE,         /*!< 全部空格 */
    R_START_SPACE,        /*!< 以空格开头 */
    R_END_SPACE,          /*!< 以空格结尾 */
    R_IP,                 /*!< IP地址 */
    R_PHONE_NUM,          /*!< 手机号11位 */
    R_MAIL,               /*!< 邮箱 */

    R_USER_DEFINE         /*!< 用户自定义 */
};

}

class BASESHARED_EXPORT RValidator
{
public:
    RValidator(){}

    virtual RValid::State validate() = 0;

protected:
    QString errorDesc;
};

/*!
 *  @brief 数值型校验器
 *  @details 支持基本数据类型的校验，支持自定义验证条件和验证范围；
 */
template<class T>
class RNumericValidator : public RValidator
{
public:
    RNumericValidator();
    RNumericValidator(T input,RValid::Condition c1, T first, RValid::Condition c2 = RValid::NoC, T second = std::numeric_limits<T>::max());
    RNumericValidator(RValid::Condition c1, T first, RValid::Condition c2 = RValid::NoC, T second = std::numeric_limits<T>::max());

    void setFirstCondition(RValid::Condition c1, T first);
    void setSecondCondition(RValid::Condition c2,T second);

    void setInput(T t);
    RValid::State validate();

private:
    bool executeCondition(RValid::Condition & c,T & t,T & input);

private:
    RValid::Condition c1;
    T first;

    RValid::Condition c2;
    T second;

    T input;
};

template<class T>
RNumericValidator<T>::RNumericValidator():RValidator(),c1(NoC),c2(NoC)
{
    this->first = std::numeric_limits<T>::min();
    this->first = std::numeric_limits<T>::max();
}

/*!
 * @brief 构建数据匹配范围
 * @param[in]  input 待验证数据
 *             t1 待验证数据与第一个参数相比较条件
 *             first 第一个参数
 *             t2 待验证数据与第二个参数相比较条件,默认为缺省
 *             second 第二个参数，默认为缺省
 *          \p
 *             若验证10 <= x <= 20,那么t1 = ge,first = 10,t2 = le,second = 20; \n
 *             若验证10 <= x,那么t1 = ge,first = 10,后面两个默认 \n
 *             若验证10 >= x,那么t1 = le,first = 10，后面两个默认 \n
 * @warning 若t2条件存在，则t1与t2之间为且的关系
 * @return 是否满足验证条件
 */
template<class T>
RNumericValidator<T>::RNumericValidator(T input,RValid::Condition c1, T first, RValid::Condition c2, T second):RValidator()
{
    this->input = input;

    this->c1 = c1;
    this->first = first;

    this->c2 = c2;
    this->second = second;
}

template<class T>
RNumericValidator<T>::RNumericValidator(RValid::Condition c1, T first, RValid::Condition c2, T second):RValidator()
{
    this->c1 = c1;
    this->first = first;

    this->c2 = c2;
    this->second = second;
}

template<class T>
void RNumericValidator<T>::setFirstCondition(RValid::Condition c1, T first)
{
    this->c1 = c1;
    this->first = first;
}

template<class T>
void RNumericValidator<T>::setSecondCondition(RValid::Condition c2, T second)
{
    this->c2 = c2;
    this->second = second;
}

template<class T>
void RNumericValidator<T>::setInput(T t)
{
    this->input = t;
}

/*!
 * @brief 验证输入的信息是否在范围内
 * @details 验证int类型的时，将字符串转int的工作交给QString自身，本身只做范围的校验
 * @param[in] input 待校验的信息
 * @return 校验结果
 */
template<class T>
RValid::State RNumericValidator<T>::validate()
{
    if(c1 == RValid::NoC || first < std::numeric_limits<T>::lowest())
        return RValid::Invalid;

    if(c2 != RValid::NoC && first > second)
        return RValid::Invalid;

    if(executeCondition(c1,first,input)){
        if(c2 != RValid::NoC){
            if(executeCondition(c2,second,input))
                return RValid::Acceptable;
        }else{
            return RValid::Acceptable;
        }
    }

    return RValid::Invalid;
}

/*!
 * @brief 对输入的参数进行逐个的验证
 * @param[in] c 判断条件 \n
 *            t 基准数据
 *            input 待验证数据
 * @return true 验证成功；false 验证失败
 */
template<class T>
bool RNumericValidator<T>::executeCondition(RValid::Condition &c, T &t, T &input)
{
    switch(c){
        case RValid::NoC:
            break;
        case RValid::Lt:
                return input < t;
            break;
        case RValid::Le:
                return input <= t;
            break;
        case RValid::Eq:
                return input == t;
            break;
        case RValid::Gt:
                return input > t;
            break;
        case RValid::Ge:
                return input >= t;
            break;
        case RValid::Ne:
                return input != t;
            break;
        default:
            break;
    }

    return false;
}

/*!
 *  @brief 字符串验证器
 */
class BASESHARED_EXPORT RStringValidator : public RValidator
{
public:
    RStringValidator();
    RStringValidator(QString input,RValid::RegularExpress re);
    RStringValidator(QString input,QString  regexp,RValid::RegularExpress re = RValid::R_USER_DEFINE);

    typedef QMap<RValid::RegularExpress,QString> RExpressMap;

    RValid::State validate();

    void setRegularExpress(RValid::RegularExpress tt);
    void setUserRegexp(QString text);

private:
    RValid::RegularExpress type;
    QString input;
    QString userRegexp;

    static RExpressMap innerRegexps;

};

#endif // RVALIDATOR_H
