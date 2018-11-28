#ifndef HEALTHSTATE_HEAD_H
#define HEALTHSTATE_HEAD_H

#include <QString>
#include <QVector>

namespace HealthState
{

enum Hs_WkCorlor
{
    wkblack = 0,
    wkyellow = 1,
    wkred = 2,
    wkOth_green
};

struct WorkStruct
{
    QString name;                       //名称  64位
    QString content;                    //内容   64
    unsigned short   contentColor;      //内容文字颜色
};

enum Hs_CpColor
{
    SubMgreen = 0,
    SubMred = 1,
    SubMgray =2,
    SubMoth_white
};

struct Componet                         //组件信息
{
    QString name;
    unsigned short  state;
};

struct SubMachine
{
    QString name;                       //分机名称
    unsigned long componetNum;          //分机组件的个数
    QVector<Componet>  compInfo;        //组件信息
};

struct HealthData
{
    unsigned long workStateNums;
    QVector<WorkStruct>  wsInfo;
    unsigned long subMachineNums;
    QVector<SubMachine>  smInfo;
};

class XlsHealthState;               //xls保存健康管理状态


}   //namespace HealthState

#endif // HEAD_H
