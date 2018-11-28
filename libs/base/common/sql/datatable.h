/*!
 *  @brief     数据库表
 *  @details   分离硬编码，方便数据字段名称的调整。
 *  @file      datatable.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.01.11
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DATATABLE_H
#define DATATABLE_H

#include "../../base_global.h"

#include <QString>

namespace DataTable
{

//系统配置表，以key-value形式存储键值
class  BASESHARED_EXPORT RimConfig
{
public:
    RimConfig();
    const QString table;
    const QString name;
    const QString value;

    const QString accountId;                //系统个人账号基数，每次申请的账号均在此数字上加1。
    const QString groupAccoungId;           //系统群账号基数，每次正确申请后在此数字加1
};

}

#endif // TABLE_H
