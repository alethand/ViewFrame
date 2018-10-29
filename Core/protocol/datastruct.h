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

            void addPointedColShow(int col){specifedCol.insert(col);}   //添加指定列显示
            void closePointedColShow(){ specifedCol.clear();}
        public:
            HugeData_Gram *mdata;
            ushort rowsLatestMsg;           /*!< 需要显示的最新消息行数 */
            QSet<int> specifedCol;        /*!< 指定可显示列         */
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
    SystemConfigInfo():defaultKeySchemes(true){
    }
    bool defaultKeySchemes;             /*!< 是否采用默认的快捷键设置，默认为true */
    QString userKeySchemesName;         /*!< 自定义快捷键设置名称 @see defaultKeySchemes 为false时有效 */
    QString locale;                     /*!< 显示的语言 */
    QString style;                      /*!< 默认样式 */
};


/*!
 *  @brief 数据源信息基本单元
 */
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
    RadiationSourceBase rsInfo;     /*!< 数据源信息>*/
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
 *  @brief 全脉冲统计参数信息基结构
 */
struct AllPluseStatisticInfoBase
{
    char arrStaInfoName[64];        /*!< 统计参数名称*/
    double dMin;                    /*!< 最小值*/
    double dMax;                    /*!< 最大值*/
    double dAve;                    /*!< 均值*/
    double dStd;                    /*!< 均方差*/
};

/*!
 *  @brief 全脉冲统计参数信息列
 */
enum AllPluseStatisticInfoHead
{
    T_StatisticNo=0,                         /*!< 行号*/
    T_StaInfoName,                  /*!< 统计参数名称*/
    T_Min,                          /*!< 最小值*/
    T_Max,                          /*!< 最大值*/
    T_Ave,                          /*!< 均值*/
    T_Std                           /*!< 均方差*/
};



/*!
 *  @brief 全脉冲原始数据属性基结构
 */
struct AllPluseOriginalInfoAttributeBase
{
    char arrOrgInfoAttributeName[64];       /*!< 原始数据属性名称*/
    int iDrawFlag;                          /*!< 属性是否绘图*/
    double dValue;                          /*!< 属性值*/
};

/*!
 *  @brief 全脉冲原始数据列
 */
enum AllPluseOriginalInfoAttributeHead
{
    T_OriginalNo=0,                             /*!< 行号*/
    T_OrgInfoAttributeName,                     /*!< 原始数据属性名称*/
    T_DrawFlag,                                 /*!< 属性是否绘图*/
    T_Value                                     /*!< 属性值*/
};


/*!
 *  @brief 全脉冲信息
 */
struct AllPluseInfo
{

    struct OriginData{/*! 原始数据*/

        struct Cntl :public Data::Elem_Cntl{
            struct Struct_Repeat{
                char name[64];                      //属性名称
                ulong ifDrawPic;                    //是否绘图
            };

            ulong ifValid;                      //是否有效
            ulong count;                        //数据个数
            QList<Struct_Repeat> details;       //重复性数据细节
        };

        struct Core:public Data::Elem_Core
        {

            QString        name;                //属性名
            QList<double>  valuelist;          //属性值

            virtual  QString getName(int index)
            {
                Q_UNUSED(index)
                return QStringLiteral("属性");
                //return name;
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

            MaxValue clacMaxValue;     /*! 最大值计算器   */
            MinValue clacMinValue;     /*! 最小值计算器   */
            MeanValue clacMeanValue;   /*! 平均值计算器   */
            Variance clacvariance;     /*! 方差计算器   */

            virtual  QString getName(int index)
            {
                switch(index)
                {
                case 0:return QStringLiteral("最大值");
                case 1:return QStringLiteral("最小值");
                case 2:return QStringLiteral("平均值");
                case 3:return QStringLiteral("方差");
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
 *  @brief 全脉冲原始数据属性
 */
struct AllPluseOriginalInfoAttribute
{
    int iDataOutsideNo;                                     /*!< 原始数据属性总批号(总数据帧依次递增)*/
    int iDataInsideNo;                                      /*!< 原始数据属性分批号(一帧数据中有多条统计信息,首批号为1,批号依次递增) */
    //QString strPulseArriveColck;                            /*!< 脉冲到达时间*/
    QDateTime dtPulseArriveColck;                             /*!< 脉冲到达时间*/
    AllPluseOriginalInfoAttributeBase allPluseOriginalInfoBase;         /*!< 原始数据属性*/
};


typedef QList<AllPluseOriginalInfoAttribute> AllPulseOriginalInfoList;  /*!< 全脉冲原始信息列表*/

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
        T_MFInfoNo=0,                                       /*!< 行号*/
        T_AcqTime,                                          /*!< 采集时间*/
        T_AcqModel,                                         /*!< 采集模式*/
        T_AcqNum,                                           /*!< 脉冲采集个数*/
        T_AcqDotNum                                         /*!< 采集点数*/
    };

    QString strAcqTime;                                    /*!< 采集时间*/
    short sAcqModel;                                        /*!< 采集模式,0xCF01:VP触发采,0xCF02:盲采,0xCF00:中频数据不采集*/
    int iAcqNum;                                            /*!< 脉冲采集个数*/
    int iAcqDotNum;                                         /*!< 采集点数*/
};


typedef QList<MidFreqInfo> MFAcquistionInfoList;      /*!< 中频采集数据列表*/

} //namespace Datastruct





#endif // DATASTRUCT_H
