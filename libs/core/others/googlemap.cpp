#include "googlemap.h"

#include <QPainter>
#include <QWidget>
#include <QDebug>

#include <cmath>

namespace Core{

GoogleMap::GoogleMap(QWidget *widget)
{
    targetwidget = widget;
    mapPath = programPath.configPath+"/map/wx/";
    zoomnum = 1.1;
    zoom = 5;
    imageWidth = imageHeight = unitImageLen = 256;
    pic_x = 0;
    pic_y = 0;
    offset_x = 0;
    offset_y = 0;
    MapmaxGrade = 10;

    CoordXInterval = 140;
    CoordYInterval = 120;

    setZoomNum(5,100,1,0,0);
}

/*
*  调用该函数刷新显示地图
*para:
* showtype:定义海图显示状态，设置true表示地图显示，false表示坐标系显示的
*/
void GoogleMap::Invalidate(bool showtype)
{
    QPainter painter(targetwidget); // 获得绘制对象
    //卫星地图
    if(showtype)
    {
        mapPath = programPath.configPath+"/map/wx/";
        displaymap(&painter,targetwidget->geometry(),mapPath);
    }
    else
    //道路地图
    {
        mapPath = programPath.configPath+"/map/dl/";
        displaymap(&painter,targetwidget->geometry(),mapPath);
        DisplayCoord(&painter,targetwidget->geometry());
    }
}

/*
 * 加载google图片，进行拼接并形成地图
 *para:
 *  painter:窗口绘图句柄
 *  widgetRect:绘制区域
 *  mapPath:google地图存放基本路径
 */
void GoogleMap::displaymap(QPainter *painter,QRect widgetRect,QString mapPath)
{
    // 计算当前放大倍数下google地图定义每行、每列最大的图片数量 1倍为4张
    int  Maxnum = pow((double)4,(double)zoom);
    Maxnum = sqrt((double)Maxnum);

    int row,col; // 当前界面应显示google图片的行数和列数
    col = widgetRect.width()/(imageWidth*zoomnum)+3;             //计算每列最多显示的列数
    row = widgetRect.height()/(imageHeight*zoomnum)+3;           //计算每行最多显示的行数

    int  imageLen = unitImageLen*zoomnum;
    imageWidth = imageHeight = imageLen;
    /* if(imageLen*row<widgetRect.height())  对高度进行放大
            imageHeight = widgetRect.height()/row; */

    // 填充背景图片
    painter->drawPixmap(0,0,widgetRect.width(),widgetRect.height(),pixmap);

    // 海图绘制
    QRect *qrecttemp;
    QString imagename; // 图片的名称（含相对路径）

    QString temp;
    for(int i=0;i<row;i++)
        for(int j=0;j<col;j++)
        {                                                      //计算每张图片的边长，zoomnum为放大倍数
            qrecttemp = new QRect(j*imageWidth+offset_x,i*imageHeight+offset_y,imageWidth,imageHeight); // 求解每个图片绘制的区域
            if(j+pic_x>=0)                                         //判断列数的范围
            {
                temp =QString("%1\\Zoom_%2\\Tile_%3x%4.Png").arg(zoom).arg(zoom).arg((j+pic_x)%Maxnum).arg(i+pic_y);
                imagename = temp;
            }
            else
            {
                 int pic_temp;
                 pic_temp = (j+pic_x)%Maxnum;
                 if(pic_temp == 0)
                       pic_temp = -Maxnum;
                 temp=QString("%1\\Zoom_%2\\Tile_%3x%4.png").arg(zoom).arg(zoom).arg(Maxnum+pic_temp).arg(i+pic_y);  //列数小于总列数，循环从num-1出算起
                 imagename = temp;
             }
           imagename = mapPath + imagename;
           // 翻译成中文
           QPixmap Seapixmap(imagename.toLocal8Bit());
           painter->drawPixmap(*qrecttemp,Seapixmap);
           delete qrecttemp;
        }
}

/*
 * 显示地球经纬坐标系
 *painter:窗口绘图句柄
 *widgetRect:绘制区域
 */
void GoogleMap::DisplayCoord(QPainter *painter,QRect widgetRect)
{
    int windowWidth = widgetRect.width();
    int windowHeight = widgetRect.height();
    // 绘制等纬线
    for(int i =CoordYInterval;i<windowHeight;i+=CoordYInterval)
    {
        painter->setPen(QPen(QColor(61,61,61,140)));
        painter->drawLine(0,i,windowWidth,i);
        // 获得当前点的精度
        double lon,lat;
        GetLngLatfromScreenNo(lon,lat,0,i);
        int tmp = (int)(lat*100);
        QString str = QString::number((float)tmp/100,'g');
        str += "°";
        //painter->setPen(Qt::black);
        painter->drawText(0,i,str);
        painter->drawText(windowWidth-50,i,str);

    }
    // 绘制等经线
    for(int i=CoordXInterval;i<windowWidth;i+=CoordXInterval)
    {
        //painter->setPen(QPen(QColor(61,61,61,140)));
        painter->drawLine(i,0,i,windowHeight);
        // 获得当前点的精度
        double lon,lat;
        GetLngLatfromScreenNo(lon,lat,i,0);
        int tmp = (int)(lon*100);
        QString str = QString::number((float)tmp/100,'g');
        str += "°";
       // painter->setPen(Qt::black);
        painter->drawText(i,windowHeight-10,str);
        painter->drawText(i,10,str);
    }
}

/*
*初始化map类的属性，pic_left左上角显示图片的列号等效经度，pic_top左上角显示图片的行号等效纬度
*/
void GoogleMap::Updateparameters(double pic_left,double pic_top)
{
    int num = pow((double)4,(double)zoom);
    num = sqrt((double)num);                   //计算出该zoom等级下每行每列的图片数
    double lngstep,latstep;
    lngstep = (LNGBOTTOM-LNGTOP)/num;          //每张图片占据的等效经度
    latstep = (LATTOP-LATBOTTOM)/num;          //每张图片占据的等效纬度        由于google map采用默卡托投影，所以等效经度就是经度，且均匀分布，等效纬度与纬度这件存在映射关系，等效纬度均匀分布，纬度不均匀分布
    pic_x = (pic_left-LNGTOP)/lngstep;         //计算该位置图片的列号，	取整
    pic_y = ( LATTOP-pic_top)/latstep;         //计算该位置图片的行号，取整

    offset_x = -((pic_left - LNGTOP) - pic_x*lngstep)/lngstep*(unitImageLen*zoomnum);    //计算图片列号后，小数部分化为整数作为x轴偏移量
    offset_y = -(( LATTOP-pic_top)-pic_y*latstep)/latstep*(unitImageLen*zoomnum);        //计算图片行号后，小数部分化为整数作为y轴偏移量
    if(offset_x>0)             //如果x轴偏移量大于0，则将其转化为负数，并且图片列号减一  ，允许pic_x为负数，使图片在显示时具有水平方向首位相接功能
    {
        pic_x--;
        offset_x -=(unitImageLen*zoomnum);
    }
    if((offset_y>0)&&(pic_y>0))   //如果y轴偏移量大于0，且图片行号大于0，将y轴偏移量转化为负数，且图片行号减一
    {
        pic_y--;
        offset_y -=(unitImageLen*zoomnum);
    }
}

/*
*  speci:更新地图的放大等级
*  para:
*      zoomnum:最新的放到倍数
*      pic_left左上角显示图片的列号等效经度；
*      pic_top左上角显示图片的行号等效纬度；
*      pic_right和pic_bottom暂不使用
*/
void  GoogleMap::setZoomNum(int zo,double pic_left,double pic_top,double pic_right  ,double pic_bottom)
{
    zoom = zo;
    leftLon = pic_left;
    topLat = pic_top;
    right = pic_right;
    bottom = pic_bottom;
    oldleftLon = leftLon;
    oldtopLat = topLat;
    if(leftLon<LNGTOP)
    {
        leftLon +=LNGBOTTOM -LNGTOP;
    }
    if(leftLon>LNGBOTTOM)
    {
        leftLon -=LNGBOTTOM -LNGTOP;
    }
    Updateparameters(leftLon,topLat);
}

/*
*   地图发生偏移处理事件
*   para:
*       point:为鼠标当前指向的点，单位为像素点
*/
void GoogleMap::addoffset(QPoint point)
{
    double lng1,lng2,lat1,lat2;
    int i_temp1 = point.x() - startPoint.x();
    int i_temp2 = point.y() - startPoint.y();

    GetLngLatfromScreenNoInner(lng2,lat2,(double)i_temp1, (double)i_temp2);     //获得等效经纬度坐标
    GetLngLatfromScreenNoInner(lng1,lat1, 0, 0);
    leftLon = lng1-lng2+oldleftLon;                      //计算出左上角新的等效经度
    topLat = lat1-lat2+oldtopLat;                        //计算出左上角新的等效纬度
    if(leftLon<LNGTOP)                                //经度范围控制
    {
        leftLon +=LNGBOTTOM -LNGTOP;
    }
    if(leftLon>LNGBOTTOM)                             //经度范围控制
    {
        leftLon -=LNGBOTTOM -LNGTOP;
    }
    Updateparameters(leftLon,topLat);                     //经度范围是在LNGTOP到LNGBOTTOM之间，纬度会超出LATTOP到LATBOTTOM这个范围。

}

/*
* 根据屏幕坐标点得到等效经纬度坐标，没有考虑首位相接 内部计算使用
*para:
*     lgn[out]:获得的等效经度
*     lat[out]:获得的等效维度
*     x[in]: 输入X轴坐标
*     y[in]: 输入y轴坐标
*/
void GoogleMap::GetLngLatfromScreenNoInner(double &lng,double &lat,double x,double y)
{
    double scx,scy;
    scx = pic_x*(unitImageLen*zoomnum)-offset_x+x;
    scy = pic_y*(unitImageLen*zoomnum)-offset_y+y;
    double lngstep,latstep;
    int num = pow((double)4,(double)zoom);
    num = sqrt((double)num);
    lngstep = (LNGBOTTOM-LNGTOP)/num;
    latstep = (LATTOP-LATBOTTOM)/num;
    lng = scx/(unitImageLen*zoomnum)*lngstep+LNGTOP;
    lat = LATTOP-scy/(unitImageLen*zoomnum)*latstep;
}

/*
*从屏幕坐标获得等效经纬度坐标
*x,y为绘图设备的屏幕坐标
*该函数可以直接交由最后一级函数调用计算出真正的经纬度
*/
void GoogleMap::GetLngLatfromScreenNo(double &lng,double &lat,double x,double y)
{
    double lngtemp,temp;
    int i;
    GetLngLatfromScreenNoInner(lngtemp,lat,x,y);
    if(lngtemp>180.0)
    {
        i = lngtemp/180;
        if(i%2)
        {
           temp = lngtemp-i*180;
           lngtemp = temp-180;
        }
        else
        {
            temp = lngtemp-i*180;
            lngtemp = temp;
        }
    }
    if(lngtemp<-180)
    {
        temp = lngtemp-((int)(lngtemp/180))*180;
        lngtemp = 180+temp;
    }
    lng = lngtemp;
    lat = (atan(exp(lat))-PI/4)*2*180/PI;
}

/*
*  根据等效经纬度得到屏幕坐标，没有考虑首位相接
* para:
*     lgn[in]:获得的等效经度
*     lat[in]:获得的等效维度
*     x[out]: 输入X轴坐标
*     y[out]: 输入y轴坐标
*/
void GoogleMap::GetPointfromLngLatNoInner(double lng,double lat,double &x,double &y)
{
    double pix,piy;
    double lngstep,latstep;
    int num = pow((double)4,(double)zoom);
    num = sqrt((double)num);
    lngstep = (LNGBOTTOM-LNGTOP)/num;
    latstep = (LATTOP-LATBOTTOM)/num;
    pix = (lng-LNGTOP)/lngstep;
    piy = ( LATTOP-lat)/latstep;
    pix *=(256*zoomnum);
    piy *=(256*zoomnum);
    pix += (lng-LNGTOP-(lng-LNGTOP)/lngstep*lngstep)/lngstep*(256*zoomnum);
    piy += (LATTOP-lat-( LATTOP-lat)/latstep*latstep)/latstep*(256*zoomnum);
    x = pix-pic_x*(256*zoomnum)+offset_x;
    y = piy-pic_y*(256*zoomnum)+offset_y;

}

/*
    真实等效经纬度导屏幕坐标的转换
*/
void GoogleMap::GetPointfromLngLatNo(double lng,double lat,double &x,double &y)
{
    lat = log(tan(PI/4 + lat*3.14/360));
    double pix,piy;
    double lngstep,latstep;
    int num = pow((double)4,(double)zoom);
    num = sqrt((double)num);
    lngstep = (LNGBOTTOM-LNGTOP)/num;
    latstep = (LATTOP-LATBOTTOM)/num;
    pix = (lng-LNGTOP)/lngstep;
    piy = ( LATTOP-lat)/latstep;
    pix *=(256*zoomnum);
    piy *=(256*zoomnum);
    pix += (lng-LNGTOP-(lng-LNGTOP)/lngstep*lngstep)/lngstep*(256*zoomnum);
    piy += (LATTOP-lat-( LATTOP-lat)/latstep*latstep)/latstep*(256*zoomnum);
    x = pix-pic_x*(256*zoomnum)+offset_x;
    int temp = x;
    while(x<targetwidget->geometry().width())
    {
        // 当获得的坐标<0时进行调整
        temp += 256*zoomnum*num;
        if(temp>=targetwidget->geometry().width())
              break;
        x = temp;
    }
    y = piy-pic_y*(256*zoomnum)+offset_y;
}

/*
* 以pt为中心对地图进行放大/缩小
* para:
*    pt:中心点
*    size:放到缩小的倍数
*/
void GoogleMap::zoompoint(QPoint pt,double size)
{
    double x,y,lng,lat;
    double truesize = size;
    if(zoomnum*size>2.0)
    {
        truesize = 2/zoomnum;
    }
    // 选择该点侧面的某点作为左上角的顶点
    x = (truesize-1)/truesize*pt.x();
    y = (truesize-1)/truesize*pt.y();
    GetLngLatfromScreenNoInner(lng,lat, x, y);
    zoomInOut(lng,lat,size);
}

/*
*  缩放为原来的size倍,将左上角经纬度坐标设置为lng、lat
*/
void GoogleMap::zoomInOut(double lng,double lat,double size)
{
    double zoomnumtemp = zoomnum;
   // zoomnumtemp *=size;
    int temp = 0;
    if(zoom>=3&&zoom<=5){                 //设置卫星轨迹的宽度
        temp = 30;
    }else if(zoom>5&&zoom<=6){
        temp = 40;
    }else{
        temp = 50;
    }
    if(size>1.0)           //放大时的操作
    {
        zoomnumtemp += 0.5;
        if(zoom<=MapmaxGrade) // 当前还未放大到最大陪数
        {
            if(zoomnumtemp>=2) // 满足放大条件，放大一级
            {
                if(zoom+1>MapmaxGrade){  //禁止其继续放大
                    return;
                }
                zoom++;
                Utility::moonDValue = Utility::moonDValue +temp;
                zoomnumtemp =1;
            }
        }
        else
        {
            zoomnumtemp=1;
            setZoomNum(zoom,leftLon,topLat,0,0);
            return ;
        }
    }
    else     //缩小时的操作
    {
        zoomnumtemp *=size;
        if(zoom==3){   //禁止其继续缩小
            return;
        }
        if((zoomnumtemp<1.0) && (zoom>3)) // 满足缩小条件，缩小一级
        {
            zoom--;
            Utility::moonDValue = (Utility::moonDValue - temp)<0?5:(Utility::moonDValue -temp);
            zoomnumtemp = zoomnum*size/0.5; // 变为下一级zoom的放大陪数
        }
        if(zoom == 1 && zoomnumtemp<1.0)
        {
            return ;
        }
    }
    if(zoomnum==2 )
        return;
    zoomnum  = zoomnumtemp+0.1;
    setZoomNum(zoom,lng,lat,0,0);
    Utility::current_map_zoom = zoom;
}

// 设置鼠标按下时的的坐标
void GoogleMap::SetLButtonPoint(QPoint point)
{
    startPoint = point;
}


// 更新地图左上角的经纬度
void GoogleMap::UpdateNewTopPoint()
{
    oldleftLon = leftLon;
    oldtopLat = topLat;
}

} //namespace Core
