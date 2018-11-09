#include "utility.h"

/*
    auth :wuenyang
    data: 13-12-07
    speci:用来保存公有的变量，方便全局使用

 */

#include <QDir>
#include "math.h"

bool Utility::is_Show_ship_info = true;

int Utility::current_map_zoom = 4; //默认情况下，google地图是显示第三级

bool Utility::showType = true;

int  Utility::moonDValue = 60;

//将经纬度由double转换成度、分、秒
QString Utility::coordinatesConversion(double num)
{
    double tempDegree;
    if(num<0){              //先求得绝对值
       tempDegree = 0-num;
    }else{
        tempDegree = num;
    }
    double degree = floor(tempDegree);
    double temp =  (tempDegree-degree)*60;   //分
    double minute = floor(temp);
    double second =floor((temp-minute)*60);   //秒
    QString tDegree = (degree)<10?"0"+QString::number(degree):QString::number(degree);
    QString tMinute = (minute<10)?"0"+QString::number(minute):QString::number(minute);
    QString tSecond = (second<10)?"0"+QString::number(second):QString::number(second);
    return tDegree+"度"+tMinute+"分"+tSecond+"秒";    //复制构造函数
}

//将数字转换成标准的两位显示
QString Utility::switchToStandard(int a)
{
    return (a<10)?"0"+QString::number(a):QString::number(a);
}


