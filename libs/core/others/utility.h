#ifndef UTILITY_H
#define UTILITY_H

#include <QString>

class Utility{
public:
   static bool is_Show_ship_info;  //用来控制是否显示舰船的位置信息
   static int current_map_zoom;   //用来保存当前地图所达到的等级，主要用来缩放舰艇和水听站的大小
   static bool showType;      //用来保存当前海图是哪一种显示方式，默认显示卫星地图(true)
   static int moonDValue;   //卫星显示的宽度

   static QString coordinatesConversion(double num);
   static QString switchToStandard(int a);
};

#endif // UTILITY_H
