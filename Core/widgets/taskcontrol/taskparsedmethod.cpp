#include "taskparsedmethod.h"

#include <QDataStream>

namespace TaskControlModel{

TaskParsedMethod::TaskParsedMethod():magicNum("TASKCONTROL_LIST")
{

}

bool TaskParsedMethod::startParse(RTextFile *file)
{
    QDataStream stream(file);
    QString flag;
    stream >> flag;

    if(flag != magicNum)
        return false;

    while(!stream.atEnd()){
        NewTaskInfo * taskInfo = new NewTaskInfo;
        stream>> (*taskInfo);
        taskList.append(taskInfo);
    }
    return true;
}

bool TaskParsedMethod::startSave(RTextFile *file)
{
    QDataStream stream(file);
    stream << magicNum;

    std::for_each(taskList.begin(),taskList.end(),[&](NewTaskInfo * info){
        stream<<(*info);
    });
    return true;
}

}
