#include "rtask.h"

namespace Core {

RTask::RTask(QObject *parent):
    taskState(IdleState),
    QThread(parent)
{
    runningFlag = false;
}

RTask::~RTask()
{

}

bool RTask::running()
{
    return (taskState == StartState);
}

void RTask::startMe()
{
    taskState = StartState;
    emit stateChanged(taskState);
}

void RTask::stopMe()
{
    taskState = StoppedState;
    emit stateChanged(taskState);
}

} //namespace ClientNetwork
