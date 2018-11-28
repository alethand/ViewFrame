#include "head.h"

#include <QMap>

namespace TaskControlModel {

QDataStream & operator<<(QDataStream & stream,const NewTaskInfo & info)
{
    stream<<info.userChecked<<info.taskName<<info.parameter<<info.excuteTime<<info.lastTime
         <<static_cast<int>(info.dstate);

    stream<<info.fields.size();
    QMap<int,Core::Datastruct::Data_Word>::const_iterator iter = info.fields.cbegin();
    while(iter != info.fields.cend()){
        stream<<iter.value();
        iter++;
    }
    stream<<info.localParsedFileName;

    return stream;
}

QDataStream & operator>>(QDataStream & stream,NewTaskInfo & info)
{
    stream>>info.userChecked>>info.taskName>>info.parameter>>info.excuteTime>>info.lastTime;
    int state = 0;

    stream>>state;
    info.dstate = static_cast<DistuributeState>(state);

    int fieldSize = 0;
    stream>>fieldSize;
    for(int i = 0;i < fieldSize; i++){
        Core::Datastruct::Data_Word data;
        stream>>data;
        info.fields.insert(i,data);
    }

    stream>>info.localParsedFileName;

    return stream;
}

} //namespace TaskControlModel
