#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>
#include "Base/util/fileutils.h"

//高低字节转换
unsigned short RExchangeHLByte(unsigned short variate)
{
    unsigned short data = 0;
    unsigned short variate_temp = variate;

    data = ((variate_temp & 0x00FF) <<8 );
    data += (((variate_temp & 0xFF00) >>8 ) & 0x00FF);

    return data;
}

//结构体转数组
QByteArray VoidToArray(void *data,int size)
{
    QByteArray buff_temp;
    char* caculate = (char*)data;
    char unit = 0;
    for(int i=0;i<size;i++)
    {
        unit = 0;
        memcpy(&unit,caculate+i,1);
        buff_temp.append(unit);
    }
    return buff_temp;
}

//校验和
char RCheckSum(QByteArray data)
{
    char checkSum = 0;
    QByteArray buff_temp = data;

    for(int i=0;i<buff_temp.size();i++)
    {
        checkSum ^= buff_temp[i];
    }
    return checkSum;
}

//校验和——重载
char RCheckSum(void *buff,int size)
{
    char checkSum = 0;
    QByteArray buff_temp = VoidToArray(buff,size);
    checkSum = RCheckSum(buff_temp);
    return checkSum;
}


#endif // UTILS_H
