#include "datastruct.h"

namespace Core{

namespace Datastruct{

//TODO 20181109保存时只需要保存字段的name、index以及索引就好，基本信息从协议中获取
QDataStream & operator<< (QDataStream &stream, const Data_Bit &data)
{
    stream<<data.name<<data.index<<data.enable<<data.visible<<data.weight<<data.precision<<data.unit<<data.displayText;
    return stream;
}

QDataStream & operator>> (QDataStream &stream, Data_Bit &data)
{

    return stream;
}

QDataStream & operator<< (QDataStream &stream, const BitField &field)
{
    stream<<field.bitList.size();
    std::for_each(field.bitList.begin(),field.bitList.end(),[&](const Data_Bit data){
        stream<<data;
    });
    return stream;
}

QDataStream & operator>> (QDataStream &stream, BitField &field)
{
    int size = 0;
    stream >> size;
    for(int i = 0;i < size;i++){
        Data_Bit data;
        stream>>data;
        field.bitList.append(data);
    }
    return stream;
}

QDataStream & operator<<(QDataStream & stream,const Data_Word & data)
{
    stream<<data.name<<data.index<<data.bytes<<data.isSigned<<data.bits
         <<data.weight<<data.precision<<data.unit<<data.displayText;

    stream<<static_cast<int>(data.type);
    switch(data.type){
    case ComboBox :
        stream<<data.value.toInt();
        break;
    case CheckBox :
    case RadioButton :
        stream<<data.value.toBool();
        break;
    case TextEdit :
        stream<<data.value.toString();
        break;
    case ValueIntEdit :
        stream<<data.value.toInt();
        break;
    case ValueFloatEdit :
        stream<<data.value.toDouble();
        break;
    case DateEdit :
        stream<<data.value.toDate();
        break;
    case TimeEdit :
        stream<<data.value.toTime();
        break;
    case Empty :
    default:
        break;
    }

    return stream;
}

QDataStream & operator>>(QDataStream & stream,Data_Word & data)
{
    stream>>data.name>>data.index>>data.bytes>>data.isSigned>>data.bits
            >>data.weight>>data.precision>>data.unit>>data.displayText;

    int type = 0;
    stream >> type;
    data.type = static_cast<ControlType>(type);

    switch(data.type){
    case ComboBox :
    {
        int tmp = 0;
        stream>>tmp;
        data.value = QVariant::fromValue(tmp);
    }
        break;
    case CheckBox :
    case RadioButton :
    {
        bool tmp = false;
        stream>>tmp;
        data.value = QVariant::fromValue(tmp);
    }
        break;
    case TextEdit :
    {
        QString tmp;
        stream>>tmp;
        data.value = QVariant::fromValue(tmp);
    }
        break;
    case ValueIntEdit :
    {
        int tmp = 0;
        stream>>tmp;
        data.value = QVariant::fromValue(tmp);
    }
        break;
    case ValueFloatEdit :
    {
        double tmp = 0;
        stream>>tmp;
        data.value = QVariant::fromValue(tmp);
    }
        break;
    case DateEdit :
    {
        QDate tmp;
        stream>>tmp;
        data.value = QVariant::fromValue(tmp);
    }
        break;
    case TimeEdit :
    {
        QTime tmp;
        stream>>tmp;
        data.value = QVariant::fromValue(tmp);
    }
        break;
    case Empty :
    default:
        break;
    }
    return stream;
}

short BaseProtocol::getBytes_Content()
{
    if(0 == bytes_Contents)  {
        for(int i=0;i < contents.count();i++)
            bytes_Contents += contents.at(i).getBytes_To(contents.at(i).count);
    }
    return bytes_Contents;
}

short SingleProtocol::getBytes_To(int pos,int base)const
{
    short allBytes = base;
    if(pos == fields.count())
        if(fields.at(pos -1).pos_InProtocol !=0)
            return fields.at(pos -1).pos_InProtocol + fields.at(pos -1).bytes;

    for(int i=0;i < pos;i++)
        allBytes +=fields.at(i).bytes;
    return allBytes;
}

} //namespace Datastruct

} //namespace Core

