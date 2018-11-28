#include "taskmanager.h"

#include "rtask.h"

namespace Core{

TaskManager::TaskManager()
{

}

void TaskManager::addTask(QString id, TaskPtr ptr)
{
   std::lock_guard<std::mutex> lg(t_mutex);
   if(taskMap.contains(id))
       return;

   taskMap.insert(id,ptr);

}

TaskPtr TaskManager::getTask(QString id)
{
    std::lock_guard<std::mutex> lg(t_mutex);
    if(!taskMap.contains(id))
        return NULL;

    return taskMap.value(id);
}

void TaskManager::releaseTask()
{
    std::lock_guard<std::mutex> lg(t_mutex);
    TaskMap::iterator iter = taskMap.begin();
    while(iter != taskMap.end()){
        iter.value()->stopMe();
        iter.value().reset();
        iter++;
    }
}

}
