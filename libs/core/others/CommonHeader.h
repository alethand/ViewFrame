#ifndef COMMONHEADER_H
#define COMMONHEADER_H

#include <QVariant>
#include <QList>

//用于填充数据访问返回值
typedef  QList<QVariant> VariantList;
typedef  QList<VariantList> DataList;

/*******定义宏变量，方便统一修改**********/
#define PI  3.1415926
#define scaleNum  1.414213                         //根号2
#define KNOT2KM 1.852                              //节和KM之间的转换比例
#define MyDValue(a,b)    (((a-b<0)?(b-a):(a-b)))   //自定义求两者之间的差

#define HEIGHT_SCRALE   0.08                     //标题栏高度与屏幕高度的比例

#define SUB_WIDGET_WIDTH    0.45                 //报警信息/航道信息等二级菜单宽度和屏幕宽度的比例
#define SUB_WIDGET_HEIGHT   0.4                  //报警信息/航道信息等二级菜单高度和屏幕高度的比例
#define TITLE_WIDGET_WIDTH  0.6                  //标题栏上按钮集合的宽度相对于屏幕的宽度比例
#define STATUS_HEIGHT_SCALE 0.25                 //状态栏相对于屏幕的比例

#define RIGHT_WIDGHT_HEIGHT_SCALE 0.2            //右侧菜单栏竖直位置相对于屏幕的比例
#define RIGHT_WIDGHT_HEIGHT 0.6                  //右侧菜单栏高度相对于屏幕高度的比例
#define RIGHT_WIDGHT_WIDTH  90                   //右侧菜单栏宽度
#define RIGHT_HEIGHT_MARIG 5                     //右侧菜单栏距离标题栏的距离

#define LEFT_WIDGHT_HEIGHT 0.64                  //左侧菜单栏高度相对于屏幕高度的比例
#define LEFT_WIDGHT_WIDTH  0.18                  //左侧菜单栏宽度相对于屏幕宽度的比例
#define LEFT_HEIGHT_MARIG 5                      //左侧菜单栏距离标题栏的距离

#define SUBSTATUS_WIDTH_SCALE  0.17              //潜艇状态栏宽度和屏幕宽度比例
#define SUBSTATUS_HEIGHT_SCALE 100               //潜艇状态栏高度

#define TABLE_ROW_HEIGHT  30
#define BUTT_WIDTH_HEIGHT 40                     //

#include <QString>
#include <QColor>

// 实体基本属性信息
struct EntityInfo
{
    int id;             //数据库编号
    double lon;         // 经度
    double lat;         // 纬度
    float  fcm;         // 航向
    float  fvm;         // 航速(节为单位)
    float  depth;       // 深度
    float height;       //高度
    int camp;           //阵营
    int kind;           //0:潜艇、1:水面舰、2:反潜机、3:渔船、4:水听站
    bool currentShip;   //是否为本艇
    float detectScope;  //监测范围
    QString type;       //用来保存实体的型号信息，如"金刚级"
    QString baseName;   //潜艇基地的名称
};

typedef QList<EntityInfo> EntityList;

//潜艇属性
struct SubInfo
{
     int id;                  //数据库编号
     QString  name;           //潜艇型号
     QString  nation;         //所在国家
     int  passageer;          //荷载人数
     int aerostation;         //最低航速
     int submerge;            //水下最高航速
     int depth;               //最大下潜深度
     QString  image;          //图片名
     QStringList searchMethod;  //探测手段
     QStringList attackMethod;  //攻击手段
};

typedef QList<SubInfo> SubInfoList;

//水面舰属性
struct WaterShipInfo{
    int id;                  //数据库编号
    QString name;            //水面舰型号
    QString nation;          //所在国家
    QStringList radar;       //雷达
    QStringList sonar;       //声纳
    QString  plane;          //舰载机
    QString  image;          //图片名
};

typedef QList<WaterShipInfo> WaterShipInfoList;

//反潜机属性
struct ASWPlaneInfo{
    int id;                   //数据库编号
    QString  name;            //反潜机型号
    QString  nation;          //所在国家
    QStringList  radar;       //雷达
    QStringList  infrared;    //红外
    QStringList  sonar;       //声纳
    QStringList  magnetic;    //磁探测仪
    QString  image;           //图片名
};

typedef QList<ASWPlaneInfo> ASWPlaneInfoList;

//渔船属性
struct FisherInfo{
    int id;             //数据库编号
    QString  name;            //反潜机型号
    QString  nation;          //所在国家
    QString  image;          //图片名
};

typedef QList<FisherInfo> FisherInfoList;

//水听站属性
struct WaterListenInfo{

};

//潜艇特有的一些属性，如声，磁等
struct SubmarineInfo
{
    double voice_val;    //每个场的值
    double ci_val;
    double hongwai_val;
    double sar_val;
    double kjg_val;
    double voice_chance;  //每个场被探测的概率
    double ci_chance;
    double hongwai_chance;
    double sar_chance;
    double kjg_chance;
    double totalSearch_chance;
};

//报警旗帜警示基本信息
struct FlagBaseInfo
{
    double lon;           // 经度
    double lat;           // 维度
    QString warningInfo;  //报警信息
    int currentIndex;     //使用历史记录之后，其所对应表格中的行值
};

//航道图钉点的基本信息
struct ChannelInfo{
    double lon;           // 经度
    double lat;           // 维度
    QColor color;         //航道点与前一点之间连线的颜色
};

//测距状态下每个点的位置信息
struct DistancePointInfo{
    double lon;           // 经度
    double lat;           // 维度
};

//航段之间安全信息
struct ChannelSafeInfo{
     double lonStart;     //危险段起始经度
     double latStart;     //危险段起始纬度
     double lonEnd;       //危险段结束经度
     double latEnd;       //危险段结束纬度
     QString name;          //舰只的类型名称
     QString searchMethod;  //探测手段
     QString advice;        //建议信息
};

//反潜机巡逻区域每个点的位置信息
struct AreaPoint{
    double lon;
    double lat;
};

//反潜机巡逻区域的基本信息
struct AreaInfo
{
    int id;
    QColor color;
    QList<AreaPoint> points;
};

typedef QList<AreaInfo> AreaList;

//图钉点的宽度和高度
#define CHANNEL_POINT_WIDTH  30
#define CHANNEL_POINT_HEIGHT 30

//临时潜艇基地的宽度和高度
#define BASE_WIDTH 60
#define BASE_HEIGHT 60

//潜艇基地宽度和高度
#define UNDERSHIP_BASE_WIDTH 80
#define UNDERSHIP_BASE_HEIGHT 85

#define CAMP_SELF 100
#define CAMP_ENEMY 101
#define CAMP_NEUTRAL 102


//此值乘以当前地图缩放的等级，即可得到当前舰只显示的宽度和高度
#define CURRENT_MAP_ZOOM_WIDTH 5
#define CURRENT_MAP_ZOOM_HEIGHT 5

//定义报警旗帜的尺寸
#define FLAG_WIDTH 55
#define FLAG_HEIGHT 55


namespace RG
{
    //环境信息种类(威胁区/水听站/潜艇基地)
    enum EnvirType
    {
        INIT_AREA,
        INIT_LISTEN,
        INIT_SUBBASE
    };

    //添加军舰种类(无/水面舰/预警飞机/潜艇基地/水听站)
    enum ShipKind
    {
        NONE,
        UNDERWATERSHIP,
        WATERSHIP,
        FISHER,
        WARNING_AIRPLANE,
        SUB_BASE,
        WATER_LISTEN
    };

}

//添加其它类型(无/长方形/正方形/扇形/圆/扇框/弧形/标注....)
enum ItemKind
{
    ITEM_NONE,
    ITEM_RECT,
    ITEM_SQUARE,
    ITEM_PIE,
    ITEM_CIRCLE,
    ITEM_FAN,
    ITEM_ARC,
    ITEM_MARK,
    ITEM_POLYGON,

    ITEM_SPECIAL1,
    ITEM_SPECIAL2,
    ITEM_SPECIAL3,
    ITEM_SPECIAL4,
    ITEM_SPECIAL5,
    ITEM_SPECIAL6,
    ITEM_SPECIAL7,
    ITEM_SPECIAL8,
    ITEM_SPECIAL9,
    ITEM_SPECIAL10,
    ITEM_SPECIAL11,
    ITEM_SPECIAL12,
    ITEM_SPECIAL13,
    ITEM_SPECIAL14
};

//SeaForm页面添加初始化航道/潜艇基地/初始化水听站/反潜区域/测距点
enum InitKind
{
    INIT_NONE,
    INIT_CHANNEL,
    INIT_BASE,
    INIT_LISTEN,
    INIT_AREA,
    INIT_DISTANCE
};

//磁场数据格式
#define  LAT_NUM  91       //纬度个数
#define  LON_NUM  361      //经度个数

struct CC
{
  int lat;             //纬度
  int lon;             //经度
  double x_nT;
  double y_nT;
  double z_nT;
};

//水深数据
struct Sound
{
    double lat;       // 纬度
    double lon;       // 经度
    double depth;     // 深度
};

/***********用于和激励程序通信时的结构体*************/
struct UnderShipInfo
{
    double lon;         // 经度
    double lat;         // 纬度
    float  fcm;         // 航向
    float  fvm;         // 航速
    float  depth;       // 深度
};

/*
  *type为0时表示用于初始化本艇的位置信息
  *
  *
  */
struct SandData
{
   int type;
   union{
       UnderShipInfo info;
   }mess;
};

//潜艇航行状态 水面航行、潜望、当前潜深、50M、80M、200M
enum SubmarineState
{
    SHIPPING,
    PERISCOPE,
    CURRENTDEPTH,
    M_50,
    M_80,
    M_100
};

//数据显示表的行号和数据在数据库中index对应关系
struct RowToIndex
{
    int rowNum;
    int indexNum;
};

//悬浮按钮的样式
enum ArrowDirect
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

//音频加载的几种状态，播放/停止/暂停/缓冲/错误
enum MusicState
{
    PLAYING,
    STOP,
    PAUSED,
    BUFFERING,
    ERROR
};

//音频播放时的几种状态选项(播放/停止/暂停/下一首/上一首)
enum PlayState
{
    Play,
    Stop,
    Paused,
    Next,
    Previous
};

#endif // COMMONHEADER_H
