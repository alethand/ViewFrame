#include "taskdispatcher.h"

#include <QUdpSocket>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

namespace TaskControlModel {

TaskDispatcher::TaskDispatcher():distributeTimer(NULL),hasDispatchIndex(-1)
{

}

TaskDispatcher::~TaskDispatcher()
{
    if(distributeTimer){
        if(distributeTimer->isActive())
            distributeTimer->stop();
        delete distributeTimer;
    }
}

bool TaskDispatcher::initTask(TaskConfigInfo &info)
{
    udpSocket = new QUdpSocket();
    return true;
}

void TaskDispatcher::bindTaskList(NewTaskList list)
{
    this->taskInfoList = list;
}

/*!
 * @brief 开始或继续任务下发
 */
void TaskDispatcher::startDispatch()
{
    if(distributeTimer == NULL){
        distributeTimer = new QTimer;
        distributeTimer->setInterval(1000);
        connect(distributeTimer,SIGNAL(timeout()),this,SLOT(checkDispatchTask()));
    }

    if(hasDispatchIndex == taskInfoList.size() - 1 || hasDispatchIndex >= taskInfoList.size()){
        emit dispatchFinished();
        return;
    }

    distributeTimer->start();
}

/*!
 * @brief 暂停任务下发
 */
void TaskDispatcher::pauseDispatch()
{
    if(distributeTimer)
        distributeTimer->stop();
}

/*!
 * @brief 重置任务下发
 * @details 将任务队列恢复至默认
 */
void TaskDispatcher::resetDispatch()
{
    hasDispatchIndex = -1;
    taskInfoList.clear();
}

/*!
 * @brief 定时下发任务
 * @details 按照每个任务的执行时间，通过定时器的方式将其下发; \p
 *          每个任务只下发一次，按照任务创建的顺序依次下发: \p
 *          1.若当前任务的执行时间早于程序执行时间，则提示下发错误； \p
 *          2.下发按照任务列表和绝对时间下发 \p
 *          3.下发时禁止编辑任务列表
 */
void TaskDispatcher::checkDispatchTask()
{
    qint64 currTime = QDateTime::currentDateTime().toTime_t();
    int i = hasDispatchIndex == -1 ? 0: hasDispatchIndex;
    for(;i < taskInfoList.size(); i++ ){
        uint etime = taskInfoList.at(i)->excuteTime.toTime_t();
        if(etime == currTime){

            //TODO 20181104对字段组织报文

            taskInfoList.at(i)->dstate = TaskControlModel::Issued;
            hasDispatchIndex = i;
            emit taskDispatch();
        }else if(etime < currTime){
            hasDispatchIndex = i;
        }
    }
    if(hasDispatchIndex == taskInfoList.size() - 1){
        distributeTimer->stop();
        emit dispatchFinished();
    }
}

} //namespace TaskControlModel
