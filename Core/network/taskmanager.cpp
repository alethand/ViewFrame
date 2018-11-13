#include "taskmanager.h"

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

}
