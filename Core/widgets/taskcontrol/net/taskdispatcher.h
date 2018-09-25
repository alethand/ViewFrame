﻿/*!
 *  @brief     任务管理网络下发模块
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASKDISPATCHER_H
#define TASKDISPATCHER_H

#include <QObject>

#include "../head.h"

class QUdpSocket;
class QTimer;

namespace TaskControlModel {

class TaskDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit TaskDispatcher();
    ~TaskDispatcher();

    bool initTask(TaskConfigInfo & info);

    void bindTaskList(TaskInfoList list);

    void startDispatch();
    void pauseDispatch();
    void resetDispatch();

signals:
    void dispatchFinished();
    void taskDispatch();

private slots:
    void checkDispatchTask();

private:
    QUdpSocket * udpSocket;
    QTimer * distributeTimer;           /*!< 下发定时器 */
    int hasDispatchIndex;               /*!< 已经下发的位置 */

    TaskInfoList taskInfoList;
};

} //namespace TaskControlModel

#endif // TASKDISPATCHER_H
