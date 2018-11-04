/*!
 *  @brief     任务列表文件解析
 *  @details   解析或保存对应的任务信息
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.04
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASKPARSEDMETHOD_H
#define TASKPARSEDMETHOD_H

#include "head.h"
#include "Base/util/fileutils.h"

namespace TaskControlModel{

class TaskParsedMethod : public RTextParseMethod
{
public:
    explicit TaskParsedMethod();
    ~TaskParsedMethod(){}

    void setTaskInfo(NewTaskList list){this->taskList = list;}
    NewTaskList getTaskInfo(){return this->taskList;}

    bool  startParse(RTextFile * file);
    bool  startSave(RTextFile * file);

private:
    NewTaskList taskList;
    QString magicNum;

};

}

#endif // TASKPARSEDMETHOD_H
