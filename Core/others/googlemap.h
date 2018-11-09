#ifndef GOOGLEMAP_H
#define GOOGLEMAP_H

#include <QPixmap>

#include "CommonHeader.h"
#include "utility.h"
#include "file/programfilepath.h"

#define LATTOP  3.14            //等效纬度的最大值
#define LNGTOP   -180.0         //等效经度的最小值
#define LATBOTTOM -3.14         //等效纬度的最小值
#define LNGBOTTOM 180//179.999999999 //等效经度的最小值

class GoogleMap
{
public:
    GoogleMap(QWidget *widget=0);
    void Invalidate(bool showtype); // 调用该函数刷新显示地图
    void GetLngLatfromScreenNo(double &lng,double &lat,double x,double y); // 根据屏幕坐标获得等效经纬度坐标
    void GetPointfromLngLatNo(double lng,double lat,double &x,double &y); // 根据等效经纬度获得屏幕坐标
    void GetLngLatfromScreenNoInner(double &lng,double &lat,double x,double y); // 根据屏幕坐标获得等效经纬度坐标
    void GetPointfromLngLatNoInner(double lng,double lat,double &x,double &y); // 根据等效经纬度获得屏幕坐标
    void addoffset(QPoint point); // 地图发生偏移处理事件
    void SetLButtonPoint(QPoint point); // 设置鼠标按下的像素点
    void zoompoint(QPoint pt,double size); // 以pt为中心对地图进行放大/缩小
    void UpdateNewTopPoint(); // 更新当前左上角坐标的经纬度
    int  getCurrentZoom(){return this->zoom;}    //获得当前地图缩放的等级


private:
    QWidget *targetwidget; // 显示海图的窗口对象
    void DisplayCoord(QPainter *painter,QRect widgetRect); //显示坐标系图
    void displaymap(QPainter *painter,QRect widgetRect,QString imagepath); // 地图绘制
    void Updateparameters(double pic_left,double pic_top); // 每次绘制时初始化绘制参数
    void setZoomNum(int zo,double pic_left,double pic_top,double pic_right  ,double pic_bottom); // 更新地图放大级数
    void zoomInOut(double lng,double lat,double size); // 设置左上角经纬度坐标为lng、lat，缩放为原来的size倍


    QString mapPath; // google地图图片存放的根路径
    int      zoom;       //要显示图片的等级
    double   zoomnum;    //图片的放大倍数 在0-2之间 当大于2时执行一次放大
    int unitImageLen; // 每张google图片的长宽 单位 像素
    int offset_x,offset_y; // 显示海图图片的偏移量
    int pic_x,pic_y; // 记录最左上角图片所使用google图片的行列号
    int pic_left;
    int imageWidth,imageHeight; // 图片当前显示的宽度和高度 放缩后
    double   leftLon;       //显示图片左上角经度
    double   topLat;        //显示图片左上角维度
    double   oldleftLon;   // 记录历史图片左上角经度
    double   oldtopLat;    // 记录历史图片左上角维度
    QPoint   startPoint;   // 上一个时刻鼠标的坐标位置
    double   right;
    double   bottom;
    int      MapmaxGrade ;         // google地图图片显示的最大等级数
    QPixmap pixmap; // 背景图片对象

    int CoordXInterval; // 坐标系显示方式下，经度与维度线间像素间隔
    int CoordYInterval;

    ProgramFilePath programPath;
};

#endif // GOOGLEMAP_H
