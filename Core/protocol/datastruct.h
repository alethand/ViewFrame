/*!
 *  @brief     类型定义
 *  @details   定义了通用的结构体、枚举等变量
 *  @file      datastruct.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.03.02
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

/****************************new DataStruct***********************************/
#include <QString>
#include <QList>
#include <QAbstractTableModel>
#include <QVariant>
#include <QSet>
#include <QDebug>
#include <QDataStream>
#include <QQueue>
#include <QRect>

#include "calculater/commonalgorithms.h"
using namespace Calculator;

namespace Protocol
{

    namespace Data
    {

        /*! 控制数据元*/
        struct Elem_Cntl{};
        struct Elem{
            QVariant data;          //数据
            ushort   numBytes;      //字节数
        };

        /*!
         * \brief The Elem_Core struct  核心数据元
         * \attention  多数据报文-核心数据 每一个数据都为1列
         */
        struct Elem_Core{

            /*!
             * \brief Elem::getName    获取当前数据的列名信息
             * \param index
             * \return
             */
          virtual  QString getName(int index) = 0;

          virtual int getCount() = 0;
          virtual QVariant getData(int index) = 0;
        };


        /*! 多数据报文 *///--即含有多个重复性结构的大数据报文


        struct HugeData_Gram{
            HugeData_Gram(){isReverse = false;}


            /*!
              * \brief isReverse  反转标识
              * \note 默认情况 coreData的个数为行，项中的个数为列个数--即项数为属性，list的子项个数随时间增长
              */


             bool isReverse;
             /// -----默认标识头为第一个数据
             Elem          markHead;  //标识头 e.g. 0x134F

             Elem_Cntl*         cntlData;  //控制类型数据
             QList<Elem_Core*>   coreData;  //核心数据

             QList<Elem>   end;        /* 结尾数据--可能有包尾，校验和等*/

             QString getHeadName(int index) const;

             int getRowCount() const ;

             int getColCount() const;

             QVariant getData(int row,int col) const;


        };


        class HugeData_Model:public QAbstractTableModel
        {
            Q_OBJECT
        public:
            explicit HugeData_Model(HugeData_Gram *data = NULL);
            void setDataSrc(HugeData_Gram *data){mdata = data;}          //设置数据源

            virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
            virtual int columnCount(const QModelIndex &parent) const;
            //inline virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
            virtual QVariant data(const QModelIndex &index, int role) const;
            virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;

            void openShowLatestMsg(int row){rowsLatestMsg = row;}        //显示最新消息
            void colseShowLatestMsg(){rowsLatestMsg = -1;}

            void openPointedColShow(int col){specifedCol = col;}   //添加指定列显示
            void closePointedColShow(){ specifedCol = -1;}
        public:
            HugeData_Gram *mdata;
            ushort rowsLatestMsg;           /*!< 需要显示的最新消息行数 */
            int specifedCol;        /*!< 指定可显示列         */
        };

    }
}


/////////////////////////////////////////////////////
#include <QString>
#include <QMap>
#include <QVector>
#include <QDateTime>

using namespace Protocol;


namespace Datastruct {

/*!
 *  @brief 系统支持的服务类型
 *  @details 用户以命令行方式输入对应的服务.
 */
enum ServiceType
{
    SERVICE_TEXT,               /*!< 使用传输文本服务,默认类型 */
    SERVICE_FILE                /*!< 使用传输文件服务 */
};

/*!
 *  @brief 数据库类型
 */
enum DatabaseType
{
    DB_NONE = 0,
    DB_MYSQL,               /*!< 使用MySql数据库*/
    DB_ORACLE               /*!< 使用Oracle数据库*/
};

/*!
 * @brief 解析结果
 */
enum ResultType
{
    PARSE_ERROR,            /*!< 发生错误*/
    VIEW_PROGRAM,           /*!< 查看程序信息*/
    EXEC_PROGRAM            /*!< 执行程序*/
};

struct CommandParameter
{
    CommandParameter()
    {
        serviceType = SERVICE_TEXT;
        dbType = DB_MYSQL;
        parseResult = PARSE_ERROR;
    }

    ServiceType serviceType;
    DatabaseType dbType;
    ResultType parseResult;
};


/*!
 *  @brief 数据库特征信息
 *  @note 保存当前使用的数据库中的功能特点
 */
struct DBFeature
{
    bool lastInsertId;          /*!< 是否支持查询最后插入数据的id */
    bool transactions;          /*!< 是否支持事务 */
};

/*!
 *  @brief 数据库配置信息
 */
struct DatabaseConfigInfo
{
    DatabaseType dbType;
    QString hostName;
    QString dbName;
    QString dbUser;
    QString dbPass;
    ushort port;
};

/*!
 *  @brief 系统配置信息
 */
struct SystemConfigInfo
{
    SystemConfigInfo():defaultKeySchemes(true),fullscreen(true),topHint(false),menubarVisible(false){
    }
    bool defaultKeySchemes;             /*!< 是否采用默认的快捷键设置，默认为true */
    bool fullscreen;                    /*!< 是否采用全屏显示 */
    bool topHint;                       /*!< 是否置顶显示 */
    bool menubarVisible;                /*!< 菜单栏是否显示 */
    QString userKeySchemesName;         /*!< 自定义快捷键设置名称 @see defaultKeySchemes 为false时有效 */
    QString locale;                     /*!< 显示的语言 */
    QString style;                      /*!< 默认样式 */
};


/*!
 *  @brief 数据源信息基本单元
 */
struct RadiationSource{
struct RadiationSourceBase
{
    ushort usStartCode;      /*!< 起始码，55AAH*/
    ushort usSourceNo;       /*!< 辐射源序号,递增*/
    uchar ucPulseType;       /*!< 载频,脉间类型,0 固定,1 脉间捷变,2 脉组捷变,3 分时分集,4 连续波,5 双频点,0xff:未知*/
    uchar ucIntraPulseType;  /*!< 载频,脉内类型,0 单载频 1 多载频 2 调频 0xff 未知*/
    uchar ucCarrierCount;    /*!< 载频,个数*/
    uchar ucContinuousWaveLabeling;     /*!< 载频,连续波标记,0 无效 1 有效*/
    ushort usPulseGroupInCount;         /*!< 载频,脉组内脉冲数*/
    uchar usFrequencyBandCode;          /*!< 载频,频段码*/
    double dRF1;                        /*!< 载频,1*/
    double dRF2;                        /*!< 载频,2*/
    double dRF3;                        /*!< 载频,3*/
    double dRF4;                        /*!< 载频,4*/
    double dRF5;                        /*!< 载频,5*/
    double dRF6;                        /*!< 载频,6*/
    double dRF7;                        /*!< 载频,7*/
    double dRF8;                        /*!< 载频,8,单位,MHz*/
    ushort usMultiPluseType;            /*!< 重频,类型,0 固定,1 抖动,2 滑变,3 成组参差,4 固定参差*/
    ushort usMultiPluseCount;           /*!< 重频,个数*/
    uint ulMultiPulseGroupInCount;      /*!< 重频,脉组内脉冲数*/
    double dPRI1;                       /*!< 重频,周期1*/
    double dPRI2;                       /*!< 重频,周期2*/
    double dPRI3;                       /*!< 重频,周期3*/
    double dPRI4;                       /*!< 重频,周期4*/
    double dPRI5;                       /*!< 重频,周期5*/
    double dPRI6;                       /*!< 重频,周期6*/
    double dPRI7;                       /*!< 重频,周期7*/
    double dPRI8;                       /*!< 重频,周期8,单位:ns*/
    ushort usPluseWidthType;            /*!< 脉宽,类型,0 固定,1 变化,0xff 未知*/
    ushort usPluseWidthCount;           /*!< 脉宽,个数,≤8*/
    uint ulPluseWidthGroupInCount;      /*!< 脉宽,脉组内脉冲数*/
    double dPW1;                        /*!< 脉宽,1*/
    double dPW2;                        /*!< 脉宽,2*/
    double dPW3;                        /*!< 脉宽,3*/
    double dPW4;                        /*!< 脉宽,4*/
    double dPW5;                        /*!< 脉宽,5*/
    double dPW6;                        /*!< 脉宽,6*/
    double dPW7;                        /*!< 脉宽,7*/
    double dPW8;                        /*!< 脉宽,8,单位:ns*/
    uchar ucDigitalPA;                  /*!< 数字幅度,0-255*/
    uchar ucAnalogPA;                   /*!< 模拟幅度,0-255*/
    double dDigitalPower;               /*!< 数字功率,单位:dBm,大于9999无效*/
    double dAnalogPower;                /*!< 模拟功率,单位:dBm,大于9999无效*/
    double dAziAngle;                   /*!< 测量信息,方位角*/
    double dEleAngle;                   /*!< 测量信息,俯仰角*/
    double dLon;                        /*!< 定位结果,经度*/
    double dLat;                        /*!< 定位结果,纬度*/
    double dHight;                      /*!< 定位结果，高度*/
    uint ulIntraPulseEffFlag;           /*!< 脉内调制信息,脉内有效标识,1 有效,0 无效*/
    uchar ucIntraPulseInfo[32];         /*!< 脉内调制信息,脉内特征信息*/
    ushort usCRC;                       /*!< CRC校验*/
    ushort usEndCode;                   /*!< 终止码*/
};

};

enum RadiationSourceHead{
    T_No = 0,               /*!< 序号 */
    T_SourceNo,       /*!< 辐射源序号,递增*/
    T_PulseType,       /*!< 载频,脉间类型,0 固定,1 脉间捷变,2 脉组捷变,3 分时分集,4 连续波,5 双频点,0xff:未知*/
    T_IntraPulseType,  /*!< 载频,脉内类型,0 单载频 1 多载频 2 调频 0xff 未知*/
    T_CarrierCount,    /*!< 载频,个数*/
    T_ContinuousWaveLabeling,     /*!< 载频,连续波标记,0 无效 1 有效*/
    T_PulseGroupInCount,         /*!< 载频,脉组内脉冲数*/
    T_FrequencyBandCode,          /*!< 载频,频段码*/
    T_RF1,                        /*!< 载频,1*/
    T_RF2,                        /*!< 载频,2*/
    T_RF3,                        /*!< 载频,3*/
    T_RF4,                        /*!< 载频,4*/
    T_RF5,                        /*!< 载频,5*/
    T_RF6,                        /*!< 载频,6*/
    T_RF7,                        /*!< 载频,7*/
    T_RF8,                        /*!< 载频,8,单位,MHz*/
    T_MultiPluseType,            /*!< 重频,类型,0 固定,1 抖动,2 滑变,3 成组参差,4 固定参差*/
    T_MultiPluseCount,           /*!< 重频,个数*/
    T_MultiPulseGroupInCount,      /*!< 重频,脉组内脉冲数*/
    T_PRI1,                       /*!< 重频,周期1*/
    T_PRI2,                       /*!< 重频,周期2*/
    T_PRI3,                       /*!< 重频,周期3*/
    T_PRI4,                       /*!< 重频,周期4*/
    T_PRI5,                       /*!< 重频,周期5*/
    T_PRI6,                       /*!< 重频,周期6*/
    T_PRI7,                       /*!< 重频,周期7*/
    T_PRI8,                       /*!< 重频,周期8,单位:ns*/
    T_PluseWidthType,            /*!< 脉宽,类型,0 固定,1 变化,0xff 未知*/
    T_PluseWidthCount,           /*!< 脉宽,个数,≤8*/
    T_PluseWidthGroupInCount,      /*!< 脉宽,脉组内脉冲数*/
    T_PW1,                        /*!< 脉宽,1*/
    T_PW2,                        /*!< 脉宽,2*/
    T_PW3,                        /*!< 脉宽,3*/
    T_PW4,                        /*!< 脉宽,4*/
    T_PW5,                        /*!< 脉宽,5*/
    T_PW6,                        /*!< 脉宽,6*/
    T_PW7,                        /*!< 脉宽,7*/
    T_PW8,                        /*!< 脉宽,8,单位:ns*/
    T_DigitalPA,                  /*!< 数字幅度,0-255*/
    T_AnalogPA,                   /*!< 模拟幅度,0-255*/
    T_DigitalPower,               /*!< 数字功率,单位:dBm,大于9999无效*/
    T_AnalogPower,                /*!< 模拟功率,单位:dBm,大于9999无效*/
    T_AziAngle,                   /*!< 测量信息,方位角*/
    T_EleAngle,                   /*!< 测量信息,俯仰角*/
    T_Lon,                        /*!< 定位结果,经度*/
    T_Lat,                        /*!< 定位结果,纬度*/
    T_Hight,                      /*!< 定位结果，高度*/
    T_ntraPulseEffFlag,           /*!< 脉内调制信息,脉内有效标识,1 有效,0 无效*/
    T_IntraPulseInfo,         /*!< 脉内调制信息,脉内特征信息*/
    T_CRC,                       /*!< CRC校验*/
    T_Counter,                       /*!< 次数*/
    T_Timer                       /*!< 时间*/
};

/*!
 *  @brief 数据源信息覆盖记录信息
 */
struct RadiationSourceRenovate
{
    RadiationSource::RadiationSourceBase rsInfo;     /*!< 数据源信息>*/
    int iHoldCount;                 /*!< 截获次数>*/
    QString strHoldTime;            /*!< 截获时间>*/
    int iInsertRow;                 /*!< 所在行>*/
};

typedef QList<RadiationSourceRenovate> RSDataList;
typedef QMap<int,RadiationSourceRenovate> RSDataMap;  /*!< 数据源信息 key：数据源批号 value:数据源的具体信息*/

/*!
 * @brief 数据显示模块消息类型
 */
enum DataDisplayMessageType
{
    MESSAGE_RADIASOURCE,            /*!< 数据源信息*/
    MESSAGE_ALLPLUSE                /*!< 全脉冲信息*/
};


/*!
 * @brief 数据显示模块表格显示类型
 */
enum DataDisplayTableType
{
    TABLE_RADIASOURCE,              /*!< 数据源表格*/
    TABLE_ALLPLUSE,                 /*!< 全脉冲表格*/
    TABLE_MFACQUISITION,            /*!< 中频表格*/
    MAP_RADIASOURCE,                /*!< 数据源地图*/
    GRAPHICSE_ALLPLUSE,             /*!< 全脉冲图形*/
    GRAPHICSE_MFACQUISITION,        /*!< 中频数据图形*/
    GRAPHICSE_SPECTRUM              /*!< 频谱数据图形*/
};

/*!
 *  @brief 数据源刷新方式
 */
enum DataRefreshModel
{
    SCROLL_RENOVATE = 1,            /*!< 滚动刷新 */
    COVER_RENOVATE                  /*!< 覆盖刷新 */
};

/*!
 *  @brief tableviewmodelcustom下使用的表格显示类型
 */
enum TableCustomKind
{
    STATISTICAL_INFO=1,             /*!< 统计信息 */
    ORIGINAL_INFO,                  /*!< 原始数据 */
    MF_ACQUISITION_INFO             /*!< 中频数据 */
};


/*!
 *  @brief 全脉冲信息
 */
struct AllPluseInfo
{

    struct OriginData{/*! 原始数据*/

//        struct Cntl :public Data::Elem_Cntl{
//            struct Struct_Repeat{
//                char name[64];                      //属性名称
//                ulong ifDrawPic;                    //是否绘图
//            };

//            ulong ifValid;                      //是否有效
//            ulong count;                        //数据个数
//            QList<Struct_Repeat> details;       //重复性数据细节
//        };

        struct Core:public Data::Elem_Core
        {

            QString        name;                //属性名
            ulong          ifDrawPic;           //是否绘图
            QList<double>  valuelist;          //属性值

            virtual  QString getName(int index)
            {
                Q_UNUSED(index)
                if(name.isEmpty())
                    return QStringLiteral("属性");
                return name;
            }

            virtual int getCount(){
                return 5;//valuelist.count();
            }
            virtual QVariant getData(int index);
        };

    };

    struct Statistic{/*! 统计参数信息*/
        struct Core:public Data::Elem_Core
        {
            QString name;                   /*! 属性名称  */
            float maxVal;                   /*! 最大值   */
            float minVal;                   /*! 最小值   */
            float meanVal;                  /*! 平均值   */
            float variance;                 /*! 方差     */

            Core()
            {
                name.resize(64);
            }
            virtual  QString getName(int index)
            {
                switch(index)
                {
                case 0:return QStringLiteral("最大值");
                case 1:return QStringLiteral("最小值");
                case 2:return QStringLiteral("平均值");
                case 3:return QStringLiteral("方差");
                default:return QString();
                }

            }

            virtual int getCount(){
                return 4;
            }
            virtual QVariant getData(int index);
        };
    };
};


/*!
 *  @brief 中频采集数据(表格显示使用，非协议格式)
 */
struct MidFreqInfo
{
    /*!
     *  @brief 中频采集数据列
     */
    enum MFAcquisitionHead
    {
        T_AcqTime = 0,                                       /*!< 采集时间 - 当前数据包获取时间*/
        T_AcqModel,                                         /*!< 采集模式 -- 数据包中字段*/
        T_AcqNum,                                           /*!< 脉冲采集个数 -- 包中blk的个数*/
        T_AcqDotNum                                         /*!< 采集点数 -- 包中所有blk的AD数据个数之和*/
    };

  /*****************yww的结构 **************/
    struct Collect{
        enum Type{
            VP_Trigger = 0xCF01,    //VP触发采
            Blind = 0xCF02,         //盲采
            NoCollect = 0xCF00          //不采集
        };
    };

    struct Core:public Data::Elem_Core
    {
        struct OneMsg{
             QTime arriveTime;           //到达时间
             QList<ushort> adData;       //ad数据
        };


        Collect::Type acqMode;  //采集模式
        QDateTime acqTime;           //采集时间
        int   iAcqNum;           //采集个数
        QList<OneMsg> listInfo;

        virtual  QString getName(int index)
        {
            switch((MFAcquisitionHead)index)
            {
            case T_AcqTime:  return QStringLiteral("采集时间");
            case T_AcqModel: return QStringLiteral("采集模式");
            case T_AcqNum:   return QStringLiteral("采集脉冲个数");
            case T_AcqDotNum:return QStringLiteral("采集点数");
            default:return QString();
            }

        }

        virtual int getCount(){
            return 4;
        }
        virtual QVariant getData(int index);
    };
};


/*****************新框架*********************/

struct ProtocolArray{
    int protocolType;
    QByteArray data;
};

typedef QQueue<ProtocolArray> ProtocolQueue;

/*!
 *  @brief 控件类型
 */
enum ControlType{
    Empty = 0,
    /*! 输入框 */
    ComboBox = 1,        /*! 下拉框 */
    CheckBox,            /*! 复选框 */
    RadioButton,         /*! 单选框 */
    TextEdit,            /*! 文本输入框 */
    ValueIntEdit,        /*! 整形输入框 */
    ValueFloatEdit,      /*! 浮点形输入框 */
    DateEdit,            /*! 日期输入框 */
    TimeEdit,            /*! 时间输入框 */

    /*! 容器框 */
    Dialog,              /*! 对话框 */
    Widget,              /*! 普通框 */
    Groupbox,            /*! */
    Table,               /*! 表格 */
    List                 /*! 列表 */
};

struct PubHead{
    PubHead():widget(NULL){}
    virtual ~PubHead(){}
    QWidget  * widget;     /*!< 实际生成的控件 */
    ControlType  type;     /*!< 字段类型 */
};

/*!
 * @brief 数据范围
 * @details 根据Field的type来转换为对应的值，可转换的包括int、double、date等
 */
struct DataRange{
    QString minValue;
    QString maxValue;
};

/*!
 *  @brief 位数据信息描述
 *  @details
 */
struct BitData{

    friend QDataStream & operator<< (QDataStream & stream,const BitData & field);
    friend QDataStream & operator>> (QDataStream & stream,BitData & field);

    QString  name;        /*!< 控件显示信息 */
    ControlType  type;    /*!< 字段类型 */
    int index;            /*!< 字段唯一标识符，初始为0。新添加字段后，此值+1 */

    int startPos;         /*!< 起始位 */
    int last;             /*!< 持续位数 */
    bool enable;          /*!< 控件是否可用 */
    bool visible;         /*!< 是否可见 */
    float weight;         /*!< 权值 */
    float precision;      /*!< 精度 */
    QString unit;         /*!< 单位 */
    QString displayText;  /*!< 显示文本 */
    QVariant defaultValue;/*!< 默认值 */
    QVariant value;       /*!< 控件中显示值 */
    QStringList list;     /*!< type为ComboBox时，保存下拉框中子项名称 */
};
typedef QList<BitData> BitList;

/*!
 *  @brief 位字段
 *  @details
 */
struct BitField{

    friend QDataStream & operator<< (QDataStream & stream,const BitField & field);
    friend QDataStream & operator>> (QDataStream & stream,BitField & field);

    BitList bitList;        /*!< 位字段集合 */
};

/*!
 *  @brief 单个字段内容数据
 *  @warning (字节数-符号位) 与 (位操作集合) 这两对值不能同时使用，属于互斥关系
 */
struct FieldData{
public:
    FieldData(){
        bytes =0;isSigned = false;
        weight = 1;
        precision = 1;
        enable = true;
        visible = true;
        bitOperator = false;
    }

    friend QDataStream & operator<<(QDataStream & stream,const FieldData & data);
    friend QDataStream & operator>>(QDataStream & stream,FieldData & data);

    QString  name;        /*!< 控件显示信息 */
    ControlType  type;    /*!< 字段类型 */

    int index;            /*!< 字段唯一标识符，初始为0。无需从xml中解析，新添加字段后，此值+1 */
    bool bitOperator;     /*!< 位操作字段，当bits中不为0，此值为true */

    ushort bytes;         /*!< 字节数 */
    bool   isSigned;      /*!< 有无符号位 */

    BitField bits;        /*!< 位操作集合 */

    bool enable;          /*!< 控件是否可用 */
    bool visible;         /*!< 是否可见，可控制此字段是否显示在表格等控件上 */
    float weight;         /*!< 权值 */
    float precision;      /*!< 精度 */
    int repeat;           /*!< 重复的次数，此字段之后连续出现的次数 */
    QString unit;         /*!< 单位 */
    DataRange range;      /*!< 数值范围 */
    QString displayText;  /*!< 显示文本 */
    QString defaultValue; /*!< 默认值 */
    QVariant value;       /*!< 控件中显示值 */
    QStringList list;     /*!< type为ComboBox时，保存下拉框中子项名称 */
};

/*!
 *  @brief  协议字段信息
 *  @details 协议每项的信息描述
 */
struct Field : public PubHead{
    Field():PubHead(){}
    ~Field(){}

    FieldData data;        /*!< 字段属性 */
};

struct ByteValue{
    QVariant value;
};
typedef QList<ByteValue> ByteValues;

struct FieldValue{
    FieldValue():index(0),protocolIndex(0),valList(NULL){}
    int protocolIndex;          /*!< 所属协议在所有协议中索引 */
    int index;                  /*!< 同一协议中字段索引 */
    ByteValue signalValue;      /*!< 适用于单个字段值 */
    ByteValues * valList;       /*!< 适用于同一个字段被重复多次;字段属于bit位操作 */
};

typedef QList<FieldValue> FieldValues;

/*!
 *  @brief  保存解析后的结果值
 */
struct ParsedResult{
    FieldValues fieldResults;
    QList<ParsedResult * > results;
};

struct NodeInfo{
    NodeInfo():window("window"),name("name"),width("width"),height("height"),layout("layout")
    ,type("type"),widget("widget"),groupBox("groupbox"),item("item"),items("items"),nodeShow("show"),nodeColumn("column"),nodeEnabled("enabled"),
    itemName("name"),itemBytes("bytes"),itemSigned("signed"),itemBits("bits"),itemText("text"),
    itemWeight("weight"),itemPrecision("precision"),itemUnit("unit"),itemType("type"),itemComboxList("list"),itemRange("range"),itemEnable("enable"),
    itemVisible("visible"),itemDefaultValue("default"),bitStart("start"),bitLast("last"),itemMax("max"),itemMin("min"),itemRepeat("repeat"){

    }
    QString window;
    QString name;
    QString width;
    QString height;
    QString layout;
    QString type;

    QString widget;
    QString groupBox;

    QString item;
    QString items;

    QString nodeShow;
    QString nodeColumn;
    QString nodeEnabled;

    QString itemName;
    QString itemBytes;
    QString itemSigned;
    QString itemEnable;
    QString itemVisible;
    QString itemBits;
    QString itemWeight;
    QString itemText;
    QString itemPrecision;
    QString itemUnit;
    QString itemRange;
    QString itemType;
    QString itemComboxList;
    QString itemDefaultValue;
    QString itemMax;
    QString itemMin;
    QString itemRepeat;

    QString bitStart;
    QString bitLast;
};

/*!
 *  @brief xml文件中控件类型名称
 */
struct WidgetType{
    WidgetType():combox("combox"),checkBox("checkBox"),radioButton("radioButton"),textEdit("textEdit"),valueint("valueint"),
        valuefloat("valuefloat"),dateEdit("dateEdit"),timeEdit("timeEdit"),dialog("dialog"),widget("widget"),table("table"),list("list"){}

    QString combox;
    QString checkBox;
    QString radioButton;
    QString textEdit;
    QString valueint;
    QString valuefloat;
    QString dateEdit;
    QString timeEdit;
    QString dialog;
    QString widget;
    QString table;
    QString list;

};

/******************************任务控制模块*****************************************/

/*!
 *  @brief 布局类型
 *  @details 20181030暂支持3种类型
 */
enum Layout
{
    None,
    Horizonal,          /*!< Gridlayout布局一行，多列 */
    Vertical,           /*!< Gridlayout布局一列，多行 */
    Grid                /*!< Gridlayout布局多行，多列 */
};

/*!
 *  @brief 窗口数据信息
 */
struct WindowData{
    WindowData(){
        isShown = true;
        isEnabled = true;
        column = 1;
    }

    QString  name;        /*!< 控件前面label显示信息 */

    Layout layout;        /*!< 布局类型 */
    bool isShown;         /*!< 可进行显示 */
    bool isEnabled;       /*!< 可编辑 */
    ushort column;        /*!< 进行显示的列数 */
    ushort width;         /*!< 宽度 */
    ushort height;        /*!< 高度 */
};

/*!
 *  @brief 容器信息显示描述
 */
struct Window : public PubHead{
    Window():PubHead(){ }
    ~Window(){}
    WindowData data;       /*!< 窗口属性 */
};

/*!
 *  @brief 节点容器
 *  @details 1.容器可嵌套；
 *           2.大的容器中可包含子容器，由 @see dataElem 来定义；
 *           3.最内的容器只包含字段信息
 */
struct Container{
    Window     continer;                  /*!< 描述当前容器的属性 */
    QList<Field*> fileds;                 /*!< 节点集合 */
    QList<Container*>  childContainer;    /*!< 子容器 */
};
/******************************任务控制模块*****************************************/


/**************************健康管理/数据显示模块************************************/
/*!
 *  @brief 单个协议描述
 */
struct SignalProtocol{
    int count;                   /*!< 当前协议数量：>0：表示固定的数量，-1：表示数量不定，需结合memoryBytes */
    int memoryBytes;            /*!< 当count字段为变长时，count字段占用几字节空间,用于解析协议数量 */
    int length;                 /*!< 协议长度 */
    QList<FieldData> fields;    /*!< 协议字段集合 */
};

/*!
 *  @brief 基本协议
 */
struct BaseProtocol{
    QString name;       /*!< 协议名 */
    int startCode;      /*!< 开始标志码 */
    int type;           /*!< 协议类型 */
    int length;         /*!< 整包数据长度 */
    int startLen;       /*!< 数据头部固定长度 */
    int count;          /*!< 最小协议包数据量，>0:表示固定数量，-1:表示数量不定，需结合memoryBytes */
    int memoryBytes;    /*!< 当count字段为变长时，count字段占用几字节空间,用于解析协议数量 */
    QList<SignalProtocol> protocols;    /*!< 协议族 */
    int endLen;         /*!< 数据尾部固定长度 */
    int endCode;        /*!< 结束标志码 */
};
/**************************健康管理/数据显示模块************************************/

/*!
 *  @brief 插件描述信息
 *  @details
 */
struct PluginInfo{
    QString id;         /*!< 插件标识 */
    QString name;       /*!< 名称 */
};

enum NetworkType{
    N_TCP,
    N_UDP
};

/*!
 *  @brief 网络连接类型
 */
enum NetworkConnectionType{
    N_Server,
    N_Client
};

/*!
 *  @brief 网络基本信息
 */
struct NetworkBase{
    QString ip;
    ushort port;
    NetworkConnectionType connectionType;
    bool multicast;         /*!< 是否开启组播 */
    QString multicastIp;    /*!< 组播Ip */
};

/*!
 *  @brief 网络配置信息
 *  @details 初始化网络模块
 */
struct NetworkInfo{
    QString id;             /*!< 网络Id */
    NetworkType protocol;   /*!< 传输类型 */
    NetworkBase baseInfo;   /*!< 基础信息 */
};

enum WindowLayout{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

/*!
 *  @brief  模块信息
 *  @details
 */
struct ModuleInfo{
    QString id;             /*!< 模块ID */
    WindowLayout layout;    /*!< 在mainwindow中的位置方向 */
    QRect geometry;         /*!< 尺寸信息 */
    bool closeable;         /*!< 是否可关闭 */
    bool visible;           /*!< 是否可见 */
    bool floatable;         /*!< 是否可移动 */
    QString name;           /*!< 模块名称 */
    QStringList protocols;  /*!< 协议id， @see  ProtocolInfo */
    QString pluginId;       /*!< 内部插件ID @see PluginInfo */
    QString networkId;      /*!< 网络模块id， @see BaseProtocol */
};

} //namespace Datastruct

#endif // DATASTRUCT_H
