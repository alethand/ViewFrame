/*!
 *  @brief     任务控制协议
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASK_CONTROL_HEAD_H
#define TASK_CONTROL_HEAD_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QDateTime>
#include <QDataStream>

#include <QDebug>

#include <Core/protocol/datastruct.h>

#define LABEL_MIN_WIDTH 120
#define LABEL_MIN_HEIGHT 25

//编辑框、combobox的固定大小
#define LINEDIT_FIXED_WIDTH 173
#define LINEDIT_FIXED_HEIGHT 25

namespace TaskControlModel {

/**
 * @brief The operationalOrder enum
 */
enum Order{
//    ADD,
//    DELETE,
//    REVISE
};

/*!
 *  @brief 表头字段
 */
enum TaskHead{
    T_No = 0,               /*!< 序号 */
    T_TYPE,                 /*!< 任务类型 */
    T_PARAMETERS,           /*!< 任务参数 */
    T_E_TIME,               /*!< 任务执行时间 */
    T_E_TIME_LONG,          /*!< 任务执行时长 */
    T_DIS_STATE             /*!< 下发状态 */
};

/*!
 *  @brief 下发状态
 */
enum DistuributeState{
    Not_Issued,       /*!< 未下发 */
    Issued,           /*!< 已下发 */
    Error_Issued      /*!< 下发错误 */
};

/*!
 *  @brief 新基本任务信息
 *  @details 动态解析xml文件，保存不同的任务的字段信息。
 */
struct NewTaskInfo
{
    NewTaskInfo(){
        this->dstate = Not_Issued;
        userChecked = false;
    }

    NewTaskInfo(const NewTaskInfo & info){
        this->userChecked = info.userChecked;
        this->taskName = info.taskName;
        this->parameter = info.parameter;
        this->excuteTime = info.excuteTime;
        this->lastTime = info.lastTime;
        this->dstate = info.dstate;
        this->fields = info.fields;
        this->localParsedFileName = info.localParsedFileName;
    }

    friend QDataStream & operator<<(QDataStream & stream,const NewTaskInfo & info);
    friend QDataStream & operator>>(QDataStream & stream,NewTaskInfo & info);

    bool userChecked;               /*!< 用户是否选中 */
    QString taskName;               /*!< 任务名称(从解析的xml文件名中提取) */
    QString parameter;              /*!< 任务参数 */
    QDateTime excuteTime;           /*!< 任务执行时间 */
    quint32 lastTime;               /*!< 任务执行时长 */
    DistuributeState dstate;        /*!< 下发状态 */
    QMap<int,Core::Datastruct::Data_Word> fields;     /*!< 某个任务下字段集合,key:int对应当前字段在所有字段中的索引，value:QVaraint,对应字段的值 */

    QString localParsedFileName;    /*!< 本地xml文件全路径 */

};

typedef QList<NewTaskInfo *> NewTaskList;

} // namespace TaskControlModel

#endif // TASK_CONTROL_HEAD_H
