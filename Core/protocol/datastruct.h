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

#include <QString>
#include <QMap>
#include <QVector>

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
 *  @brief 全脉冲统计参数信息
 */
struct AllPluseStatisticInfoBase
{
    char cStaInfoName;              /*!< 统计参数名称*/
    double dMin;                    /*!< 最小值*/
    double dMax;                    /*!< 最大值*/
    double dAve;                    /*!< 均值*/
    double dStd;                    /*!< 均方差*/
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


} //namespace Datastruct

#endif // DATASTRUCT_H
